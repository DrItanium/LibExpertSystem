(defclass MultiBlockContainer (is-a Diplomat List InteropObject)
 (multislot Entrances (visibility public))
 (multislot Exits (visibility public))
 (multislot SafePaths (visibility public))
 (multislot Joins (visibility public))
 (multislot Splits (visibility public)))

(defmessage-handler MultiBlockContainer .AddBlock (?BLK)
 (bind ?self:Contents (create$ ?self:Contents ?BLK)))

(defmessage-handler MultiBlockContainer .AddBlocks ($?blks)
 (bind ?self:Contents (create$ ?self:Contents ?blks)))

(defmessage-handler MultiBlockContainer .AddEntrance (?BLK)
 (bind ?self:Entrances (create$ ?self:Entrances ?BLK)))

(defmessage-handler MultiBlockContainer .AddExit (?BLK)
 (bind ?self:Exits (create$ ?self:Exits ?BLK)))

(defmessage-handler MultiBlockContainer .AddSafePath (?BLK)
 (bind ?self:SafePaths (create$ ?self:SafePaths ?BLK)))

(defmessage-handler MultiBlockContainer .AddJoin (?BLK)
 (bind ?self:Joins (create$ ?self:Joins ?BLK)))

(defmessage-handler MultiBlockContainer .AddSplit (?BLK)
 (bind ?self:Splits (create$ ?self:Splits ?BLK)))
