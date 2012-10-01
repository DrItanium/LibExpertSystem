#include "LoopConstruction.h"

void CLIPSLoopBuilder::setFields(Loop* loop, char* parent) {
   char* t = (char*)getName().c_str();
   FunctionNamer& n = getNamer();
   CLIPSObjectBuilder::setFields((PointerAddress)loop, parent);
   BasicBlock* latch = loop->getLoopLatch();
   BasicBlock* header = loop->getHeader();
   BasicBlock* loopPredecessor = loop->getLoopPredecessor();
   BasicBlock* loopPreheader = loop->getLoopPreheader();
   SmallVector<BasicBlock*, 128> exitBlocks;
   loop->getExitBlocks(exitBlocks);
   addField("Depth", loop->getLoopDepth());
   addField("BackEdgeCount", loop->getNumBackEdges());
   if(latch != 0) addField("LatchBlock", header->getName()); 
   if(loopPredecessor != 0) addField("LoopPredecessor", loopPredecessor->getName());
   if(loopPreheader != 0) addField("LoopPreheader", loopPreheader->getName());
   if(header != 0) {
      addField("Entrances", header->getName());
      addField("HeaderBlock", header->getName());
   }
   MultifieldBuilder contents (loop->getSubLoops().size() + loop->getNumBlocks());
   //openField("Contents");
   //register loops first...that way we don't need to worry about
   //incorrect nesting of child blocks
   unsigned index = 1;
   for(Loop::iterator q = loop->begin(), qu = loop->end(); q != qu; ++q, ++index) {
      Loop* subLoop = *q;
      std::string result = Route(subLoop, t, n);
      subLoops.setSlot(index, result);
      //appendValue(result);
   }
   for(Loop::block_iterator s = loop->block_begin(), e = loop->block_end(); s != e; ++s) {
      BasicBlock* bb = (*s);
      if(!n.pointerRegistered((PointerAddress)bb)) {
         contents.setSlot(index, Route(bb, t, n));
         //appendValue(Route(bb, t, n));
         index++;
      }
   }
   //closeField();
   setField("Contents", &contents);
   MultifieldBuilder exits (exitBlocks.size());
   //openField("Exits");
   unsigned index2 = 1;
   for(SmallVectorImpl<BasicBlock*>::iterator i = exitBlocks.begin(),
         e = exitBlocks.end(); i != e; ++i, ++index2) {
      BasicBlock* bb = (*i);
      exits.setSlot(index2, bb->getName());
      //appendValue(bb->getName());
   }
   //closeField();
   setField("Exits", &exits);
}
void CLIPSLoopBuilder::build(Loop* loop, char* parent) {
   open();
   addFields(loop, parent);
   close();
   convertToKnowledge();
}
