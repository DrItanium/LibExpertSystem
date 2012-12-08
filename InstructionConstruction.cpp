#include "InstructionConstruction.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/ADT/DenseMap.h"
#define make_build_method(cname,iname) \
	void CLIPS ## cname ## Builder ::build ( iname * inst, KnowledgeConstruction* kc, char* parent) { \
		open(); \
		addFields(inst, kc, parent); \
		close(); \
		std::string str = getCompletedString(); \
		kc->addToKnowledgeBase((PointerAddress) inst, str);\
	}

void CLIPSInstructionBuilder::addFields(Instruction* instruction, KnowledgeConstruction *kc, char* parent, bool addDestinationRegisters) {
   User* tmp = (User*)instruction;
   CLIPSUserBuilder::addFields(tmp, kc, parent);
   FunctionNamer& namer = getNamer();
   std::string par (parent);
   PointerAddress pa = namer.registerInstructionWithBasicBlock(par);
   addField("TimeIndex", pa);
   addField("Operation", (char*) instruction->getOpcodeName());
   if(instruction->mayWriteToMemory()) addTrueField("MayWriteToMemory");
   if(instruction->mayReadFromMemory()) addTrueField("MayReadFromMemory");
   if(instruction->mayReadOrWriteMemory()) addTrueField("MayReadOrWriteMemory");
   if(instruction->mayHaveSideEffects()) addTrueField("MayHaveSideEffects");
   if(instruction->isBinaryOp()) addTrueField("IsBinaryOp");
   if(instruction->isTerminator()) addTrueField("IsTerminator");
   if(instruction->isShift()) addTrueField("IsShift");
   if(instruction->isCast()) addTrueField("IsCast");
   if(instruction->isArithmeticShift()) addTrueField("IsArithmeticShift"); 
   if(instruction->isLogicalShift()) addTrueField("IsLogicalShift");
   if(instruction->isAssociative()) addTrueField("IsAssociative");
   if(instruction->isCommutative()) addTrueField("IsCommutative");
   if(!instruction->getType()->isVoidTy() && instruction->hasName() && addDestinationRegisters) {
      addField("DestinationRegisters", instruction->getName());	
   }
   if(!instruction->use_empty()) {
      //      DenseMap<BasicBlock*,unsigned> counterArgument;
      //BasicBlock* directParent =  instruction->getParent();
      //we could have an instruction not used in the block it's created :D
      openField("Consumers");
      for(Value::use_iterator i = instruction->use_begin(), 
            e = instruction->use_end(); i != e; ++i) {
         User* target = *i;
         PointerAddress ptr = (PointerAddress)target;
         if(namer.pointerRegistered(ptr)) {
            appendValue(namer.nameFromPointer(ptr));
         } else if(isa<Function>(target) || isa<Instruction>(target)) {
            appendValue(target->getName());
         } else {
            appendValue(kc->route(target, namer));
         }
      }
      closeField();
      /*
         openField("BlocksUsedIn");
         for(DenseMap<BasicBlock*, unsigned>::iterator a = counterArgument.begin(), b = counterArgument.end(); a != b; ++a) {
         BasicBlock* bb = a->first;
         appendValue(bb->getName()); //the name of the basic block
         appendValue(a->second); //the number of iterations
         }
         closeField();
         */
   }
}
make_build_method(Instruction,Instruction)
/*
void CLIPSInstructionBuilder::build(Instruction* inst, KnowledgeConstruction *kc, char* parent) {
	open();
	addFields(inst, kc, parent);
	close();
	std::string &str = getCompletedString();
	kc.addToKnowledgeBase((PointerAddress)inst, str);
}
*/
void CLIPSPHINodeBuilder::addFields(PHINode* instruction, KnowledgeConstruction *kc, char* parent)
{
   //I don't think we want to do User's addField as it's not necessary
   //We should do Value instead
   CLIPSValueBuilder::addFields((Value*)instruction, kc, parent);
   std::string par(parent);
   FunctionNamer& namer = getNamer();
   PointerAddress pa = namer.registerInstructionWithBasicBlock(par);
   addField("TimeIndex", pa);
   addField("Operation", instruction->getOpcodeName());
   if(instruction->mayWriteToMemory()) addTrueField("MayWriteToMemory");
   if(instruction->mayReadFromMemory()) addTrueField("MayReadFromMemory");
   if(instruction->mayReadOrWriteMemory()) addTrueField("MayReadOrWriteMemory");
   if(instruction->mayHaveSideEffects()) addTrueField("MayHaveSideEffects");
   if(instruction->isBinaryOp()) addTrueField("IsBinaryOp");
   if(instruction->isTerminator()) addTrueField("IsTerminator");
   if(instruction->isShift()) addTrueField("IsShift");
   if(instruction->isCast()) addTrueField("IsCast");
   if(instruction->isArithmeticShift()) addTrueField("IsArithmeticShift"); 
   if(instruction->isLogicalShift()) addTrueField("IsLogicalShift");
   if(instruction->isAssociative()) addTrueField("IsAssociative");
   if(instruction->isCommutative()) addTrueField("IsCommutative");
   //TODO: insert code to add the potential constant value
   addField("DestinationRegisters", instruction->getName());
   openField("Operands");
   //we use pattern matching within CLIPS to our advantage :D
   for(unsigned i = 0; i < instruction->getNumIncomingValues(); ++i) {
      Value* target = instruction->getIncomingValue(i);
      BasicBlock* from = instruction->getIncomingBlock(i);
      if(isa<UndefValue>(target)) appendValue("undef");
      else if(Instruction* inst = dyn_cast<Instruction>(target)) appendValue(inst->getName());
      else appendValue(kc->route(target, namer));
      appendValue(from->getName());
   }
   closeField();
   if(!instruction->use_empty()) {
      //DenseMap<BasicBlock*,unsigned> counterArgument;
      //BasicBlock* directParent =  instruction->getParent();
      //we could have an instruction not used in the block it's created :D
      openField("Consumers");
      for(Value::use_iterator i = instruction->use_begin(), e = instruction->use_end(); i != e; ++i) {
         User* target = *i;
         /*
         //this part was taken from isUsedOutsideOfBlock in llvm::Instruction
         It has been modified though
         PHINode* PN = dyn_cast<PHINode>(target);
         if(PN == 0) {
         Instruction* j = cast<Instruction>(target);
         BasicBlock* target = j->getParent();
         if(!counterArgument.count(target)) //       {
         std::pair<BasicBlock*,unsigned> pair(target, 1);
         counterArgument.insert(pair); 
         } else {
         std::pair<BasicBlock*,unsigned>& pair = counterArgument.FindAndConstruct(target);
         pair.second = pair.second + 1;
         }
         } else {
         //PHI nodes are evaluated within the basic block denoted in each cell
         BasicBlock* incBlock = PN->getIncomingBlock(i);
         if(incBlock != directParent && !counterArgument.count(incBlock)) {
         std::pair<BasicBlock*,unsigned> pair(incBlock, 1);
         counterArgument.insert(pair); 
         } else {
         std::pair<BasicBlock*,unsigned>& pair = counterArgument.FindAndConstruct(incBlock);
         pair.second = pair.second + 1;
         }
         }
         //end graft
         */
         PointerAddress ptr = (PointerAddress)target;
         if(namer.pointerRegistered(ptr)) {
            appendValue(namer.nameFromPointer(ptr));
         } else if(isa<Function>(target) || isa<Instruction>(target)) {
            appendValue(target->getName());
         } else {
            appendValue(kc->route(target, namer));
         }
      }
      closeField();
      /*
         openField("BlocksUsedIn");
         for(DenseMap<BasicBlock*, unsigned>::iterator a = counterArgument.begin(), b = counterArgument.end(); a != b; ++a) {
         BasicBlock* bb = a->first;
         appendValue(bb->getName()); //the name of the basic block
         appendValue(a->second); //the number of iterations
         }
         closeField();
         */
   }
}
make_build_method(PHINode, PHINode)
void CLIPSStoreInstructionBuilder::addFields(StoreInst* target, KnowledgeConstruction *kc, char* parent) {
   CLIPSInstructionBuilder::addFields((Instruction*)target, kc, parent, false);
   addField("Alignment", target->getAlignment());  
   if(target->isAtomic()) addTrueField("IsAtomic");
   if(target->isSimple()) addTrueField("IsSimple");
   if(target->isUnordered()) addTrueField("IsUnordered");
   if(target->isVolatile()) addTrueField("IsVolatile");
   openField("DestinationRegisters");
   Value* pointer = target->getPointerOperand();
   if(pointer->hasName()) {
      appendValue(pointer->getName());
   } else {
      appendValue(kc->route(pointer, getNamer()));
   }
   closeField();
}
make_build_method(StoreInstruction, StoreInst)
void CLIPSBinaryOperatorBuilder::addFields(BinaryOperator* target, KnowledgeConstruction *kc, char* parent) {
   CLIPSInstructionBuilder::addFields((Instruction*)target, kc, parent);
   if(target->hasNoUnsignedWrap()) addTrueField("HasNoUnsignedWrap");
   if(target->hasNoSignedWrap()) addTrueField("HasNoSignedWrap");
   if(target->isExact()) addTrueField("IsExact");
}
make_build_method(BinaryOperator, BinaryOperator)

