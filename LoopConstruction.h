#ifndef _loop_builder_h
#define _loop_builder_h
#include "ConstructionTools.h"
class CLIPSLoopBuilder : public CLIPSObjectBuilder {
   public:
      CLIPSLoopBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& ti) : CLIPSObjectBuilder(nm, "Loop", namer, ti) { }
      void addFields(Loop* loop, char* parent);
      void build(Loop* loop, char* parent);
};
#endif
