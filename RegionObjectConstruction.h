#ifndef _region_object_construction_h
#define _region_object_construction_h
#include "ConstructionTools.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/Analysis/LoopInfo.h"
#include <set>
#include <map>
extern "C" {
#include "clips.h"
}
class CLIPSRegionBuilder : public CLIPSObjectBuilder {
	private:
		LoopInfo* li;
	public:
		CLIPSRegionBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& ti, LoopInfo* l) : CLIPSObjectBuilder(nm, "Region", namer, ti) {
			li = l;
		}
		LoopInfo* getLoopInfo() { return li; }
		void setFields(Region* region, char* parent);
};

#endif
