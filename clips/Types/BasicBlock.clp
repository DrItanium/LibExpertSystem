; BasicBlock.clp - A CLIPS wrapper for the LLVM type
(defclass BasicBlock (is-a Diplomat LLVMValue List)
 (multislot UnlinkedInstructions (visibility public))
 (multislot Predecessors)
 (multislot Successors)
 (multislot ProducingBlocks)
 (multislot ConsumingBlocks))
 ;(multislot InvariantOperands)
 ;(multislot VariantOperands))
 
(defmessage-handler BasicBlock .IsSplitBlock () 
 "Checks to see if this block is a split block. Meaning it has more than one
 successor"
 (> (length ?self:Successors) 1))

(defmessage-handler BasicBlock .IsJoinBlock () 
 "Checks to see if this block is a join block. Meaning it has more than one
 predecessor"
 (> (length ?self:Predecessors) 1))

(defmessage-handler BasicBlock .AddPredecessor (?p)
 (bind ?self:Predecessors (create$ ?self:Predecessors ?p)))

(defmessage-handler BasicBlock .AddPredecessors ($?p)
 (bind ?self:Predecessors (create$ ?self:Predecessors ?p)))

(defmessage-handler BasicBlock .AddSuccessor (?p)
 (bind ?self:Successors (create$ ?self:Successors ?p)))

(defmessage-handler BasicBlock .AddSuccessors ($?p)
 (bind ?self:Successors (create$ ?self:Successors ?p)))
