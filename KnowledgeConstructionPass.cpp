#include "KnowledgeConstructionPass.h"
char KnowledgeConstruction::ID = 0;
INITIALIZE_PASS_BEGIN(KnowledgeConstruction, "knowledge", "Knowledge constructor", false, false)
INITIALIZE_PASS_END(KnowledgeConstruction, "knowledge", "Knowledge constructor", false, false)
//for opt
static RegisterPass<KnowledgeConstruction> X("knowledge", "Knowledge constructor", false, false);
