
(defclass Slice (is-a TaggedObject)
  (slot TargetBlock (type SYMBOL) (visibility public))
  (slot TargetPath (type SYMBOL) (visibility public))
  (multislot Contents (visibility public)))
