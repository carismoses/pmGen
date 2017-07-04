#include "TypeGen.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/SmallVector.h"

// #include "Helper.h"
// #include "TypeFinder.h"
// #include "llvm/ADT/StringExtras.h"
// #include "llvm/ADT/STLExtras.h"
//OLD#include "llvm/TypeSymbolTable.h"
// #include "llvm/IR/ValueSymbolTable.h"
// #include "llvm/Support/FormattedStream.h"
// #include <cctype>

using namespace llvm;

/// getTypeNamesMap takes the input and static_casts it
/// as a pointer to a DenseMap
static DenseMap<const Type *, std::string> &getTypeNamesMap(void *M) {
	return *static_cast<DenseMap<const Type *, std::string>*>(M);
}
/*
  void TypeGen::clear() {
  getTypeNamesMap(TypeNames).clear();
  }

  bool TypeGen::hasTypeName(const Type *Ty) const {
  return getTypeNamesMap(TypeNames).count(Ty);
  }
*/
void TypeGen::addTypeName(const Type *Ty, const std::string &N) {
  getTypeNamesMap(TypeNames).insert(std::make_pair(Ty, N));
  }

TypeGen::TypeGen() {
	TypeNames = new DenseMap<const Type *, std::string>();
}

/// destructor: gets the DenseMap from the TypeNames
/// and deletes it
TypeGen::~TypeGen() {
	delete &getTypeNamesMap(TypeNames);
}

/// CalcTypeName - Write the specified type to the specified raw_ostream, making
/// use of type names or up references to shorten the type name where possible.
void TypeGen::CalcTypeName(
                           const Type *Ty,
                           SmallVectorImpl<const Type *> &TypeStack,
                           raw_ostream &OS, bool IgnoreTopLevelName) {
    // Check to see if the type is named. (didn't we already do this in print()?)
    if (!IgnoreTopLevelName) {
        DenseMap<const Type *, std::string> &TM = getTypeNamesMap(TypeNames);
        DenseMap<const Type *, std::string>::iterator I = TM.find(Ty);
        if (I != TM.end()) {
            OS << I->second;
            return;
        }
    }

    /*
    // Check to see if the Type is already on the stack...
    unsigned Slot = 0, CurSize = TypeStack.size();
    while (Slot < CurSize && TypeStack[Slot] != Ty) ++Slot; // Scan for type

    // This is another base case for the recursion.  In this case, we know
    // that we have looped back to a type that we have previously visited.
    // Generate the appropriate upreference to handle this.
    if (Slot < CurSize) {
        OS << '\\' << unsigned(CurSize-Slot);     // Here's the upreference
        return;
    }

    TypeStack.push_back(Ty);    // Recursive case: Add us to the stack..

    switch (Ty->getTypeID()) {
    case Type::VoidTyID:      OS << "void"; break;
    case Type::FloatTyID:					break;
    case Type::DoubleTyID:					break;
    case Type::X86_FP80TyID:				break;
    case Type::FP128TyID:					break;
    case Type::PPC_FP128TyID:				break;
    case Type::LabelTyID:     OS << "label"; break;
    case Type::MetadataTyID:  OS << "metadata"; break;
    case Type::X86_MMXTyID:					break;
    case Type::IntegerTyID: {
        unsigned bitWidth=cast<IntegerType>(Ty)->getBitWidth();
        switch (bitWidth){
        case 1:			  OS << "bit";  break;
        case 8:			  OS << "int";  break;
        case 16:		  OS << "int";  break;
        case 32:		  OS << "int";  break;
        default:						break;
        }
        break;
    }
    case Type::FunctionTyID: {
        const FunctionType *FTy = cast<FunctionType>(Ty);
        CalcTypeName(FTy->getReturnType(), TypeStack, OS);
        OS << " (";
        for (FunctionType::param_iterator I = FTy->param_begin(),
                 E = FTy->param_end(); I != E; ++I) {
            if (I != FTy->param_begin())
                OS << ", ";
            CalcTypeName(*I, TypeStack, OS);
        }
        if (FTy->isVarArg()) {
            if (FTy->getNumParams()) OS << ", ";
            OS << "...";
        }
        OS << ')';
        break;
    }
    case Type::StructTyID: {
        const StructType *STy = cast<StructType>(Ty);
        OS << "TYPEDEF ";
        CalcTypeName(Ty,TypeStack,OS);
        OS << " {";
        int i=0;
        for (StructType::element_iterator I = STy->element_begin(),
                 E = STy->element_end(); I != E; ++I) {
            OS << ' ';
            CalcTypeName(*I, TypeStack, OS);
            OS << " u"<<i;
            i++;
            if (++I == STy->element_end())
                OS << ' ';
            else
                OS << ';';
        }
        OS << '}';
        break;
    }
    case Type::PointerTyID: {
        const PointerType *PTy = cast<PointerType>(Ty);
        CalcTypeName(PTy->getElementType(), TypeStack, OS);
        //  if (unsigned AddressSpace = PTy->getAddressSpace())
        //    OS << " addrspace(" << AddressSpace << ')';
        //  OS << '*';
        break;
    }
    case Type::ArrayTyID: {
        const ArrayType *ATy = cast<ArrayType>(Ty);
        CalcTypeName(ATy->getElementType(),TypeStack,OS);
        OS << '[' << ATy->getNumElements() << ']';
        break;
    }

    case Type::VectorTyID: {
        const VectorType *PTy = cast<VectorType>(Ty);
        OS << "<" << PTy->getNumElements() << " x ";
        CalcTypeName(PTy->getElementType(), TypeStack, OS);
        OS << '>';
        break;
    }
        // no longer a type
        // case Type::OpaqueTyID:
        // OS << "opaque";
        // break;
    default:
        OS << "<unrecognized-type>";
        break;
    }

    

    TypeStack.pop_back();       // Remove self from stack.
    */
}

