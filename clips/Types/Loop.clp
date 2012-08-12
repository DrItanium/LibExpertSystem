(defclass Loop (is-a Region)
 (slot BackEdgeCount (type NUMBER) (range 0 ?VARIABLE))
 (multislot BackEdges)
 (multislot ExitBlocks)
 (slot HeaderBlock)
 (slot LatchBlock)
 (slot LoopPreheader)
 (slot LoopPredecessor))

(defmessage-handler Loop .AddExitBlocks ($?exits)
 (bind ?self:ExitBlocks (create$ ?self:ExitBlocks ?exits)))

(defmessage-handler Loop .AddBackEdge (?BLK)
 (bind ?self:BackEdges (create$ ?self:BackEdges ?BLK)))

