#ifndef _clips_basic_block_builder_h
#define _clips_basic_block_builder_h
#include "ConstructionTools.h"

class CLIPSBasicBlockBuilder : public CLIPSValueBuilder {
   public:
      CLIPSBasicBlockBuilder(std::string nm, FunctionNamer& namer) : CLIPSValueBuilder(nm, "BasicBlock", namer) { }
      void addFields(BasicBlock* bb, KnowledgeConstruction &kc, char* parent, bool constructInstructions = true) {
         CLIPSValueBuilder::addFields((Value*)bb, kc, parent);
         char* name = (char*)bb->getName().data();
         FunctionNamer& namer = getNamer();
         namer.registerBasicBlock(name);
         if(bb->isLandingPad()) addTrueField("IsLandingPad");
         if(bb->hasAddressTaken()) addTrueField("HasAddressTaken");
         pred_iterator pi = pred_begin(bb), pe = pred_end(bb);
         if(pi != pe) {
            openField("Predecessors");
            for(; pi != pe; ++pi) {
               BasicBlock* pred = *pi;
               appendValue(pred->getName());
            }
            closeField();
         }
         succ_iterator si = succ_begin(bb), se = succ_end(bb);
         if(si != se) {
            openField("Successors");
            for(; si != se; ++si) {
               BasicBlock* succ = *si;
               appendValue(succ->getName());
            }
            closeField();
         }
         if(constructInstructions && !bb->empty()) {
            std::string tmp;
            raw_string_ostream data(tmp);
            openField("Contents");
            for(BasicBlock::iterator i = bb->begin(), e = bb->end(); i != e; ++i) {
               Instruction* inst = i;
               std::string res = kc.route(inst, namer, name);
               appendValue(res);
               data << " " << res;
            }
            closeField();
            addField("Produces", data.str()); 
         }
      }
		void build(BasicBlock* bb, KnowledgeConstruction &kc, char* parent) {
			open();
			addFields(bb, kc, parent, true);
			close();
			std::string &str = getCompletedString();
			kc.addToKnowledgeBase((PointerAddress)bb, str);
		}
};
#endif
