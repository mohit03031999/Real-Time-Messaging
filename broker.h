#ifndef BROKER_H
#define BROKER_H

#include "ridesharing.h"

#include <stdio.h>
#include <stdlib.h>

//Function to insert the request in the buffer
//Paratemers: Request Type to be stored , Head of the buffer
void enqueue_request(RequestType request_type, request_queue * head);

//Function to remove the request from the buffer using FIFO
//Paratemers: Head of the buffer
//Return: Request removed from the buffer
RequestType consumer_dequeue(request_queue * head);

//Function to count the request(passed as argument) currenlty stored in buffer
//Paratemers: Request Type , Head of the buffer
//Return: Count of the request type
int request_type_count(RequestType request_type, request_queue head);

#endif // BROKER_H