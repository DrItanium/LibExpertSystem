(deffunction instruction-to-fact (?STR ?PARENT ?index ?pointer)
 "Takes the string representation of the instruction in question and loads it
 into the appropriate element"
 (assert (instruction ?STR parent ?PARENT index ?index pointer ?pointer)))

(deffunction basicblock-to-fact (?Name ?Parent ?pointer)
 "Takes in the makeup a basic block and asserts a fact to construct a basic
 block from through the use of rules"
 (assert (basicblock ?Name parent ?Parent pointer ?pointer)))

(deffunction region-to-fact (?Name ?Parent ?pointer)
 "Converts a region into a fact to be converted into a clips representation"
 (assert (region ?Name parent ?Parent pointer ?pointer)))

(deffunction loop-to-fact (?Name ?Parent ?pointer)
 "Converts a loop into a fact to be convertedf into a clips representation"
 (assert (loop ?Name parent ?Parent pointer ?pointer)))
 
(deffunction function-to-fact (?Name ?pointer)
 "takes the name of a function and turns it into a fact to be converted"
 (assert (function ?Name pointer ?pointer)))


(deffunction imply-successor (?Succ ?Target)
 "Tells clips that the first argument is a successor of the second"
 (assert (successor ?Succ of ?Target)))

(deffunction imply-predecessor (?Pred ?Target)
 "Tells clips that the first argument is a predecessor of the second"
 (assert (predecessor ?Pred of ?Target)))
