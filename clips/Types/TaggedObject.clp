(defclass TaggedObject (is-a USER)
 "Supertype of all objects used in wavefront scheduling and the Knowlege
 Construction Engine. It uses the ID field instead of the name field to get
 around a matching issue. Plus I didn't know the name field existed until I was
 nearly done implementing wavefront scheduling.... The Parent field is used to
 describe the parent of the given object. It is a nifty way of being able to
 jump around the contents of a function within the confines of an expert
 system."
 (slot Class (visibility public) (type SYMBOL))
 (slot ID (visibility public) (type SYMBOL))
 (slot Parent (visibility public) (default-dynamic nil)))

(defmessage-handler TaggedObject init after ()
 (bind ?self:ID (instance-name-to-symbol (instance-name ?self)))
 (bind ?self:Class (class ?self)))

