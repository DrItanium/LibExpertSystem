; This is a template instead of a class and is meant to provide fix up hints
; with respect to CFG modification
; Written By Joshua Scoggins (6/25/2012)

(deftemplate ControlModification
 (slot ModificationType (type SYMBOL) (allowed-values Relinquish Transfer))
 (slot From (type SYMBOL))
 (slot To (type SYMBOL))
 (multislot Subject (type SYMBOL)))
