#include "Helper.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/raw_ostream.h"
#include <string>

// #include "SlotTracker.h"
// #include "TypeGen.h"
// #include "TypeFinder.h"
// #include "Define.h"
// #include "llvm/IR/DerivedTypes.h"
// #include "llvm/IR/InlineAsm.h"
// #include "llvm/IR/Operator.h"
// #include "llvm/ADT/StringExtras.h"
// #include "llvm/Support/raw_ostream.h"
// #include "llvm/ADT/StringRef.h"
// #include "llvm/IR/Metadata.h"
// #include "llvm/IR/Instructions.h"
// #include "llvm/ADT/APFloat.h"
// #include "llvm/IR/Constants.h"
// #include "llvm/IR/Constant.h"
//OLD#include "llvm/Assembly/Writer.h"
//OLD#include "llvm/Module.h"
//OLD#include "llvm/IntrinsicInst.h"
//OLD#include "llvm/ADT/SmallString.h"
//OLD#include "llvm/ADT/DenseSet.h"

using namespace llvm;

/*
// PrintEscapedString - Print each character of the specified string, escaping
// it if it is not printable or if it is an escape char.
void Helper::PrintEscapedString(StringRef Name, raw_ostream &Out) {
  for (unsigned i = 0, e = Name.size(); i != e; ++i) {
    unsigned char C = Name[i];
    if (isprint(C) && C != '\\' && C != '"')
      Out << C;
    else
      Out << '\\' << hexdigit(C >> 4) << hexdigit(C & 0x0F);
  }
}

void Helper::test(raw_ostream &OS,StringRef Name){
	OS<<Name.str();
}
*/
void Helper::PrintLLVMName(raw_ostream &OS, StringRef Name, PrefixType Prefix) {
	assert(Name.data() && "Cannot get empty name!");
	switch (Prefix) {
		default: llvm_unreachable("Bad prefix!");
		case NoPrefix: break;
		case GlobalPrefix: OS << '_'; break;
		case LabelPrefix:  break;
		case LocalPrefix:  break;
	}

	if (Name[0]>='0' && Name[0]<='9') OS << 'v';
	for (unsigned i=0,e=Name.size();i!=e;++i){
		char C=Name[i];
		if (!(C>='0' && C<='9') && !(C>='A' && C<='Z') && !(C>='a' && C<='z')){
			OS<<'_';
		}else OS<<C;
	}
	// Scan the name to see if it needs quotes first.
	bool NeedsQuotes = isdigit(Name[0]);
	if (!NeedsQuotes) {
		for (unsigned i = 0, e = Name.size(); i != e; ++i) {
			char C = Name[i];
			if (!isalnum(C) && C != '-' && C != '.' && C != '_') {
				NeedsQuotes = true;
				break;
			}
		}
	}

    // below was commented out by original developer
	// If we didn't need any quotes, just write out the name in one blast.
    //	if (!NeedsQuotes) {
    //		OS << Name;
    //		return;
    //	}

	// Okay, we need quotes.  Output the quotes and escape any scary characters as
	// needed.
    //	OS << '"';
    //	PrintEscapedString(Name, OS);
    //	OS << '"';

}

