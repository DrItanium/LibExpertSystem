(defclass PathAggregate 
  "The PathAggregate is a very useful data structure that keeps track of all
  information regarding paths and scheduling for a given block on the wavefront.
  Like all objects in this project, it is a tagged object so that it can be
  easily queried without knowing it's name directly. "
  (is-a TaggedObject)
  (slot OriginalStopIndex (type NUMBER))
  (multislot MemoryInvalid (visibility public))
  (multislot MemoryValid (visibility public))
  (multislot PotentiallyValid (visibility public))
  (multislot MemoryBarriers (visibility public))
  (multislot CallBarriers (visibility public))
  (multislot CompletelyInvalid (visibility public))
  ;compensation path vector aspect of the PathAggregate
  (multislot InstructionList (visibility public))
  (multislot ReplacementActions (visibility public))
  (multislot InstructionPropagation (visibility public))
  (multislot ScheduledInstructions (visibility public))
  (multislot CompensationPathVectors (visibility public))
  (multislot TargetCompensationPathVectors (visibility public))
  (multislot MovableCompensationPathVectors (visibility public))
  (multislot ImpossibleCompensationPathVectors (visibility public))
  (multislot StalledCompensationPathVectors (visibility public)))
