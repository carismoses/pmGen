#include "TypeFinder.h"
#include "TypeGen.h"
#include "Helper.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/ADT/StringExtras.h"

using namespace llvm;

void TypeFinder::Run(const Module &M) {
    AddModuleTypesToPrinter(&M);

    // Get types from the value symbol table.  This gets opaque types referened
    // only through derived named types -- not sure if this is still true.
    const ValueSymbolTable &ST = M.getValueSymbolTable();
    for (ValueSymbolTable::const_iterator VI = ST.begin(), E = ST.end();
           VI != E; ++VI)
        // IncorporateType(TI->second);
        IncorporateValue(VI->second);
    
    // Get types from global variables. (aren't all global variable
    // types in the ValueSymbolTable ??)
    for (Module::const_global_iterator I = M.global_begin(),
             E = M.global_end(); I != E; ++I) {
        IncorporateType(I->getType());
        if (I->hasInitializer())
            IncorporateValue(I->getInitializer());
    }
    
    // Get types from aliases.
    for (Module::const_alias_iterator I = M.alias_begin(),
             E = M.alias_end(); I != E; ++I) {
        IncorporateType(I->getType());
        IncorporateValue(I->getAliasee());
    }
    
    // Get types from functions.
    // function iterator
    for (Module::const_iterator FI = M.begin(), E = M.end(); FI != E; ++FI) {
        IncorporateType(FI->getType());
        // basic blocks iterator
        for (Function::const_iterator BB = FI->begin(), E = FI->end();
             BB != E;++BB)
            for (BasicBlock::const_iterator II = BB->begin(),
                     E = BB->end(); II != E; ++II) {
                const Instruction &I = *II;
                // Incorporate the type of the instruction and all its operands.
                IncorporateType(I.getType());
                for (User::const_op_iterator OI = I.op_begin(), OE = I.op_end();
                     OI != OE; ++OI)
                    IncorporateValue(*OI);
            }
    }
}

void TypeFinder::IncorporateType(const Type *Ty) {
    // Check to see if we're already visited this type.
    if (!VisitedTypes.insert(Ty).second)
        return;

    // If this is a structure type that we HAVEN'T SEEN YET
    if (Ty->isStructTy() && !TP.hasTypeName(Ty)) {
        const StructType* STy = cast<StructType>(Ty);
        if (STy->getNumElements()){
            if (STy->hasName()){
                TP.addTypeName(Ty, STy->getName());
            }
            else{
                TP.addTypeName(Ty, "n_"+utostr(unsigned(NumberedTypes.size())));
            }
            NumberedTypes.push_back(Ty);
        }
    }

    // Recursively walk all contained types.
    for (Type::subtype_iterator I = Ty->subtype_begin(),
             E = Ty->subtype_end(); I != E; ++I)
        IncorporateType(*I);
}

void TypeFinder::IncorporateValue(const Value *V) {
    // might need to delete this since no longe rusing IncorporateType()
    // directly from module Type (now Value) SymbolTable
    if (V == 0 || !isa<Constant>(V) || isa<GlobalValue>(V)) return;

    // Already visited?
    if (!VisitedConstants.insert(V).second)
        return;

    // Check this type.
    IncorporateType(V->getType());

    // Look in operands for types.
    const Constant *C = cast<Constant>(V);
    for (Constant::const_op_iterator I = C->op_begin(),
             E = C->op_end(); I != E;++I)
        IncorporateValue(*I);
}

void TypeFinder::AddModuleTypesToPrinter(const Module *M) {
    if (M == 0) return;

    // If the module has a symbol table, take all global types and stuff their
    // names into the TypeNames map.
    const ValueSymbolTable &ST = M->getValueSymbolTable();
    for (ValueSymbolTable::const_iterator VI = ST.begin(), E = ST.end();
         VI != E; ++VI) {

        // get Value from SymbolTableEntry
        const Value *Val = VI->second; 
        // get the Type of the Value 
        const Type *Ty = Val->getType();

        // Possible Optimizations here (from orig developer but syntax has changed):
        // As a heuristic, don't insert pointer to primitive types, because
        // they are used too often to have a single useful name.
        // Likewise don't insert primitives either.

        if (const PointerType *PTy = dyn_cast<PointerType>(Ty)) {
            const Type *PETy = PTy->getElementType();
            if ((PETy->getPrimitiveSizeInBits() != 0) || PETy->isIntegerTy())
                // !PETy->isOpaqueTy())   // opaque type is not a subtype of struct
                continue;

            // this is a hack...
            // do not add if the type is a Pointer to a Function with only a variable
            // number of arguments
            // this was creating a type whose name was not defined
            // (maybe could just eliminate any type who has a subtype with a
            // function type in it?)
            if (const FunctionType *PEPTy = dyn_cast<FunctionType>(PETy)) {
                //check for variable number of args
                if (PEPTy->isVarArg() && (PEPTy->getNumParams() == 0)){
                    continue;
                }
            }
        }
        if (Ty->isIntegerTy() || (Ty->getPrimitiveSizeInBits() != 0))
            continue;

        // Get the name as a string and insert it into TypeNames.
        std::string NameStr;
        raw_string_ostream NameROS(NameStr);
        formatted_raw_ostream NameOS(NameROS);
        // old code printed Type string from TypeSymbolTable,
        // is that the same as the Value string from the Value symbol table
        Helper::PrintLLVMName(NameOS, VI->first(), LocalPrefix);
        NameOS.flush();
        TP.addTypeName(Ty, NameStr);
    }
}
