(defclass Wavefront (is-a Hint)
  (multislot Open (visibility public))
  (multislot DeleteNodes (visibility public))
  (multislot Closed (visibility public)))

(defmessage-handler Wavefront init after ()
                    (bind ?self:Type Wavefront))
