#ifndef PMG_FUNCTIONGEN_H
#define PMG_FUNCTIONGEN_H

#include "llvm/IR/Attributes.h"
#include <map>

namespace llvm {

class TypeGen;
class NewSlotTracker;
class Argument;
class BasicBlock;
class Instruction;
class Function;
class Value;
class Module;
class formatted_raw_ostream;
class StringRef;

class FunctionGen {
	TypeGen &TypeGener;
	NewSlotTracker &Machine;
	formatted_raw_ostream &Out;
	const Module *TheModule;
	std::map<StringRef,int> gos;
	int count;
	int retCount;
	bool flag;
public:
	FunctionGen(TypeGen &TG,NewSlotTracker &ST,formatted_raw_ostream &OS,const Module *M):TypeGener(TG),Machine(ST),Out(OS),TheModule(M){}
	~FunctionGen(){}
	void printFunction(const Function *F);
	void printArgument(const Argument *Arg,AttributeSet Attrs);
	void printBasicBlock(const BasicBlock *BB);
	void printInstruction(const Instruction &I);
private:
	void localValueDeclare(const Instruction &I);
	void writeOperand(const Value *Operand,bool PrintType);
	void writeParamOperand(const Value *Operand,AttributeSet Attrs);
};

}

#endif
