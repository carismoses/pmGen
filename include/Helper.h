#ifndef PMG_HELPER_H
#define PMG_HELPER_H

#include "TypeGen.h"
#include "SlotTracker.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/User.h"
#include "llvm/IR/Constant.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/StringRef.h"
#include <string>
#include <map>

namespace llvm {

    enum PrefixType {
        GlobalPrefix,
        LabelPrefix,
        LocalPrefix,
        NoPrefix
    };

    class NewSlotTracker;
    class Value;
    class raw_ostream;
    class TypeGen;
    class Module;
    class User;
    class Constant;
    class MDNode;
    class StringRef;
    class GlobalVariable;
    
    
    class ConStr {
        std::map<StringRef,std::string> conStr;

    public:
        ConStr(){conStr.empty();}	
        static ConStr *pConStr;
        bool isConStr(const GlobalVariable *V);
        bool isExist(const StringRef name);
        std::string getString(const StringRef name);
        ConStr *get();
    };
    
    class Helper {

    public:
        static void test(raw_ostream &OS,StringRef Name);

        static NewSlotTracker *createSlotTracker(const Value *V);

        static const Module *getModuleFromVal(const Value *V);
      
        /// PrintLLVMName - Turn the specified name into an 'LLVM name', which is either
        /// prefixed with % (if the string only contains simple characters) or is
        /// surrounded with ""'s (if it has special chars in it).  Print it out.
        static void PrintLLVMName(raw_ostream &OS, StringRef Name, PrefixType Prefix); 
        static void PrintLLVMName(raw_ostream &OS, const Value *V);

        static void PrintEscapedString(StringRef Name,raw_ostream &Out);

        static void WriteAsOperandInternal(
                                           raw_ostream &Out, const Value *V,
                                           TypeGen *TypePrinter,
                                           NewSlotTracker *Machine,
                                           const Module *Context);

        /// need seperate function for metadata since it is no longer
        /// a subclass of Value
        static void WriteAsOperandInternal(
                                    raw_ostream &Out, const Metadata *MD,
                                    TypeGen *TypePrinter,
                                    NewSlotTracker *Machine,
                                    const Module *Context,
                                    bool FromValue);
            
        static const char *getPredicateText(unsigned predicate);
	
        static void WriteOptimizationInfo(raw_ostream &Out, const User *U);	

        static void WriteConstantInternal(
                                          raw_ostream &Out,const Constant *CV,
                                          TypeGen &TypePrinter,
                                          NewSlotTracker *Machine,
                                          const Module *Context);

        static void WriteMDNodeBodyInternal(
                                            raw_ostream &Out,const MDNode *Node,
                                            TypeGen *TypePrinter,
                                            NewSlotTracker *Machine,
                                            const Module *Context);

        static void WriteAsOperand(
                                   raw_ostream &Out,const Value *V,
                                   bool PrintType,const Module *Context);

        /// initializes a PROMELA channel I think?
        ///
        static void InitBE(raw_ostream &Out,bool BorE);

        static void InitGValue(
                               raw_ostream &Out,const GlobalVariable *GV,
                               TypeGen *TypePrinter,
                               NewSlotTracker *Machine,
                               const Module *Context);
	
        static void Formatting(std::string &s);
    };
}

#endif
