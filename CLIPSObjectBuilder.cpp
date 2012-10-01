#include "ConstructionTools.h"
using namespace llvm;
char openParen[2] = "(";
char closeParen[2] = ")";
char space[2] = " ";
CLIPSObjectBuilder::CLIPSObjectBuilder(std::string n, std::string t, 
      FunctionNamer& uidCreator, TypeLibrarian& librarian) {
   void* classPtr;
   name = n;
   type = t;
   namer = &uidCreator;
   tl = &librarian; 
   classPtr = tl->lookupClass(t);
   char* buf = CharBuffer(1024); 
   sprintf(buf, "(%s of %s)", name.c_str(), t.c_str());
   rawInstance = MakeInstance(buf);
   free(buf);
   //rawInstance = CreateRawInstance(classPtr, (char*)name.c_str());

//   llvm::errs() << "Finished creating raw instance " << name << "\n";
}
CLIPSObjectBuilder::~CLIPSObjectBuilder() {
}
void CLIPSObjectBuilder::setField(const char* n, DATA_OBJECT_PTR ptr) {
   DirectPutSlot(rawInstance, (char*)n, ptr);
}
void CLIPSObjectBuilder::setField(const char* n, int type, void* value) {
   DATA_OBJECT tmp;
   tmp.type = type;
   tmp.value = value;
   DATA_OBJECT_PTR q = &tmp;
   setField(n, q);
}
void CLIPSObjectBuilder::setField(const char* n, PointerAddress v) {
   setField(n, INTEGER, AddLong(v));
}
void CLIPSObjectBuilder::setField(const char* n, unsigned v) {
   setField(n, INTEGER, AddLong(v));
}
void CLIPSObjectBuilder::setField(const char* n, uint64_t v) {
   setField(n, INTEGER, AddLong(v));
}
void CLIPSObjectBuilder::setField(const char* n, int64_t v) {
   setField(n, INTEGER, AddLong(v));
}
void CLIPSObjectBuilder::setField(const char* n, int v) {
   setField(n, INTEGER, AddLong(v));
}
void CLIPSObjectBuilder::setField(const char* n, float v) {
   setField(n, FLOAT, AddDouble(v));
}
void CLIPSObjectBuilder::setField(const char* n, double v) {
   setField(n, FLOAT, AddDouble(v));
}
void CLIPSObjectBuilder::setField(const char* n, char* v, bool isStringType) {
   void* data = AddSymbol(v);
   int type = SYMBOL;
   if(isStringType) type = STRING;
   setField(n, type, data);
}
void CLIPSObjectBuilder::setField(const char* n, const char* v, 
      bool isStringType) {
   void* data = AddSymbol((char*)v);
   int type = SYMBOL;
   if(isStringType) type = STRING;
   setField(n, type, data);
}
void CLIPSObjectBuilder::setField(const char* n, std::string v, 
      bool isStringType) {
   void* data = AddSymbol((char*)v.c_str());
   int type = SYMBOL;
   if(isStringType) type = STRING;
   setField(n, type, data);
}
void CLIPSObjectBuilder::setField(const char* n, llvm::StringRef v, 
      bool isStringType) {
   void* data = AddSymbol((char*)v.data());
   int type = SYMBOL;
   if(isStringType) type = STRING;
   setField(n, type, data);
}
void CLIPSObjectBuilder::setField(const char* n, bool v) {
   if(v) {
      setFieldTrue(n);
   } else {
      setFieldFalse(n);
   }

}
void CLIPSObjectBuilder::setFieldTrue(const char* n) {
   setField(n, "TRUE");
}
void CLIPSObjectBuilder::setFieldFalse(const char* n) {
   setField(n, "FALSE");
}

void CLIPSObjectBuilder::setField(const char* n, MultifieldBuilder* builder) {
   DATA_OBJECT tmp;
   //DATA_OBJECT_PTR q = &tmp;
   builder->bindToDataObjectPointer(&tmp);
   setField(n, &tmp);
}
void CLIPSObjectBuilder::setParent(char* p) {
   setField("Parent", p);
}
void CLIPSObjectBuilder::setParent(const char* p) {
   setField("Parent", p);
}
void CLIPSObjectBuilder::setPointer(PointerAddress v) {
   setField("Pointer", v);
}
void CLIPSObjectBuilder::addField(const char* n, PointerAddress v) { 
   setField(n, v);
}
void CLIPSObjectBuilder::addField(const char* n, unsigned v) { 
   setField(n, v);
}
void CLIPSObjectBuilder::addField(const char* n, float v) { 
   setField(n, v);
}
void CLIPSObjectBuilder::addField(const char* n, double v) { 
   setField(n, v);
}
void CLIPSObjectBuilder::addField(const char* n, char* v) { 
   setField(n, v);
}
void CLIPSObjectBuilder::addField(const char* n, const char* v) { 
   setField(n, v);
}
void CLIPSObjectBuilder::addField(const char* n, std::string v) {
   setField(n, v);
}
void CLIPSObjectBuilder::addField(const char* n, bool v) { 
   setField(n, v);
}
void CLIPSObjectBuilder::addField(const char* n, int v) { 
   setField(n, v);
}
void CLIPSObjectBuilder::addField(const char* n, uint64_t v) { 
   setField(n, v);
}
void CLIPSObjectBuilder::addField(const char* n, int64_t v) {
   setField(n, v);
}
void CLIPSObjectBuilder::addField(const char* n, llvm::StringRef v) {
   setField(n, v);
}
void CLIPSObjectBuilder::setFields(PointerAddress pointer, char* parent) {
   setField("ID", name);
   setField("Class", type);
   setParent(parent);
   setPointer(pointer);
   std::string& name = getName();
   namer->tryRegisterPointerToName(pointer, name);
}
void CLIPSObjectBuilder::addTrueField(const char* n) {
   setFieldTrue(n);
}
void CLIPSObjectBuilder::addFalseField(const char* n) {
   setFieldFalse(n);
}
void CLIPSObjectBuilder::addStringField(const char* n, const std::string& string) {
   setField(n, string, true);
}
void CLIPSObjectBuilder::setFieldFromChoice(const char* n, bool value, 
                                            char* onTrue, char* onFalse) {
   if(value) {
      setField(n, onTrue);
   } else {
      setField(n, onFalse);
   }
}
void CLIPSObjectBuilder::printOutContents() {
   DATA_OBJECT obj, result;
   SetType(obj, INSTANCE_ADDRESS);
   SetValue(obj, rawInstance);
   Send(&obj, "print", NULL, &result);
}
