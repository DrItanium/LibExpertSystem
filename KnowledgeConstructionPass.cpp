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
#include "KnowledgeConstructionEngine.h"
#include "LLVMWrapper.h"
#include "JSEdgeRemoval.h"
#include "PassDependencies.h"
extern "C" {
  #include "clips.h"
}

using namespace llvm;
char nilObject[41] = "(nil of ConstantPointerNull (Pointer 0))";
namespace {
   struct KnowledgeConstruction : public FunctionPass {
      static char ID;
      public:
      KnowledgeConstruction() : FunctionPass(ID) {
         //InitializeRuntimeAnalyzer();
      }
      virtual void getAnalysisUsage(AnalysisUsage &Info) const {
         Info.addRequired<LoopInfo>();
         Info.addRequired<RegionInfo>();
      }
      virtual bool runOnFunction(Function& fn) {
         //do not actually wavefront schedule if we only have one block
			/*
         if(fn.size() > 1) {
            char* funcName = (char*)fn.getName().data();
            DenseMap<PointerAddress, std::string> translation;
            std::pair<PointerAddress, std::string> pair(0,"nil");
            translation.insert(pair);
            FunctionNamer namer (translation);
            Reset();
            MakeInstance(nilObject);
            ModifyFunctionContents(fn, namer);
            LoopInfo &li = getAnalysis<LoopInfo>();
            RouteLoopInfo(li, funcName, namer);
            RegionInfo &ri = getAnalysis<RegionInfo>();
            llvm::Region* top = ri.getTopLevelRegion();
            Route(top, funcName, namer);
            Run(-1L);
            return true;
         } else {
            return false;
         }
			*/
      }
   };
}
char KnowledgeConstruction::ID = 0;
INITIALIZE_PASS_BEGIN(KnowledgeConstruction, "knowledge", "Knowledge constructor", false, false)
INITIALIZE_PASS_END(KnowledgeConstruction, "knowledge", "Knowledge constructor", false, false)
//for opt
static RegisterPass<KnowledgeConstruction> X("knowledge", "Knowledge constructor", false, false);
