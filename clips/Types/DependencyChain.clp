;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defclass DependencyChain (is-a TaggedObject)
 (slot Producers (default-dynamic (make-instance (gensym*) of List)))
 (slot Consumers (default-dynamic (make-instance (gensym*) of List))))

(defmessage-handler DependencyChain init after ()
 (bind ?id (send ?self get-ID))
 (send ?self:Producers put-Parent ?id) 
 (send ?self:Consumers put-Parent ?id))
(defmessage-handler DependencyChain .ProducerCount ()
 (send ?self:Producers .Count))
(defmessage-handler DependencyChain .ConsumerCount ()
 (send ?self:Consumers .Count))
(defmessage-handler DependencyChain .AddProducers ($?p)
 (send ?self:Producers .AddRange ?p))
(defmessage-handler DependencyChain .AddProducer (?p) 
 "Adds a producer to the list of producers"
 (send ?self:Producers .SetAdd ?p))
(defmessage-handler DependencyChain .AddConsumer (?p)
 "Adds a producer to the list of consumers"
 (send ?self:Consumers .SetAdd ?p))

(defmessage-handler DependencyChain .AddConsumers ($?s)
 (send ?self:Consumers .AddRange ?s))

(defmessage-handler DependencyChain .IsProducer (?p)
 (send ?self:Producers .Contains ?p))

(defmessage-handler DependencyChain .IsConsumer (?p)
 (send ?self:Consumers .Contains ?p))

(defmessage-handler DependencyChain .HasProducers ()
 (not (send ?self:Producers .IsEmpty)))

(defmessage-handler DependencyChain .HasConsumers ()
 (not (send ?self:Consumers .IsEmpty)))

(defmessage-handler DependencyChain .RemoveConsumer (?v)
  (send ?self:Consumers .Remove ?v))

(defmessage-handler DependencyChain .RemoveProducer (?v)
  (send ?self:Producers .Remove ?v))

(defmessage-handler DependencyChain .ProducersContainsSubset ($?v)
 (send ?self:Producers .ContainsSubset $?v))

(defmessage-handler DependencyChain .ConsumersContainsSubset ($?v)
 (send ?self:Consumers .ContainsSubset $?v))

(defmessage-handler DependencyChain .Producers () 
 "Returns the list of producers from the dependency information"
 (return (send ?self:Producers get-Contents)))

(defmessage-handler DependencyChain .Consumers () 
 "Returns the list of consumers from the dependency information"
 (return (send ?self:Consumers get-Contents)))


