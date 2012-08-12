(defclass Argument (is-a LLVMValue)
 (slot Index (type NUMBER) (range 0 ?VARIABLE))
 (slot HasByValueAttribute (type SYMBOL) (allowed-values FALSE TRUE))
 (slot ParameterAlignment (type NUMBER) (range 0 ?VARIABLE))
 (slot HasNestAttribute (type SYMBOL) (allowed-values FALSE TRUE))
 (slot HasNoAliasAttribute (type SYMBOL) (allowed-values FALSE TRUE))
 (slot HasNoCaptureAttribute (type SYMBOL) (allowed-values FALSE TRUE))
 (slot HasStructRetAttribute (type SYMBOL) (allowed-values FALSE TRUE))
 (multislot Attributes))


