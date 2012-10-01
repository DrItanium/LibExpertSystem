#include "MultifieldBuilder.h"
unsigned MultifieldBuilder::getCount() {
   return count; 
}
void* MultifieldBuilder::getMultifieldPointer() {
   return multifieldPointer;
}
void MultifieldBuilder::setType(unsigned index, int type) {
   SetMFType(multifieldPointer, index, type);
}
void MultifieldBuilder::setValue(unsigned index, void* value) {
   SetMFValue(multifieldPointer, index, value);
}
void MultifieldBuilder::setSlot(unsigned index, int type, void* value) {
   setType(index, type);
   setValue(index, value);
//   printf("setSlot(%d, %d, value)\n", index, type);
   numberPopulated++;
}
void MultifieldBuilder::setSlot(unsigned index, PointerAddress value) {
   setSlot(index, INTEGER, AddLong(value));
}
void MultifieldBuilder::setSlot(unsigned index, long value) {
   setSlot(index, INTEGER, AddLong(value));
}
void MultifieldBuilder::setSlot(unsigned index, int value) {
   setSlot(index, INTEGER, AddLong(value));
}
void MultifieldBuilder::setSlot(unsigned index, unsigned value) {
   setSlot(index, INTEGER, AddLong(value));
}
void MultifieldBuilder::setSlot(unsigned index, float value) {
   setSlot(index, FLOAT, AddDouble(value));
}
void MultifieldBuilder::setSlot(unsigned index, double value) {
   setSlot(index, FLOAT, AddDouble(value));
}
void MultifieldBuilder::setSlot(unsigned index, char* v, bool isStringType) {
   if(isStringType) {
      setSlot(index, STRING, AddSymbol(v)); 
   } else {
      setSlot(index, SYMBOL, AddSymbol(v));
   }
}

void MultifieldBuilder::setSlot(unsigned index, const char* v, bool isStringType) {
   setSlot(index, (char*)v, isStringType);
}

void MultifieldBuilder::setSlot(unsigned index, std::string v, bool isStringType) {
   setSlot(index, (char*)v.c_str(), isStringType);
}
void MultifieldBuilder::setSlot(unsigned index, llvm::StringRef v, bool isStringType) {
   setSlot(index, (char*)v.data(), isStringType);
}
void MultifieldBuilder::setTrue(unsigned index) {
   setSlot(index, "TRUE", false);
}
void MultifieldBuilder::setFalse(unsigned index) {
   setSlot(index, "FALSE", false);
}

void MultifieldBuilder::bindToDataObjectPointer(DATA_OBJECT_PTR ptr) {
   SetpType(ptr, MULTIFIELD);
   SetpValue(ptr, multifieldPointer); 
   SetpDOBegin(ptr, 1);
   SetpDOEnd(ptr, numberPopulated);
}
