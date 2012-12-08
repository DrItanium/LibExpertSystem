#ifndef _loop_builder_h
#define _loop_builder_h
#include "ConstructionTools.h"
#include "KnowledgeConstructionPass.h"
class CLIPSLoopBuilder : public CLIPSObjectBuilder {
   public:
      CLIPSLoopBuilder(std::string nm, FunctionNamer& namer) : CLIPSObjectBuilder(nm, "Loop", namer) { }
      void addFields(Loop* loop, KnowledgeConstruction &kc, char* parent) {
         char* t = (char*)getName().c_str();
         FunctionNamer& n = getNamer();
         CLIPSObjectBuilder::addFields((PointerAddress)loop, kc, parent);
         BasicBlock* latch = loop->getLoopLatch();
         BasicBlock* header = loop->getHeader();
         BasicBlock* loopPredecessor = loop->getLoopPredecessor();
         BasicBlock* loopPreheader = loop->getLoopPreheader();
         SmallVector<BasicBlock*, 128> exitBlocks;
         loop->getUniqueExitBlocks(exitBlocks);
         addField("Depth", loop->getLoopDepth());
         addField("BackEdgeCount", loop->getNumBackEdges());
         if(latch != 0) addField("LatchBlock", header->getName()); 
         if(loopPredecessor != 0) addField("LoopPredecessor", loopPredecessor->getName());
         if(loopPreheader != 0) addField("LoopPreheader", loopPreheader->getName());
         if(header != 0) {
            addField("Entrances", header->getName());
            addField("HeaderBlock", header->getName());
         }
         openField("Contents");
         //register loops first...that way we don't need to worry about
         //incorrect nesting of child blocks
         for(Loop::iterator q = loop->begin(), qu = loop->end(); q != qu; ++q) {
            Loop* subLoop = *q;
            std::string result = kc.route(subLoop, n, t);
            appendValue(result);
         }
         for(Loop::block_iterator s = loop->block_begin(), e = loop->block_end(); s != e; ++s) {
            BasicBlock* bb = (*s);
            if(!n.pointerRegistered((PointerAddress)bb)) 
               appendValue(kc.route(bb, n, t));
         }
         closeField();
         openField("Exits");
         for(SmallVectorImpl<BasicBlock*>::iterator i = exitBlocks.begin(),
               e = exitBlocks.end(); i != e; ++i) {
            BasicBlock* bb = (*i);
            appendValue(bb->getName());
         }
         closeField();
      }
		void build(Loop* loop, KnowledgeConstruction &kc, char* parent) {
			open();
			addFields(loop, kc, parent);
			close();
			std::string str = getCompletedString();
			kc.addToKnowledgeBase((PointerAddress)loop, str); 
		}
};
#endif
