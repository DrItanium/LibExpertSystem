(batch-load 
 (create$ "Library/Loader.clp" 
  "Types/TypeLoader.clp"))
;  "Stages/StageRegistration.clp"
;  "Stages/Fixup/StageGroupLoader.clp"
;  "Stages/Analysis/StageGroupLoader.clp"
;  "Stages/Path/StageGroupLoader.clp"
;  "Stages/Wavefront/StageGroupLoader.clp"
;  "Stages/Final/StageGroupLoader.clp"))

;If you want to do things that occur at start up then make
; the changes here :). Uncomment the line below to enable debug mode :D

(deffacts debug-stats 
 ;(Debug)
 ;(Profile)
 ;(Rules)
 ;(Blocks)
 ;(Children)
 ;(Separator)
 ;(Paths)
 ;(Loop)
 ;(Facts)
 ;(RunStatistics)
 ;(Memory)
 )
(defrule GetInitialMemoryConsumption
 (Debug)
 (Memory)
 =>
 (printout t "Initial Memory Consumption " (/ (mem-used) 131072) " MB" crlf))
(defrule GetRunStatistics
 (Debug)
 (RunStatistics)
 =>
 (watch statistics))

(defrule WatchRules
 (Debug)
 (Rules)
 =>
 (watch rules))

(defrule ProfileConstructs 
 (Debug)
 (Profile)
 =>
 (profile-reset)
 (profile constructs)
 (set-profile-percent-threshold 1))
