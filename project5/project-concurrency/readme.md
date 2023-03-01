# Project 5 Concurrency 
## Brandon Lui (blui@wpi.edu) and Rishi Patel (rpatel3@wpi.edu)

### The design of your solution.
Our solution to this project involves three major steps: algorithm, threads, and barriers. We started by learning about the Hillis Steele algorithm through documentation and a YouTube video. Once we understood the algorithm, we wrote pseudocode on paper and then transferred it to a non-threaded version on the computer. The non-threaded version can still be seen commented out on line ~175. By taking the log2(n), we can see how many times we need to add the number to 2^i left before we have reached the prefix solution. We are adding 2^i to the left because it doubles each time. If one does not have a number that far to the left, the previous number is kept.

To implement concurrency, we divided the computation between threads by splitting the amount each thread can handle based on the total number of numbers and the requested number of threads. If there is an uneven amount of threads and numbers, the last thread takes the remainder. We split the indexes for each thread based on the start and end points to run the sum concurrently.

To ensure correctness and avoid deadlocks, we employed a reusable barrier, which places a barrier at the beginning and end of the critical section. The two-barrier approach allows us to check when all the threads have reached the same point before continuing to the critical section using a mutex. This ensures that no thread is left waiting indefinitely for another thread to reach the barrier. Once the calculations for each part of the row are done, we wait until every thread has finished before passing onto the next pass of the for loop, in order to syncronize the threads. This approach maximizes concurrency by allowing concurrent calculations in each row and concurrent copying over of the output into the input once the barrier threshold has been reached.

### How your design maximizes opportunities for concurrency.
Our critical section was kept to a minimum to maximize opportunities for concurrency. We only synchronize the part that copies the output array over to the input array, which is then used as the previous array in the next pass of the for loop in the calculate() function. This is the only part that needs a barrier, as the calculations for each part of the row can be calculated independently.

### How your design divides data and computation between the threads.
We divided the computation between threads by splitting the amount each thread can handle based on the total number of numbers and the requested number of threads. If there is an uneven amount of threads and numbers, the last thread takes the remainder. We split the indexes for each thread based on the start and end points to run the sum concurrently.

### How your design employs concurrency mechanisms to ensure correctness, avoid deadlocks, and coordinate threads.
We employed a reusable barrier to ensure correctness, avoid deadlocks, and coordinate threads. The barrier places a barrier at the beginning and end of the critical section. The two-barrier approach allows us to check when all the threads have reached the same point before continuing to the critical section. Once the calculations for each part of the row are done, we wait until every thread has finished before passing onto the next pass of the for loop.

Overall, our solution averages 2.2X time vs the serial solution. We learned a lot about threads and how the algorithm works through the design process.