#include "llvm_io_router.h"
#include "clips.h"

static int FindLLVM(void *,char *);
static int ExitLLVM(void *,int);
static int PrintLLVM(void *,char *,char *);

extern "C" void SetupLLVMIORouter(void* theEnv) {
	EnvAddRouter(theEnv, "llvm", 40, FindLLVM, PrintLLVM,
			NULL, NULL, ExitLLVM);
}

static int FindLLVM(void* theEnv, char * logicalName) {

   if ((strcmp(logicalName,"stdout") == 0) ||
         (strcmp(logicalName, "llvm") == 0) ||
         (strcmp(logicalName, WDISPLAY) == 0) ||
         (strcmp(logicalName,WERROR) == 0))
   { return(TRUE); }
   return(FALSE);
}

static int ExitLLVM(void* theEnv, int num) {
	//Do nothing
   return (1);
}

static int PrintLLVM(void* theEnv, char* logicalName, char* str) {
	llvm::errs() << str;
	//do not deactivate any routers we are capturing everything through
	//llvm::errs()
   return (1);
}
