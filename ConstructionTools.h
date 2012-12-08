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
#include "KnowledgeConstructionPass.h"
extern "C" {
#include "clips.h"
}
//#include "Routers.h"
#define CharBuffer(size) (char*)calloc(size, sizeof(char))

using namespace llvm;
class CLIPSObjectBuilder {
   private:
      bool converted;
      bool opened;
      bool closed;
      std::string name;
      std::string type;
      std::string tmp;
      raw_string_ostream* stream;
      FunctionNamer* namer;
   public:
      CLIPSObjectBuilder(std::string n, std::string t, FunctionNamer& uidCreator);
      ~CLIPSObjectBuilder();
		std::string getCompletedString() { return stream->str(); }
      std::string& getName() { return name; }
      std::string& getType() { return type; }
      FunctionNamer& getNamer() { return *namer; }
      raw_string_ostream& getStream() { return *stream; }
      bool isOpen() { return opened; }
      bool isClosed() { return closed; }
      bool isKnowledge() { return converted; }
      void setParent(char* p); 
      void setParent(const char* p); 
      void setPointer(PointerAddress v); 
      void addField(const char* n, unsigned v);
      void addField(const char* n, PointerAddress v); 
      void addField(const char* n, int v); 
      void addField(const char* n, float v);
      void addField(const char* n, double v); 
      void addField(const char* n, char* v); 
      void addField(const char* n, const char* v); 
      void addField(const char* n, std::string v); 
		void addField(const char* n, llvm::StringRef ref);
      void addField(const char* n, bool v); 
      void addField(const char* n, uint64_t v);
      void addField(const char* n, int64_t v); 
      void addStringField(const char* n, const std::string& str);
		void addTrueField(const char* n);
		void addFalseField(const char* n);
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
      void convertToKnowledge(void* theEnv);
      void addFields(PointerAddress pointer, KnowledgeConstruction* kc, char* parent);
};

class CLIPSValueBuilder : public CLIPSObjectBuilder {
   public:
      CLIPSValueBuilder(std::string nm, std::string ty, FunctionNamer& namer);
      void setType(Type* t, KnowledgeConstruction* kc);
      void addFields(Value* val, KnowledgeConstruction* kc, char* parent);
		void build(Value* val, KnowledgeConstruction* kc, char* parent);
};
class CLIPSUserBuilder : public CLIPSValueBuilder {
   public:
      CLIPSUserBuilder(std::string nm, std::string ty, FunctionNamer& namer);
      void addFields(User* user, KnowledgeConstruction* kc, char* parent);
		void build(User* user, KnowledgeConstruction* kc, char* parent);
};
//Thank you DOSBOX for this little tidbit :D
#endif
