#ifndef _knowledge_construction_pass_h
#define _knowledge_construction_pass_h
#include "llvm/Pass.h"
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
#include "KnowledgeConstructionEngine.h"
#include "PassDependencies.h"
#include "EnvironmentCreationPass.h"
#include "FunctionNamer.h"
#include "llvm/ADT/DenseSet.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/Value.h"
#include <string>
#include <map>
extern "C" {
#include "clips.h"
}

using namespace llvm;
//char nilObject[41] = "(nil of ConstantPointerNull (Pointer 0))";
namespace {
	struct KnowledgeConstruction : public FunctionPass {
		static char ID;
		llvm::DenseMap<PointerAddress, std::string>* instances;
		public:
		KnowledgeConstruction() : FunctionPass(ID) {
			instances = new llvm::DenseMap<PointerAddress, std::string>();
		}
		~KnowledgeConstruction();
		llvm::DenseMap<PointerAddress, std::string>* getInstances() { return instances; }
		virtual void getAnalysisUsage(AnalysisUsage &Info) const {
			Info.addRequired<LoopInfo>();
			Info.addRequired<RegionInfo>();
			Info.addRequired<FunctionNamer>();
			//we don't need the environment yet and it's actually smarter to just
			//wait until the custom pass so that the programmer has complete
			//control over the code
		}
		//TODO: add the route commands

		std::string route(Value* val, char* parent, FunctionNamer& namer);
		std::string route(Value* val, FunctionNamer& namer);
		std::string route(User* user, char* parent, FunctionNamer& namer);
		std::string route(Constant* cnst, FunctionNamer& namer);
		std::string route(Constant* cnst, char* parent, FunctionNamer& namer);
		std::string route(Instruction* inst, char* parent, FunctionNamer& namer);
		std::string route(Type* t, FunctionNamer& namer);
		std::string route(Operator* op, char* parent, FunctionNamer& namer);
		std::string route(BasicBlock* bb, char* parent, FunctionNamer& namer, bool constructInstructions = true);
		std::string route(Region* region, char* parent, FunctionNamer& namer);
		std::string route(Argument* arg, char* parent, FunctionNamer& namer);
		std::string route(Loop* loop, char* parent, FunctionNamer& namer);
		void route(RegionInfo& ri, char* parent, FunctionNamer& namer);
		void route(LoopInfo& li, char* parent, FunctionNamer& namer);
		void updateFunctionContents(Function& fn, FunctionNamer& namer);
		virtual bool runOnFunction(Function& fn) {
			char* funcName;
			//get the function namer object
			LoopInfo &loops = getAnalysisUsage<LoopInfo>();
			RegionInfo &regions = getAnalysisUsage<RegionInfo>();
			FunctionNamer &namer = getAnalysisUsage<FunctionNamer>();
			funcName = (char*)fn.getName().data();
			fn->reset();
			instances->clear();
			updateFunctionContents(fn, namer);
			route(loops, funcName, namer);
			route(regions, funcName, namer);
			//at this point we shall have a set of instance strings
			return false;
		}
	};
}
#endif
