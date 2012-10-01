#ifndef _clips_basic_block_builder_h
#define _clips_basic_block_builder_h
#include "ConstructionTools.h"
#include<iterator>

class CLIPSBasicBlockBuilder : public CLIPSValueBuilder {
   public:
      CLIPSBasicBlockBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSValueBuilder(nm, "BasicBlock", namer, tl) { }
      void setFields(BasicBlock* bb, char* parent, bool constructInstructions = true) {
         CLIPSValueBuilder::setFields((Value*)bb, parent);
         char* name = (char*)bb->getName().data();
         FunctionNamer& namer = getNamer();
         TypeLibrarian& tl = getLibrarian();
         namer.registerBasicBlock(name);
         if(bb->isLandingPad()) setFieldTrue("IsLandingPad");
         if(bb->hasAddressTaken()) setFieldTrue("HasAddressTaken");
         pred_iterator pi = pred_begin(bb), pe = pred_end(bb);
         if(pi != pe) {
            unsigned size = std::distance(pi, pe);
            MultifieldBuilder b0(size);
            for(unsigned index = 1; pi != pe; ++pi, ++index) {
               BasicBlock* pred = *pi;
               b0.setSlot(index, pred->getName());
            }
            setField("Predecessors", &b0);
         }
         succ_iterator si = succ_begin(bb), se = succ_end(bb);
         if(si != se) {
            MultifieldBuilder b1(bb->getTerminator()->getNumSuccessors());
            //openField("Successors");
            for(unsigned index = 1; si != se; ++si, ++index) {
               BasicBlock* succ = *si;
               b1.setSlot(index, succ->getName());
             //  appendValue(succ->getName());
            }
            setField("Successors", &b1);
            //closeField();
         }
         if(constructInstructions && !bb->empty()) {
            unsigned size = (unsigned)bb->getInstList().size();
            MultifieldBuilder builder(size);
            unsigned index = 1;
            for(BasicBlock::iterator i = bb->begin(), e = bb->end(); i != e; ++i, ++index) {
               Instruction* inst = i;
               std::string res = Route(inst, name, namer, tl);
               builder.setSlot(index, res);
            }
            setField("Contents", &builder);
            setField("Produces", &builder);
         }
      }
};
#endif
