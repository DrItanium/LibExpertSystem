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
		CLIPSRegionBuilder(std::string nm, FunctionNamer& namer, LoopInfo* l) : CLIPSObjectBuilder(nm, "Region", namer) {
			li = l;
		}
		LoopInfo* getLoopInfo() { return li; }
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
			//std::set<std::string> contents;
			//std::map<std::string, std::set<BasicBlock*> > directFacts;
         //open contents 
			openField((char*)"Contents");
			for(Region::element_iterator i = region->element_begin(), e = region->element_end(); i != e; ++i) {
				//it's really simple we just take a look and 
				RegionNode* rn = *i;
				if(!rn->isSubRegion()) {
					BasicBlock* bb = rn->getEntry(); 
               appendValue(Route(bb, name, namer));
				} else {
					Region* subRegion = rn->getNodeAs<Region>();
               appendValue(Route(subRegion, name, namer, getLoopInfo()));
					//contents.insert(Route(subRegion, name, namer, getLoopInfo()));
				}
         }
         closeField();

               /*
					if(namer.pointerRegistered((PointerAddress)bb)) {
						Loop* target = region->outermostLoopInRegion(getLoopInfo(), bb);
						//this means that it's part of a loop or we are at a
						//top-level region
						if(target == 0) {
							//add it to the list of block ownership 
							//this will cause a fact to be asserted
							std::string loopName = namer.nameFromPointer((PointerAddress)li->getLoopFor(bb));
							if(directFacts.count(loopName) == 0) {
								std::set<BasicBlock*> set;
								set.insert(bb);
								directFacts[loopName] = set;
							} else {
								std::set<BasicBlock*>& set = directFacts[loopName];
								set.insert(bb);
							}
						} else {
							contents.insert(namer.nameFromPointer((PointerAddress)target));
						}
					} else {
						contents.insert(Route(bb, name, namer));
					}
				} else {
					Region* subRegion = rn->getNodeAs<Region>();
					contents.insert(Route(subRegion, name, namer, getLoopInfo()));
				}
			}
			if(!directFacts.empty()) {
				if(!region->isTopLevelRegion()) {
					IncrementGCLocks();
					void* templatePtr;
					//only do this once!
					templatePtr = FindDeftemplate((char*)"ControlModification"); 
					DATA_OBJECT relinquishElement;
					DATA_OBJECT nameElement;
					relinquishElement.type = SYMBOL;
					relinquishElement.value = AddSymbol((char*)"Relinquish");
					nameElement.type = SYMBOL;
					nameElement.value = AddSymbol((char*)(getName().c_str()));
					for(std::map<std::string, std::set<BasicBlock*> >::iterator i = directFacts.begin(), e = directFacts.end(); i != e; ++i) {
						//let's get a little creative here!
						//I have defined a template of ControlModification
						void* newFact;
						void* theMultifield;
						DATA_OBJECT theValue;
						newFact = CreateFact(templatePtr);
						if(newFact == NULL) {
							llvm::errs() << "Warning!!! Unable to create ControlModification fact!\n";
						} else {
							PutFactSlot(newFact, (char*)"ModificationType", &relinquishElement);
							PutFactSlot(newFact, (char*)"To", &nameElement);
							theValue.type = SYMBOL;
							theValue.value = AddSymbol((char*)(i->first).c_str());
							PutFactSlot(newFact, (char*)"From", &theValue);
							std::set<BasicBlock*>& set = i->second;
							unsigned index = 1;
							theMultifield = CreateMultifield(set.size());
							for(std::set<BasicBlock*>::iterator j = set.begin(), k = set.end(); j != k; ++j, ++index) {
								BasicBlock* bb = (*j);
								char* value = (char*)bb->getName().data();
								SetMFType(theMultifield, index, SYMBOL);
								SetMFValue(theMultifield, index, AddSymbol(value));
							}
							SetDOBegin(theValue, 1);
							SetDOEnd(theValue, set.size());
							theValue.type = MULTIFIELD;
							theValue.value = theMultifield;
							PutFactSlot(newFact, (char*)"Subject", &theValue);
							Assert(newFact);
						}
					}
					DecrementGCLocks();
				} else {
					for(std::map<std::string, std::set<BasicBlock*> >::iterator i = directFacts.begin(), e = directFacts.end(); i != e; ++i) {
						contents.insert(i->first);
					}
				}
			}
			if(!contents.empty()) {
				openField((char*)"Contents");
				for(std::set<std::string>::iterator a = contents.begin(), b = contents.end(); a != b; ++a) {
					appendValue(*a);
				}
				closeField();
			} 
         */
		}
};

#endif
