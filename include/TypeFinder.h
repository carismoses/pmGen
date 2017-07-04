#ifndef PMG_TYPEFINDER_H
#define PMG_TYPEFINDER_H

#include "TypeGen.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Module.h"
#include "llvm/ADT/DenseSet.h"

namespace llvm {

    /// TypeFinder goes through all module level types, global variables, and BBs->Functions->Instructions
    /// and finds all Types. It adds them to NumberedTypes and TP
    class TypeFinder {
        // To avoid walking constant expressions multiple times and other IR
        // objects, we keep several helper maps.
        DenseSet<const Value*> VisitedConstants;
        DenseSet<const Type*> VisitedTypes;

        TypeGen &TP;
        std::vector<const Type*> &NumberedTypes;
    public:
        TypeFinder(TypeGen &tp, std::vector<const Type*> &numberedTypes)
            : TP(tp), NumberedTypes(numberedTypes) {}

        /// main function for finding all types at all levels
        void Run(const Module &M);
    private:
        /// Adds given type to TP and NumberedTypes
        void IncorporateType(const Type *Ty);

        /// IncorporateValue - This method is used to walk operand lists finding
        /// types hiding in constant expressions and other operands that won't be
        /// walked in other ways.  GlobalValues, basic blocks, instructions, and
        /// inst operands are all explicitly enumerated.
        void IncorporateValue(const Value *V);

        // Get types from all global variables and function identifiers
        // and add them to TP.TypeNames
        void AddModuleTypesToPrinter(const Module *M);
    };

}

#endif
