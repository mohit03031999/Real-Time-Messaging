# Real-time Messaging for Ridesharing
Implemented a real time messaging system to simulate a producer and consumer problem, using POSIX unnamed semaphores and 
POSIX threads.

The programm performs the follwing steps: 
1) The program create two producers and two consumers of rider requests as pthreads.  
2) Producer threads produces and publishes the rider requests to the buffer until reaching the limit of the production, then exits.  
3) Consumer threads consumes all requests from the buffer before exiting.  
4) Main thread waits for the consumer threads to complete consuming the last request before exiting.  
