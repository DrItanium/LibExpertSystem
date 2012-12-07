#include "ConstructionTools.h"
using namespace llvm;
char openParen[2] = "(";
char closeParen[2] = ")";
char space[2] = " ";
CLIPSObjectBuilder::CLIPSObjectBuilder(std::string n, std::string t, FunctionNamer& uidCreator) {
   converted = false;
   opened = false;
   closed = false;
   name = n;
   type = t;
   stream = new raw_string_ostream(tmp);
   namer = &uidCreator;
}
CLIPSObjectBuilder::~CLIPSObjectBuilder() {
   delete stream;
}
void CLIPSObjectBuilder::setParent(char* p) { addField("Parent", p); }
void CLIPSObjectBuilder::setParent(const char* p) { addField("Parent", p); }
void CLIPSObjectBuilder::setPointer(PointerAddress v) { addField("Pointer", v); }
void CLIPSObjectBuilder::addField(const char* n, unsigned v) { 
   if(v != 0) {
      (*stream) << openParen << n << space << v << closeParen; 
   }
}
void CLIPSObjectBuilder::addField(const char* n, PointerAddress v) { 
   if(v != 0) {
      (*stream) << openParen << n << space << v << closeParen; 
   }
}
void CLIPSObjectBuilder::addField(const char* n, float v) { 
   (*stream) << openParen << n << space << v << closeParen; 
}
void CLIPSObjectBuilder::addField(const char* n, double v) { 
   (*stream) << openParen << n << space << v << closeParen; 
}
void CLIPSObjectBuilder::addField(const char* n, char* v) { 
   (*stream) << openParen << n << space << v << closeParen; 
}
void CLIPSObjectBuilder::addField(const char* n, const char* v) { 
   (*stream) << openParen << n << space << v << closeParen; 
}
void CLIPSObjectBuilder::addField(const char* n, std::string v) {
   if(v.length() > 0) (*stream) << openParen << n << space << v << closeParen; 
}
void CLIPSObjectBuilder::addField(const char* n, bool v) { 
   (*stream) << openParen << n;
   if(v) {
      (*stream) << " TRUE)";
   } else {
      (*stream) << " FALSE)";
   }
}
void CLIPSObjectBuilder::addField(const char* n, int v) { 
   if(v != 0) {
      (*stream) << openParen << n << space << v << closeParen; 
   }
}
void CLIPSObjectBuilder::addField(const char* n, uint64_t v) { 
   if(v != 0) {
      (*stream) << openParen << n << space << v << closeParen; 
   }
}
void CLIPSObjectBuilder::addField(const char* n, int64_t v) {
   if(v != 0) {
      (*stream) << openParen << n << space << v << closeParen; 
   }
}
void CLIPSObjectBuilder::addField(const char* n, llvm::StringRef ref) {
   (*stream) << openParen << n << space << ref << closeParen;
}
void CLIPSObjectBuilder::open() {
   if(!opened) {
      (*stream) << "(" << name << " of " << type << " ";
      opened = true;
   }
}
void CLIPSObjectBuilder::close() {
   if(!closed) {
      (*stream) << closeParen; //close it
      closed = true;
   }
}
void CLIPSObjectBuilder::convertToKnowledge(void* theEnv) {
   if(closed && !converted) {
      EnvMakeInstance(theEnv, (char*)stream->str().c_str());
      converted = true;
   }
}
void CLIPSObjectBuilder::addFields(PointerAddress pointer, char* parent) {
   setParent(parent);
   setPointer(pointer);
   std::string& name = getName();
   namer->tryRegisterPointerToName(pointer, name);
}
void CLIPSObjectBuilder::addTrueField(const char* n) {
   (*stream) << openParen << n << " TRUE)";
}
void CLIPSObjectBuilder::addFalseField(const char* n) {
   (*stream) << openParen << n << " FALSE)";
}
void CLIPSObjectBuilder::openField(const char* name) {
   (*stream) << openParen << name;
}
void CLIPSObjectBuilder::appendValue(unsigned v) {
   (*stream) << space << v;
}
void CLIPSObjectBuilder::appendValue(PointerAddress v) {
   (*stream) << space << v;
}
void CLIPSObjectBuilder::appendValue(int v) {
   (*stream) << space << v;
}
void CLIPSObjectBuilder::appendValue(float v) {
   (*stream) << space << v;
}
void CLIPSObjectBuilder::appendValue(double v) {
   (*stream) << space << v;
}
void CLIPSObjectBuilder::appendValue(char* v) {
   (*stream) << space << v;
}
void CLIPSObjectBuilder::appendValue(const char* v) {
   (*stream) << space << v;
}
void CLIPSObjectBuilder::appendValue(std::string v) {
   if(!v.empty()) {
      (*stream) << space << v;
   }
}
void CLIPSObjectBuilder::appendValue(llvm::StringRef v) {
   (*stream) << space << v;
}
void CLIPSObjectBuilder::appendValue(bool v) {
   if(v) {
      appendTrue();
   } else {
      appendFalse();
   }
}
void CLIPSObjectBuilder::appendValue(uint64_t v) {
   (*stream) << space << v;
}
void CLIPSObjectBuilder::appendValue(int64_t v) {
   (*stream) << space << v;
}
void CLIPSObjectBuilder::appendTrue() {
   (*stream) << " TRUE";
}
void CLIPSObjectBuilder::appendFalse() {
   (*stream) << " FALSE";
}
void CLIPSObjectBuilder::closeField() {
   (*stream) << closeParen;
}
void CLIPSObjectBuilder::addStringField(const char* n, const std::string& string) {
   if(string.size() > 0) {
      (*stream) << "(" << n << " \"" << string << "\")";
   }
}
