#include "KnowledgeConstructionPass.h"
char KnowledgeConstruction::ID = 0;
static RegisterPass<KnowledgeConstruction> kc("knowledge", "Knowledge constructor", false, false);
INITIALIZE_PASS(KnowledgeConstruction, "knowledge", "Knowledge constructor", false, false)
//for opt
