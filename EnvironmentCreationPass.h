#ifndef _environment_creation_pass_h
#define _environment_creation_pass_h
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
			environment = CreateEnvironment();
		}
		~EnvironmentConstruction() {
			if(environment) {
				DestroyEnvironment(environment);
			}
		}
		bool environmentCreated(); 
		void* getEnvironment();
		void reset();
		void clear();
		void run(ExecutionDuration count);
		void saveInstances(char* fileName);
		void assertFact(char* str);
		bool makeInstances(char* str);
		bool makeInstance(char* str);
		void batchStar(char* str);
		virtual bool runOnFunction(Function& fn) {
			clear();
			return false;
		}
	};
}
#endif
