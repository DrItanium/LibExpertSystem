#include "FunctionNamer.h"
#include <string>

FunctionNamer::FunctionNamer(llvm::DenseMap<PointerAddress, std::string>& table) {
	gensymID = 0L;
	registerID = 0L;
	basicBlockID = 0L;
	regionID = 0L;
   loopID = 0L;
	names = &table;
   instructionIndices = new std::map<std::string, PointerAddress>();
}
FunctionNamer::~FunctionNamer() { 
   delete instructionIndices; 
}
PointerAddress FunctionNamer::nextGensymID() { 
   return gensymID++; 
}
PointerAddress FunctionNamer::nextRegisterID() {
   return registerID++; 
}
PointerAddress FunctionNamer::nextBasicBlockID() { 
   return basicBlockID++; 
}
PointerAddress FunctionNamer::nextRegionID() { 
   return regionID++; 
}
PointerAddress FunctionNamer::nextLoopID() { 
   return loopID++; 
}
PointerAddress FunctionNamer::getRegisterIDCount() { 
   return registerID; 
}
PointerAddress FunctionNamer::getBasicBlockIDCount() {
   return basicBlockID; 
}
PointerAddress FunctionNamer::getRegionIDCount() { 
   return regionID; 
}
PointerAddress FunctionNamer::getGensymIDCount() { 
   return gensymID; 
}
PointerAddress FunctionNamer::getLoopIDCount() { 
   return loopID; 
}
void FunctionNamer::reset() {
	registerID = 0L;
	basicBlockID = 0L;
	regionID = 0L;
	gensymID = 0L;
   loopID = 0L;
	names->clear();
   instructionIndices->clear();
}
void FunctionNamer::makeBasicBlockID(llvm::raw_string_ostream& container) {
	container << "b" << nextBasicBlockID();
}
void FunctionNamer::makeRegisterID(llvm::raw_string_ostream& container) {
	container << "i" << nextRegisterID();
}
void FunctionNamer::makeRegionID(llvm::raw_string_ostream& container) {
	container << "r" << nextRegionID();
}
void FunctionNamer::makeGensymID(llvm::raw_string_ostream& container) {
	container << "g" << nextGensymID();
}
void FunctionNamer::makeLoopID(llvm::raw_string_ostream& container) {
   container << "l" << nextLoopID();
}
void FunctionNamer::makeBasicBlockID(char* container) {
	sprintf(container, "b%lld", nextBasicBlockID());
}
void FunctionNamer::makeRegisterID(char* container) {
	sprintf(container, "i%lld", nextRegisterID());
}
void FunctionNamer::makeRegionID(char* container) {
	sprintf(container, "r%lld", nextRegionID());
}
void FunctionNamer::makeGensymID(char* container) {
	sprintf(container, "g%lld", nextGensymID());
}
void FunctionNamer::makeLoopID(char* container) {
	sprintf(container, "l%lld", nextLoopID());
}
bool FunctionNamer::pointerRegistered(PointerAddress ptr) {
   return names->count(ptr);
}
void FunctionNamer::registerPointerToName(PointerAddress ptr, std::string& name) {
   std::pair<PointerAddress, std::string&> pair (ptr, name);
   names->insert(pair);
}
std::string FunctionNamer::nameFromPointer(PointerAddress ptr) {
   return names->lookup(ptr);
}
void FunctionNamer::tryRegisterPointerToName(PointerAddress ptr, std::string& name) {
   //if the pointer isn't registered then register it
   if(!names->count(ptr)) {
      std::pair<PointerAddress, std::string&> pair (ptr, name);
      names->insert(pair);
   }
}

void FunctionNamer::registerBasicBlock(std::string pointer) {
   std::pair<std::string, PointerAddress> pair (pointer, 0L);
   instructionIndices->insert(pair);
}
bool FunctionNamer::basicBlockRegistered(std::string pointer) {
   return instructionIndices->count(pointer);
}
PointerAddress FunctionNamer::numberOfInstructionsForBasicBlock(std::string name) {
   return (*instructionIndices)[name];
}
PointerAddress FunctionNamer::registerInstructionWithBasicBlock(std::string name) {
   PointerAddress& old = (*instructionIndices)[name];
   PointerAddress tmp = old;
   old = old + 1L;
   //llvm::errs() << "instructionIndicies[" << name << "] = " << (*instructionIndices)[name] << " and old = " << old << '\n';
   return tmp;
}
