#include "LLVMWrapper.h"
#include <string>
#include <cstdio>

char* createPhiNodeTitle= (char*)"llvm-make-phi-node";
char* llvmPrintTitle = (char*)"llvm-print";
extern "C" void LLVMPrint(void* theEnv) {
	DATA_OBJECT argument;
	char *tempPtr;
	if((EnvArgCountCheck(theEnv, llvmPrintTitle,EXACTLY,1) == -1)) { return; }
	if(EnvArgTypeCheck(theEnv, llvmPrintTitle,1,STRING,&argument) == 0) { return; }
	tempPtr = DOToString(argument);
	llvm::errs() << tempPtr << '\n';
}
extern "C" void BatchLoad(void* theEnv) {
	DATA_OBJECT argument;
	void* multifieldPtr;
	int end, i;
	char* tmpPtr;
	if((EnvArgCountCheck(theEnv, "batch-load", EXACTLY, 1) == -1)) { return ; }
	if((EnvArgTypeCheck(theEnv,  "batch-load", 1, MULTIFIELD, &argument) == 0)) { return; }
	end = GetDOEnd(argument);
	multifieldPtr = GetValue(argument);
	for(i = GetDOBegin(argument); i <= end; i++) {
		if((GetMFType(multifieldPtr, i) == STRING)) {
			tmpPtr = ValueToString(GetMFValue(multifieldPtr, i));
			BatchStar(tmpPtr);
		}
	}
}
extern "C" void* IsIdenticalTo(void *theEnv) {
	DATA_OBJECT arg0;
   DATA_OBJECT arg1;
	if((EnvArgCountCheck(theEnv, "llvm-instruction-is-identical-to", EXACTLY, 2) == -1)) { return (FalseSymbol()); }
	if((EnvArgTypeCheck(theEnv, "llvm-instruction-is-identical-to", 1, INTEGER, &arg0) == 0)) { return (FalseSymbol()); }
	if((EnvArgTypeCheck(theEnv, "llvm-instruction-is-identical-to", 2, INTEGER, &arg1) == 0)) { return (FalseSymbol()); }
   llvm::Instruction* inst0 = (llvm::Instruction*)(PointerAddress)DOToLong(arg0);
   llvm::Instruction* inst1 = (llvm::Instruction*)(PointerAddress)DOToLong(arg1);
   if(inst0->isIdenticalTo(inst1)) {
      return TrueSymbol();
   } else {
      return FalseSymbol();
   }
}
extern "C" void* IsIdenticalToWhenDefined(void *theEnv)
{
	DATA_OBJECT arg0;
   DATA_OBJECT arg1;
	if((EnvArgCountCheck(theEnv, "llvm-instruction-is-identical-to-when-defined", EXACTLY, 2) == -1)) { return (FalseSymbol()); }
	if((EnvArgTypeCheck(theEnv, "llvm-instruction-is-identical-to-when-defined", 1, INTEGER, &arg0) == 0)) { return (FalseSymbol()); }
	if((EnvArgTypeCheck(theEnv, "llvm-instruction-is-identical-to-when-defined", 2, INTEGER, &arg1) == 0)) { return (FalseSymbol()); }
   llvm::Instruction* inst0 = (llvm::Instruction*)(PointerAddress)DOToLong(arg0);
   llvm::Instruction* inst1 = (llvm::Instruction*)(PointerAddress)DOToLong(arg1);
   if(inst0->isIdenticalToWhenDefined(inst1)) {
      return TrueSymbol();
   } else {
      return FalseSymbol();
   }
}
extern "C" void* IsSameOperationAs(void *theEnv)
{
	DATA_OBJECT arg0;
   DATA_OBJECT arg1;
	if((EnvArgCountCheck(theEnv, "llvm-instruction-is-same-operation-as", EXACTLY, 2) == -1)) { return (FalseSymbol()); }
	if((EnvArgTypeCheck(theEnv, "llvm-instruction-is-same-operation-as", 1, INTEGER, &arg0) == 0)) { return (FalseSymbol()); }
	if((EnvArgTypeCheck(theEnv, "llvm-instruction-is-same-operation-as", 2, INTEGER, &arg1) == 0)) { return (FalseSymbol()); }
   llvm::Instruction* inst0 = (llvm::Instruction*)(PointerAddress)DOToLong(arg0);
   llvm::Instruction* inst1 = (llvm::Instruction*)(PointerAddress)DOToLong(arg1);
   if(inst0->isSameOperationAs(inst1)) {
      return TrueSymbol();
   } else {
      return FalseSymbol();
   }

}