/*

void Helper::PrintLLVMName(raw_ostream &OS, const GlobalVariable &GV) {
	PrintLLVMName(OS, GV.getName(),
                isa<GlobalValue>(GV) ? GlobalPrefix : LocalPrefix);
}


const Module *Helper::getModuleFromVal(const Value *V) {
  if (const Argument *MA = dyn_cast<Argument>(V))
    return MA->getParent() ? MA->getParent()->getParent() : 0;

  if (const BasicBlock *BB = dyn_cast<BasicBlock>(V))
    return BB->getParent() ? BB->getParent()->getParent() : 0;

  if (const Instruction *I = dyn_cast<Instruction>(V)) {
    const Function *M = I->getParent() ? I->getParent()->getParent() : 0;
    return M ? M->getParent() : 0;
  }
  
  if (const GlobalValue *GV = dyn_cast<GlobalValue>(V))
    return GV->getParent();
  return 0;
}



SlotTracker *Helper::createSlotTracker(const Value *V) {
  if (const Argument *FA = dyn_cast<Argument>(V))
    return new SlotTracker(FA->getParent());

  if (const Instruction *I = dyn_cast<Instruction>(V))
    return new SlotTracker(I->getParent()->getParent());

  if (const BasicBlock *BB = dyn_cast<BasicBlock>(V))
    return new SlotTracker(BB->getParent());

  if (const GlobalVariable *GV = dyn_cast<GlobalVariable>(V))
    return new SlotTracker(*GV->getParent());

  if (const GlobalAlias *GA = dyn_cast<GlobalAlias>(V))
    return new SlotTracker(*GA->getParent());

  if (const Function *Func = dyn_cast<Function>(V))
    return new SlotTracker(Func);

  // MetaData is no longer a subclass of Value
  // (see line 452 of llvm/Metadata.h)
  // if (const MDNode *MD = dyn_cast<MDNode>(V)) {
  //   if (!MD->isFunctionLocal())
  //     return new SlotTracker(MD->getFunction());

  //  return new SlotTracker((Function *)0);
  // }

  return 0;
}

const char *Helper::getPredicateText(unsigned predicate) {
  const char * pred = "unknown";
  switch (predicate) {
  case FCmpInst::FCMP_FALSE: pred = "false"; break;
  case FCmpInst::FCMP_OEQ:   pred = "=="; break;
  case FCmpInst::FCMP_OGT:   pred = ">"; break;
  case FCmpInst::FCMP_OGE:   pred = ">="; break;
  case FCmpInst::FCMP_OLT:   pred = "<"; break;
  case FCmpInst::FCMP_OLE:   pred = "<="; break;
  case FCmpInst::FCMP_ONE:   pred = "!="; break;
  case FCmpInst::FCMP_ORD:   pred = "ord"; break;
  case FCmpInst::FCMP_UNO:   pred = "uno"; break;
  case FCmpInst::FCMP_UEQ:   pred = "=="; break;
  case FCmpInst::FCMP_UGT:   pred = ">"; break;
  case FCmpInst::FCMP_UGE:   pred = ">="; break;
  case FCmpInst::FCMP_ULT:   pred = "<"; break;
  case FCmpInst::FCMP_ULE:   pred = "<="; break;
  case FCmpInst::FCMP_UNE:   pred = "!="; break;
  case FCmpInst::FCMP_TRUE:  pred = "true"; break;
  case ICmpInst::ICMP_EQ:    pred = "=="; break;
  case ICmpInst::ICMP_NE:    pred = "!="; break;
  case ICmpInst::ICMP_SGT:   pred = ">"; break;
  case ICmpInst::ICMP_SGE:   pred = ">="; break;
  case ICmpInst::ICMP_SLT:   pred = "<"; break;
  case ICmpInst::ICMP_SLE:   pred = "<="; break;
  case ICmpInst::ICMP_UGT:   pred = ">"; break;
  case ICmpInst::ICMP_UGE:   pred = ">="; break;
  case ICmpInst::ICMP_ULT:   pred = "<"; break;
  case ICmpInst::ICMP_ULE:   pred = "<="; break;
  }
  return pred;
}


void Helper::WriteOptimizationInfo(raw_ostream &Out, const User *U) {
  if (const OverflowingBinaryOperator *OBO =
        dyn_cast<OverflowingBinaryOperator>(U)) {
    if (OBO->hasNoUnsignedWrap())
      Out << " nuw";
    if (OBO->hasNoSignedWrap())
      Out << " nsw";
  } else if (const PossiblyExactOperator *Div =
               dyn_cast<PossiblyExactOperator>(U)) {
    if (Div->isExact())
      Out << " exact";
  } else if (const GEPOperator *GEP = dyn_cast<GEPOperator>(U)) {
    if (GEP->isInBounds())
      Out << " inbounds";
  }
}



void Helper::WriteConstantInternal(raw_ostream &Out, const Constant *CV,
                                  TypeGen &TypePrinter,
                                  SlotTracker *Machine,
                                  const Module *Context) {
    return;
}
    
    if (const ConstantInt *CI = dyn_cast<ConstantInt>(CV)) {
    if (CI->getType()->isIntegerTy(1)) {
      Out << (CI->getZExtValue() ? "true" : "false");
      return;
    }
    Out << CI->getValue();
    return;
  }

  if (const ConstantFP *CFP = dyn_cast<ConstantFP>(CV)) {
    if (&CFP->getValueAPF().getSemantics() == &APFloat::IEEEdouble ||
        &CFP->getValueAPF().getSemantics() == &APFloat::IEEEsingle) {
      // We would like to output the FP constant value in exponential notation,
      // but we cannot do this if doing so will lose precision.  Check here to
      // make sure that we only output it in exponential format if we can parse
      // the value back and get the same value.
      //
      bool ignored;
      bool isDouble = &CFP->getValueAPF().getSemantics()==&APFloat::IEEEdouble;
      double Val = isDouble ? CFP->getValueAPF().convertToDouble() :
                              CFP->getValueAPF().convertToFloat();
      SmallString<128> StrVal;
      raw_svector_ostream(StrVal) << Val;

      // Check to make sure that the stringized number is not some string like
      // "Inf" or NaN, that atof will accept, but the lexer will not.  Check
      // that the string matches the "[-+]?[0-9]" regex.
      //
      if ((StrVal[0] >= '0' && StrVal[0] <= '9') ||
          ((StrVal[0] == '-' || StrVal[0] == '+') &&
           (StrVal[1] >= '0' && StrVal[1] <= '9'))) {
        // Reparse stringized version!
        if (atof(StrVal.c_str()) == Val) {
          Out << StrVal.str();
          return;
        }
      }
      // Otherwise we could not reparse it to exactly the same value, so we must
      // output the string in hexadecimal format!  Note that loading and storing
      // floating point types changes the bits of NaNs on some hosts, notably
      // x86, so we must not use these types.
      assert(sizeof(double) == sizeof(uint64_t) &&
             "assuming that double is 64 bits!");
      char Buffer[40];
      APFloat apf = CFP->getValueAPF();
      // Floats are represented in ASCII IR as double, convert.
      if (!isDouble)
        apf.convert(APFloat::IEEEdouble, APFloat::rmNearestTiesToEven,
                          &ignored);
      Out << "0x" <<
              utohex_buffer(uint64_t(apf.bitcastToAPInt().getZExtValue()),
                            Buffer+40);
      return;
    }

    // Some form of long double.  These appear as a magic letter identifying
    // the type, then a fixed number of hex digits.
    Out << "0x";
    if (&CFP->getValueAPF().getSemantics() == &APFloat::x87DoubleExtended) {
      Out << 'K';
      // api needed to prevent premature destruction
      APInt api = CFP->getValueAPF().bitcastToAPInt();
      const uint64_t* p = api.getRawData();
      uint64_t word = p[1];
      int shiftcount=12;
      int width = api.getBitWidth();
      for (int j=0; j<width; j+=4, shiftcount-=4) {
        unsigned int nibble = (word>>shiftcount) & 15;
        if (nibble < 10)
          Out << (unsigned char)(nibble + '0');
        else
          Out << (unsigned char)(nibble - 10 + 'A');
        if (shiftcount == 0 && j+4 < width) {
          word = *p;
          shiftcount = 64;
          if (width-j-4 < 64)
            shiftcount = width-j-4;
        }
      }
      return;
    } else if (&CFP->getValueAPF().getSemantics() == &APFloat::IEEEquad)
      Out << 'L';
    else if (&CFP->getValueAPF().getSemantics() == &APFloat::PPCDoubleDouble)
      Out << 'M';
    else
      llvm_unreachable("Unsupported floating point type");
    // api needed to prevent premature destruction
    APInt api = CFP->getValueAPF().bitcastToAPInt();
    const uint64_t* p = api.getRawData();
    uint64_t word = *p;
    int shiftcount=60;
    int width = api.getBitWidth();
    for (int j=0; j<width; j+=4, shiftcount-=4) {
      unsigned int nibble = (word>>shiftcount) & 15;
      if (nibble < 10)
        Out << (unsigned char)(nibble + '0');
      else
        Out << (unsigned char)(nibble - 10 + 'A');
      if (shiftcount == 0 && j+4 < width) {
        word = *(++p);
        shiftcount = 64;
        if (width-j-4 < 64)
          shiftcount = width-j-4;
      }
    }
    return;
  }

  if (isa<ConstantAggregateZero>(CV)) {
    Out << "zeroinitializer";
    return;
  }
  
  if (const BlockAddress *BA = dyn_cast<BlockAddress>(CV)) {
    Out << "blockaddress(";
    WriteAsOperandInternal(Out, BA->getFunction(), &TypePrinter, Machine,
                           Context);
    Out << ", ";
    WriteAsOperandInternal(Out, BA->getBasicBlock(), &TypePrinter, Machine,
                           Context);
    Out << ")";
    return;
  }

  if (const ConstantArray *CA = dyn_cast<ConstantArray>(CV)) {
    // As a special case, print the array as a string if it is an array of
    // i8 with ConstantInt values.
    //
    const Type *ETy = CA->getType()->getElementType();
    if (CA->isString()) {
      Out << "c\"";
      PrintEscapedString(CA->getAsString(), Out);
      Out << '"';
    } else {                // Cannot output in string format...
      Out << '[';
      if (CA->getNumOperands()) {
        TypePrinter.print(ETy, Out);
        Out << ' ';
        WriteAsOperandInternal(Out, CA->getOperand(0),
                               &TypePrinter, Machine,
                               Context);
        for (unsigned i = 1, e = CA->getNumOperands(); i != e; ++i) {
          Out << ", ";
          TypePrinter.print(ETy, Out);
          Out << ' ';
          WriteAsOperandInternal(Out, CA->getOperand(i), &TypePrinter, Machine,
                                 Context);
        }
      }
      Out << ']';
    }
    return;
  }

  if (const ConstantStruct *CS = dyn_cast<ConstantStruct>(CV)) {
    if (CS->getType()->isPacked())
      Out << '<';
    Out << '{';
    unsigned N = CS->getNumOperands();
    if (N) {
      Out << ' ';
      TypePrinter.print(CS->getOperand(0)->getType(), Out);
      Out << ' ';

      WriteAsOperandInternal(Out, CS->getOperand(0), &TypePrinter, Machine,
                             Context);

      for (unsigned i = 1; i < N; i++) {
        Out << ", ";
        TypePrinter.print(CS->getOperand(i)->getType(), Out);
        Out << ' ';

        WriteAsOperandInternal(Out, CS->getOperand(i), &TypePrinter, Machine,
                               Context);
      }
      Out << ' ';
    }

    Out << '}';
    if (CS->getType()->isPacked())
      Out << '>';
    return;
  }

  if (const ConstantVector *CP = dyn_cast<ConstantVector>(CV)) {
    const Type *ETy = CP->getType()->getElementType();
    assert(CP->getNumOperands() > 0 &&
           "Number of operands for a PackedConst must be > 0");
    Out << '<';
    TypePrinter.print(ETy, Out);
    Out << ' ';
    WriteAsOperandInternal(Out, CP->getOperand(0), &TypePrinter, Machine,
                           Context);
    for (unsigned i = 1, e = CP->getNumOperands(); i != e; ++i) {
      Out << ", ";
      TypePrinter.print(ETy, Out);
      Out << ' ';
      WriteAsOperandInternal(Out, CP->getOperand(i), &TypePrinter, Machine,
                             Context);
    }
    Out << '>';
    return;
  }

  if (isa<ConstantPointerNull>(CV)) {
    Out << "null";
    return;
  }

  if (isa<UndefValue>(CV)) {
    Out << "undef";
    return;
  }

  if (const ConstantExpr *CE = dyn_cast<ConstantExpr>(CV)) {
    //Out << CE->getOpcodeName();
	Out << ' ';
    //WriteOptimizationInfo(Out, CE);
    //if (CE->isCompare())
    //  Out << ' ' << getPredicateText(CE->getPredicate());
	switch (CE->getOpcode()){
		case GetElementPtr:
			ConStr conStr;
			User::const_op_iterator OI=CE->op_begin();
			User::const_op_iterator OE=CE->op_end();
			bool isStruct=false;
			if (conStr.isExist(dyn_cast<Value>(*OI)->getName())){
				Out<<'"';
				std::string name=conStr.getString(dyn_cast<Value>(*OI)->getName());
				Helper::PrintEscapedString(name,Out);
				Out<<'"'<<' ';
				return ;
			}
			if (cast<PointerType>((*OI)->getType())->getElementType()->getTypeID()==Type::StructTyID) isStruct=true;
			WriteAsOperandInternal(Out,*OI,&TypePrinter,Machine,Context);
			const Type *type=cast<PointerType>((*OI)->getType())->getElementType();
			OI++;
			if (isStruct){
				if (dyn_cast<ConstantInt>(*OI)->getZExtValue()!=0){
					Out <<'[';
					WriteAsOperandInternal(Out,*OI,&TypePrinter,Machine,Context);
					Out <<']';
				}
			}else{
//				Out<<'[';
//				WriteAsOperandInternal(Out,*OI,&TypePrinter,Machine,Context);
//				Out<<']';
			}
			if (OI==OE) return ;
			for (++OI;OI!=OE;++OI){
				if (isStruct){
					const StructType *STY=cast<StructType>(type);
					unsigned num=dyn_cast<ConstantInt>(*OI)->getZExtValue();
					type=STY->getElementType(num);
					if (type->getTypeID()!=Type::StructTyID) isStruct=false;
					Out<<".u"<<num;				
				}else{
					const ArrayType *ATY = cast<ArrayType>(type);
					type=ATY->getElementType();
					if (type->getTypeID()==Type::StructTyID) isStruct=true;
					Out<<'[';
					WriteAsOperandInternal(Out,*OI,&TypePrinter,Machine,Context);
					Out<<']';
				}
			}

			return;
	}
	Out << CE->getOpcodeName();
    Out << " (";

    for (User::const_op_iterator OI=CE->op_begin(); OI != CE->op_end(); ++OI) {
      TypePrinter.print((*OI)->getType(), Out);
      Out << ' ';
      WriteAsOperandInternal(Out, *OI, &TypePrinter, Machine, Context);
      if (OI+1 != CE->op_end())
        Out << ", ";
    }

    if (CE->hasIndices()) {
      const SmallVector<unsigned, 4> &Indices = CE->getIndices();
      for (unsigned i = 0, e = Indices.size(); i != e; ++i)
        Out << ", " << Indices[i];
    }

    if (CE->isCast()) {
      Out << " to ";
      TypePrinter.print(CE->getType(), Out);
    }

    Out << ')';
    return;
  }

  Out << "<placeholder or erroneous Constant>";
}


void Helper::WriteMDNodeBodyInternal(raw_ostream &Out, const MDNode *Node,
                                    TypeGen *TypePrinter,
                                    SlotTracker *Machine,
                                    const Module *Context) {
  Out << "!{";
  for (unsigned mi = 0, me = Node->getNumOperands(); mi != me; ++mi) {
    const Value *V = Node->getOperand(mi);
    if (V == 0)
      Out << "null";
    else {
      TypePrinter->print(V->getType(), Out);
      Out << ' ';
      WriteAsOperandInternal(Out, Node->getOperand(mi), 
                             TypePrinter, Machine, Context);
    }
    if (mi + 1 != me)
      Out << ", ";
  }
  
  Out << "}";
}

    

/// WriteAsOperand - Write the name of the specified value out to the specified
/// ostream.  This can be useful when you just want to print int %reg126, not
/// the whole instruction that generated it.

/// this used to take in a Value instead of a Global Variable 
void Helper::WriteAsOperandInternal(raw_ostream &Out, const GlobalVariable &GV,
                                   TypeGen *TypePrinter,
                                   SlotTracker *Machine,
                                   const Module Context) {

  if (GV.hasName()) {
    PrintLLVMName(Out, GV);
    return;
  }
  
}

  const Constant CV = dyn_cast<Constant>(*GV);
  // was getting errors about not being ab;e to convert CV to a bool
  // if (CV && !isa<GlobalValue>(CV)) {
  if (!isa<GlobalValue>(CV)) {
    assert(TypePrinter && "Constants require TypeGen!");
    WriteConstantInternal(Out, &CV, *TypePrinter, &Machine, *Context);
    return;
  }

  if (const InlineAsm IA = dyn_cast<InlineAsm>(*GV)) {
    Out << "asm ";
    if (IA.hasSideEffects())
      Out << "sideeffect ";
    if (IA.isAlignStack())
      Out << "alignstack ";
    Out << '"';
    PrintEscapedString(IA.getAsmString(), Out);
    Out << "\", \"";
    PrintEscapedString(IA.getConstraintString(), Out);
    Out << '"';
    return;
  }
  // error: can't convert MDNode to bool
  // if (const MDNode N = dyn_cast<MDNode>(*GV)) {
  const MDNode N = dyn_cast<MDNode>(*GV)
    // MDNode no longer has as isFunctionLocal() method 
    // if (N->isFunctionLocal()) {
      // Print metadata inline, not via slot reference number.
      //   WriteMDNodeBodyInternal(Out, N, TypePrinter, Machine, Context);
      // return;
      // }
  
      // if (!Machine) {
        // if (N->isFunctionLocal())
        //Machine = new SlotTracker(N->getFunction());
        // else
    Machine = new SlotTracker(*Context);
        // }
    int Slot = Machine.getMetadataSlot(N);
    if (Slot == -1)
      Out << "<badref>";
    else
      Out << '!' << Slot;
    return;
    // }

  if (const MDString MDS = dyn_cast<MDString>(*GV)) {
    Out << "!\"";
    PrintEscapedString(MDS.getString(), Out);
    Out << '"';
    return;
  }

  // these types no longer exist
  // if (GV.getValueID() == Value::PseudoSourceValueVal ||
  // GV.getValueID() == Value::FixedStackPseudoSourceValueVal) {
  // GV.print(Out);
  // return;
  // }

  char Prefix = 'v';
  int Slot;
  if (Machine) {
    if (const GlobalValue GGV = dyn_cast<GlobalValue>(*GV)) {
      Slot = Machine->getGlobalSlot(GGV);
      Prefix = '_';
    } else {
      Slot = Machine->getLocalSlot(&GV);
    }
  } else {
    Machine = createSlotTracker(&GV);
    if (Machine) {
      if (const GlobalValue GGV = dyn_cast<GlobalValue>(*GV)) {
        Slot = Machine->getGlobalSlot(GGV);
        Prefix = '@';
      } else {
        Slot = Machine->getLocalSlot(&GV);
      }
      delete Machine;
    } else {
      Slot = -1;
    }
  }

  if (Slot != -1)
    Out << Prefix << Slot;
  else
    Out << "<badref>";
}


void Helper::WriteAsOperand(raw_ostream &Out, const Value *V,
                          bool PrintType, const Module *Context) {

  // Fast path: Don't construct and populate a TypeGen object if we
  // won't be needing any types printed.
  if (!PrintType &&
      ((!isa<Constant>(V) && !isa<MDNode>(V)) ||
       V->hasName() || isa<GlobalValue>(V))) {
    WriteAsOperandInternal(Out, V, 0, 0, Context);
    return;
  }

  if (Context == 0) Context = getModuleFromVal(V);

  TypeGen TypePrinter;
  std::vector<const Type*> NumberedTypes;
  TypeFinder typeFinder(TypePrinter,NumberedTypes);
  typeFinder.Run(*Context);
  if (PrintType) {
    TypePrinter.print(V->getType(), Out);
    Out << ' ';
  }

  WriteAsOperandInternal(Out, V, &TypePrinter, 0, Context);
}

void Helper::InitBE(raw_ostream &Out,bool BorE){
	if (BorE){
		Out<<"init {\nchan _syn = [0] of { int };\n";
	}else Out << "  run _main(_syn);\n}\n";
	return ;
}

void Helper::InitGValue(raw_ostream &Out,const GlobalVariable *GV,TypeGen *TypePrinter,
		SlotTracker *Machine,const Module *Context){

	const Value *ini=GV->getInitializer();
	const Constant *CV=dyn_cast<Constant>(ini);
	std::string name=GV->getName();
	name="_"+name;

	if (const ConstantInt *CI=dyn_cast<ConstantInt>(CV)){
		Out<<"  "<<name<<" = "<<CI->getValue();
		Out << ";\n";
	}else if (const ConstantArray *CA=dyn_cast<ConstantArray>(CV)){
		for (unsigned i=0,e=CA->getNumOperands();i!=e;++i){
			const ConstantInt *T=dyn_cast<ConstantInt>(CA->getOperand(i));
			if (T){
				Out <<"  "<<name<<'['<<i<<']'<<" = "<<T->getValue();
				Out << ";\n";
			}
		}
	}else if (const ConstantStruct *CS=dyn_cast<ConstantStruct>(CV)){
		for (unsigned i=0,e=CS->getNumOperands();i!=e;++i){
			const ConstantInt *T=dyn_cast<ConstantInt>(CS->getOperand(i));
			if (T){
				Out <<"  "<<name<<".u"<<i<<" = "<<T->getValue();
				Out << ";\n";
			}
		}
	}
}

void Helper::Formatting(std::string &s){
	int i,st,ed;
	std::string tmp=s;
	bool flag=true;
	if (s.size()==0) return ;
	for (i=0;i<s.size();i++){
		if (s[i]=='[') { st=i;flag=false; }
		if (s[i]==']') { ed=i; }
	}
	if (flag) return ;
	int j=0;
	for (i=0;i<st;i++)	s[j++]=tmp[i];
	for (i=ed+1;i<s.size()-2;i++) s[j++]=tmp[i];
	for (i=st;i<=ed;i++) s[j++]=tmp[i];
	return ;
}


ConStr *ConStr::pConStr=new ConStr();
bool ConStr::isConStr(const GlobalVariable &V){
	if (V.hasInitializer()){
        // the ConstantArray type no longer had the necessary functions
		//const ConstantArray *CA=dyn_cast<ConstantArray>(V.getInitializer());
        const ConstantDataSequential *CDS=dyn_cast<ConstantDataSequential>(V.getInitializer());
		if (CDS && CDS->isString()){
			get()->conStr.insert(std::pair<StringRef,std::string>(V.getName(),CDS->getAsString()));
			return true;
		}
	}
	return false;
}


bool ConStr::isExist(const StringRef name){
	if (get()->conStr.find(name)!=get()->conStr.end()) return true;
	else return false;
}

std::string ConStr::getString(const StringRef name){
	return get()->conStr.find(name)->second;
}

ConStr *ConStr::get(){
	return ConStr::pConStr;
}
*/

