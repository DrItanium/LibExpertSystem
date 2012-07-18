#include "InstructionConstruction.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/ADT/DenseMap.h"

void CLIPSInstructionBuilder::addFields(Instruction* instruction, char* parent, bool addDestinationRegisters) {
   User* tmp = (User*)instruction;
   CLIPSUserBuilder::addFields(tmp, parent);
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
      //Add the register into the destination register container
      //send(".AddDestinationRegisters", instruction->getName()); 
   }
   if(!instruction->use_empty()) {
      //      DenseMap<BasicBlock*,unsigned> counterArgument;
      //BasicBlock* directParent =  instruction->getParent();
      //we could have an instruction not used in the block it's created :D
      openField("Consumers");
      for(Value::use_iterator i = instruction->use_begin(), 
            e = instruction->use_end(); i != e; ++i) {
         User* target = *i;
         /*
         //this part was taken from isUsedOutsideOfBlock in llvm::Instruction
         //It has been modified though
         PHINode* PN = dyn_cast<PHINode>(target);
         if(PN == 0) {
         Instruction* j = cast<Instruction>(target);
         BasicBlock* target = j->getParent();
         if(!counterArgument.count(target)) {
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
            appendValue(Route(target, namer));
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
void CLIPSPHINodeBuilder::addFields(PHINode* instruction, char* parent)
{
   //I don't think we want to do User's addField as it's not necessary
   //We should do Value instead
   CLIPSValueBuilder::addFields((Value*)instruction, parent);
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
      else appendValue(Route(target, namer));
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
            appendValue(Route(target, namer));
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
void CLIPSStoreInstructionBuilder::addFields(StoreInst* target, char* parent) {
   CLIPSInstructionBuilder::addFields((Instruction*)target, parent, false);
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
      appendValue(Route(pointer, getNamer()));
   }
   closeField();
}
void CLIPSBinaryOperatorBuilder::addFields(BinaryOperator* target, char* parent) {
   CLIPSInstructionBuilder::addFields((Instruction*)target, parent);
   if(target->hasNoUnsignedWrap()) addTrueField("HasNoUnsignedWrap");
   if(target->hasNoSignedWrap()) addTrueField("HasNoSignedWrap");
   if(target->isExact()) addTrueField("IsExact");
}
void CLIPSCallInstructionBuilder::addFields(CallInst* target, char* parent) {
   CLIPSInstructionBuilder::addFields((Instruction*)target, parent);
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
         appendValue(Route(target->getArgOperand(i), namer));
      }
      closeField();
   }
}
void CLIPSVAArgInstructionBuilder::addFields(VAArgInst* inst, char* parent) {
   CLIPSUnaryInstructionBuilder::addFields((UnaryInstruction*)inst, parent);
}
void CLIPSLoadInstructionBuilder::addFields(LoadInst* inst, char* parent) {
   CLIPSUnaryInstructionBuilder::addFields((UnaryInstruction*)inst, parent);
   addField("Alignment", inst->getAlignment());  
   if(inst->isAtomic()) addTrueField("IsAtomic");
   if(inst->isSimple()) addTrueField("IsSimple");
   if(inst->isUnordered()) addTrueField("IsUnordered");
   if(inst->isVolatile()) addTrueField("IsVolatile");
}
void CLIPSExtractValueInstructionBuilder::addFields(ExtractValueInst* inst, char* parent) {
   CLIPSUnaryInstructionBuilder::addFields((UnaryInstruction*)inst, parent);
   addField("AggregateOperand", Route(inst->getAggregateOperand(), getNamer()));
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
void CLIPSCastInstructionBuilder::addFields(CastInst* inst, char* parent) {
   CLIPSUnaryInstructionBuilder::addFields((UnaryInstruction*)inst, parent);
   if(inst->isIntegerCast()) addTrueField("IsIntegerCast");
   if(inst->isLosslessCast()) addTrueField("IsLosslessCast");
   FunctionNamer& namer = getNamer();
   addField("SourceType", Route(inst->getSrcTy(), namer)); 
   addField("DestinationType", Route(inst->getDestTy(), namer));
}
void CLIPSAllocaInstructionBuilder::addFields(AllocaInst* inst, char* parent) {
   CLIPSUnaryInstructionBuilder::addFields((UnaryInstruction*)inst, parent);
   if(inst->isArrayAllocation()) addTrueField("IsArrayAllocation");
   if(inst->isStaticAlloca()) addTrueField("IsStaticAllocation");
   addField("Alignment", inst->getAlignment());
   addField("ArraySize", Route(inst->getArraySize(), getNamer()));
}
void CLIPSUnaryInstructionBuilder::addFields(UnaryInstruction* inst, char* parent) {
   CLIPSInstructionBuilder::addFields((Instruction*)inst, parent);
}
void CLIPSSelectInstructionBuilder::addFields(SelectInst* inst, char* parent) {
   CLIPSInstructionBuilder::addFields((Instruction*)inst, parent);
   FunctionNamer& namer = getNamer();
   addField("Condition", Route(inst->getCondition(), namer));
   addField("TrueValue", Route(inst->getTrueValue(), namer));
   addField("FalseValue", Route(inst->getFalseValue(), namer));
}
void CLIPSUnreachableInstructionBuilder::addFields(UnreachableInst* instruction, char* parent) {
   CLIPSTerminatorInstructionBuilder::addFields((TerminatorInst*)instruction, parent);
}
void CLIPSCompareInstructionBuilder::addFields(CmpInst* target, char* parent) {
   CLIPSInstructionBuilder::addFields((Instruction*)target, parent);
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
void CLIPSFPCompareInstructionBuilder::addFields(FCmpInst* target, char* parent) {
   CLIPSCompareInstructionBuilder::addFields((CmpInst*)target,parent);
   if(target->isRelational()) addTrueField("IsRelational");
}
void CLIPSIntCompareInstructionBuilder::addFields(ICmpInst* target, char* parent) {
   CLIPSCompareInstructionBuilder::addFields((CmpInst*)target,parent);
   addField("SignedPredicate", target->getSignedPredicate());
   addField("UnsignedPredicate", target->getUnsignedPredicate());
   if(target->isRelational()) addTrueField("IsRelational");
}
void CLIPSGetElementPtrInstructionBuilder::addFields(GetElementPtrInst* target, char* parent) {
   CLIPSInstructionBuilder::addFields((Instruction*)target, parent);
   if(target->hasIndices()) addTrueField("HasIndices");
   if(target->hasAllZeroIndices()) addTrueField("HasAllZeroIndices");
   if(target->hasAllConstantIndices()) addTrueField("HasAllConstantIndices");
   if(target->isInBounds()) addTrueField("IsInBounds");
}
void CLIPSTerminatorInstructionBuilder::addFields(TerminatorInst* target, char* parent) {
   CLIPSInstructionBuilder::addFields((Instruction*)target, parent);
   addField("NumSuccessors", target->getNumSuccessors());
}
void CLIPSBranchInstructionBuilder::addFields(BranchInst* inst, char* parent) {
   CLIPSTerminatorInstructionBuilder::addFields((TerminatorInst*)inst, parent);
   if(inst->isUnconditional()) addTrueField("IsUnconditional");
   if(inst->isConditional()) {
      addTrueField("IsConditional");
      addField("Condition", Route(inst->getCondition(), getNamer()));
   }
}
void CLIPSIndirectBranchInstructionBuilder::addFields(IndirectBrInst* inst, char* parent) {
   CLIPSTerminatorInstructionBuilder::addFields((TerminatorInst*)inst, parent);
   addField("Address", Route(inst->getAddress(), getNamer()));
}
void CLIPSInvokeInstructionBuilder::addFields(InvokeInst* target, char* parent) {
   CLIPSTerminatorInstructionBuilder::addFields((TerminatorInst*)target, parent);
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
         appendValue(Route(target->getArgOperand(i), namer));
      }
      closeField();
   }
}
void CLIPSResumeInstructionBuilder::addFields(ResumeInst* inst, char* parent) {
   CLIPSTerminatorInstructionBuilder::addFields((TerminatorInst*)inst, parent);
}
void CLIPSReturnInstructionBuilder::addFields(ReturnInst* inst, char* parent) {
   CLIPSTerminatorInstructionBuilder::addFields((TerminatorInst*)inst, parent);
   Value* result = inst->getReturnValue();
   if(result != 0) addField("ReturnValue", Route(result, getNamer()));
}
void CLIPSSwitchInstructionBuilder::addFields(SwitchInst* inst, char* parent) {
   CLIPSValueBuilder::addFields((Value*)inst, parent);
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
   addField("Condition", Route(inst->getCondition(), getNamer()));
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