extern "C" PointerAddress CloneInstruction(void *theEnv) {
   //this returns the pointer to the clone...it's up to CLIPS to create the
   //object to wrap around it
   DATA_OBJECT arg0, arg1;
   char* name;
   if((EnvArgCountCheck(theEnv, "llvm-clone-instruction", EXACTLY, 2) == -1)) { return 0L; }
   if((EnvArgTypeCheck(theEnv, "llvm-clone-instruction", 1, INTEGER, &arg0) == 0)) { return 0L; }
   if((EnvArgTypeCheck(theEnv, "llvm-clone-instruction", 2, SYMBOL, &arg1) == 0)) { return 0L; }
   name = DOToString(arg1);
   llvm::Instruction* inst0 = (llvm::Instruction*)(PointerAddress)DOToLong(arg0);
   llvm::Instruction* clone = inst0->clone();
   if(inst0->hasName()) {
      clone->setName(Twine(name));
   }
   PointerAddress pa = (PointerAddress)clone;
   return pa;
}

extern "C" void* UnlinkAndMoveInstructionBefore(void *theEnv) {
   //this function only updates the llvm portion...it's up to clips
   //to update it's portion
   DATA_OBJECT arg0, arg1;
   if((EnvArgCountCheck(theEnv, "llvm-unlink-and-move-instruction-before", EXACTLY, 2) == -1)) { return FalseSymbol(); }
   if((EnvArgTypeCheck(theEnv, "llvm-unlink-and-move-instruction-before", 1, INTEGER, &arg0) == -1)) { return FalseSymbol(); }
   if((EnvArgTypeCheck(theEnv, "llvm-unlink-and-move-instruction-before", 2, INTEGER, &arg1) == -1)) { return FalseSymbol(); }
   llvm::Instruction* from = (llvm::Instruction*)(PointerAddress)DOToLong(arg0);
   llvm::Instruction* before = (llvm::Instruction*)(PointerAddress)DOToLong(arg1);
   from->removeFromParent();
   from->insertBefore(before);
   return TrueSymbol();
}

extern "C" void* UnlinkAndMoveInstructionAfter(void *theEnv) {
   //this function only updates the llvm portion...it's up to clips
   //to update it's portion
   DATA_OBJECT arg0, arg1;
   if((EnvArgCountCheck(theEnv, "llvm-unlink-and-move-instruction-after", EXACTLY, 2) == -1)) { return FalseSymbol(); }
   if((EnvArgTypeCheck(theEnv, "llvm-unlink-and-move-instruction-after", 1, INTEGER, &arg0) == -1)) { return FalseSymbol(); }
   if((EnvArgTypeCheck(theEnv, "llvm-unlink-and-move-instruction-after", 2, INTEGER, &arg1) == -1)) { return FalseSymbol(); }
   llvm::Instruction* from = (llvm::Instruction*)(PointerAddress)DOToLong(arg0);
   llvm::Instruction* after = (llvm::Instruction*)(PointerAddress)DOToLong(arg1);
   from->removeFromParent();
   from->insertAfter(after);
   return TrueSymbol();
}
extern "C" void* MoveInstructionBefore(void *theEnv) {
   //this function only updates the llvm portion...it's up to clips
   //to update it's portion
   DATA_OBJECT arg0, arg1;
   if((EnvArgCountCheck(theEnv, "llvm-move-instruction-before", EXACTLY, 2) == -1)) { return FalseSymbol(); }
   if((EnvArgTypeCheck(theEnv, "llvm-move-instruction-before", 1, INTEGER, &arg0) == -1)) { return FalseSymbol(); }
   if((EnvArgTypeCheck(theEnv, "llvm-move-instruction-before", 2, INTEGER, &arg1) == -1)) { return FalseSymbol(); }
   llvm::Instruction* from = (llvm::Instruction*)(PointerAddress)DOToLong(arg0);
   llvm::Instruction* before = (llvm::Instruction*)(PointerAddress)DOToLong(arg1);
   from->insertBefore(before);
   return TrueSymbol();
}

extern "C" void* MoveInstructionAfter(void *theEnv) {
   //this function only updates the llvm portion...it's up to clips
   //to update it's portion
   DATA_OBJECT arg0, arg1;
   if((EnvArgCountCheck(theEnv, "llvm-move-instruction-after", EXACTLY, 2) == -1)) { return FalseSymbol(); }
   if((EnvArgTypeCheck(theEnv, "llvm-move-instruction-after", 1, INTEGER, &arg0) == -1)) { return FalseSymbol(); }
   if((EnvArgTypeCheck(theEnv, "llvm-move-instruction-after", 2, INTEGER, &arg1) == -1)) { return FalseSymbol(); }
   llvm::Instruction* from = (llvm::Instruction*)(PointerAddress)DOToLong(arg0);
   llvm::Instruction* after = (llvm::Instruction*)(PointerAddress)DOToLong(arg1);
   from->insertAfter(after);
   return TrueSymbol();
}

