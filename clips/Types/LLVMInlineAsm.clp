(defclass LLVMInlineAsm (is-a LLVMValue)
 (slot HasSideEffects (type SYMBOL) (allowed-values FALSE TRUE))
 (slot IsAlignStack (type SYMBOL) (allowed-values FALSE TRUE))
 (slot FunctionType (allowed-classes LLVMType))
 (slot AsmString (type STRING))
 (slot ConstraintString (type STRING)))

