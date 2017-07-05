#ifndef PMG_IO_H
#define PMG_IO_H

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

using namespace llvm;

/// uses parseIRFile from llvm to parse the input .bc file into a Module
Module* parseModule(std::string InputFileName,LLVMContext &Context);

#endif
