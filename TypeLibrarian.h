#ifndef _type_librarian_h
#define _type_librarian_h
#include "FunctionNamer.h"
extern "C" {
#include "clips.h"
}
using namespace llvm;
class TypeLibrarian {
   private:
      struct cmp_str {
         bool operator()(char const *a, char const *b) {
            return std::strcmp(a, b) < 0;
         }
      };
      std::map<char*, PointerAddress, cmp_str>* names;
   public:
      TypeLibrarian();
      ~TypeLibrarian();
      void* lookupClass(std::string name);
      void* lookupClass(char* name);
      void* lookupClass(const char* name);
}
#endif
