#include "llvm_io_router.h"
#include "llvm/Support/raw_ostream.h"
extern "C" {
   #include "clips.h"
}

extern "C" int FindLLVM(void *,char *);
extern "C" int ExitLLVM(void *,int);
extern "C" int PrintLLVM(void *,char *,char *);

extern "C" void SetupLLVMIORouter(void* theEnv) {
	EnvAddRouter(theEnv, "llvm", 40, FindLLVM, PrintLLVM,
			NULL, NULL, ExitLLVM);
}

extern "C" int FindLLVM(void* theEnv, char * logicalName) {

   if ((strcmp(logicalName,"stdout") == 0) ||
         (strcmp(logicalName, "llvm") == 0) ||
         (strcmp(logicalName, WDISPLAY) == 0) ||
         (strcmp(logicalName,WERROR) == 0))
   { return(TRUE); }
   return(FALSE);
}

extern "C" int ExitLLVM(void* theEnv, int num) {
	//Do nothing
   return (1);
}

extern "C" int PrintLLVM(void* theEnv, char* logicalName, char* str) {
	llvm::errs() << str;
	//do not deactivate any routers we are capturing everything through
	//llvm::errs()
   return (1);
}
