#ifndef PMG_IO_H
#define PMG_IO_H

//OLD#include "llvm/LLVMContext.h"
//OLD#include "llvm/Module.h"
//OLD#include "llvm/Assembly/Parser.h"
//OLD#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/AsmParser/Parser.h"
#include "llvm/Support/raw_ostream.h"

// still need to find 5.0 equivalent of these 2 headers
//OLD#include "llvm/Bitcode/ReaderWriter.h"
//OLD#include "llvm/Support/IRReader.h"

using namespace llvm;

std::unique_ptr< Module > parseModule(std::string InputFileName,LLVMContext &Context);

#endif
