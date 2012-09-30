#include "TypeLibrarian.h"

TypeLibrarian::TypeLibrarian() {
   names = new std::map<char*, PointerAddress, cmp_str>(); 
}

TypeLibrarian::~TypeLibrarian() {
   delete names;
}

void* TypeLibrarian::lookupClass(std::string name) {
   return lookupClass(name.c_str()); 
}

void* TypeLibrarian::lookupClass(const char* name) {
   return lookupClass((char*)name);
}

void* TypeLibrarian::lookupClass(char* name) {
   if(names->count(name)) {
      return (void*)((*names)[name]);
   } else {
      void* ptr = FindDefclass(name); 
      (*names)[name] = (PointerAddress)ptr;
      return ptr;
   }
}