void CLIPSCallInstructionBuilder::addFields(CallInst* target, KnowledgeConstruction *kc, char* parent) {
   CLIPSInstructionBuilder::addFields((Instruction*)target, kc, parent);
   if(target->isTailCall()) addTrueField("IsTailCall");
   if(target->isNoInline()) addTrueField("IsNoInline"); 
   if(target->canReturnTwice()) addTrueField("CanReturnTwice");
   if(target-> doesNotAccessMemory()) addTrueField("DoesNotAccessMemory");
   if(target-> onlyReadsMemory()) addTrueField("OnlyReadsMemory"); 
   if(target-> doesNotReturn()) addTrueField("DoesNotReturn"); 
   if(target-> doesNotThrow()) addTrueField("DoesNotThrow"); 
   if(target-> hasStructRetAttr()) addTrueField("HasStructRetAttr");
   if(target-> hasByValArgument()) addTrueField("HasByValArgument");
   if(target-> isInlineAsm()) addTrueField("IsInlineAsm");
   Function* fn = target->getCalledFunction();
   openField("CalledFunction");
   if(fn != NULL) appendValue(fn->getName());
   else appendValue("indirect");
   closeField();

   if(target->getNumArgOperands() > 0) {
      openField("ArgumentOperands");
      FunctionNamer& namer = getNamer();
      for(unsigned i = 0; i < target->getNumArgOperands(); ++i) {
         //add function args
         appendValue(kc->route(target->getArgOperand(i), namer));
      }
      closeField();
   }
}
make_build_method(CallInstruction,CallInst)

