; An InteropObject stores a pointer to a type outside CLIPS

(defclass InteropObject (is-a USER)
 (slot Pointer (visibility public) (type NUMBER)))

