#include "io.h"
#include "ridesharing.h"
#include "producer.h"
#include "consumer.h"

#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cmath>
#include <semaphore.h>

#define NANO 1000000000  /* Conversion to Nanosecond */
#define MILLI 1000 /* Conversion from milisecond to Second */

int main(int argc, char ** argv)
{
    char Option; 
    int limit_human_driver = 4;  // No more than 4 rider requests for a human driver 
    int max_rider = 12;          // Buffer can hold up to a maximum of 12 rider requests 
    int total_request = 120;     // Default value for Maximum Number of Request
    int cost_saving = 0;         // Time for the Cost-Saving dispatcher(consumer) requires dispatching a request
    int fast_matching = 0;       // Time for the Fast-Matching dispatcher(consumer) requires dispatching a request
    int human_driver = 0;        // Time to produce a ride request for an human driver
    int autonomous_car = 0;      // Time to produce a ride request for an Autonomous car

    while ((Option = getopt(argc, argv, "n:c:f:h:a:")) != -1)
    {
        switch (Option)
        {
            case 'n': /* Limit of total number of requests  */
                total_request = atoi(optarg);  
                break;

            case 'c': /* Number of milliseconds N that the cost-saving dispatcher(consumer) requires dispatching a request  */
                cost_saving = atoi(optarg);
                break;

            case 'f': /* Number of milliseconds N that the fast-matching dispatcher(consumer) requires dispatching a request  */
                fast_matching = atoi(optarg);
                break;
            
            case 'h': /* Number of milliseconds required to produce a ride request for an human driver.  */
                human_driver = atoi(optarg);
                break;
            case 'a': /* Number of milliseconds required to produce a ride request for an autonomous car.  */
                autonomous_car = atoi(optarg);
                break;
        }
    }

    //Intilaization for Thread Variables
    THREAD_VARIABLES *thread_var = (THREAD_VARIABLES *)malloc(sizeof(THREAD_VARIABLES));

    thread_var->request_id = HumanDriver;               // Initial Request_id should Human Driver
	thread_var->consumer_id = CostAlgoDispatch;         // Initial Consumer should be Cost-Saving ALgorithm

    thread_var->total_produced = 0;                     // Initially Total request produces are 0
    thread_var->total_consumed = 0;                     // Initially Total request consumed are 0
    thread_var->request_limit = total_request;          // Initializing the Total request that can be produced to 120.
    thread_var->max_queue_limit = max_rider;            // Initializing the Total request that can be stored in buffer to 12.
    thread_var->max_human_rider = limit_human_driver;   // Initializing the Total Human Rider request that can be stored in buffer to 4.
    
    thread_var->HDR_count = 0;                          // Initially Total HDR request are 0
    thread_var->RDR_count = 0;                          // Initially Total RDR request are 0
    thread_var->Costsaving_HDR_count = 0;               // Initially Total Costsaving with HDR request are 0
    thread_var->fastmaching_HDR_count = 0;              // Initially Total Fast Matching with HDR request are 0
    thread_var->Costsaving_RDR_count = 0;               // Initially Total Costsaving with RDR request are 0
    thread_var->fastmaching_RDR_count = 0;              // Initially Total Fast Matching with RDR request are 0
    
    for(int i = 0; i < 2; i++)
    {
        thread_var->consumed[i] = new int[2];
    }

	
    thread_var->cost_saving_time.tv_sec = cost_saving/MILLI;     
    thread_var->cost_saving_time.tv_nsec = (cost_saving % MILLI)*(NANO/MILLI);      //Initialize tv_nsec   
    
    thread_var->fast_matching_time.tv_sec = fast_matching/MILLI;                
    thread_var->fast_matching_time.tv_nsec = (fast_matching % MILLI)*(NANO/MILLI);  //Initialize tv_nsec  
    
    thread_var->human_driver_time.tv_sec = human_driver/MILLI;    
    thread_var->human_driver_time.tv_nsec = (human_driver % MILLI)*(NANO/MILLI);    //Initialize tv_nsec  
    
    thread_var->autonomous_car_time.tv_sec = autonomous_car/MILLI;                
    thread_var->autonomous_car_time.tv_nsec = (autonomous_car % MILLI)*(NANO/MILLI); //Initialize tv_nsec  

	thread_var->queue = NULL;      // Initially the Link List is NULL
    thread_var->request_count = 0; // Size of the buffer at any time

    // initialize semaphores
    sem_init(&thread_var->queue_access, 0, 1);			// To get access to Buffer
	sem_init(&thread_var->type, 0, 1);				    // To set producer or consumer type

    // Pthread variables
    pthread_t human_driver_thread ;
	pthread_t robot_driver_thread;
	pthread_t costalgodispatch_thread;
	pthread_t fastalgodispatch_thread;

    // Pthread creation
    pthread_create(&human_driver_thread, NULL, producer, thread_var);
	pthread_create(&robot_driver_thread, NULL, producer, thread_var);
	pthread_create(&costalgodispatch_thread, NULL, consumer, thread_var);
	pthread_create(&fastalgodispatch_thread, NULL, consumer, thread_var);

    pthread_join(human_driver_thread, NULL);
    pthread_join(robot_driver_thread, NULL);
	pthread_join(costalgodispatch_thread, NULL);
	pthread_join(fastalgodispatch_thread, NULL);

    // Destroying semaphores	
    sem_destroy(&thread_var->queue_access);		
    sem_destroy(&thread_var->type);			
    
    // Printing final report
    int *consumed[ConsumerTypeN];
    for(int i = 0; i < 2; i++)
        {
            consumed[i] = new int[RequestTypeN];
        }
    int produced[]={thread_var->HDR_count,thread_var->RDR_count};
    consumed[0][0] = thread_var->Costsaving_HDR_count;
    consumed[0][1] = thread_var->Costsaving_RDR_count;
    consumed[1][0] = thread_var->fastmaching_HDR_count;
    consumed[1][1] = thread_var->fastmaching_RDR_count;

    io_production_report(produced,consumed);
    return 0;
}