(defclass Slice 
 "A slice is a section of a path that is used during wavefront scheduling to
 figure out if it is safe to schedule an instruction into the target block that
 is on the wavefront. The Parent of the slice is the stop block, the target
 block is the block on the wavefront, the target path is the symbolic name of
 the path that the section was taken from, and Contents are the elements that
 make up the slice."
 (is-a TaggedObject)
  (slot TargetBlock (type SYMBOL) (visibility public))
  (slot TargetPath (type SYMBOL) (visibility public))
  (multislot Contents (visibility public)))
