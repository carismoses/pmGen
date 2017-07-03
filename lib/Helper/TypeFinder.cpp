#include "TypeFinder.h"
#include "TypeGen.h"
#include "llvm/IR/Module.h"

// #include "llvm/ADT/DenseMap.h"
// #include "llvm/IR/Type.h"
// #include "Helper.h"
// #include "llvm/ADT/StringExtras.h"
// #include "llvm/ADT/STLExtras.h"
//OLD#include "llvm/TypeSymbolTable.h"
// #include "llvm/IR/ValueSymbolTable.h"
// #include "llvm/Support/raw_ostream.h"
// #include "llvm/Support/FormattedStream.h"
// #include <cctype>

using namespace llvm;

void TypeFinder::Run(const Module &M) {
    AddModuleTypesToPrinter(&M);

    /*
    // Get types from the type symbol table.  This gets opaque types referened
    // only through derived named types.
    const ValueSymbolTable &ST = M.getValueSymbolTable();
    for (ValueSymbolTable::cgonst_iterator TI = ST.begin(), E = ST.end();
           TI != E; ++TI)
        IncorporateType(TI->second);

    // change from incorporate type (from TypeSymbolTable) to incorporate value
    // (from ValueSymbolTable)
    const ValueSymbolTable &ST = M.getValueSymbolTable();
    for (ValueSymbolTable::const_iterator VI = ST.begin(), E = ST.end();
         VI != E; ++VI)
        IncorporateValue(VI->second);
    
    // Get types from global variables.
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
    */
}

/*
void TypeFinder::IncorporateType(const Type *Ty) {
    // Check to see if we're already visited this type.
    if (!VisitedTypes.insert(Ty).second)
        return;

    // If this is a structure or opaque type, add a name for the type.
    // now only struct types can be opaque
    // changing to just if structure, now only structs can be opaque
    if (Ty->isStructTy() && cast<StructType>(Ty)->getNumElements()) {
        //|| Ty->isOpaqueTy()) && !TP.hasTypeName(Ty)) {
        TP.addTypeName(Ty, "n_"+utostr(unsigned(NumberedTypes.size())));
        NumberedTypes.push_back(Ty);
    }

    // Recursively walk all contained types.
    for (Type::subtype_iterator I = Ty->subtype_begin(),
             E = Ty->subtype_end(); I != E; ++I)
        IncorporateType(*I);
}

/// IncorporateValue - This method is used to walk operand lists finding
/// types hiding in constant expressions and other operands that won't be
/// walked in other ways.  GlobalValues, basic blocks, instructions, and
/// inst operands are all explicitly enumerated.
void TypeFinder::IncorporateValue(const Value *V) {
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
*/

void TypeFinder::AddModuleTypesToPrinter(const Module *M) {
    if (M == 0) return;

    
    // If the module has a symbol table, take all global types and stuff their
    // names into the TypeNames map.
    const ValueSymbolTable &ST = M->getValueSymbolTable();
    /*
    for (ValueSymbolTable::const_iterator VI = ST.begin(), E = ST.end();
         VI != E; ++VI) {

        // not sure how this isn't throwing an error.
        // VI-> should be a value, not a type
        //const Type *Ty = cast<Type>(VI->second);
        const Type *Ty = VI->second;
        
        // As a heuristic, don't insert pointer to primitive types, because
        // they are used too often to have a single useful name.
        if (const PointerType *PTy = dyn_cast<PointerType>(Ty)) {
            const Type *PETy = PTy->getElementType();
            if ((PETy->isPrimitiveType() || PETy->isIntegerTy()) &&
                !PETy->isOpaqueTy())
                continue;
        }

        // Likewise don't insert primitives either.
        if (Ty->isIntegerTy() || Ty->isPrimitiveType())
            continue;

        // Get the name as a string and insert it into TypeNames.
        std::string NameStr;
        raw_string_ostream NameROS(NameStr);
        formatted_raw_ostream NameOS(NameROS);
        Helper::PrintLLVMName(NameOS, TI->first, LocalPrefix);
        NameOS.flush();
        TP.addTypeName(Ty, NameStr);
    }
    */
}

