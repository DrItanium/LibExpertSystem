(defclass LLVMType (is-a LLVMObject)
  (slot IsVoidType (type SYMBOL) (allowed-values FALSE TRUE))
  (slot IsHalfType  (type SYMBOL) (allowed-values FALSE TRUE))
  (slot IsFloatType (type SYMBOL) (allowed-values FALSE TRUE))
  (slot IsDoubleType  (type SYMBOL) (allowed-values FALSE TRUE))
  (slot IsX86FP80Type  (type SYMBOL) (allowed-values FALSE TRUE))
  (slot IsFP128Type  (type SYMBOL) (allowed-values FALSE TRUE))
  (slot IsPPCFP128Type  (type SYMBOL) (allowed-values FALSE TRUE))
  (slot IsFloatingPointType  (type SYMBOL) (allowed-values FALSE TRUE))
  (slot IsX86MMXType  (type SYMBOL) (allowed-values FALSE TRUE))
  (slot IsFPOrFPVectorType  (type SYMBOL) (allowed-values FALSE TRUE))
  (slot IsLabelType  (type SYMBOL) (allowed-values FALSE TRUE))
  (slot IsMetadataType  (type SYMBOL) (allowed-values FALSE TRUE))
  (slot IsIntegerType  (type SYMBOL) (allowed-values FALSE TRUE))
  (slot IsIntOrIntVectorType  (type SYMBOL) (allowed-values FALSE TRUE))
  (slot IsFunctionType  (type SYMBOL) (allowed-values FALSE TRUE))
  (slot IsStructType  (type SYMBOL) (allowed-values FALSE TRUE))
  (slot IsArrayType  (type SYMBOL) (allowed-values FALSE TRUE))
  (slot IsPointerType  (type SYMBOL) (allowed-values FALSE TRUE))
  (slot IsVectorType  (type SYMBOL) (allowed-values FALSE TRUE))
  (slot IsEmptyType  (type SYMBOL) (allowed-values FALSE TRUE))
  (slot IsPrimitiveType  (type SYMBOL) (allowed-values FALSE TRUE))
  (slot IsDerivedType  (type SYMBOL) (allowed-values FALSE TRUE))
  (slot IsFirstClassType  (type SYMBOL) (allowed-values FALSE TRUE))
  (slot IsSingleValueType  (type SYMBOL) (allowed-values FALSE TRUE))
  (slot IsAggregateType  (type SYMBOL) (allowed-values FALSE TRUE))
  (slot IsSized  (type SYMBOL) (allowed-values FALSE TRUE))
  (slot PrimitiveSizeInBits (type NUMBER) (range 0 ?VARIABLE))
  (slot ScalarSizeInBits (type NUMBER) (range 0 ?VARIABLE))
  (slot FPMantissaWidth (type NUMBER) (range 0 ?VARIABLE))
  (slot ScalarType (allowed-classes LLVMType))
  (multislot Subtypes)
  (multislot TypeMakeup (type SYMBOL))
  (slot IsFunctionVarArg (type SYMBOL) (allowed-values FALSE TRUE))
  (slot QuickType (type SYMBOL)))

(defclass IntegerType (is-a LLVMType)
 (slot BitWidth (type NUMBER) (range 0 ?VARIABLE))
 (slot BitMask (type NUMBER))
 (slot SignBit (type NUMBER))
 (slot IsPowerOf2ByteWidth (type SYMBOL) (allowed-values FALSE TRUE)))

(defclass FunctionType (is-a LLVMType)
 (slot IsVarArg (type SYMBOL) (allowed-values FALSE TRUE))
 (slot ReturnType (allowed-classes LLVMType))
 (multislot Parameters))


(defmessage-handler FunctionType GetNumParams ()
 (length ?self:Parameters))

(defmessage-handler FunctionType GetParamType (?index)
 (nth$ ?index ?self:Parameters))

(defclass CompositeType (is-a LLVMType)
 (multislot Indicies))

(defclass StructType (is-a CompositeType)
 (slot Name (type SYMBOL))
 (slot IsPacked (type SYMBOL) (allowed-values FALSE TRUE))
 (slot IsLiteral (type SYMBOL) (allowed-values FALSE TRUE))
 (slot IsOpaque (type SYMBOL) (allowed-values FALSE TRUE))
 (slot IsSized (type SYMBOL) (allowed-values FALSE TRUE))
 (slot HasName (type SYMBOL) (allowed-values FALSE TRUE))
 (multislot Body))

(defmessage-handler StructType SetBody (?element ?isPacked)
 (bind ?self:IsPacked ?isPacked)
 (bind ?self:Body ?element))

(defmessage-handler StructType SetBody$ ($?types)
 (bind ?self:Body (create$ ?self:Body ?types)))

(defmessage-handler StructType GetNumElements ()
 (length ?self:Body))

(defmessage-handler StructType Elements ()
 (return ?self:Body))

(defmessage-handler StructType GetElementType (?n)
 (if (> ?n (length ?self:Body)) then 
  (printout t "ERROR: Element out of range for message GetElementType of StructType!" crlf)
  (exit)
  else 
  (return (nth$ ?n ?self:Body))))

(defclass SequentialType (is-a CompositeType)
 (slot ElementType (allowed-classes LLVMType)))

(defclass VectorType (is-a SequentialType)
 (slot BitWidth (type NUMBER) (range 0 ?VARIABLE))
 (slot NumElements (type NUMBER) (range 0 ?VARIABLE)))

(defclass PointerType (is-a SequentialType)
 (slot AddressSpace (type NUMBER) (range 0 ?VARIABLE)))

(defclass ArrayType (is-a SequentialType)
 (slot NumElements (type NUMBER) (range 0 ?VARIABLE)))

