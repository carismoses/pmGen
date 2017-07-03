#-------------------------------------------------------------------------------
# Sample makefile for building the code samples. Read inline comments for
# documentation.
#
# Eli Bendersky (eliben@gmail.com)
# This code is in the public domain
#-------------------------------------------------------------------------------
# Modified by Caris Moses for pmGen project
#-------------------------------------------------------------------------------

# The following variables will likely need to be customized, depending on where
# and how you built LLVM & Clang. They can be overridden by setting them on the
# make command line: "make VARNAME=VALUE", etc.

# LLVM_SRC_PATH is the path to the root of the checked out source code. This
# directory should contain the configure script, the include/ and lib/
# directories of LLVM, Clang in tools/clang/, etc.
#
# The most recent build from source of LLVM I did used the following cmake
# invocation:
#
# $ cmake -G "Unix Makefiles"
#
# Alternatively, if you're building vs. a binary distribution of LLVM
# (downloaded from llvm.org), then LLVM_SRC_PATH can point to the main untarred
# directory of the binary download (the directory that has bin/, lib/, include/
# and other directories inside).
# See the build_vs_released_binary.sh script for an example.
LLVM_SRC_PATH := /home/mujin/mujin/jhbuildapppickworker/checkoutroot/llvm

# LLVM_BUILD_PATH is the directory in which you built LLVM - where you ran
# configure or cmake.
# For linking vs. a binary build of LLVM, point to the main untarred directory.
# LLVM_BIN_PATH is the directory where binaries are placed by the LLVM build
# process. It should contain the tools like opt, llc and clang. The default
# reflects a release build with CMake and Ninja. binary build of LLVM, point it
# to the bin/ directory.
LLVM_BUILD_PATH := /home/mujin/mujin/jhbuildapppickworker/docker/build/llvm
LLVM_BIN_PATH 	:= $(LLVM_BUILD_PATH)/bin

$(info -----------------------------------------------)
$(info Using LLVM_SRC_PATH = $(LLVM_SRC_PATH))
$(info Using LLVM_BUILD_PATH = $(LLVM_BUILD_PATH))
$(info Using LLVM_BIN_PATH = $(LLVM_BIN_PATH))
$(info -----------------------------------------------)

# CXX has to be a fairly modern C++ compiler that supports C++11. gcc 4.8 and
# higher or Clang 3.2 and higher are recommended. Best of all, if you build LLVM
# from sources, use the same compiler you built LLVM with.
# Note: starting with release 3.7, llvm-config will inject flags that gcc may
# not support (for example '-Wcovered-switch-default'). If you run into this
# problem, build with CXX set to a modern clang++ binary instead of g++.
CXX := g++
CXXFLAGS := -fno-rtti -O0 -g
PLUGIN_CXXFLAGS := -fpic

LLVM_CXXFLAGS := `$(LLVM_BIN_PATH)/llvm-config --cxxflags`
# may need to add --includedirs to this?
LLVM_LDFLAGS := `$(LLVM_BIN_PATH)/llvm-config --ldflags --libs --system-libs`

# Plugins shouldn't link LLVM and Clang libs statically, because they are
# already linked into the main executable (opt or clang). LLVM doesn't like its
# libs to be linked more than once because it uses globals for configuration
# and plugin registration, and these trample over each other.
LLVM_LDFLAGS_NOLIBS := `$(LLVM_BIN_PATH)/llvm-config --ldflags`
PLUGIN_LDFLAGS := -shared

# These are required when compiling vs. a source distribution of Clang. For
# binary distributions llvm-config --cxxflags gives the right path.
CLANG_INCLUDES := \
	-I$(LLVM_SRC_PATH)/tools/clang/include \
	-I$(LLVM_BUILD_PATH)/tools/clang/include

# List of Clang libraries to link. The proper -L will be provided by the
# call to llvm-config
# Note that I'm using -Wl,--{start|end}-group around the Clang libs; this is
# because there are circular dependencies that make the correct order difficult
# to specify and maintain. The linker group options make the linking somewhat
# slower, but IMHO they're still perfectly fine for tools that link with Clang.
#CLANG_LIBS := \
#	-Wl,--start-group \
#	-lclangAST \
#	-lclangASTMatchers \
#	-lclangAnalysis \
#	-lclangBasic \
#	-lclangDriver \
#	-lclangEdit \
#	-lclangFrontend \
#	-lclangFrontendTool \
#	-lclangLex \
#	-lclangParse \
#	-lclangSema \
#	-lclangEdit \
#	-lclangRewrite \
#	-lclangRewriteFrontend \
#	-lclangStaticAnalyzerFrontend \
#	-lclangStaticAnalyzerCheckers \
#	-lclangStaticAnalyzerCore \
#	-lclangSerialization \
#	-lclangToolingCore \
#	-lclangTooling \
#	-lclangFormat \
#	-Wl,--end-group

