#include "EnvironmentCreationPass.h"

using namespace llvm;
bool EnvironmentConstruction::environmentCreated() { return (environment != 0); }
void* EnvironmentConstruction::getEnvironment() { return environment; }
void EnvironmentConstruction::reset() { 
	if(environment) {
		EnvReset(environment); 
	}
}
void EnvironmentConstruction::clear() {
	if(environment) {
		EnvClear(environment);
	}
}
void EnvironmentConstruction::run(ExecutionDuration count) {
	if(environment) {
		if(count <= 0L) {
			EnvRun(environment, -1L);
		} else {
			EnvRun(environment, count);
		}
	}
}
void EnvironmentConstruction::saveInstances(char* fileName) {
	if(environment) {
		EnvSaveInstances(environment, fileName, VISIBLE_SAVE, NULL, TRUE);
	}
}
void EnvironmentConstruction::assertFact(char* str) {
	if(environment) {
		EnvAssertString(environment, str);
	}
}
bool EnvironmentConstruction::makeInstances(char* str) {
	if(environment) {
		return EnvLoadInstancesFromString(environment, str, -1) != -1;
	} else {
		return false;
	}
}
bool EnvironmentConstruction::makeInstance(char* str) {
	if(environment) {
		return (EnvMakeInstance(environment, str) != NULL);
	} else {
		return false;
	}
}
void EnvironmentConstruction::batchStar(char* str) {
	if(environment) {
		EnvBatchStar(environment, str);
	}
}
char EnvironmentConstruction::ID = 0;
//for opt
static RegisterPass<EnvironmentConstruction> env("environment", "environment constructor", false, true);
INITIALIZE_PASS(EnvironmentConstruction, "environment", "environment constructor", false, true)
