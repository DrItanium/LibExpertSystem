(defclass Diplomat 
 "A Diplomat is a collection of fields that interact with other
 objects. The name comes from an old idea I tried to implement during
 implementing wavefront scheduling."
 (is-a LLVMObject)
 (slot IsOpen (type SYMBOL) (visibility public) 
	(allowed-values FALSE TRUE))
 (slot HasCallBarrier (type SYMBOL) (visibility public) 
  (allowed-values FALSE TRUE))
 (slot HasMemoryBarrier (type SYMBOL) (visibility public) 
  (allowed-values FALSE TRUE))
 (multislot PreviousPathElements (visibility public))
 (multislot NextPathElements (visibility public))
 (multislot Consumes)
 (multislot Produces)
 (multislot Paths (visibility public)))
 ;(multislot Chokes (visibility public))
 ;(multislot ChokedBy (visibility public))
 ;(multislot RegionalAllies (visibility public))
 ;(multislot FunctionalAllies (visibility public)))

;(defmessage-handler Diplomat .AddRegionalAlly (?a)
; (bind ?self:RegionalAllies (create$ ?self:RegionalAllies ?a)))
;
;(defmessage-handler Diplomat .AddFunctionalAlly (?a)
; (bind ?self:FunctionalAllies (create$ ?self:FunctionalAllies ?a)))

;(defmessage-handler Diplomat .AddPath (?path)
; (bind ?self:Paths (create$ ?self:Paths ?path)))

;(defmessage-handler Diplomat .AddChokes (?blk)
; (bind ?self:Chokes (create$ ?self:Chokes ?blk)))
;
;(defmessage-handler Diplomat .AddChokedBy (?blk)
; (bind ?self:ChokedBy (create$ ?self:ChokedBy ?blk)))

