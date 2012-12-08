#ifndef _constant_construction_h
#define _constant_construction_h
#include "ConstructionTools.h"
using namespace llvm;
class CLIPSConstantBuilder : public CLIPSUserBuilder {
   public:
      CLIPSConstantBuilder(std::string nm, std::string ty, FunctionNamer& namer) : CLIPSUserBuilder(nm, ty, namer) { }
      void addFields(Constant* cnst, KnowledgeConstruction &kc, char* parent) {
         CLIPSUserBuilder::addFields((User*)cnst, kc, parent);
         if(cnst->isNullValue()) addTrueField("IsNullValue"); 
         if(cnst->isAllOnesValue()) addTrueField("IsAllOnesValue"); 
         if(cnst->isNegativeZeroValue()) addTrueField("IsNegativeZeroValue"); 
         if(cnst->canTrap()) addTrueField("CanTrap"); 
         if(cnst->isConstantUsed()) addTrueField("IsConstantUsed"); 
      }
		void build(Constant* cnst, KnowledgeConstruction &kc, char* parent) {
			open();
			addFields(cnst, kc, parent);
			close();
			std::string &str = getCompletedString();
			kc.addToKnowledgeBase((PointerAddress)cnst, str);
		}
};

class CLIPSBlockAddressBuilder : public CLIPSConstantBuilder {
   public:
      CLIPSBlockAddressBuilder(std::string nm, FunctionNamer& namer) : CLIPSConstantBuilder(nm, "BlockAddress", namer) { }
      void addFields(BlockAddress* addr, KnowledgeConstruction &kc, char* parent) {
         CLIPSConstantBuilder::addFields((Constant*)addr, kc, parent); 
         addField("Target", addr->getBasicBlock()->getName());
         addField("Function", addr->getFunction()->getName());
      }
		void build(BlockAddress* cnst, KnowledgeConstruction &kc, char* parent) {
			open();
			addFields(cnst, kc, parent);
			close();
			std::string &str = getCompletedString();
			kc.addToKnowledgeBase((PointerAddress)cnst, str);
		}
};
class CLIPSConstantAggregateZeroBuilder : public CLIPSConstantBuilder {
   public:
      CLIPSConstantAggregateZeroBuilder(std::string nm, FunctionNamer& namer) : CLIPSConstantBuilder(nm, "ConstantAggregateZero", namer) { }
      void addFields(ConstantAggregateZero* addr, KnowledgeConstruction &kc, char* parent) {
         CLIPSConstantBuilder::addFields((Constant*)addr, kc, parent); 
      }
		void build(ConstantAggregateZero* cnst, KnowledgeConstruction &kc, char* parent) {
			open();
			addFields(cnst, kc, parent);
			close();
			std::string &str = getCompletedString();
			kc.addToKnowledgeBase((PointerAddress)cnst, str);
		}
};
class CLIPSConstantArrayBuilder : public CLIPSConstantBuilder {
   public:
      CLIPSConstantArrayBuilder(std::string nm, FunctionNamer& namer) : CLIPSConstantBuilder(nm, "ConstantArray", namer) { }
      void addFields(ConstantArray* addr, KnowledgeConstruction &kc, char* parent) {
         CLIPSConstantBuilder::addFields((Constant*)addr, kc, parent); 
      }
		void build(ConstantArray* cnst, KnowledgeConstruction &kc, char* parent) {
			open();
			addFields(cnst, kc, parent);
			close();
			std::string &str = getCompletedString();
			kc.addToKnowledgeBase((PointerAddress)cnst, str);
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
 void addFields(ConstantDataSequential* addr, char* parent)
 {
 CLIPSConstantBuilder::addFields((Constant*)addr, parent); 
 addField("ElementByteSize", addr->getElementByteSize());
 addField("IsString", addr->isString());
 addField("IsCString", addr->isCString());
 addField("ElementCount", addr->getNumElements());
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
      void addFields(ConstantExpr* addr, KnowledgeConstruction &kc, char* parent) {
         CLIPSConstantBuilder::addFields((Constant*)addr, kc, parent); 
         if(addr->isCast()) addTrueField("IsCast");
         if(addr->isCompare()) addTrueField("IsCompare");
         if(addr->hasIndices()) addTrueField("HasIndices");
         if(addr->isGEPWithNoNotionalOverIndexing()) addTrueField("IsGEPWithNoNotionalOverIndexing");
         addField("Predicate", addr->getPredicate());
         addField("Operation", addr->getOpcodeName());
        // ArrayRef<unsigned> indices = addr->getIndices();
        // llvm::errs() << "Index Count: " << indices.size() << '\n';
        // for(ArrayRef<unsigned>::iterator start = 
        //       indices->begin(), end = indices->end();
        //       start != end; ++start) {
        //    
        // }
      }
		void build(ConstantExpr* cnst, KnowledgeConstruction &kc, char* parent) {
			open();
			addFields(cnst, kc, parent);
			close();
			std::string &str = getCompletedString();
			kc.addToKnowledgeBase((PointerAddress)cnst, str);
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
 void addFields(BinaryConstantExpr* addr, char* parent)
 {
 CLIPSConstantBuilder::addFields((Constant*)addr, parent); 
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
 void addFields(CompareConstantExpr* addr, char* parent)
 {
 CLIPSConstantBuilder::addFields((Constant*)addr, parent); 
 addField("ActualPredicate", addr->predicate);
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
      void addFields(ConstantFP* addr, KnowledgeConstruction &kc, char* parent) {
         CLIPSConstantBuilder::addFields((Constant*)addr, kc, parent); 
         if(addr->isZero()) {
            addTrueField("IsZero");
            addField("Value", 0.0);
         } else {
            if(addr->isNegative()) addTrueField("IsNegative");
            if(addr->isNaN()) addTrueField("IsNaN");
            addField("Value", addr->getValueAPF().convertToDouble());
         }
         //While the APF is arbitrary precision it's not that big of a deal to 
         //lose precision for my purposes
      }
		void build(ConstantFP* cnst, KnowledgeConstruction &kc, char* parent) {
			open();
			addFields(cnst, kc, parent);
			close();
			std::string &str = getCompletedString();
			kc.addToKnowledgeBase((PointerAddress)cnst, str);
		}
};

class CLIPSConstantIntegerBuilder : public CLIPSConstantBuilder {
   public:
      CLIPSConstantIntegerBuilder(std::string nm, FunctionNamer& namer) : CLIPSConstantBuilder(nm, "ConstantInteger", namer) { }
      void addFields(ConstantInt* addr, KnowledgeConstruction &kc, char* parent) {
         CLIPSConstantBuilder::addFields((Constant*)addr, kc, parent); 
         addField("Width", addr->getBitWidth());
         if(addr->isZero()) {
            addTrueField("IsZero");
            addField("Value", 0);
         } else {
            if(addr->isNegative()) {
               addTrueField("IsNegative");
               if(addr->isMinusOne()) addTrueField("IsMinusOne");
               addField("Value", addr->getSExtValue());			
            } else {
               if(addr->isOne()) addTrueField("IsOne");
               addField("Value", addr->getLimitedValue());
            }
         }
      }
		void build(ConstantInt* cnst, KnowledgeConstruction &kc, char* parent) {
			open();
			addFields(cnst, kc, parent);
			close();
			std::string &str = getCompletedString();
			kc.addToKnowledgeBase((PointerAddress)cnst, str);
		}
};

class CLIPSConstantPointerNullBuilder : public CLIPSConstantBuilder {
   public:
      CLIPSConstantPointerNullBuilder(std::string nm, FunctionNamer& namer) : CLIPSConstantBuilder(nm, "ConstantPointerNull", namer) { }
      void addFields(ConstantPointerNull* addr, KnowledgeConstruction &kc, char* parent) {
         CLIPSConstantBuilder::addFields((Constant*)addr, kc, parent); 
      }
		void build(ConstantPointerNull* cnst, KnowledgeConstruction &kc, char* parent) {
			open();
			addFields(cnst, kc, parent);
			close();
			std::string &str = getCompletedString();
			kc.addToKnowledgeBase((PointerAddress)cnst, str);
		}
};

class CLIPSConstantStructBuilder : public CLIPSConstantBuilder {
   public:
      CLIPSConstantStructBuilder(std::string nm, FunctionNamer& namer) : CLIPSConstantBuilder(nm, "ConstantStruct", namer) { }
      void addFields(ConstantStruct* addr, KnowledgeConstruction &kc, char* parent) {
         CLIPSConstantBuilder::addFields((Constant*)addr, kc, parent); 
      }
		void build(ConstantStruct* cnst, KnowledgeConstruction &kc, char* parent) {
			open();
			addFields(cnst, kc, parent);
			close();
			std::string &str = getCompletedString();
			kc.addToKnowledgeBase((PointerAddress)cnst, str);
		}
};

class CLIPSConstantVectorBuilder : public CLIPSConstantBuilder {
   public:
      CLIPSConstantVectorBuilder(std::string nm, FunctionNamer& namer) : CLIPSConstantBuilder(nm, "ConstantVector", namer) { }
      void addFields(ConstantVector* addr, KnowledgeConstruction &kc, char* parent) {
         CLIPSConstantBuilder::addFields((Constant*)addr, kc, parent); 
         addField("SplatValue", kc.route(addr->getSplatValue(), getNamer()));
      }
		void build(ConstantVector* cnst, KnowledgeConstruction &kc, char* parent) {
			open();
			addFields(cnst, kc, parent);
			close();
			std::string &str = getCompletedString();
			kc.addToKnowledgeBase((PointerAddress)cnst, str);
		}
};

class CLIPSGlobalValueBuilder : public CLIPSConstantBuilder
{
   public:
      CLIPSGlobalValueBuilder(std::string nm, std::string ty, FunctionNamer& namer) : CLIPSConstantBuilder(nm, ty, namer) { }
      void addFields(GlobalValue* addr, KnowledgeConstruction &kc, char* parent) {
         CLIPSConstantBuilder::addFields((Constant*)addr, kc, parent); 
         addField("Alignment", addr->getAlignment());
         if(addr->hasUnnamedAddr()) addTrueField("HasUnnamedAddr"); 
         if(addr->hasDefaultVisibility()) addTrueField("HasDefaultVisibility"); 
         if(addr->hasHiddenVisibility()) addTrueField("HasHiddenVisibility"); 
         if(addr->hasProtectedVisibility()) addTrueField("HasProtectedVisibility"); 
         if(addr->hasSection()) {
            addTrueField("HasSection");
            addField("Section", addr->getSection());
         }
         if(addr->hasExternalLinkage()) addTrueField("HasExternalLinkage"); 
         if(addr->hasAvailableExternallyLinkage()) addTrueField("HasAvailableExternallyLinkage"); 
         if(addr->hasLinkOnceLinkage()) addTrueField("HasLinkOnceLinkage"); 
         if(addr->hasWeakLinkage()) addTrueField("HasWeakLinkage");
         if(addr->hasAppendingLinkage()) addTrueField("HasAppendingLinkage"); 
         if(addr->hasInternalLinkage()) addTrueField("HasInternalLinkage"); 
         if(addr->hasPrivateLinkage()) addTrueField("HasPrivateLinkage"); 
         if(addr->hasLinkerPrivateLinkage()) addTrueField("HasLinkerPrivateLinkage"); 
         if(addr->hasLinkerPrivateWeakLinkage()) addTrueField("HasLinkerPrivateWeakLinkage"); 
         if(addr->hasLinkerPrivateWeakDefAutoLinkage()) addTrueField("HasLinkerPrivateWeakDefAutoLinkage"); 
         if(addr->hasLocalLinkage()) addTrueField("HasLocalLinkage");
         if(addr->hasDLLImportLinkage()) addTrueField("HasDLLImportLinkage");
         if(addr->hasDLLExportLinkage()) addTrueField("HasDLLExportLinkage");
         if(addr->hasExternalWeakLinkage()) addTrueField("HasExternalWeakLinkage");
         if(addr->hasCommonLinkage()) addTrueField("HasCommonLinkage");
         if(addr->mayBeOverridden()) addTrueField("MayBeOverridden");
         if(addr->isWeakForLinker()) addTrueField("IsWeakForLinker");
      }
		void build(GlobalValue* cnst, KnowledgeConstruction &kc, char* parent) {
			open();
			addFields(cnst, kc, parent);
			close();
			std::string &str = getCompletedString();
			kc.addToKnowledgeBase((PointerAddress)cnst, str);
		}
};


class CLIPSGlobalAliasBuilder : public CLIPSGlobalValueBuilder {
   public:
      CLIPSGlobalAliasBuilder(std::string nm, FunctionNamer& namer) 
         : CLIPSGlobalValueBuilder(nm, "GlobalAlias", namer) { }
      void addFields(GlobalAlias* addr, KnowledgeConstruction &kc, char* parent) {
         CLIPSGlobalValueBuilder::addFields((GlobalValue*)addr, kc, parent); 
         addField("Aliasee", kc.route(addr->getAliasee(), getNamer()));
      }
		void build(GlobalAlias* cnst, KnowledgeConstruction &kc, char* parent) {
			open();
			addFields(cnst, kc, parent);
			close();
			std::string &str = getCompletedString();
			kc.addToKnowledgeBase((PointerAddress)cnst, str);
		}
};

class CLIPSGlobalVariableBuilder : public CLIPSGlobalValueBuilder {
   public:
      CLIPSGlobalVariableBuilder(std::string nm, FunctionNamer& namer) 
         : CLIPSGlobalValueBuilder(nm, "GlobalVariable", namer) { }

      void addFields(GlobalVariable* addr, KnowledgeConstruction &kc, char* parent) {
         CLIPSGlobalValueBuilder::addFields((GlobalValue*)addr, kc, parent); 
         if(addr->hasInitializer()) {
            addTrueField("HasInitializer");
            addField("Initializer", kc.route(addr->getInitializer(), getNamer()));
         }
         if(addr->hasDefinitiveInitializer()) addTrueField("HasDefinitiveInitializer");
         if(addr->hasUniqueInitializer()) addTrueField("HasUniqueInitializer");
         if(addr->isConstant()) addTrueField("IsConstant"); 
         //if(addr->isThreadLocal()) addTrueField("IsThreadLocal");
      }
		void build(GlobalVariable* cnst, KnowledgeConstruction &kc, char* parent) {
			open();
			addFields(cnst, kc, parent);
			close();
			std::string &str = getCompletedString();
			kc.addToKnowledgeBase((PointerAddress)cnst, str);
		}
};

class CLIPSUndefValueBuilder : public CLIPSConstantBuilder {
   public:
      CLIPSUndefValueBuilder(std::string nm, FunctionNamer& namer) : 
         CLIPSConstantBuilder(nm, "UndefValue", namer) { }

      void addFields(UndefValue* addr, KnowledgeConstruction &kc, char* parent) {
         CLIPSConstantBuilder::addFields((Constant*)addr, kc, parent); 
      }
		void build(UndefValue* cnst, KnowledgeConstruction &kc, char* parent) {
			open();
			addFields(cnst, kc, parent);
			close();
			std::string &str = getCompletedString();
			kc.addToKnowledgeBase((PointerAddress)cnst, str);
		}
};

#endif
