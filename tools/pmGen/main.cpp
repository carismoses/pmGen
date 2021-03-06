#include "IO.h"
#include "TypeGen.h"
#include "Helper.h"
#include "TypeFinder.h"
#include "SlotTracker.h"
#include "FunctionGen.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/ValueSymbolTable.h"
#include <unistd.h>

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
    Module* m = parseModule(InputFileName,Context);
	TypeGen TypeGener;
    
    // just initializes the Module* member of SlotTable
    SlotTracker SlotTable(m);
    ConStr conStr;
    std::vector<const Type*> numberedTypes;

    // just initializes TypeGener and numberedTypes members of typeFinder
    TypeFinder typeFinder(TypeGener,numberedTypes);

    // populate typeFinder and numberedTypes with all types from Module
    // typegen stored TypeNames and numberedTypes stores numbered types
    // still not sure what the difference is...
    typeFinder.Run(*m);
    formatted_raw_ostream OS(outs());

    // print all type names
    // the getValueSymbolTable() gets the symbol table of all global variables and function identifiers    
    TypeGener.gen(numberedTypes,m->getValueSymbolTable(),outs());

    // if there are global variables
    if (!m->global_empty()) outs()<<'\n';

    Helper::InitBE(initProc,true);

    for (Module::const_global_iterator GI=m->global_begin(),GE=m->global_end();
         GI!=GE;++GI) {
        // if it has an initializer and is of type ConstantDataSequential.isString(), add it to conStr
        // else, enter this loop
        if (!conStr.isConStr(&*GI)){
            GVTmp.clear();
            // not sure what this is doing. getType() doesn't appear to be  function of GlobalVariable...
            TypeGener.print(GI->getType()->getElementType(),gvTmp);
            
            gvTmp<<' ';
            // if(const Value *V = dyn_cast<Value>(GI))
            Helper::WriteAsOperandInternal(gvTmp,&*GI,&TypeGener,&SlotTable,GI->getParent());
            gvTmp<<";\n";
            gvTmp << "\n";
            gvTmp.flush();
            Helper::Formatting(GVTmp);
            if (GI->hasInitializer())
                Helper::InitGValue(initProc,&*GI,&TypeGener,&SlotTable,GI->getParent());
            outs()<<GVTmp;
        }
    }
    
    FunctionGen functionGener(TypeGener,SlotTable,OS,m);
    for (Module::const_iterator FI=m->begin(),FE=m->end();FI!=FE;++FI){
        functionGener.printFunction(&*FI);
    }

    Helper::InitBE(initProc,false);
    outs()<<initProc.str();
    return 0;
}
