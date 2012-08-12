(deffunction objects-of-type (?type) 
 "Wrapper over the find-all-instances which just returns all objects of the
 given type"
 (return (find-all-instances ((?i ?type)) TRUE)))

(deffunction has-object-named (?type ?name)
 (return (> (length (find-all-instances ((?i ?type)) 
							(eq ?name (send ?i get-ID)))) 0)))

(deffunction get-instance (?type ?name)
 (if (has-object-named ?type ?name) then
  (return (nth$ 1 (find-all-instances ((?i ?type)) (eq ?name (send ?i get-ID)))))
  else
  (return [nil])))