void CLIPSVAArgInstructionBuilder::addFields(VAArgInst* inst, KnowledgeConstruction *kc, char* parent) {
   CLIPSUnaryInstructionBuilder::addFields((UnaryInstruction*)inst, kc, parent);
}
make_build_method(VAArgInstruction, VAArgInst)

void CLIPSLoadInstructionBuilder::addFields(LoadInst* inst, KnowledgeConstruction *kc, char* parent) {
   CLIPSUnaryInstructionBuilder::addFields((UnaryInstruction*)inst, kc, parent);
   addField("Alignment", inst->getAlignment());  
   if(inst->isAtomic()) addTrueField("IsAtomic");
   if(inst->isSimple()) addTrueField("IsSimple");
   if(inst->isUnordered()) addTrueField("IsUnordered");
   if(inst->isVolatile()) addTrueField("IsVolatile");
}
make_build_method(LoadInstruction, LoadInst)

void CLIPSExtractValueInstructionBuilder::addFields(ExtractValueInst* inst, KnowledgeConstruction *kc, char* parent) {
   CLIPSUnaryInstructionBuilder::addFields((UnaryInstruction*)inst, kc, parent);
   addField("AggregateOperand", kc->route(inst->getAggregateOperand(), getNamer()));
   openField("Indices");
   for(ExtractValueInst::idx_iterator i = inst->idx_begin(), e = inst->idx_end();
         i != e; ++i) {
      char* buf = CharBuffer(32);
      sprintf(buf, "%d", *i);
      appendValue(buf);
      free(buf);
   }
   closeField();
}

make_build_method(ExtractValueInstruction, ExtractValueInst)

void CLIPSCastInstructionBuilder::addFields(CastInst* inst, KnowledgeConstruction *kc, char* parent) {
   CLIPSUnaryInstructionBuilder::addFields((UnaryInstruction*)inst, kc, parent);
   if(inst->isIntegerCast()) addTrueField("IsIntegerCast");
   if(inst->isLosslessCast()) addTrueField("IsLosslessCast");
   FunctionNamer& namer = getNamer();
   addField("SourceType", kc->route(inst->getSrcTy(), namer)); 
   addField("DestinationType", kc->route(inst->getDestTy(), namer));
}
make_build_method(CastInstruction, CastInst)

void CLIPSAllocaInstructionBuilder::addFields(AllocaInst* inst, KnowledgeConstruction *kc, char* parent) {
   CLIPSUnaryInstructionBuilder::addFields((UnaryInstruction*)inst, kc, parent);
   if(inst->isArrayAllocation()) addTrueField("IsArrayAllocation");
   if(inst->isStaticAlloca()) addTrueField("IsStaticAllocation");
   addField("Alignment", inst->getAlignment());
   addField("ArraySize", kc->route(inst->getArraySize(), getNamer()));
}
make_build_method(AllocaInstruction, AllocaInst)

