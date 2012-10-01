#ifndef multifield_builder_h
#define multifield_builder_h
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Casting.h"
#include "llvm/Instruction.h"
#include "llvm/Instructions.h"
#include "llvm/IntrinsicInst.h"
#include "llvm/Argument.h"
#include "llvm/Analysis/RegionInfo.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/User.h"
#include "llvm/Value.h"
#include "llvm/Type.h"
#include "llvm/BasicBlock.h"
#include "llvm/Analysis/RegionIterator.h"
#include "llvm/Operator.h"
#include "llvm/Metadata.h"
#include "llvm/InlineAsm.h"
#include "llvm/Argument.h"
#include "llvm/Constant.h"
#include "llvm/Constants.h"
#include "llvm/GlobalVariable.h"
#include "llvm/GlobalValue.h"
#include "llvm/GlobalAlias.h"
#include "llvm/CodeGen/PseudoSourceValue.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Operator.h"
#include "llvm/Function.h"
#include "llvm/Analysis/LoopInfo.h"
#include "FunctionNamer.h"

extern "C" {
#include "clips.h"
}
using namespace llvm;
class MultifieldBuilder {
   private:
      void* multifieldPointer;
      unsigned count;
   public:
      MultifieldBuilder(unsigned size) {
         multifieldPointer = CreateMultifield(size);
         count = size;
      }
      unsigned getCount();
      void* getMultifieldPointer();
      void setType(unsigned index, int type);
      void setValue(unsigned index, void *value);
      void setSlot(unsigned index, int type, void* value);
      void setSlot(unsigned index, PointerAddress value);
      void setSlot(unsigned index, long value);
      void setSlot(unsigned index, int value);
      void setSlot(unsigned index, unsigned value);
      void setSlot(unsigned index, float value);
      void setSlot(unsigned index, double value);
      void setSlot(unsigned index, char* v, bool isStringType = false);
      void setSlot(unsigned index, const char* v, bool isStringType = false);
      void setSlot(unsigned index, std::string string, bool isStringType = false);
      void setSlot(unsigned index, llvm::StringRef ref, bool isStringType = false);
      void setTrue(unsigned index);
      void setFalse(unsigned index);
      void bindToDataObjectPointer(DATA_OBJECT_PTR ptr);
};
#endif
