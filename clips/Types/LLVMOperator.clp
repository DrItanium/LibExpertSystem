(defclass LLVMOperator (is-a LLVMUser)
 (slot Opcode (type NUMBER) (range 0 ?VARIABLE)))
(defclass FPMathOperator (is-a LLVMOperator)
 (slot FPAccuracy (type FLOAT)))
(defclass OverflowingBinaryOperator (is-a LLVMOperator)
 (slot HasNoUnsignedWrap (type SYMBOL) (allowed-values FALSE TRUE))
 (slot HasNoSignedWrap (type SYMBOL) (allowed-values FALSE TRUE)))
(defclass PossiblyExactOperator (is-a LLVMOperator)
 (slot IsExact (type SYMBOL) (allowed-values FALSE TRUE)))
