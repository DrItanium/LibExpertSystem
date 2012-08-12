(defclass TaggedObject (is-a USER)
 (slot Class (visibility public) (type SYMBOL))
 (slot ID (visibility public) (type SYMBOL))
 (slot Parent (visibility public) (default-dynamic nil)))

(defmessage-handler TaggedObject init after ()
 (bind ?self:ID (instance-name-to-symbol (instance-name ?self)))
 (bind ?self:Class (class ?self)))

