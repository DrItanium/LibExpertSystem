#ifndef _clips_object_builder_h
#define _clips_object_builder_h
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
#include "TypeLibrarian.h"
#include "MultifieldBuilder.h"
extern "C" {
#include "clips.h"
}
#include "Routers.h"
#define CharBuffer(size) (char*)calloc(size, sizeof(char))
using namespace llvm;
class CLIPSObjectBuilder {
   private:
      void* rawInstance;
      std::string name;
      std::string type;
      FunctionNamer* namer;
      TypeLibrarian* tl;
   public:
      CLIPSObjectBuilder(std::string n, std::string t, FunctionNamer& uidCreator);
      ~CLIPSObjectBuilder();
      std::string& getName() { return name; }
      std::string& getType() { return type; }
      FunctionNamer& getNamer() { return *namer; }
      TypeLibrarian& getLibrarian() { return *tl; }
      void setParent(char* p); 
      void setParent(const char* p); 
      void setPointer(PointerAddress v); 
      void setField(const char* n, DATA_OBJECT_PTR ptr);
      void setField(const char* n, int type, void* value);
      void setField(const char* n, PointerAddress v);
      void setField(const char* n, unsigned v);
      void setField(const char* n, uint64_t v);
      void setField(const char* n, int64_t v);
      void setField(const char* n, int v);
      void setField(const char* n, long v);
      void setField(const char* n, float v);
      void setField(const char* n, double v);
      void setField(const char* n, char* v, bool isStringType = false);
      void setField(const char* n, const char* v, bool isStringType = false);
      void setField(const char* n, std::string v, bool isStringType = false);
      void setField(const char* n, llvm::StringRef v, bool isStringType = false);
      void setField(const char* n, bool v);
      void setFieldTrue(const char* n);
      void setFieldFalse(const char* n);
      void setField(const char* n, MultifieldBuilder* builder);
      void addField(const char* n, unsigned v);
      void addField(const char* n, PointerAddress v); 
      void addField(const char* n, int v); 
      void addField(const char* n, float v);
      void addField(const char* n, double v); 
      void addField(const char* n, char* v); 
      void addField(const char* n, const char* v); 
      void addField(const char* n, std::string v); 
		void addField(const char* n, llvm::StringRef v);
      void addField(const char* n, bool v); 
      void addField(const char* n, uint64_t v);
      void addField(const char* n, int64_t v); 
      void addStringField(const char* n, const std::string& str);
		void addTrueField(const char* n);
		void addFalseField(const char* n);
      /*
		void openField(const char* name);
		void appendValue(unsigned v);
		void appendValue(PointerAddress v);
		void appendValue(int v);
		void appendValue(float v);
		void appendValue(double v);
		void appendValue(char* v);
		void appendValue(const char* v);
		void appendValue(std::string v);
		void appendValue(llvm::StringRef ref);
		void appendValue(bool v);
		void appendValue(uint64_t v);
		void appendValue(int64_t v);
		void appendTrue();
		void appendFalse();
		void closeField();
      void open();
      void close();
      void convertToKnowledge();
      */
      void addFields(PointerAddress pointer, char* parent);
};

class CLIPSValueBuilder : public CLIPSObjectBuilder {
   public:
      CLIPSValueBuilder(std::string nm, std::string ty, FunctionNamer& namer);
      void setType(Type* t);
      void addFields(Value* val, char* parent);
};
class CLIPSUserBuilder : public CLIPSValueBuilder {
   public:
      CLIPSUserBuilder(std::string nm, std::string ty, FunctionNamer& namer);
      void addFields(User* user, char* parent);
};
#endif
