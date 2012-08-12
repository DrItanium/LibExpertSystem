
(deffunction symbol-to-pointer-list
				 "Converts a given list of symbols that represent InteropObjects and pulls the
				 pointer value out of it. This function assumes order is important"
				 (?list)
				 (bind ?result (create$))
				 (foreach ?e ?list
							 (bind ?obj (symbol-to-instance-name ?e))
							 (bind ?result (create$ ?result (send ?obj get-Pointer))))
				 (return ?result))