/// printTypeInt - The internal guts of printing out a type that has a
/// potentially named portion.
///
void TypeGen::print(const Type *Ty, raw_ostream &OS,
                    bool IgnoreTopLevelName) {
    // Check to see if the type is named.
    DenseMap<const Type*, std::string> &TM = getTypeNamesMap(TypeNames);
    if (!IgnoreTopLevelName) {
        DenseMap<const Type*, std::string>::iterator I = TM.find(Ty);
        // if the type name (Ty) is in the map from TypeNames
        if (I != TM.end()) {
            OS << I->second; // this returns the type from the TM
            return;
        }
    }

    // Otherwise we have a type that has not been named but is a derived type.
    // Carefully recurse the type hierarchy to print out any contained symbolic
    // names.
    SmallVector<const Type *, 16> TypeStack;
    std::string TypeName;

    raw_string_ostream TypeOS(TypeName);
    CalcTypeName(Ty, TypeStack, TypeOS, IgnoreTopLevelName);
    /*
      OS << TypeOS.str();

      // Cache type name for later use.
      if (!IgnoreTopLevelName)
      TM.insert(std::make_pair(Ty, TypeOS.str()));
    */
    return;
}

void TypeGen::gen(std::vector<const Type*> numberedTypes,const ValueSymbolTable &ST,raw_ostream &OS){
    const Type *type;

    // not sure what the difference between numbered and named types is
    // why are types from the symbol table named??

    //Emit all numbered types.
    for (int NI=0,NE=numberedTypes.size();NI!=NE;++NI){
        type=numberedTypes[NI];
        this->printAtLeastOneLevel(type,OS);
        OS<<'\n';
    }

    //Print the named types.
    for (ValueSymbolTable::const_iterator VI=ST.begin(),VE=ST.end();VI!=VE;++VI){
        this->printAtLeastOneLevel(VI->second->getType(),OS);
     	OS<<'\n';
    }
}
