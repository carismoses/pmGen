#ifndef PMG_TYPEFINDER_H
#define PMG_TYPEFINDER_H

//OLD#include "llvm/Assembly/Writer.h"
//OLD#include "llvm/Module.h"
//OLD#include "llvm/DerivedTypes.h"
//OLD#include "llvm/Support/ErrorHandling.h"

#include "TypeGen.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Module.h"
#include "llvm/ADT/DenseSet.h"

namespace llvm {

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

        void Run(const Module &M);
    private:
        void IncorporateType(const Type *Ty);

        /// IncorporateValue - This method is used to walk operand lists finding
        /// types hiding in constant expressions and other operands that won't be
        /// walked in other ways.  GlobalValues, basic blocks, instructions, and
        /// inst operands are all explicitly enumerated.
        void IncorporateValue(const Value *V);
        void AddModuleTypesToPrinter(TypeGen &TP,const Module *M);
    };

}

#endif
