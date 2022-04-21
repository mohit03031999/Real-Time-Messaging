#include "consumer.h"

void * consumer(void * ptr)
{
    //Initializing the Thread Varibales
    THREAD_VARIABLES *consume = (THREAD_VARIABLES *)ptr;

	sem_wait(&consume->type); // semaphore down
    ConsumerType consumer_type;

    if (consume->consumer_id == CostAlgoDispatch)
    {
        consumer_type = CostAlgoDispatch;
        consume->consumer_id = FastAlgoDispatch;
    }
    else if(consume->consumer_id == FastAlgoDispatch)
    {
        consumer_type = FastAlgoDispatch;
    }

   sem_post(&consume->type); // semaphore up

    // While there are still request in the buffer and the total produced is less than the Limit
    while(consume->total_produced < consume->request_limit || consume->queue != NULL)
    {
        // Wait for the buffer access
		sem_wait(&consume->queue_access);

            // Check if there are request in the buffer
            if(consume->request_count == 0)
            {
                sem_post(&consume->queue_access);
                continue;
            }
        
            //Remove the request from the buffer according to FIFO and get the removed request type
            RequestType temp_request_type = consumer_dequeue(&consume->queue);
            
            //Decrement the current reuqest count in the buffer
            consume->request_count--;

            
            int count_HDR = request_type_count(HumanDriver, consume->queue);  //Current HDR request count in the buffer
            int count_RDR = request_type_count(RoboDriver, consume->queue);   //Current RDR request count in the buffer
        
            int inRequestQueue[]={count_HDR,count_RDR};

            // If the consumer type is cost saving, 
            if (consumer_type == CostAlgoDispatch)
            {
                //The consumed request type is HDR then increment the corresponding count
                if(temp_request_type == HumanDriver)
                    consume->Costsaving_HDR_count++;
                //Else increment the RDR corresponding count
                else
                    consume->Costsaving_RDR_count++;
                int cost_consumed[]={consume->Costsaving_HDR_count,consume->Costsaving_RDR_count};
                
                // Print request has been removed from the request queue and print the current status of the broker request queue.
                io_remove_type(consumer_type,temp_request_type,inRequestQueue,cost_consumed);
            }
            // If the consumer type is Fast Algo,
            else if (consumer_type == FastAlgoDispatch)
            {
                //The consumed request type is HDR then increment the corresponding count
                if(temp_request_type == HumanDriver)
                    consume->fastmaching_HDR_count++;
                else
                    consume->fastmaching_RDR_count++;
                int fast_consumed[]={consume->fastmaching_HDR_count,consume->fastmaching_RDR_count};
                
                // Print request has been removed from the request queue and print the current status of the broker request queue.
                io_remove_type(consumer_type,temp_request_type,inRequestQueue,fast_consumed); 
            }

		sem_post(&consume->queue_access);

		// Suspending the the thread corresponding to each request sleep time mentioned by the user
		if (consumer_type == CostAlgoDispatch)
			nanosleep(&consume->cost_saving_time, NULL); 
		else if (consumer_type == FastAlgoDispatch)
			nanosleep(&consume->fast_matching_time, NULL); 
	}
}