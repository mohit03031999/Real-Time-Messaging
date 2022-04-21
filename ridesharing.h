#ifndef RIDESHARING_H
#define RIDESHARING_H

#include <semaphore.h>

/*
 * Arrays with producer and consumer names
 * These can be indexed with the enumerated types below
 * and are defined in io.c
 */
extern const char *producerNames[];
extern const char *producerAbbrevs[];  
extern const char *consumerNames[]; 

/*
 * Enumerated types to be used by the producers and consumers
 * These are expected in the input/output functions (io.h)
 * should be useful in your producer and consumer code.
 */

/**
 * The broker can hold up to a maximum of 12 undispatched requests 
 * in its request queue at any given time.
 * To promote the use of autonomous cars (as human drivers cost more), 
 * the broker would hold no more than 4 undispatched requests for a human driver at any given time. 
*/

/*
 * Two rider request services (producers of requests) are offered: 
 *   one for publishing (producing) a ride request for a human driver, 
 *   one for publishing (producing) a ride request for an autonomous car.
 * Each ride request service (producer) only publishes (produces) 
 * one type of requests,
 * so RequestType is synomonous with the producer type
 */
typedef enum Requests {
  HumanDriver = 0,   // ride with a human driver
  RoboDriver = 1,  // ride with an autonomous car 
  RequestTypeN = 2,   // number of ride request types
} RequestType;

/* 
 * Two dispatcher services (consumers of requests) are available using 
 * different driver search algorithms
 *   one uses a cost saving algorithm to find a nearby driver
 *   one uses a fast-matching algorithm to find a nearby driver 
 *   Requests are taken off from the broker request queue (by dispatchers) 
 *   in the order that they are published (produced).
*/
typedef enum Consumers {
  CostAlgoDispatch = 0,   // dispatch to a nearby driver based on cost
  FastAlgoDispatch = 1,  // dispatch to a nearby driver based on fast matching 
  ConsumerTypeN = 2,   // Number of consumers
} ConsumerType;

// Buffer for storing the request and pointer to next request
typedef struct node * request_queue; 

struct node{
    RequestType request_type;
    request_queue next;
};

typedef struct{
    // semaphores
    sem_t queue_access;  // only one tread can have access to buffer
    sem_t type;         // configuration for production or consumption

    RequestType request_id; 
	  ConsumerType consumer_id;

    int total_produced; // total requests produced so far
    int total_consumed; // total reuqests consumed so far
    int request_limit;  // Limit for the production of request

    int HDR_count;              //Count number of HDR
    int RDR_count;              //Count number of RDR
    int Costsaving_HDR_count;   //Count HDR request consumed by Cost Saving 
    int Costsaving_RDR_count;   //Count RDR request consumed by Cost Saving
    int fastmaching_HDR_count;  //Count HDR request consumed by Fast Matching Algorithm
    int fastmaching_RDR_count;  //Count RDR request consumed by Fast Matching Algorithm

    int *consumed[2]; // Array of pointers to consumed arrays for each consumer
    int produced[2];  // count for each RequestType

	  struct timespec cost_saving_time;     //Time cost-saving dispatcher (consumer) requires dispatching a request
    struct timespec fast_matching_time;   //Time Fast-matching dispatcher (consumer) requires dispatching a request
    struct timespec human_driver_time;    //Time required to produce a ride request for a human driver.  
    struct timespec autonomous_car_time;  //Time required to produce a ride request for an autonomous car.   
    
    request_queue queue; // Queue for request in the head of linked list
    int request_count;  // Count of request currently in queue

    int max_queue_limit;  //Limit for buffer
    int max_human_rider;  //Maximum Human Rider that can stored in the buffer
}THREAD_VARIABLES;

#endif
