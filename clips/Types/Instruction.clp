;------------------------------------------------------------------------------
; Instruction.clp - Base LLVM Instruction class
; Written by Joshua Scoggins
;------------------------------------------------------------------------------

(defclass Instruction (is-a LLVMUser InteropObject)
 (slot NumSuccessors)
 (slot HasValueHandle (type SYMBOL) (allowed-values FALSE TRUE))
 (slot IsDereferenceablePointer (type SYMBOL) (allowed-values FALSE TRUE))
 (slot MayWriteToMemory (type SYMBOL) (allowed-values FALSE TRUE)) 
 (slot MayReadFromMemory (type SYMBOL) (allowed-values FALSE TRUE))
 (slot MayReadOrWriteMemory (type SYMBOL) (allowed-values FALSE TRUE))
 (slot MayHaveSideEffects (type SYMBOL) (allowed-values FALSE TRUE))
 (slot HasCallDependency (type SYMBOL) (allowed-values FALSE TRUE))
 (slot IsBinaryOp (type SYMBOL) (allowed-values FALSE TRUE))
 (slot IsTerminator (type SYMBOL) (allowed-values FALSE TRUE))
 (slot IsShift (type SYMBOL) (allowed-values FALSE TRUE))
 (slot IsCast (type SYMBOL) (allowed-values FALSE TRUE))
 (slot IsArithmeticShift (type SYMBOL) (allowed-values FALSE TRUE))
 (slot IsLogicalShift (type SYMBOL) (allowed-values FALSE TRUE))
 (slot IsAssociative (type SYMBOL) (allowed-values FALSE TRUE))
 (slot IsCommutative (type SYMBOL) (allowed-values FALSE TRUE))
 (slot Predicate (type SYMBOL))
 (slot Operation (type SYMBOL))
 (slot TimeIndex (type NUMBER) (default-dynamic 0))
 ;(slot ExecutionLength (type NUMBER) (default-dynamic -1))
 (slot MemoryTarget (type SYMBOL) (default-dynamic nil))
 (multislot LocalDependencies)
 (multislot NonLocalDependencies)
 (multislot Consumers)
 (multislot Producers)
 (multislot DestinationRegisters)
 (multislot SourceRegisters (visibility public)))
 ;(multislot BlocksUsedIn (visibility public)))
 ;(slot InstructionType (type SYMBOL) (default-dynamic nil)))
;------------------------------------------------------------------------------
(defmessage-handler Instruction init after ()
 ;copy the Operands into SourceRegisters and Producers list
 ;this should save a ton of time... :D
 (bind ?self:SourceRegisters ?self:Operands)
 (bind ?self:Producers ?self:Operands))
;------------------------------------------------------------------------------
(defmessage-handler Instruction .IncrementTimeIndex ()
 (bind ?self:TimeIndex (+ 1 ?self:TimeIndex)))
;------------------------------------------------------------------------------
(defmessage-handler Instruction .DecrementTimeIndex ()
 (bind ?self:TimeIndex (- ?self:TimeIndex 1)))
;------------------------------------------------------------------------------
