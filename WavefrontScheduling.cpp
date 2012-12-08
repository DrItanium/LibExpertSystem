/* This pass is meant to show how one uses the Knowledge Construction Engine
 * (KCE). It's pretty simple. Running this pass requires a lot more work and
 * I'm...heh....working on that. 
 * - Joshua Scoggins (8/12/2012)
 */
#include "llvm/Pass.h"
#include "llvm/Function.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/RegionInfo.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/DenseSet.h"
#include "LLVMWrapper.h"
#include "JSEdgeRemoval.h"
#include "PassDependencies.h"
#include "EnvironmentCreationPass.h"
#include "KnowledgeConstructionPass.h"
extern "C" {
#include "clips.h"
}

using namespace llvm;
char nilObject[41] = "(nil of ConstantPointerNull (Pointer 0))";
namespace llvm {
   struct WavefrontScheduling : public FunctionPass {
      static char ID;
      public:
      WavefrontScheduling() : FunctionPass(ID) {
      }
      virtual void getAnalysisUsage(AnalysisUsage &Info) const {
			Info.addRequired<LoopInfo>();
			Info.addRequired<RegionInfo>();
			Info.addRequired<JSEdgeRemoval>();
      }
      virtual bool runOnFunction(Function& fn) {
         //do not actually wavefront schedule if we only have one block
         if(fn.size() > 1) {

				RegionInfo &ri = getAnalysis<RegionInfo>();
				LoopInfo &li = getAnalysis<LoopInfo>();
				EnvironmentConstruction env;
				env.batchStar("Init.clp");
				env.reset();
				env.makeInstance(nilObject);
				KnowledgeConstruction kc(env);
				kc.route(fn, li, ri);
				env.run(-1L);
            return true;
         } else {
            return false;
         }
      }
   };
}
char WavefrontScheduling::ID = 0;
//for opt
static RegisterPass<WavefrontScheduling> wave("wavefront", "Wavefront Scheduling", false, false);
INITIALIZE_PASS_BEGIN(WavefrontScheduling, "wavefront", "Wavefront Scheduling", false, false)
INITIALIZE_PASS_DEPENDENCY(JSEdgeRemoval)
INITIALIZE_PASS_END(WavefrontScheduling, "wavefront", "Wavefront Scheduling", false, false)
