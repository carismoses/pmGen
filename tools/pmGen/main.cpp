#include "test.h"
#include "IO.h"
#include "TypeFinder.h"
#include "Helper.h"
#include "SlotTracker.h"
//#include "FunctionGen.h"

#include "llvm/Support/FormattedStream.h"

#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Module.h"

#include "llvm/AsmParser/Parser.h"
#include "llvm/IR/LLVMContext.h"
//#include "llvm/IR/ModuleSlotTracker.h"
//OLD#include "llvm/Assembly/Writer.h"
//OLD#include "llvm/TypeSymbolTable.h"

// this is just for testing
#include <iostream>

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
	}
	else {
		errs() << "Please input the .bc file.\n";
		exit(1);
	}

	std::unique_ptr<Module> m=parseModule(InputFileName,Context);

    outs()<<*m<<"\n-----------------------\n\n";

    /*
	//Module::global_iterator begin=m->global_begin();
	//Module::global_iterator end=m->global_end();
	TypeGen TypeGener;
	SlotTracker SlotTable(*m);
    
	ConStr conStr;
	std::vector<const Type*> numberedTypes;
	TypeFinder typeFinder(TypeGener,numberedTypes);
	typeFinder.Run(*m);
    
    formatted_raw_ostream OS(outs());

    // getTypeSymbolTable just returns an (empty?) table. it contains a map of type, symbol pairs
	// i believe gen is supposed to populate this table
    TypeGener.gen(numberedTypes,m->getValueSymbolTable(),outs());
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