extern "C" void* UnlinkAndDeleteInstruction(void *theEnv) {
   DATA_OBJECT arg0;
   if((EnvArgCountCheck(theEnv, "llvm-unlink-and-delete-instruction", EXACTLY, 1) == -1)) { return FalseSymbol(); }
   if((EnvArgTypeCheck(theEnv, "llvm-unlink-and-delete-instruction", 1, INTEGER, &arg0) == 0)) { return FalseSymbol(); }
   llvm::Instruction* inst0 = (llvm::Instruction*)(PointerAddress)DOToLong(arg0);
   inst0->eraseFromParent();
   return TrueSymbol();
}

extern "C" void* ScheduleInstructions(void *theEnv) {
   /* CLIPS Name: llvm-schedule-block
    * Input Arguments
    *  Arg0: PointerAddress of the TerminatorInstruction of the block
    *  Arg1: Multislot of pointers that are part of the block in question
    * Returns:
    *  A boolean value signifying success or failure
    */
   DATA_OBJECT arg0, arg1;
	void* multifieldPtr;
	int end, i;
   if((EnvArgCountCheck(theEnv, "llvm-schedule-block", EXACTLY, 2) == -1)) { return FalseSymbol(); }
   if((EnvArgTypeCheck(theEnv, "llvm-schedule-block", 1, INTEGER, &arg0) == -1)) { return FalseSymbol(); }
   if((EnvArgTypeCheck(theEnv, "llvm-schedule-block", 2, MULTIFIELD, &arg1) == -1)) { return FalseSymbol(); }
   llvm::Instruction* last = (llvm::Instruction*)(PointerAddress)DOToLong(arg0);
	end = GetDOEnd(arg1);
	multifieldPtr = GetValue(arg1);
	for(i = GetDOBegin(arg1); i <= end; i++) {
		if(GetMFType(multifieldPtr, i) == INTEGER) {
         llvm::Instruction* tmpInst = (llvm::Instruction*)(PointerAddress)ValueToLong(GetMFValue(multifieldPtr, i));
         tmpInst->removeFromParent();
         tmpInst->insertBefore(last);
		} else {
         return FalseSymbol();
      }
	}
   return TrueSymbol();
}
extern "C" void* ReplaceAllUsesOf(void* theEnv) {
	DATA_OBJECT arg0, arg1;
   if((EnvArgCountCheck(theEnv, "llvm-replace-all-uses", EXACTLY, 2) == -1)) { return FalseSymbol(); }
   if((EnvArgTypeCheck(theEnv, "llvm-replace-all-uses", 1, INTEGER, &arg0) == -1)) { return FalseSymbol(); }
   if((EnvArgTypeCheck(theEnv, "llvm-replace-all-uses", 2, INTEGER, &arg1) == -1)) { return FalseSymbol(); }
   llvm::Instruction* from = (llvm::Instruction*)(PointerAddress)DOToLong(arg0);
   llvm::Instruction* to = (llvm::Instruction*)(PointerAddress)DOToLong(arg1);
	from->replaceAllUsesWith(to);
	return TrueSymbol();
}
extern "C" void* ReplaceUsesOf(void *theEnv) {
   /* CLIPS Name: llvm-replace-uses
    * Input Arguments
    *  Arg0: PointerAddress of the original name
    *  Arg1: PointerAddress of the new name 
    *  Arg1: Multislot of pointers that are the instructions to replace 
    * Returns:
    *  A boolean value signifying success or failure
    */
   DATA_OBJECT arg0, arg1, arg2;
	void* multifieldPtr;
	int end, i;
   if((EnvArgCountCheck(theEnv, "llvm-replace-uses", EXACTLY, 3) == -1)) { return FalseSymbol(); }
   if((EnvArgTypeCheck(theEnv, "llvm-replace-uses", 1, INTEGER, &arg0) == -1)) { return FalseSymbol(); }
   if((EnvArgTypeCheck(theEnv, "llvm-replace-uses", 2, INTEGER, &arg1) == -1)) { return FalseSymbol(); }
   if((EnvArgTypeCheck(theEnv, "llvm-replace-uses", 3, MULTIFIELD, &arg2) == -1)) { return FalseSymbol(); }
   llvm::Instruction* from = (llvm::Instruction*)(PointerAddress)DOToLong(arg0);
   llvm::Instruction* to = (llvm::Instruction*)(PointerAddress)DOToLong(arg1);
	end = GetDOEnd(arg2);
	multifieldPtr = GetValue(arg2);
	for(i = GetDOBegin(arg2); i <= end; i++) {
		if(GetMFType(multifieldPtr, i) == INTEGER) {
         llvm::Instruction* tmpInst = (llvm::Instruction*)(PointerAddress)ValueToLong(GetMFValue(multifieldPtr, i));
         tmpInst->replaceUsesOfWith(from, to);
		} else {
         return FalseSymbol();
      }
	}
   return TrueSymbol();
}
extern "C" PointerAddress CreatePhiNode(void *theEnv) {
   /* CLIPS Name: llvm-make-phi-node
    * Input Arguments
    *  Arg0: Name
    *  Arg1: Pointer to DataType
    *  Arg2: Count of 
    *  Arg0: PointerAddress of the original name
    *  Arg1: PointerAddress of the new name 
    *  Arg1: Multislot of pointers that are the instructions to replace 
    * Returns:
    *  The pointer address to the phi node
    */
    //(Pointer (llvm-make-phi-node ?name ?bPtr ?count $?pointers))
   DATA_OBJECT arg0, arg1, arg2, arg3, arg4;
	void* multifieldPtr;
	int end, i;
   if((EnvArgCountCheck(theEnv, createPhiNodeTitle, EXACTLY, 5) == -1)) { return 0L; }
   if((EnvArgTypeCheck(theEnv, createPhiNodeTitle, 1, SYMBOL, &arg0) == 0)) { return 0L; }
   if((EnvArgTypeCheck(theEnv, createPhiNodeTitle, 2, INTEGER, &arg1) == -1)) { return 0L; }
   if((EnvArgTypeCheck(theEnv, createPhiNodeTitle, 3, INTEGER, &arg2) == -1)) { return 0L; }
   if((EnvArgTypeCheck(theEnv, createPhiNodeTitle, 4, INTEGER, &arg3) == -1)) { return 0L; }
   if((EnvArgTypeCheck(theEnv, createPhiNodeTitle, 5, MULTIFIELD, &arg4) == -1)) { return 0L; }
   llvm::Type* dataType = (llvm::Type*)(PointerAddress)DOToLong(arg1);
   unsigned count = (unsigned)DOToLong(arg2);
   int index = 0;
   llvm::Instruction* bPtr = (llvm::Instruction*)(PointerAddress)DOToLong(arg3);
   llvm::PHINode* phi = llvm::PHINode::Create(dataType, count, Twine(DOToString(arg0)), bPtr);
   PointerAddress phiPtr = (PointerAddress)phi;
	end = GetDOEnd(arg4);
	multifieldPtr = GetValue(arg4);
	for(i = GetDOBegin(arg4); i <= end; i+=2, index++) {
			PointerAddress aliasPtr = (PointerAddress)ValueToLong(GetMFValue(multifieldPtr, i));
			PointerAddress blockPtr = (PointerAddress)ValueToLong(GetMFValue(multifieldPtr, i + 1));
         llvm::Instruction* alias = (llvm::Instruction*)aliasPtr;
			llvm::BasicBlock* block = (llvm::BasicBlock*)blockPtr;
			phi->addIncoming(alias, block);
	}
   return phiPtr;
}

