; SetExtensions.clp - Contains set operations such as superset and equality
; Written By Joshua Scoggins (6/29/2012)

(deffunction superset (?a ?b)
             (and (>= (length$ ?a) (length$ ?b))
                  (subsetp ?b ?a) (not (subsetp ?a ?b))))

(deffunction equal$ (?a ?b)
             (and 
               (= (length$ ?a) (length$ ?b))
               (subsetp ?b ?a) 
               (subsetp ?a ?b)))

(deffunction has-common-element (?a ?b)
             (foreach ?c ?a
                      (if (neq (member$ ?c ?b) FALSE) then 
                        (return TRUE)))
             (return FALSE))
;(deffunction complement (?a ?b)
; (bind ?result ?a)
; (foreach ?c ?b
;  (bind ?index (member$ ?c ?a))
;  (

(deffunction disjoint (?a ?b)
             (not (has-common-element ?a ?b)))

