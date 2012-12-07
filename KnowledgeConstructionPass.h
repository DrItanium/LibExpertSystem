#ifndef _knowledge_construction_pass_h
#define _knowledge_construction_pass_h
#include "llvm/Pass.h"
#include "llvm/Function.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/RegionInfo.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/DenseSet.h"
#include "KnowledgeConstructionEngine.h"
#include "LLVMWrapper.h"
#include "JSEdgeRemoval.h"
#include "PassDependencies.h"
#include "EnvironmentCreationPass.h"
extern "C" {
  #include "clips.h"
}

using namespace llvm;
//char nilObject[41] = "(nil of ConstantPointerNull (Pointer 0))";
namespace {
   struct KnowledgeConstruction : public FunctionPass {
      static char ID;
      public:
      KnowledgeConstruction() : FunctionPass(ID) { }
      virtual void getAnalysisUsage(AnalysisUsage &Info) const {
         Info.addRequired<LoopInfo>();
         Info.addRequired<RegionInfo>();
			Info.addRequired<EnvironmentConstruction>();
      }
		//TODO: add the route commands

      virtual bool runOnFunction(Function& fn) {
			EnvironmentConstruction &env = getAnalysisUsage<EnvironmentConstruction>();
			return false;
      }
   };
}
#endif
