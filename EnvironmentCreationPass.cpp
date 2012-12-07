#include "llvm/Pass.h"
#include "llvm/Function.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/RegionInfo.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/DenseSet.h"
#include "PassDependencies.h"
extern "C" {
#include "clips.h"
}

using namespace llvm;
namespace {
	typedef long long ExecutionDuration;
	struct EnvironmentConstruction : public FunctionPass {
		static char ID;
		void* environment;
		public:
		EnvironmentConstruction() : FunctionPass(ID) {

		}
		~EnvironmentConstruction() {
			if(environment) {
				DestroyEnvironment(environment);
			}
		}
		bool environmentCreated() { return (environment != 0); }
		void* getEnvironment() { return environment; }
		void resetEnvironment() { 
			if(environment) {
				EnvReset(environment); 
			}
		}
		void clearEnvironment() {
			if(environment) {
				EnvClear(environment);
			}
		}
		void runEnvironment(ExecutionDuration count) {
			if(environment) {
				if(count <= 0L) {
					EnvRun(environment, -1L);
				} else {
					EnvRun(environment, count);
				}
			}
		}
		void saveInstances(char* fileName) {
			if(environment) {
			  EnvSaveInstances(environment, fileName, VISIBLE_SAVE, NULL, TRUE);
			}
		}
		void assertFact(char* str) {
			if(environment) {
				EnvAssertString(environment, str);
			}
		}
		bool makeInstances(char* str) {
			if(environment) {
				return EnvLoadInstancesFromString(environment, str, -1) != -1;
			} else {
				return false;
			}
		}
		bool makeInstance(char* str) {
			if(environment) {
				return (EnvMakeInstance(environment, str) != NULL);
			} else {
				return false;
			}
		}
		void batchStar(char* str) {
			if(environment) {
				EnvBatchStar(environment, str);
			}
		}
		virtual bool runOnFunction(Function& fn) {
			environment = CreateEnvironment();
			return false;
		}
	};
}
char EnvironmentConstruction::ID = 0;
INITIALIZE_PASS_BEGIN(EnvironmentConstruction, "environment", "environment constructor", false, false)
INITIALIZE_PASS_END(EnvironmentConstruction, "environment", "environment constructor", false, false)
//for opt
static RegisterPass<EnvironmentConstruction> X("environment", "environment constructor", false, false);
