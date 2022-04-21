#include "broker.h"

// Function to insert the request in the buffer
void enqueue_request(RequestType rider_request, request_queue * head)
{
    request_queue ptr = (request_queue)malloc(sizeof(request_queue)); // Allocating the memory
    ptr->request_type = rider_request;
    ptr->next = *head;
    *head = ptr;
}

// Function to remove the request from the buffer and return the request removed
RequestType consumer_dequeue(request_queue * head){

    request_queue last = NULL; //Creating a pointer
    request_queue ptr = *head; //Creating a pointer pointing to head 
    
    //Iterating over the buffer
    while(ptr->next != NULL)
    {
        last = ptr;
        ptr = ptr->next;
    }

    //If only one request in the buffer
    if(last == NULL)
    {
        request_queue tmp =(request_queue)malloc(sizeof(request_queue));
        tmp = ptr;
        *head = NULL;

        return tmp->request_type;
    }

    //If more than one request in the buffer
    last->next = NULL;
    return ptr->request_type;
}

//Function to count the requestype(passed as argument) stored in the buffer
int request_type_count(RequestType rider_request, request_queue head)
{
    request_queue ptr = head;
    int count = 0;
    while(ptr != NULL){
        if(ptr->request_type == rider_request) 
        {
            count++;
        }
        ptr = ptr->next;
    }
    return count;
}