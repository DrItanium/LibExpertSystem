#ifndef _constant_construction_h
#define _constant_construction_h
#include "ConstructionTools.h"
using namespace llvm;
class CLIPSConstantBuilder : public CLIPSUserBuilder {
   public:
      CLIPSConstantBuilder(std::string nm, std::string ty, FunctionNamer& namer) : CLIPSUserBuilder(nm, ty, namer) { }
      void setFields(Constant* cnst, char* parent) {
         CLIPSUserBuilder::setFields((User*)cnst, parent);
         if(cnst->isNullValue()) setFieldTrue("IsNullValue"); 
         if(cnst->isAllOnesValue()) setFieldTrue("IsAllOnesValue"); 
         if(cnst->isNegativeZeroValue()) setFieldTrue("IsNegativeZeroValue"); 
         if(cnst->canTrap()) setFieldTrue("CanTrap"); 
         if(cnst->isConstantUsed()) setFieldTrue("IsConstantUsed"); 
      }
};

class CLIPSBlockAddressBuilder : public CLIPSConstantBuilder {
   public:
      CLIPSBlockAddressBuilder(std::string nm, FunctionNamer& namer) : CLIPSConstantBuilder(nm, "BlockAddress", namer) { }
      void setFields(BlockAddress* addr, char* parent) {
         CLIPSConstantBuilder::setFields((Constant*)addr, parent); 
         setField("Target", addr->getBasicBlock()->getName());
         setField("Function", addr->getFunction()->getName());
      }
};
class CLIPSConstantAggregateZeroBuilder : public CLIPSConstantBuilder {
   public:
      CLIPSConstantAggregateZeroBuilder(std::string nm, FunctionNamer& namer) : CLIPSConstantBuilder(nm, "ConstantAggregateZero", namer) { }
      void setFields(ConstantAggregateZero* addr, char* parent) {
         CLIPSConstantBuilder::setFields((Constant*)addr, parent); 
      }
};
class CLIPSConstantArrayBuilder : public CLIPSConstantBuilder {
   public:
      CLIPSConstantArrayBuilder(std::string nm, FunctionNamer& namer) : CLIPSConstantBuilder(nm, "ConstantArray", namer) { }
      void setFields(ConstantArray* addr, char* parent) {
         CLIPSConstantBuilder::setFields((Constant*)addr, parent); 
      }
};
/* ConstantDataSequential doesn't exist in llvm 3.0 :/
 * But I have a feeling it is in newer versions of LLVM so I'm keeping the code
 * here for those newer versions
 * - Joshua Scoggins 5/20/2012 @ 1726 PST
 class CLIPSConstantDataSequentialBuilder : public CLIPSConstantBuilder
 {

 public:
 CLIPSConstantDataSequentialBuilder(std::string nm, FunctionNamer& namer) : CLIPSConstantBuilder(nm, "ConstantDataSequential", namer) { }
 void setFields(ConstantDataSequential* addr, char* parent)
 {
 CLIPSConstantBuilder::setFields((Constant*)addr, parent); 
 setField("ElementByteSize", addr->getElementByteSize());
 setField("IsString", addr->isString());
 setField("IsCString", addr->isCString());
 setField("ElementCount", addr->getNumElements());
 }
 void imbueKnowledge(ConstantDataSequential* addr)
 {
 CLIPSConstantBuilder::imbueKnowledge((Constant*)addr);
 send("put-ElementType", Route(addr->getElementType()));
 }
 };
//TODO: Put in ConstantDataArray and ConstantDataVector if I upgrade to newer
//versions of LLVM
*/

class CLIPSConstantExpressionBuilder : public CLIPSConstantBuilder {
   public:
      CLIPSConstantExpressionBuilder(std::string nm, FunctionNamer& namer) : CLIPSConstantBuilder(nm, "ConstantExpression", namer) { }
      void setFields(ConstantExpr* addr, char* parent) {
         CLIPSConstantBuilder::setFields((Constant*)addr, parent); 
         if(addr->isCast()) setFieldTrue("IsCast");
         if(addr->isCompare()) setFieldTrue("IsCompare");
         if(addr->hasIndices()) setFieldTrue("HasIndices");
         if(addr->isGEPWithNoNotionalOverIndexing()) setFieldTrue("IsGEPWithNoNotionalOverIndexing");
         setField("Predicate", addr->getPredicate());
         setField("Operation", addr->getOpcodeName());
        // ArrayRef<unsigned> indices = addr->getIndices();
        // llvm::errs() << "Index Count: " << indices.size() << '\n';
        // for(ArrayRef<unsigned>::iterator start = 
        //       indices->begin(), end = indices->end();
        //       start != end; ++start) {
        //    
        // }
      }
};

