(defclass LLVMUser (is-a LLVMValue)
 (multislot Operands (visibility public)))

(defmessage-handler LLVMUser .AddOperand (?op)
 (bind ?self:Operands (create$ ?self:Operands ?op)))
(defmessage-handler LLVMUser .AddOperands ($?ops)
 (bind ?self:Operands (create$ ?self:Operands ?ops)))