# Internal paths in this project: where to find sources, and where to put
# build artifacts.
#PMGEN_ROOT := /home/mujin/mujin/jhbuildapppickworker/checkoutroot/pmGen

#would like to change to src
SRC_DIR := tools/pmGen
LIB_DIR := lib
BUILD_DIR := build
BIN_DIR := bin
INCLUDE_DIR := include

PMGEN_INCDIRS = -I $(INCLUDE_DIR)
#-I $(LLVM_SRC_PATH)/include

.PHONY: all
all: make_builddir \
	make_bindir \
	emit_build_config \
	$(BIN_DIR)/pmGen

# uncomment when make tests
#.PHONY: test
#test: emit_build_config
#	python3 test/all_tests.py

.PHONY: make_builddir
make_builddir:
	@test -d $(BUILD_DIR) || mkdir $(BUILD_DIR)

.PHONY: make_bindir
make_bindir:
	@test -d $(BIN_DIR) || mkdir $(BIN_DIR)

.PHONY: emit_build_config
emit_build_config: make_builddir
	@echo $(LLVM_BIN_PATH) > $(BUILD_DIR)/_build_config

# making pmGen executable
$(BIN_DIR)/pmGen: $(BUILD_DIR)/main.o $(BUILD_DIR)/IO.o $(BUILD_DIR)/TypeGen.o $(BUILD_DIR)/TypeFinder.o $(BUILD_DIR)/SlotTracker.o #$(BUILD_DIR)/Helper.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LLVM_LDFLAGS)  

# compiling main.o
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp
	$(CXX) $(PMGEN_INCDIRS) $(CXXFLAGS) $(LLVM_CXXFLAGS) -c $(LLVM_LDFLAGS) -o $@ $^

# compiling IO.o
$(BUILD_DIR)/IO.o: $(LIB_DIR)/IO/IO.cpp
	$(CXX) $(PMGEN_INCDIRS) $(CXXFLAGS) $(LLVM_CXXFLAGS) -c $(LLVM_LDFLAGS) -o $@ $^

$(BUILD_DIR)/TypeGen.o: $(LIB_DIR)/Helper/TypeGen.cpp
	$(CXX) $(PMGEN_INCDIRS) $(CXXFLAGS) $(LLVM_CXXFLAGS) -c $(LLVM_LDFLAGS) -o $@ $^

$(BUILD_DIR)/TypeFinder.o: $(LIB_DIR)/Helper/TypeFinder.cpp
	$(CXX) $(PMGEN_INCDIRS) $(CXXFLAGS) $(LLVM_CXXFLAGS) -c $(LLVM_LDFLAGS) -o $@ $^

#$(BUILD_DIR)/Helper.o: $(LIB_DIR)/Helper/Helper.cpp
#	$(CXX) $(PMGEN_INCDIRS) $(CXXFLAGS) $(LLVM_CXXFLAGS) -c $(LLVM_LDFLAGS) -o $@ $^

$(BUILD_DIR)/SlotTracker.o: $(LIB_DIR)/Helper/SlotTracker.cpp
	$(CXX) $(PMGEN_INCDIRS) $(CXXFLAGS) $(LLVM_CXXFLAGS) -c $(LLVM_LDFLAGS) -o $@ $^

#$(BUILD_DIR)/test.o: $(LIB_DIR)/test/test.c
#	$(CXX) $(PMGEN_INCDIRS) $(CXXFLAGS) $(LLVM_CXXFLAGS) $^ $(LLVM_LDFLAGS) -o $@

#$(BIN_DIR)/helper.a: $(BUILD_DIR)/helper.o
#	$(CXX) $(PMGEN_INCDIRS) $(CXXFLAGS) $(LLVM_CXXFLAGS) $^ $(LLVM_LDFLAGS) -o $@

#$(BIN_DIR)/io.a: $(BUILD_DIR)/io.o
#	$(CXX) $(PMGEN_INCDIRS) $(CXXFLAGS) $(LLVM_CXXFLAGS) $^ $(LLVM_LDFLAGS) -o $@

#$(BIN_DIR)/test.a: $(BUILD_DIR)/test.o
#	$(CXX) $(PMGEN_INCDIRS) $(CXXFLAGS) $(LLVM_CXXFLAGS) $^ $(LLVM_LDFLAGS) -o $@

.PHONY: clean format

clean:
	rm -rf $(BUILD_DIR)/* *.dot test/*.pyc test/__pycache__
	rm -rf $(BIN_DIR)/* *.dot test/*.pyc test/__pycache__

format:
	find . -name "*.cpp" | xargs clang-format -style=file -i
