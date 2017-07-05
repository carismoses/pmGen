#include "IO.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"

using namespace std;

Module* parseModule(std::string InputFileName,LLVMContext &Context){
	SMDiagnostic Err;

    // returned as a unique_ptr so have to release
	Module* m=parseIRFile(InputFileName,Err,Context).release();
	return m;
}

