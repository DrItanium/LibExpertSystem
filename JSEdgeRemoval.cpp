#include "JSEdgeRemoval.h"

char JSEdgeRemoval::ID = 0;
static RegisterPass<JSEdgeRemoval> js("jsedge", "JS Edge Removal", false, false);
INITIALIZE_PASS(JSEdgeRemoval, "jsedge", "JS Edge Removal", false, false)
