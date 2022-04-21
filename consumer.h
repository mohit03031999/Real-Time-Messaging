#ifndef CONSUMER_H
#define CONSUMER_H

#include "broker.h"
#include "io.h"

#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <iostream>

//Pthraed function to consume the request from the buffer.
//Paratemers: Shared Data Structure of type THREAD_VARIABLES
void *consumer(void * ptr);

#endif