void CLIPSUnaryInstructionBuilder::addFields(UnaryInstruction* inst, KnowledgeConstruction *kc, char* parent) {
   CLIPSInstructionBuilder::addFields((Instruction*)inst, kc, parent);
}
make_build_method(UnaryInstruction, UnaryInstruction)

void CLIPSSelectInstructionBuilder::addFields(SelectInst* inst, KnowledgeConstruction *kc, char* parent) {
   CLIPSInstructionBuilder::addFields((Instruction*)inst, kc, parent);
   FunctionNamer& namer = getNamer();
   addField("Condition", kc->route(inst->getCondition(), namer));
   addField("TrueValue", kc->route(inst->getTrueValue(), namer));
   addField("FalseValue", kc->route(inst->getFalseValue(), namer));
}
make_build_method(SelectInstruction, SelectInst)
void CLIPSUnreachableInstructionBuilder::addFields(UnreachableInst* instruction, KnowledgeConstruction *kc, char* parent) {
   CLIPSTerminatorInstructionBuilder::addFields((TerminatorInst*)instruction, kc, parent);
}
make_build_method(UnreachableInstruction, UnreachableInst)

void CLIPSCompareInstructionBuilder::addFields(CmpInst* target, KnowledgeConstruction *kc, char* parent) {
   CLIPSInstructionBuilder::addFields((Instruction*)target, kc, parent);
   addField("Predicate", target->getPredicate());
   addField("InversePredicate", target->getInversePredicate());
   addField("SwappedPredicate", target->getSwappedPredicate());
   if(target->isFPPredicate()) addTrueField("IsFPPredicate");
   if(target->isIntPredicate()) addTrueField("IsIntPredicate");
   if(target->isCommutative()) addTrueField("IsCommutative");
   if(target->isEquality()) addTrueField("IsEquality");
   if(target->isSigned()) addTrueField("IsSigned");
   if(target->isUnsigned()) addTrueField("IsUnsigned");
   if(target->isTrueWhenEqual()) addTrueField("IsTrueWhenEqual");
   if(target->isFalseWhenEqual()) addTrueField("IsFalseWhenEqual");
}
make_build_method(CompareInstruction, CmpInst)
void CLIPSFPCompareInstructionBuilder::addFields(FCmpInst* target, KnowledgeConstruction *kc, char* parent) {
   CLIPSCompareInstructionBuilder::addFields((CmpInst*)target,kc,parent);
   if(target->isRelational()) addTrueField("IsRelational");
}
make_build_method(FPCompareInstruction, FCmpInst)

void CLIPSIntCompareInstructionBuilder::addFields(ICmpInst* target, KnowledgeConstruction *kc, char* parent) {
   CLIPSCompareInstructionBuilder::addFields((CmpInst*)target,kc, parent);
   addField("SignedPredicate", target->getSignedPredicate());
   addField("UnsignedPredicate", target->getUnsignedPredicate());
   if(target->isRelational()) addTrueField("IsRelational");
}
make_build_method(IntCompareInstruction, ICmpInst)
void CLIPSGetElementPtrInstructionBuilder::addFields(GetElementPtrInst* target, KnowledgeConstruction *kc, char* parent) {
   CLIPSInstructionBuilder::addFields((Instruction*)target, kc, parent);
   if(target->hasIndices()) addTrueField("HasIndices");
   if(target->hasAllZeroIndices()) addTrueField("HasAllZeroIndices");
   if(target->hasAllConstantIndices()) addTrueField("HasAllConstantIndices");
   if(target->isInBounds()) addTrueField("IsInBounds");
}
make_build_method(GetElementPtrInstruction, GetElementPtrInst)

void CLIPSTerminatorInstructionBuilder::addFields(TerminatorInst* target, KnowledgeConstruction *kc, char* parent) {
   CLIPSInstructionBuilder::addFields((Instruction*)target, kc, parent);
   addField("NumSuccessors", target->getNumSuccessors());
}
make_build_method(TerminatorInstruction, TerminatorInst)

void CLIPSBranchInstructionBuilder::addFields(BranchInst* inst, KnowledgeConstruction *kc, char* parent) {
   CLIPSTerminatorInstructionBuilder::addFields((TerminatorInst*)inst, kc, parent);
   if(inst->isUnconditional()) addTrueField("IsUnconditional");
   if(inst->isConditional()) {
      addTrueField("IsConditional");
      addField("Condition", kc->route(inst->getCondition(), getNamer()));
   }
}
make_build_method(BranchInstruction, BranchInst)

