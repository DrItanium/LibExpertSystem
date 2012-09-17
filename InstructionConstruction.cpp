#include "InstructionConstruction.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/ADT/DenseMap.h"
#include "MultifieldBuilder.h"

void CLIPSInstructionBuilder::setFields(Instruction* instruction, char* parent, bool addDestinationRegisters) {
   User* tmp = (User*)instruction;
   CLIPSUserBuilder::setFields(tmp, parent);
   FunctionNamer& namer = getNamer();
   std::string par (parent);
   PointerAddress pa = namer.registerInstructionWithBasicBlock(par);
   setField("TimeIndex", pa);
   setField("Operation", (char*) instruction->getOpcodeName());
   if(instruction->mayWriteToMemory()) setFieldTrue("MayWriteToMemory");
   if(instruction->mayReadFromMemory()) setFieldTrue("MayReadFromMemory");
   if(instruction->mayReadOrWriteMemory()) setFieldTrue("MayReadOrWriteMemory");
   if(instruction->mayHaveSideEffects()) setFieldTrue("MayHaveSideEffects");
   if(instruction->isBinaryOp()) setFieldTrue("IsBinaryOp");
   if(instruction->isTerminator()) setFieldTrue("IsTerminator");
   if(instruction->isShift()) setFieldTrue("IsShift");
   if(instruction->isCast()) setFieldTrue("IsCast");
   if(instruction->isArithmeticShift()) setFieldTrue("IsArithmeticShift"); 
   if(instruction->isLogicalShift()) setFieldTrue("IsLogicalShift");
   if(instruction->isAssociative()) setFieldTrue("IsAssociative");
   if(instruction->isCommutative()) setFieldTrue("IsCommutative");
   if(!instruction->getType()->isVoidTy() && instruction->hasName() && addDestinationRegisters) {
      //Add the register into the destination register container
      setField("DestinationRegisters", instruction->getName());	
   }
   if(!instruction->use_empty()) {
      unsigned index = 1;
      MultifieldBuilder builder(instruction->getNumUses());
      //we could have an instruction not used in the block it's created :D
      for(Value::use_iterator i = instruction->use_begin(), 
            e = instruction->use_end(); i != e; ++i, ++index) {
         User* target = *i;
         PointerAddress ptr = (PointerAddress)target;
         if(namer.pointerRegistered(ptr)) {
            builder.setSlot(index, namer.nameFromPointer(ptr));
         } else if(isa<Function>(target) || isa<Instruction>(target)) {
            builder.setSlot(index, target->getName());
         } else {
            builder.setSlot(index, Route(target, namer));
         }
      }
      MultifieldBuilder* bPtr = &builder;
      setField("Consumers", bPtr); 
   }
}
void CLIPSPHINodeBuilder::setFields(PHINode* instruction, char* parent)
{
   //I don't think we want to do User's setField as it's not necessary
   //We should do Value instead
   CLIPSValueBuilder::setFields((Value*)instruction, parent);
   std::string par(parent);
   FunctionNamer& namer = getNamer();
   PointerAddress pa = namer.registerInstructionWithBasicBlock(par);
   setField("TimeIndex", pa);
   setField("Operation", instruction->getOpcodeName());
   if(instruction->mayWriteToMemory()) setFieldTrue("MayWriteToMemory");
   if(instruction->mayReadFromMemory()) setFieldTrue("MayReadFromMemory");
   if(instruction->mayReadOrWriteMemory()) setFieldTrue("MayReadOrWriteMemory");
   if(instruction->mayHaveSideEffects()) setFieldTrue("MayHaveSideEffects");
   if(instruction->isBinaryOp()) setFieldTrue("IsBinaryOp");
   if(instruction->isTerminator()) setFieldTrue("IsTerminator");
   if(instruction->isShift()) setFieldTrue("IsShift");
   if(instruction->isCast()) setFieldTrue("IsCast");
   if(instruction->isArithmeticShift()) setFieldTrue("IsArithmeticShift"); 
   if(instruction->isLogicalShift()) setFieldTrue("IsLogicalShift");
   if(instruction->isAssociative()) setFieldTrue("IsAssociative");
   if(instruction->isCommutative()) setFieldTrue("IsCommutative");
   //TODO: insert code to add the potential constant value
   setField("DestinationRegisters", instruction->getName());
   unsigned total = instruction->getNumIncomingValues();
   MultifieldBuilder operands(total * 2);
   unsigned index = 1;
   //we use pattern matching within CLIPS to our advantage :D
   for(unsigned i = 0; i < total; ++i, index += 2) {
      Value* target = instruction->getIncomingValue(i);
      BasicBlock* from = instruction->getIncomingBlock(i);
      if(isa<UndefValue>(target)) {
         operands.setSlot(index, "undef");
      } else if(Instruction* inst = dyn_cast<Instruction>(target)) {
         operands.setSlot(index, inst->getName());
      } else {
         operands.setSlot(index, Route(target, namer));
      }
      operands.setSlot(index + 1, from->getName());
   }
   setField("Operands", &operands);
   if(!instruction->use_empty()) {
      MultifieldBuilder consumers(instruction->getNumUses());
      unsigned index = 1;
      for(Value::use_iterator i = instruction->use_begin(), e = instruction->use_end(); i != e; ++i, ++index) {
         User* target = *i;
         PointerAddress ptr = (PointerAddress)target;
         if(namer.pointerRegistered(ptr)) {
            consumers.setSlot(index, namer.nameFromPointer(ptr));
         } else if(isa<Function>(target) || isa<Instruction>(target)) {
            consumers.setSlot(index, target->getName());
         } else {
            consumers.setSlot(index, Route(target, namer));
         }
      }
      setField("Consumers", &consumers);
   }
}
void CLIPSStoreInstructionBuilder::setFields(StoreInst* target, char* parent) {
   CLIPSInstructionBuilder::setFields((Instruction*)target, parent, false);
   setField("Alignment", target->getAlignment());  
   if(target->isAtomic()) setFieldTrue("IsAtomic");
   if(target->isSimple()) setFieldTrue("IsSimple");
   if(target->isUnordered()) setFieldTrue("IsUnordered");
   if(target->isVolatile()) setFieldTrue("IsVolatile");
   Value* pointer = target->getPointerOperand();
   if(pointer->hasName()) {
      setField("DestinationRegisters", pointer->getName());
   } else {
      setField("DestinationRegisters", Route(pointer, getNamer()));
   }
}
void CLIPSBinaryOperatorBuilder::setFields(BinaryOperator* target, char* parent) {
   CLIPSInstructionBuilder::setFields((Instruction*)target, parent);
   if(target->hasNoUnsignedWrap()) setFieldTrue("HasNoUnsignedWrap");
   if(target->hasNoSignedWrap()) setFieldTrue("HasNoSignedWrap");
   if(target->isExact()) setFieldTrue("IsExact");
}
void CLIPSCallInstructionBuilder::setFields(CallInst* target, char* parent) {
   CLIPSInstructionBuilder::setFields((Instruction*)target, parent);
   if(target->isTailCall()) setFieldTrue("IsTailCall");
   if(target->isNoInline()) setFieldTrue("IsNoInline"); 
   if(target->canReturnTwice()) setFieldTrue("CanReturnTwice");
   if(target-> doesNotAccessMemory()) setFieldTrue("DoesNotAccessMemory");
   if(target-> onlyReadsMemory()) setFieldTrue("OnlyReadsMemory"); 
   if(target-> doesNotReturn()) setFieldTrue("DoesNotReturn"); 
   if(target-> doesNotThrow()) setFieldTrue("DoesNotThrow"); 
   if(target-> hasStructRetAttr()) setFieldTrue("HasStructRetAttr");
   if(target-> hasByValArgument()) setFieldTrue("HasByValArgument");
   if(target-> isInlineAsm()) setFieldTrue("IsInlineAsm");
   Function* fn = target->getCalledFunction();
   setFieldFromChoice("CalledFunction", (fn != NULL), 
                      (char*)fn->getName().data(), (char*)"indirect");
   if(target->getNumArgOperands() > 0) {
      unsigned size = target->getNumArgOperands();
      MultifieldBuilder builder(size); 
      FunctionNamer& namer = getNamer();
      for(unsigned i = 0; i < size; ++i) {
         //add function args
         builder.setSlot(i + 1, Route(target->getArgOperand(i), namer));
      }
      setField("ArgumentOperands", &builder);
   }
}
void CLIPSVAArgInstructionBuilder::setFields(VAArgInst* inst, char* parent) {
   CLIPSUnaryInstructionBuilder::setFields((UnaryInstruction*)inst, parent);
}
void CLIPSLoadInstructionBuilder::setFields(LoadInst* inst, char* parent) {
   CLIPSUnaryInstructionBuilder::setFields((UnaryInstruction*)inst, parent);
   setField("Alignment", inst->getAlignment());  
   if(inst->isAtomic()) setFieldTrue("IsAtomic");
   if(inst->isSimple()) setFieldTrue("IsSimple");
   if(inst->isUnordered()) setFieldTrue("IsUnordered");
   if(inst->isVolatile()) setFieldTrue("IsVolatile");
}
void CLIPSExtractValueInstructionBuilder::setFields(ExtractValueInst* inst, char* parent) {
   CLIPSUnaryInstructionBuilder::setFields((UnaryInstruction*)inst, parent);
   setField("AggregateOperand", Route(inst->getAggregateOperand(), getNamer()));
   MultifieldBuilder builder(inst->getNumIndices());
   unsigned index = 1;
   for(ExtractValueInst::idx_iterator i = inst->idx_begin(), e = inst->idx_end();
         i != e; ++i, ++index) {
      char* buf = CharBuffer(32);
      sprintf(buf, "%d", *i);
      builder.setSlot(index, buf);
      free(buf);
   }
   MultifieldBuilder* bPtr = &builder;
   setField("Indices", bPtr);
}
void CLIPSCastInstructionBuilder::setFields(CastInst* inst, char* parent) {
   CLIPSUnaryInstructionBuilder::setFields((UnaryInstruction*)inst, parent);
   if(inst->isIntegerCast()) setFieldTrue("IsIntegerCast");
   if(inst->isLosslessCast()) setFieldTrue("IsLosslessCast");
   FunctionNamer& namer = getNamer();
   setField("SourceType", Route(inst->getSrcTy(), namer)); 
   setField("DestinationType", Route(inst->getDestTy(), namer));
}
void CLIPSAllocaInstructionBuilder::setFields(AllocaInst* inst, char* parent) {
   CLIPSUnaryInstructionBuilder::setFields((UnaryInstruction*)inst, parent);
   if(inst->isArrayAllocation()) setFieldTrue("IsArrayAllocation");
   if(inst->isStaticAlloca()) setFieldTrue("IsStaticAllocation");
   setField("Alignment", inst->getAlignment());
   setField("ArraySize", Route(inst->getArraySize(), getNamer()));
}
void CLIPSUnaryInstructionBuilder::setFields(UnaryInstruction* inst, char* parent) {
   CLIPSInstructionBuilder::setFields((Instruction*)inst, parent);
}
void CLIPSSelectInstructionBuilder::setFields(SelectInst* inst, char* parent) {
   CLIPSInstructionBuilder::setFields((Instruction*)inst, parent);
   FunctionNamer& namer = getNamer();
   setField("Condition", Route(inst->getCondition(), namer));
   setField("TrueValue", Route(inst->getTrueValue(), namer));
   setField("FalseValue", Route(inst->getFalseValue(), namer));
}
void CLIPSUnreachableInstructionBuilder::setFields(UnreachableInst* instruction, char* parent) {
   CLIPSTerminatorInstructionBuilder::setFields((TerminatorInst*)instruction, parent);
}
void CLIPSCompareInstructionBuilder::setFields(CmpInst* target, char* parent) {
   CLIPSInstructionBuilder::setFields((Instruction*)target, parent);
   setField("Predicate", target->getPredicate());
   setField("InversePredicate", target->getInversePredicate());
   setField("SwappedPredicate", target->getSwappedPredicate());
   if(target->isFPPredicate()) setFieldTrue("IsFPPredicate");
   if(target->isIntPredicate()) setFieldTrue("IsIntPredicate");
   if(target->isCommutative()) setFieldTrue("IsCommutative");
   if(target->isEquality()) setFieldTrue("IsEquality");
   if(target->isSigned()) setFieldTrue("IsSigned");
   if(target->isUnsigned()) setFieldTrue("IsUnsigned");
   if(target->isTrueWhenEqual()) setFieldTrue("IsTrueWhenEqual");
   if(target->isFalseWhenEqual()) setFieldTrue("IsFalseWhenEqual");
}
void CLIPSFPCompareInstructionBuilder::setFields(FCmpInst* target, char* parent) {
   CLIPSCompareInstructionBuilder::setFields((CmpInst*)target,parent);
   if(target->isRelational()) setFieldTrue("IsRelational");
}
void CLIPSIntCompareInstructionBuilder::setFields(ICmpInst* target, char* parent) {
   CLIPSCompareInstructionBuilder::setFields((CmpInst*)target,parent);
   setField("SignedPredicate", target->getSignedPredicate());
   setField("UnsignedPredicate", target->getUnsignedPredicate());
   if(target->isRelational()) setFieldTrue("IsRelational");
}
void CLIPSGetElementPtrInstructionBuilder::setFields(GetElementPtrInst* target, char* parent) {
   CLIPSInstructionBuilder::setFields((Instruction*)target, parent);
   if(target->hasIndices()) setFieldTrue("HasIndices");
   if(target->hasAllZeroIndices()) setFieldTrue("HasAllZeroIndices");
   if(target->hasAllConstantIndices()) setFieldTrue("HasAllConstantIndices");
   if(target->isInBounds()) setFieldTrue("IsInBounds");
}
void CLIPSTerminatorInstructionBuilder::setFields(TerminatorInst* target, char* parent) {
   CLIPSInstructionBuilder::setFields((Instruction*)target, parent);
   setField("NumSuccessors", target->getNumSuccessors());
}
void CLIPSBranchInstructionBuilder::setFields(BranchInst* inst, char* parent) {
   CLIPSTerminatorInstructionBuilder::setFields((TerminatorInst*)inst, parent);
   if(inst->isUnconditional()) setFieldTrue("IsUnconditional");
   if(inst->isConditional()) {
      setFieldTrue("IsConditional");
      setField("Condition", Route(inst->getCondition(), getNamer()));
   }
}
void CLIPSIndirectBranchInstructionBuilder::setFields(IndirectBrInst* inst, char* parent) {
   CLIPSTerminatorInstructionBuilder::setFields((TerminatorInst*)inst, parent);
   setField("Address", Route(inst->getAddress(), getNamer()));
}
void CLIPSInvokeInstructionBuilder::setFields(InvokeInst* target, char* parent) {
   CLIPSTerminatorInstructionBuilder::setFields((TerminatorInst*)target, parent);
   if(target->isNoInline()) setFieldTrue("IsNoInline");
   if(target->doesNotAccessMemory()) setFieldTrue("DoesNotAccessMemory");
   if(target->onlyReadsMemory()) setFieldTrue("OnlyReadsMemory");
   if(target->doesNotReturn()) setFieldTrue("DoesNotReturn");
   if(target->hasStructRetAttr()) setFieldTrue("HasStructRetAttr");
   if(target->hasByValArgument()) setFieldTrue("HasByValArgument");

   setField("NormalDestination", target->getNormalDest()->getName());
   setField("UnwindDestination", target->getUnwindDest()->getName());
   Function* fn = target->getCalledFunction();
   if(fn != NULL) {
      setField("CalledFunction", fn->getName());
   } else {
      setField("CalledFunction", "indirect");
   }
   unsigned total = target->getNumArgOperands();
   if(total > 0) {

      MultifieldBuilder builder(total);
      FunctionNamer& namer = getNamer();
      for(unsigned i = 0;i < total; ++i) {
         builder.setSlot(i + 1, Route(target->getArgOperand(i), namer));
      }
      MultifieldBuilder* bPtr = &builder;
      setField("Arguments", bPtr);
   }
}
void CLIPSResumeInstructionBuilder::setFields(ResumeInst* inst, char* parent) {
   CLIPSTerminatorInstructionBuilder::setFields((TerminatorInst*)inst, parent);
}
void CLIPSReturnInstructionBuilder::setFields(ReturnInst* inst, char* parent) {
   CLIPSTerminatorInstructionBuilder::setFields((TerminatorInst*)inst, parent);
   Value* result = inst->getReturnValue();
   if(result != 0) setField("ReturnValue", Route(result, getNamer()));
}
void CLIPSSwitchInstructionBuilder::setFields(SwitchInst* inst, char* parent) {
   CLIPSValueBuilder::setFields((Value*)inst, parent);
   std::string par(parent);
   FunctionNamer& namer = getNamer();
   PointerAddress pa = namer.registerInstructionWithBasicBlock(par);
   setField("TimeIndex", pa);
   setField("Operation", (char*) inst->getOpcodeName());
   if(inst->mayWriteToMemory()) setFieldTrue("MayWriteToMemory");
   if(inst->mayReadFromMemory()) setFieldTrue("MayReadFromMemory");
   if(inst->mayReadOrWriteMemory()) setFieldTrue("MayReadOrWriteMemory");
   if(inst->mayHaveSideEffects()) setFieldTrue("MayHaveSideEffects");
   if(inst->isBinaryOp()) setFieldTrue("IsBinaryOp");
   if(inst->isTerminator()) setFieldTrue("IsTerminator");
   if(inst->isShift()) setFieldTrue("IsShift");
   if(inst->isCast()) setFieldTrue("IsCast");
   if(inst->isArithmeticShift()) setFieldTrue("IsArithmeticShift"); 
   if(inst->isLogicalShift()) setFieldTrue("IsLogicalShift");
   if(inst->isAssociative()) setFieldTrue("IsAssociative");
   if(inst->isCommutative()) setFieldTrue("IsCommutative");
   setField("Condition", Route(inst->getCondition(), getNamer()));
   setField("DefaultDestination", inst->getDefaultDest()->getName());
   unsigned succCount = inst->getNumSuccessors();
   if(succCount > 0) {
      setField("NumSuccessors", succCount);
      MultifieldBuilder b(succCount * 2);
      unsigned index = 1;
      for(unsigned i = 0; i < succCount; ++i, index += 2) {
         b.setSlot(index, i);
         BasicBlock* target = inst->getSuccessor(i);
         b.setSlot(index + 1, target->getName());
      }
      MultifieldBuilder* bp = &b;
      setField("Operands", bp);
   }
}
