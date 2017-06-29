#include "IO.h"
#include "llvm/Support/SourceMgr.h"
//OLD#include "llvm/Support/IRReader.h"
#include "llvm/IRReader/IRReader.h"

using namespace std;

std::unique_ptr<Module> parseModule(std::string InputFileName,LLVMContext &Context){
	SMDiagnostic Err;
	std::unique_ptr<Module> m=parseIRFile(InputFileName,Err,Context);
	return m;
}