void CLIPSIndirectBranchInstructionBuilder::addFields(IndirectBrInst* inst, KnowledgeConstruction *kc, char* parent) {
   CLIPSTerminatorInstructionBuilder::addFields((TerminatorInst*)inst, kc, parent);
   addField("Address", kc->route(inst->getAddress(), getNamer()));
}
make_build_method(IndirectBranchInstruction, IndirectBrInst)

void CLIPSInvokeInstructionBuilder::addFields(InvokeInst* target, KnowledgeConstruction *kc, char* parent) {
   CLIPSTerminatorInstructionBuilder::addFields((TerminatorInst*)target, kc, parent);
   if(target->isNoInline()) addTrueField("IsNoInline");
   if(target->doesNotAccessMemory()) addTrueField("DoesNotAccessMemory");
   if(target->onlyReadsMemory()) addTrueField("OnlyReadsMemory");
   if(target->doesNotReturn()) addTrueField("DoesNotReturn");
   if(target->hasStructRetAttr()) addTrueField("HasStructRetAttr");
   if(target->hasByValArgument()) addTrueField("HasByValArgument");

   addField("NormalDestination", target->getNormalDest()->getName());
   addField("UnwindDestination", target->getUnwindDest()->getName());
   Function* fn = target->getCalledFunction();
   openField("CalledFunction");
   if(fn != NULL) {
      appendValue(fn->getName());
   } else {
      appendValue("indirect");
   }
   closeField();
   if(target->getNumArgOperands() > 0) {
      openField("Arguments");
      FunctionNamer& namer = getNamer();
      for(unsigned i = 0;i < target->getNumArgOperands(); ++i) {
         appendValue(kc->route(target->getArgOperand(i), namer));
      }
      closeField();
   }
}
make_build_method(InvokeInstruction, InvokeInst)

void CLIPSResumeInstructionBuilder::addFields(ResumeInst* inst, KnowledgeConstruction *kc, char* parent) {
   CLIPSTerminatorInstructionBuilder::addFields((TerminatorInst*)inst, kc, parent);
}
make_build_method(ResumeInstruction, ResumeInst)

void CLIPSReturnInstructionBuilder::addFields(ReturnInst* inst, KnowledgeConstruction *kc, char* parent) {
   CLIPSTerminatorInstructionBuilder::addFields((TerminatorInst*)inst, kc, parent);
   Value* result = inst->getReturnValue();
   if(result != 0) addField("ReturnValue", kc->route(result, getNamer()));
}
make_build_method(ReturnInstruction, ReturnInst)

void CLIPSSwitchInstructionBuilder::addFields(SwitchInst* inst, KnowledgeConstruction *kc, char* parent) {
   CLIPSValueBuilder::addFields((Value*)inst, kc, parent);
   std::string par(parent);
   FunctionNamer& namer = getNamer();
   PointerAddress pa = namer.registerInstructionWithBasicBlock(par);
   addField("TimeIndex", pa);
   addField("Operation", (char*) inst->getOpcodeName());
   if(inst->mayWriteToMemory()) addTrueField("MayWriteToMemory");
   if(inst->mayReadFromMemory()) addTrueField("MayReadFromMemory");
   if(inst->mayReadOrWriteMemory()) addTrueField("MayReadOrWriteMemory");
   if(inst->mayHaveSideEffects()) addTrueField("MayHaveSideEffects");
   if(inst->isBinaryOp()) addTrueField("IsBinaryOp");
   if(inst->isTerminator()) addTrueField("IsTerminator");
   if(inst->isShift()) addTrueField("IsShift");
   if(inst->isCast()) addTrueField("IsCast");
   if(inst->isArithmeticShift()) addTrueField("IsArithmeticShift"); 
   if(inst->isLogicalShift()) addTrueField("IsLogicalShift");
   if(inst->isAssociative()) addTrueField("IsAssociative");
   if(inst->isCommutative()) addTrueField("IsCommutative");
   addField("Condition", kc->route(inst->getCondition(), getNamer()));
   addField("DefaultDestination", inst->getDefaultDest()->getName());
   unsigned succCount = inst->getNumSuccessors();
   if(succCount > 0) {
      addField("NumSuccessors", succCount);
      openField("Operands");
      for(unsigned i = 0; i < succCount; ++i) {
         appendValue(i);
         BasicBlock* target = inst->getSuccessor(i);
         appendValue(target->getName());
      }
      closeField();
   }
}
make_build_method(SwitchInstruction, SwitchInst)

#undef make_build_method