extern "C" void* UnlinkInstruction(void *theEnv) {
   DATA_OBJECT arg0;
   if((EnvArgCountCheck(theEnv, "llvm-unlink-instruction", EXACTLY, 1) == -1)) { return FalseSymbol(); }
   if((EnvArgTypeCheck(theEnv, "llvm-unlink-instruction", 1, INTEGER, &arg0) == -1)) { return FalseSymbol(); }
   llvm::Instruction* inst = (llvm::Instruction*)(PointerAddress)DOToLong(arg0);
	inst->removeFromParent();
	return TrueSymbol();
}

extern "C" void* DeleteInstruction(void *theEnv) {
   DATA_OBJECT arg0;
   if((EnvArgCountCheck(theEnv, "llvm-delete-instruction", EXACTLY, 1) == -1)) { return FalseSymbol(); }
   if((EnvArgTypeCheck(theEnv, "llvm-delete-instruction", 1, INTEGER, &arg0) == -1)) { return FalseSymbol(); }
   llvm::Instruction* inst = (llvm::Instruction*)(PointerAddress)DOToLong(arg0);
	delete inst;
	return TrueSymbol();
}
                        
void InitializeRuntimeAnalyzer() {
	InitializeEnvironment();
	SetConserveMemory(1);
	BatchStar("Init.clp");
}

