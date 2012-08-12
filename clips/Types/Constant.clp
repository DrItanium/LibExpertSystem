(defclass Constant (is-a LLVMUser)
 (slot IsNullValue (type SYMBOL) (allowed-values FALSE TRUE))
 (slot IsAllOnesValue (type SYMBOL) (allowed-values FALSE TRUE))
 (slot IsNegativeZeroValue (type SYMBOL) (allowed-values FALSE TRUE))
 (slot CanTrap (type SYMBOL) (allowed-values FALSE TRUE))
 (slot IsConstantUsed (type SYMBOL) (allowed-values FALSE TRUE))
 (slot RelocationInfo (type SYMBOL) (default nil) (allowed-symbols nil NoRelocations
												 LocalRelocation GlobalRelocations)))

(defmessage-handler Constant init after ()
 (bind ?self:Type Constant))

(defclass BlockAddress (is-a Constant)
 (slot TargetBlock (type SYMBOL)))
(defclass ConstantAggregateZero (is-a Constant)
 (slot Element (type SYMBOL)))
(defclass ConstantArray (is-a Constant))
(defclass ConstantDataSequential (is-a Constant)
 (multislot Elements)
 (slot ElementType (type SYMBOL))
 (slot ElementCount (type NUMBER) (range 0 ?VARIABLE))
 (slot ElementByteSize (type NUMBER) (range 0 ?VARIABLE))
 (slot IsString (type SYMBOL) (allowed-values FALSE TRUE))
 (slot IsCString (type SYMBOL) (allowed-values FALSE TRUE)))

(defclass ConstantDataArray (is-a ConstantDataSequential))
(defclass ConstantDataVector (is-a ConstantDataSequential))
(defclass ConstantExpression (is-a Constant)
 (slot Operation)
 (slot IsCast (type SYMBOL) (allowed-values FALSE TRUE))
 (slot IsCompare (type SYMBOL) (allowed-values FALSE TRUE))
 (slot HasIndices (type SYMBOL) (allowed-values FALSE TRUE))
 (slot IsGEPWithNoNotionalOverIndexing (type SYMBOL) (allowed-values FALSE TRUE))
 (slot Opcode (type NUMBER) (range 0 ?VARIABLE))
 (slot Predicate (type NUMBER) (range 0 ?VARIABLE))
 (multislot Indices)
 (slot OpcodeName (type SYMBOL)))

(defclass BinaryConstantExpression (is-a ConstantExpression))
(defclass CompareConstantExpression (is-a ConstantExpression)
 (slot Predicate (type NUMBER)))

(defclass ConstantFloatingPoint (is-a Constant)
 (slot IsZero (type SYMBOL) (allowed-values FALSE TRUE))
 (slot IsNegative (type SYMBOL) (allowed-values FALSE TRUE))
 (slot IsNaN (type SYMBOL) (allowed-values FALSE TRUE))
 (slot Value (type FLOAT))
 )
;TODO: IsExactlyValue calls

(defclass ConstantInteger (is-a Constant)
 (slot ZeroExtendedValue (type NUMBER))
 (slot SignExtendedValue (type NUMBER))
 (slot Width (type NUMBER) (range 0 ?VARIABLE))
 (slot Value (type SYMBOL NUMBER))
 (slot IsNegative (type SYMBOL) (allowed-values FALSE TRUE))
 (slot IsZero (type SYMBOL) (allowed-values FALSE TRUE))
 (slot IsOne (type SYMBOL) (allowed-values FALSE TRUE))
 (slot IsMinusOne (type SYMBOL) (allowed-values FALSE TRUE)))
(defclass PointerConstant (is-a Constant)
 (slot PointerType (type SYMBOL)))
(defclass ConstantPointerNull (is-a PointerConstant))
(defclass ConstantStruct (is-a PointerConstant))
(defclass ConstantVector (is-a PointerConstant)
 (slot SplatValue (type SYMBOL)))

(defclass GlobalValue (is-a Constant)
 ;(slot Alignment (type NUMBER))
 ;(slot HasUnnamedAddress (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot HasDefaultVisibility (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot HasHiddenVisibility (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot HasProtectedVisibility (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot HasSection (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot Section (type SYMBOL))
 ;(slot UseEmptyExceptConstants (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot HasExternalLinkage (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot HasAvailableExternallyLinkage (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot HasLinkOnceLinkage (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot HasWeakLinkage (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot HasAppendingLinkage (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot HasInternalLinkage (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot HasPrivateLinkage (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot HasLinkerPrivateLinkage (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot HasLinkerPrivateWeakLinkage (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot HasLinkerPrivateWeakDefAutoLinkage (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot HasLocalLinkage (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot HasDLLImportLinkage (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot HasDLLExportLinkage (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot HasExternalWeakLinkage (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot HasCommonLinkage (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot IsWeakForLinker (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot MayBeOverridden (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot IsDeclaration (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot IsMaterializable (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot IsDematerializable (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot HasUnnamedAddr (type SYMBOL) (allowed-values FALSE TRUE))
 (slot Initializer))

(defclass GlobalVariable (is-a GlobalValue)
 (slot HasInitializer (type SYMBOL) (allowed-values FALSE TRUE))
 (slot HasDefinitiveInitializer (type SYMBOL) (allowed-values FALSE TRUE))
 (slot HasUniqueInitializer (type SYMBOL) (allowed-values FALSE TRUE))
 (slot IsConstant (type SYMBOL) (allowed-values FALSE TRUE))
 ;(slot IsThreadLocal (type SYMBOL) (allowed-values FALSE TRUE))
	)
(defclass GlobalAlias (is-a GlobalValue)
 (slot Aliasee))

(defclass UndefValue (is-a Constant))


