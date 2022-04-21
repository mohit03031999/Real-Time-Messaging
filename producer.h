#ifndef PRODUCER_H
#define PRODUCER_H

#include "ridesharing.h"
#include "broker.h"
#include "io.h"

#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <iostream>

//Pthraed function to add the request to the buffer.
//Paratemers: Shared Data Structure of type THREAD_VARIABLES
void * producer(void * ptr);

#endif 