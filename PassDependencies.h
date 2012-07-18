#ifndef _passdependencies_h
#define _passdependencies_h
#include "llvm/Pass.h"
#include "llvm/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/CFG.h"
namespace llvm {
   void initializeWavefrontSchedulingPass(PassRegistry &Registry);
   void initializeJSEdgeRemovalPass(PassRegistry &Registry);
}
#endif
