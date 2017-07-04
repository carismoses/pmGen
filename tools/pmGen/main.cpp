#include "IO.h"
#include "TypeGen.h"
#include "Helper.h"
#include "TypeFinder.h"
#include "SlotTracker.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Type.h"
#include <unistd.h>

// #include "TypeFinder.h"
// #include "Helper.h"
// #include "SlotTracker.h"
// #include "FunctionGen.h"
// #include "llvm/Support/FormattedStream.h"
// #include "llvm/AsmParser/Parser.h"
// #include "llvm/IR/ModuleSlotTracker.h"
//OLD#include "llvm/Assembly/Writer.h"
//OLD#include "llvm/TypeSymbolTable.h"

using namespace llvm;

int main (int argc, char ** argv)
{
	LLVMContext Context;
	std::string InputFileName;
	std::string InitProc;
	std::string GVTmp;
	raw_string_ostream initProc(InitProc);
	raw_string_ostream gvTmp(GVTmp);
    
	if (argc>1){
		InputFileName=argv[1];
        
        // check if the file exists
        if (access(InputFileName.c_str(), F_OK ) == -1) {
            errs() << "Given file does not exist. \n";
            exit(1);
        }
    }
    else {
        errs() << "Please input the .bc file.\n";
        exit(1);
    }

    // get Module from input file
    std::unique_ptr<Module> m = parseModule(InputFileName,Context);
    
	TypeGen TypeGener;
    SlotTracker SlotTable(*m);
    ConStr conStr;
    std::vector<const Type*> numberedTypes;
    TypeFinder typeFinder(TypeGener,numberedTypes);

    // populate TypeGener and numberedTypes with all types from Module
    typeFinder.Run(*m);

    TypeGener.gen(numberedTypes,m->getValueSymbolTable(),outs());

    /*
    if (!m->global_empty()) outs()<<'\n';
    Helper::InitBE(initProc,true);
    std::cout << "working to this point" << "\n";
    for (Module::const_global_iterator GI=m->global_begin(),GE=m->global_end();
    GI!=GE;++GI) {
    // don't want string constant global variables?
    if (!conStr.isConStr(*GI)){
    GVTmp.clear();
    TypeGener.print(GI->getType()->getElementType(),gvTmp);
    gvTmp<<' ';	
    Helper::WriteAsOperandInternal(gvTmp,*GI,&TypeGener,&SlotTable,GI->getParent());
    gvTmp<<";\n";
    gvTmp.flush();
    Helper::Formatting(GVTmp);
    if (GI->hasInitializer())
    Helper::InitGValue(initProc,GI,&TypeGener,&SlotTable,GI->getParent());
    outs()<<GVTmp;
    }
    }

    FunctionGen functionGener(TypeGener,SlotTable,OS,m);
    for (Module::const_iterator FI=m->begin(),FE=m->end();FI!=FE;++FI){
    functionGener.printFunction(FI);
    }
    Helper::InitBE(initProc,false);
    outs()<<initProc.str();
    */	
    return 0;
}
