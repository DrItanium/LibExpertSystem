#ifndef _knowledge_construction_pass_h
#define _knowledge_construction_pass_h
#include "llvm/Pass.h"
#include "llvm/Function.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/RegionInfo.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/DenseSet.h"
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
      virtual void getAnalysisUsage(AnalysisUsage &Info) const {
         Info.addRequired<LoopInfo>();
         Info.addRequired<RegionInfo>();
			Info.addRequired<FunctionNamer>();
			//we don't need the environment yet and it's actually smarter to just
			//wait until the custom pass so that the programmer has complete
			//control over the code
      }
		//TODO: add the route commands
	
      virtual bool runOnFunction(Function& fn) {
			//get the function namer object
			LoopInfo &loops = getAnalysisUsage<LoopInfo>();
			RegionInfo &regions = getAnalysisUsage<RegionInfo>();
			FunctionNamer &fn = getAnalysisUsage<FunctionNamer>();
			fn->reset();
			instances->clear();

			//EnvironmentConstruction &env = getAnalysisUsage<EnvironmentConstruction>();
			return false;
      }
   };
}
#endif
