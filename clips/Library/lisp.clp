(deffunction quote "Returns a multifield - Obsolete!" ($?a) (return $?a))

(deffunction bye () 
(printout t "Bye." crlf)
(exit))

(deffunction 1+ (?A) (+ ?A 1))
(deffunction 1- (?A) (- ?A 1))

(deffunction emptyp (?list)
 "Checks to see if a given multislot is empty"
 (return (and (multifieldp ?list) (eq 0 (length ?list)))))
