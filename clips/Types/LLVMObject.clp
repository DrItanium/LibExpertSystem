(defclass LLVMObject (is-a TaggedObject InteropObject)
 (slot Name (visibility public))
 (multislot WritesTo (visibility public))
 (multislot ReadsFrom (visibility public)))
