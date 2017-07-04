#include "IO.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"

using namespace std;

std::unique_ptr<Module> parseModule(std::string InputFileName,LLVMContext &Context){
	SMDiagnostic Err;
	std::unique_ptr<Module> m=parseIRFile(InputFileName,Err,Context);
	return m;
}