/*
 * This does not exist in my version of LLVM so...yeah it's commented out for
 * the time being
 * -Joshua Scoggins 5/20/2012 @ 1746
 class CLIPSBinaryConstantExpressionBuilder : public CLIPSConstantExpressionBuilder
 {
 public:
 CLIPSBinaryConstantExpressionBuilder(std::string nm, FunctionNamer& namer) : CLIPSConstantExpressionBuilder(nm, "BinaryConstantExpression", namer) { }
 void setFields(BinaryConstantExpr* addr, char* parent)
 {
 CLIPSConstantBuilder::setFields((Constant*)addr, parent); 
 }
 void imbueKnowledge(BinaryConstantExpr* addr)
 {
 CLIPSConstantBuilder::imbueKnowledge((Constant*)addr);
 }
 };
 class CLIPSCompareConstantExpressionBuilder : public CLIPSConstantExpressionBuilder
 {
 public:
 CLIPSCompareConstantExpressionBuilder(std::string nm, FunctionNamer& namer) : CLIPSConstantExpressionBuilder(nm, "CompareConstantExpression", namer) { }
 void setFields(CompareConstantExpr* addr, char* parent)
 {
 CLIPSConstantBuilder::setFields((Constant*)addr, parent); 
 setField("ActualPredicate", addr->predicate);
 }
 void imbueKnowledge(CompareConstantExpr* addr)
 {
 CLIPSConstantBuilder::imbueKnowledge((Constant*)addr);
 }
 };
 */

class CLIPSConstantFloatingPointBuilder : public CLIPSConstantBuilder {
   public:
      CLIPSConstantFloatingPointBuilder(std::string nm, FunctionNamer& namer) : CLIPSConstantBuilder(nm, "ConstantFloatingPoint", namer) { }
      void setFields(ConstantFP* addr, char* parent) {
         CLIPSConstantBuilder::setFields((Constant*)addr, parent); 
         if(addr->isZero()) {
            setFieldTrue("IsZero");
            setField("Value", 0.0);
         } else {
            if(addr->isNegative()) setFieldTrue("IsNegative");
            if(addr->isNaN()) setFieldTrue("IsNaN");
            setField("Value", addr->getValueAPF().convertToDouble());
         }
         //While the APF is arbitrary precision it's not that big of a deal to 
         //lose precision for my purposes
      }
};

class CLIPSConstantIntegerBuilder : public CLIPSConstantBuilder {
   public:
      CLIPSConstantIntegerBuilder(std::string nm, FunctionNamer& namer) : CLIPSConstantBuilder(nm, "ConstantInteger", namer) { }
      void setFields(ConstantInt* addr, char* parent) {
         CLIPSConstantBuilder::setFields((Constant*)addr, parent); 
         setField("Width", addr->getBitWidth());
         if(addr->isZero()) {
            setFieldTrue("IsZero");
            setField("Value", 0);
         } else {
            if(addr->isNegative()) {
               setFieldTrue("IsNegative");
               if(addr->isMinusOne()) setFieldTrue("IsMinusOne");
               setField("Value", addr->getSExtValue());			
            } else {
               if(addr->isOne()) setFieldTrue("IsOne");
               setField("Value", addr->getLimitedValue());
            }
         }
      }
};

class CLIPSConstantPointerNullBuilder : public CLIPSConstantBuilder {
   public:
      CLIPSConstantPointerNullBuilder(std::string nm, FunctionNamer& namer) : CLIPSConstantBuilder(nm, "ConstantPointerNull", namer) { }
      void setFields(ConstantPointerNull* addr, char* parent) {
         CLIPSConstantBuilder::setFields((Constant*)addr, parent); 
      }
};

class CLIPSConstantStructBuilder : public CLIPSConstantBuilder {
   public:
      CLIPSConstantStructBuilder(std::string nm, FunctionNamer& namer) : CLIPSConstantBuilder(nm, "ConstantStruct", namer) { }
      void setFields(ConstantStruct* addr, char* parent) {
         CLIPSConstantBuilder::setFields((Constant*)addr, parent); 
      }
};

class CLIPSConstantVectorBuilder : public CLIPSConstantBuilder {
   public:
      CLIPSConstantVectorBuilder(std::string nm, FunctionNamer& namer) : CLIPSConstantBuilder(nm, "ConstantVector", namer) { }
      void setFields(ConstantVector* addr, char* parent) {
         CLIPSConstantBuilder::setFields((Constant*)addr, parent); 
         setField("SplatValue", Route(addr->getSplatValue(), getNamer()));
      }
};

