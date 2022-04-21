#include "producer.h"

void * producer(void * ptr)
{
    //Initializing the Thread Varibales
    THREAD_VARIABLES *prod = (THREAD_VARIABLES *)ptr;
	
	sem_wait(&prod->type); // semaphore down
    
    RequestType request_type_temp;

    if (prod->request_id == HumanDriver)
    {
        request_type_temp = HumanDriver;
        prod->request_id = RoboDriver;
    }
    else if(prod->request_id == RoboDriver)
    {
        request_type_temp = RoboDriver;
    }
    sem_post(&prod->type); // Semaphore up

    // while total produced is less than limit
    while(prod->total_produced < prod->request_limit)
    {
        // Wait for the queue access
		sem_wait(&prod->queue_access);

        //Check if the queue is full
        if(prod->request_count >= prod->max_queue_limit)
        {
            sem_post(&prod->queue_access);
            continue;
        }

        //If the current request type is HDR,check if the current count of HDR in the buffer is less than 4
        if (request_type_temp == HumanDriver && request_type_count(HumanDriver, prod->queue) >= prod->max_human_rider)
        {
            sem_post(&prod->queue_access);
            continue;
        }

        //Update the total request produced and the current buffer count
        prod->total_produced++;
        prod->request_count++;

        if (request_type_temp == HumanDriver)
        {
            prod->HDR_count++;  //Incrementing the HDR count if the current request is HDR
        }
        else if (request_type_temp == RoboDriver)
        {
            prod->RDR_count++;  //Incrementing the RDR count if the current request is RDR
        }
        
                
        enqueue_request(request_type_temp, &prod->queue); // Add Request to the buffer
        
        int count_HDR = request_type_count(HumanDriver, prod->queue);  //Calculate thhe current count of HDR requests in buffer
        int count_RDR = request_type_count(RoboDriver, prod->queue);   //Calculate thhe current count of RDR requests in buffer
        
        int inRequestQueue[]={count_HDR,count_RDR};
        int temp_produced[]={prod->HDR_count,prod->RDR_count};

        //Print the current status of the broker request queue.
        io_add_type(request_type_temp,inRequestQueue,temp_produced);  
		
        sem_post(&prod->queue_access);

		// Suspending the the thread corresponding to each request sleep time mentioned by the user
		if (request_type_temp == HumanDriver)
            nanosleep(&prod->human_driver_time,NULL);   
		else if (request_type_temp == RoboDriver)
			nanosleep(&prod->autonomous_car_time,NULL); 
	}
}