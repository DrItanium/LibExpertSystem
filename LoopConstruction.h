#ifndef LOOP_CONSTRUCTION_H
#define LOOP_CONSTRUCTION_H 
#include "ConstructionTools.h"

class CLIPSLoopBuilder : public CLIPSObjectBuilder {
   public:
      CLIPSLoopBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& ti);
      void setFields(Loop* loop, char* parent);
};
#endif
