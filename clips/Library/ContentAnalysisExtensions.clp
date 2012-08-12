(deffunction memberp 
 "Peforms a check to see if the given value is part of the target multifield"
 (?a $?b) 
 (neq (member$ ?a ?b) FALSE))

;A few things that I have just found out about
; When doing rule conditions
;1) (exists ...) is a valid conditional element
;2) (test doesn't have to wrap all conditionals
;3) (and ...) is a way to make sure that all of the
;   given conditions are met
;4) (or ...) is a way to make sure that at least one of the matches occurs
;
