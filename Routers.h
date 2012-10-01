#ifndef _routers_h
#define _routers_h
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Casting.h"
#include "llvm/Instruction.h"
#include "llvm/Instructions.h"
#include "llvm/IntrinsicInst.h"
#include "llvm/Argument.h"
#include "llvm/Analysis/RegionInfo.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/User.h"
#include "llvm/Value.h"
#include "llvm/Type.h"
#include "llvm/BasicBlock.h"
#include "llvm/Analysis/RegionIterator.h"
#include "llvm/Operator.h"
#include "llvm/Metadata.h"
#include "llvm/InlineAsm.h"
#include "llvm/Argument.h"
#include "llvm/Constant.h"
#include "llvm/Constants.h"
#include "llvm/GlobalVariable.h"
#include "llvm/GlobalValue.h"
#include "llvm/GlobalAlias.h"
#include "llvm/CodeGen/PseudoSourceValue.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Operator.h"
#include "llvm/Function.h"
#include "llvm/Analysis/LoopInfo.h"
#include "FunctionNamer.h"
#include "TypeLibrarian.h"
#include "llvm/Support/Casting.h"
extern "C" {
#include "clips.h"
}
using namespace llvm;
std::string Route(Value* val, char* parent, FunctionNamer& namer, TypeLibrarian& tl);
std::string Route(Value* val, FunctionNamer& namer, TypeLibrarian& tl);
std::string Route(User* user, char* parent, FunctionNamer& namer, TypeLibrarian& tl);
std::string Route(Constant* cnst, FunctionNamer& namer, TypeLibrarian& tl);
std::string Route(Constant* cnst, char* parent, FunctionNamer& namer, TypeLibrarian& tl);
std::string Route(Instruction* inst, char* parent, FunctionNamer& namer, TypeLibrarian& tl);
std::string Route(Type* t, FunctionNamer& namer, TypeLibrarian& tl);
std::string Route(Operator* op, char* parent, FunctionNamer& namer, TypeLibrarian& tl);
std::string Route(BasicBlock* bb, char* parent, FunctionNamer& namer, TypeLibrarian& tl, bool constructInstructions = true);
std::string Route(Region* region, char* parent, FunctionNamer& namer, TypeLibrarian& tl, LoopInfo* li);
std::string Route(Argument* arg, char* parent, FunctionNamer& namer, TypeLibrarian& tl);
std::string Route(Loop* loop, char* parent, FunctionNamer& namer, TypeLibrarian& tl);
void RouteLoopInfo(LoopInfo& li, char* parent, FunctionNamer& namer, TypeLibrarian& tl);
void ModifyFunctionContents(Function& fn, FunctionNamer& namer, TypeLibrarian& tl);
#endif
