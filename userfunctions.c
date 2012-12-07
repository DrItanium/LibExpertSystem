/*******************************************************/
/*      "C" Language Integrated Production System      */
/*                                                     */
/*               CLIPS Version 6.24  04/21/06          */
/*                                                     */
/*                USER FUNCTIONS MODULE                */
/*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Created file to seperate UserFunctions and     */
/*            EnvUserFunctions from main.c.                  */
/*                                                           */
/*************************************************************/

/***************************************************************************/
/*                                                                         */
/* Permission is hereby granted, free of charge, to any person obtaining   */
/* a copy of this software and associated documentation files (the         */
/* "Software"), to deal in the Software without restriction, including     */
/* without limitation the rights to use, copy, modify, merge, publish,     */
/* distribute, and/or sell copies of the Software, and to permit persons   */
/* to whom the Software is furnished to do so.                             */
/*                                                                         */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS */
/* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF              */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT   */
/* OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY  */
/* CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES */
/* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN   */
/* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF */
/* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.          */
/*                                                                         */
/***************************************************************************/

#include "clips.h"
void UserFunctions(void);
void EnvUserFunctions(void *);

/*********************************************************/
/* UserFunctions: Informs the expert system environment  */
/*   of any user defined functions. In the default case, */
/*   there are no user defined functions. To define      */
/*   functions, either this function must be replaced by */
/*   a function with the same name within this file, or  */
/*   this function can be deleted from this file and     */
/*   included in another file.                           */
/*********************************************************/
void UserFunctions()
{   
}

/***********************************************************/
/* EnvUserFunctions: Informs the expert system environment */
/*   of any user defined functions. In the default case,   */
/*   there are no user defined functions. To define        */
/*   functions, either this function must be replaced by   */
/*   a function with the same name within this file, or    */
/*   this function can be deleted from this file and       */
/*   included in another file.                             */
/***********************************************************/
#if WIN_BTC
#pragma argsused
#endif
typedef long long PointerAddress;
void EnvUserFunctions(
      void *theEnv)
{
   extern void LLVMPrint(void *theEnv);
   extern void BatchLoad(void *theEnv);
   extern void* IsIdenticalTo(void *theEnv);
   extern void* IsIdenticalToWhenDefined(void *theEnv);
   extern void* IsSameOperationAs(void *theEnv);
   extern PointerAddress CloneInstruction(void *theEnv);
   extern PointerAddress CreatePhiNode(void *theEnv);
   extern void* MoveInstructionBefore(void *theEnv);
   extern void* MoveInstructionAfter(void *theEnv);
   extern void* UnlinkAndMoveInstructionBefore(void *theEnv);
   extern void* UnlinkAndMoveInstructionAfter(void *theEnv);
   extern void* UnlinkAndDeleteInstruction(void *theEnv);
   extern void* ScheduleInstructions(void *theEnv);
   extern void* ReplaceUsesOf(void *theEnv);
   extern void* ReplaceAllUsesOf(void *theEnv);
	extern void* UnlinkInstruction(void* theEnv);
	extern void* DeleteInstruction(void* theEnv);

   EnvDefineFunction(theEnv, "llvm-print",'v', PTIEF LLVMPrint, "LLVMPrint");
   EnvDefineFunction(theEnv, "batch-load",'v', PTIEF BatchLoad, "BatchLoad");
   EnvDefineFunction(theEnv, "llvm-instruction-is-identical-to", 'w', PTIEF IsIdenticalTo, "IsIdenticalTo");
   EnvDefineFunction(theEnv, "llvm-instruction-is-identical-to-when-defined", 'w', PTIEF IsIdenticalToWhenDefined, "IsIdenticalToWhenDefined");
   EnvDefineFunction(theEnv, "llvm-instruction-is-same-operation-as", 'w', PTIEF IsSameOperationAs, "IsSameOperationAs");
   EnvDefineFunction(theEnv, "llvm-move-instruction-after", 'w', PTIEF MoveInstructionAfter, "MoveInstructionAfter");
   EnvDefineFunction(theEnv, "llvm-move-instruction-before", 'w', PTIEF MoveInstructionBefore, "MoveInstructionBefore");
   EnvDefineFunction(theEnv, "llvm-unlink-and-move-instruction-after", 'w',  PTIEF UnlinkAndMoveInstructionAfter, "UnlinkAndMoveInstructionAfter");
   EnvDefineFunction(theEnv, "llvm-unlink-and-move-instruction-before", 'w', PTIEF UnlinkAndMoveInstructionBefore, "UnlinkAndMoveInstructionBefore");
   EnvDefineFunction(theEnv, "llvm-unlink-and-delete-instruction", 'w', PTIEF UnlinkAndDeleteInstruction, "UnlinkAndDeleteInstruction");
   EnvDefineFunction(theEnv, "llvm-clone-instruction", 'l', PTIEF CloneInstruction, "CloneInstruction");
   EnvDefineFunction(theEnv, "llvm-make-phi-node", 'l', PTIEF CreatePhiNode, "CreatePhiNode");
   EnvDefineFunction(theEnv, "llvm-replace-uses", 'w', PTIEF ReplaceUsesOf, "ReplaceUsesOf");
   EnvDefineFunction(theEnv, "llvm-replace-all-uses", 'w', PTIEF ReplaceAllUsesOf, "ReplaceAllUsesOf");
   EnvDefineFunction(theEnv, "llvm-unlink-instruction", 'w', PTIEF UnlinkInstruction, "UnlinkInstruction");
   EnvDefineFunction(theEnv, "llvm-delete-instruction", 'w', PTIEF DeleteInstruction, "DeleteInstruction");
   EnvDefineFunction(theEnv, "llvm-schedule-block", 'w', PTIEF ScheduleInstructions, "ScheduleInstructions");
	SetupLLVMIORouter(theEnv);


#if MAC_MCW || WIN_MCW || MAC_XCD
#pragma unused(theEnv)
#endif
}

