(defclass CompensationPathVector (is-a TaggedObject)
  (multislot Failures (visibility public))
  (multislot Paths (visibility public))
  (multislot ScheduleTargets (visibility public))
  (multislot Slices (visibility public))
  (multislot Aliases (visibility public))
  (slot OriginalBlock (visibility public)))