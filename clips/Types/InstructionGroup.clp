;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; InstructionGroup.clp - Contains functions and classes associated with an 
;;                      - instruction group
;; Written by Joshua Scoggins
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defclass InstructionGroup (is-a TaggedObject)
 (multislot Contents (visibility public))
 (slot TimeIndex (type NUMBER) (visibility public)))

