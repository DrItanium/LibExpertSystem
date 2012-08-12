(defclass List (is-a TaggedObject)
 (slot Length (type NUMBER) (default-dynamic 0))
 (multislot Contents (visibility public)))

(defmessage-handler List .Add (?element)
 (slot-direct-insert$ Contents (+ ?self:Length 1) ?element)
 (bind ?self:Length (+ ?self:Length 1)))

(defmessage-handler List .AddBefore (?point ?element)
 (bind ?ind (nth$ ?point ?self:Contents))
 (slot-direct-insert$ Contents ?ind ?element)
 (bind ?self:Length (+ 1 ?self:Length)))
 
(defmessage-handler List .AddAfter (?point ?element)
 (bind ?ind (+ 1 (nth$ ?point ?self:Contents)))
 (slot-direct-insert$ Contents ?ind ?element)
 (bind ?self:Length (+ 1 ?self:Length)))

(defmessage-handler List .Remove (?element)
 (bind ?self:Length (- ?self:Length 1))
 (bind ?self:Contents (delete-member$ ?self:Contents ?element)))

(defmessage-handler List .RemoveAt (?index)
 (bind ?self:Contents (- ?self:Length 1))
 (slot-direct-delete$ Contents ?index ?index))


(defmessage-handler List .AddRangeAt (?index $?elements)
 (slot-direct-insert$ Contents ?index $?elements))

(defmessage-handler List .ElementAt (?index)
 (nth$ ?index ?self:Contents))

(defmessage-handler List .Contains (?item)
 (neq (member$ ?item ?self:Contents) FALSE))

(defmessage-handler List .ContainsSubset ($?subset)
 (subsetp ?subset ?self:Contents))
