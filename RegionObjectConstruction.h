#ifndef _region_object_construction_h
#define _region_object_construction_h
#include "ConstructionTools.h"
#include "llvm/ADT/SmallSet.h"
#include <set>
#include <map>
extern "C" {
#include "clips.h"
}
class CLIPSRegionBuilder : public CLIPSObjectBuilder {
	public:
		CLIPSRegionBuilder(std::string nm, FunctionNamer& namer) : CLIPSObjectBuilder(nm, "Region", namer) { }
		void addFields(Region* region, char* parent) {
			//we should edit the parent to be a loop if it turns out that we
			//are part of a loop
			CLIPSObjectBuilder::addFields((PointerAddress)region, parent);
			addField("Depth", region->getDepth());
			if(region->isTopLevelRegion()) addTrueField("IsTopLevelRegion");
			if(region->isSimple()) addTrueField("IsSimple");
			BasicBlock* entry = region->getEntry();
			BasicBlock* exit = region->getExit();
			if(entry != NULL) addField("Entrances", entry->getName());
			if(exit != NULL) addField("Exits", exit->getName());
			char* name = (char*)getName().c_str();
			FunctionNamer& namer = getNamer();
         //open contents 
			openField((char*)"Contents");
			for(Region::element_iterator i = region->element_begin(), e = region->element_end(); i != e; ++i) {
				RegionNode* rn = *i;
				if(!rn->isSubRegion()) {
					BasicBlock* bb = rn->getEntry(); 
               appendValue(Route(bb, name, namer));
				} else {
					Region* subRegion = rn->getNodeAs<Region>();
               appendValue(Route(subRegion, name, namer));
				}
         }
         closeField();

		}
};

#endif
