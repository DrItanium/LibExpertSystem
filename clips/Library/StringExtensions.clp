(deffunction starts-with (?STR ?SYM)
 "Checks to see if the target string starts with the given string"
				 (bind ?sLen (str-length ?SYM))
				 (return (eq ?SYM (sub-string 1 ?sLen ?STR))))

(deffunction ends-with (?STR ?SYM)
 "Checks to see if the target string ends with the given string"
 (bind ?syLen (str-length ?SYM))
 (bind ?strLen (str-length ?STR))
 (return (eq ?SYM (sub-string (- ?strLen ?syLen) ?strLen ?STR))))


(deffunction to-string (?VAL) 
 "Returns a string representation of the given input"
 (str-cat ?VAL))
