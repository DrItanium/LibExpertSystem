#ifndef _FUNCTION_NAMER_H
#define _FUNCTION_NAMER_H
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/DenseSet.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/Value.h"
#include <string>
#include <map>
extern "C" {
	#include <cstdio>
	#include <cstring>
}
typedef long long PointerAddress;

class FunctionNamer {
	private:
		PointerAddress gensymID;
      PointerAddress loopID;
		PointerAddress registerID;
		PointerAddress basicBlockID;
		PointerAddress regionID;
		llvm::DenseMap<PointerAddress, std::string>* names;
      std::map<std::string, PointerAddress>* instructionIndices;
      
	public:
      FunctionNamer(llvm::DenseMap<PointerAddress, std::string>& translationTable);
      ~FunctionNamer();
      PointerAddress nextLoopID();
		PointerAddress nextRegisterID();
		PointerAddress nextBasicBlockID();
		PointerAddress nextRegionID();
		PointerAddress nextGensymID();
		PointerAddress getRegisterIDCount();
		PointerAddress getBasicBlockIDCount();
		PointerAddress getRegionIDCount();
		PointerAddress getGensymIDCount();
      PointerAddress getLoopIDCount();
		llvm::DenseMap<PointerAddress, std::string>& getTranslationTable() { return *names; }
		void reset();
		void makeBasicBlockID(llvm::raw_string_ostream& container);
		void makeRegisterID(llvm::raw_string_ostream& container);
		void makeRegionID(llvm::raw_string_ostream& container);
		void makeGensymID(llvm::raw_string_ostream& container);
      void makeLoopID(llvm::raw_string_ostream& container);
		void makeBasicBlockID(char* container);
		void makeRegisterID(char* container);
		void makeRegionID(char* container);
		void makeGensymID(char* container);
      void makeLoopID(char* container);
      bool basicBlockRegistered(std::string name);
      void registerBasicBlock(std::string name);
      PointerAddress numberOfInstructionsForBasicBlock(std::string name);
      PointerAddress registerInstructionWithBasicBlock(std::string name);
      bool pointerRegistered(PointerAddress ptr);
      void registerPointerToName(PointerAddress ptr, std::string& name);
      std::string nameFromPointer(PointerAddress ptr);
      void tryRegisterPointerToName(PointerAddress ptr, std::string& name);
};
#endif
