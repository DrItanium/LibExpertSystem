; An InteropObject stores a pointer to a type outside CLIPS

(defclass InteropObject 
 "The class that allows CLIPS to interact with LLVM. Well that was what it was
 designed for. However this class can be used to allow objects outside CLIPS to
 be modified by request within CLIPS. It is up to the programmer to provide
 functions to act on these pointers though."
 (is-a USER)
 (slot Pointer (visibility public) (type NUMBER)))