class CLIPSGlobalValueBuilder : public CLIPSConstantBuilder
{
   public:
      CLIPSGlobalValueBuilder(std::string nm, std::string ty, FunctionNamer& namer) : CLIPSConstantBuilder(nm, ty, namer) { }
      void setFields(GlobalValue* addr, char* parent) {
         CLIPSConstantBuilder::setFields((Constant*)addr, parent); 
         //setField("Alignment", addr->getAlignment());
         //if(addr->hasUnnamedAddr()) setFieldTrue("HasUnnamedAddr"); 
         //if(addr->hasDefaultVisibility()) setFieldTrue("HasDefaultVisibility"); 
         //if(addr->hasHiddenVisibility()) setFieldTrue("HasHiddenVisibility"); 
         //if(addr->hasProtectedVisibility()) setFieldTrue("HasProtectedVisibility"); 
         //if(addr->hasSection()) {
         //   setFieldTrue("HasSection");
         //   setField("Section", addr->getSection());
         //}
         //if(addr->hasExternalLinkage()) setFieldTrue("HasExternalLinkage"); 
         //if(addr->hasAvailableExternallyLinkage()) setFieldTrue("HasAvailableExternallyLinkage"); 
         //if(addr->hasLinkOnceLinkage()) setFieldTrue("HasLinkOnceLinkage"); 
         //if(addr->hasWeakLinkage()) setFieldTrue("HasWeakLinkage");
         //if(addr->hasAppendingLinkage()) setFieldTrue("HasAppendingLinkage"); 
         //if(addr->hasInternalLinkage()) setFieldTrue("HasInternalLinkage"); 
         //if(addr->hasPrivateLinkage()) setFieldTrue("HasPrivateLinkage"); 
         //if(addr->hasLinkerPrivateLinkage()) setFieldTrue("HasLinkerPrivateLinkage"); 
         //if(addr->hasLinkerPrivateWeakLinkage()) setFieldTrue("HasLinkerPrivateWeakLinkage"); 
         //if(addr->hasLinkerPrivateWeakDefAutoLinkage()) setFieldTrue("HasLinkerPrivateWeakDefAutoLinkage"); 
         //if(addr->hasLocalLinkage()) setFieldTrue("HasLocalLinkage");
         //if(addr->hasDLLImportLinkage()) setFieldTrue("HasDLLImportLinkage");
         //if(addr->hasDLLExportLinkage()) setFieldTrue("HasDLLExportLinkage");
         //if(addr->hasExternalWeakLinkage()) setFieldTrue("HasExternalWeakLinkage");
         //if(addr->hasCommonLinkage()) setFieldTrue("HasCommonLinkage");
         //if(addr->mayBeOverridden()) setFieldTrue("MayBeOverridden");
         //if(addr->isWeakForLinker()) setFieldTrue("IsWeakForLinker");
      }
};


class CLIPSGlobalAliasBuilder : public CLIPSGlobalValueBuilder {
   public:
      CLIPSGlobalAliasBuilder(std::string nm, FunctionNamer& namer) 
         : CLIPSGlobalValueBuilder(nm, "GlobalAlias", namer) { }
      void setFields(GlobalAlias* addr, char* parent) {
         CLIPSGlobalValueBuilder::setFields((GlobalValue*)addr, parent); 
         setField("Aliasee", Route(addr->getAliasee(), getNamer()));
      }
};

class CLIPSGlobalVariableBuilder : public CLIPSGlobalValueBuilder {
   public:
      CLIPSGlobalVariableBuilder(std::string nm, FunctionNamer& namer) 
         : CLIPSGlobalValueBuilder(nm, "GlobalVariable", namer) { }

      void setFields(GlobalVariable* addr, char* parent) {
         CLIPSGlobalValueBuilder::setFields((GlobalValue*)addr, parent); 
         if(addr->hasInitializer()) {
            setFieldTrue("HasInitializer");
            setField("Initializer", Route(addr->getInitializer(), getNamer()));
         }
         if(addr->hasDefinitiveInitializer()) setFieldTrue("HasDefinitiveInitializer");
         if(addr->hasUniqueInitializer()) setFieldTrue("HasUniqueInitializer");
         if(addr->isConstant()) setFieldTrue("IsConstant"); 
         //if(addr->isThreadLocal()) setFieldTrue("IsThreadLocal");
      }
};

class CLIPSUndefValueBuilder : public CLIPSConstantBuilder {
   public:
      CLIPSUndefValueBuilder(std::string nm, FunctionNamer& namer) : 
         CLIPSConstantBuilder(nm, "UndefValue", namer) { }

      void setFields(UndefValue* addr, char* parent) {
         CLIPSConstantBuilder::setFields((Constant*)addr, parent); 
      }
};

#endif
