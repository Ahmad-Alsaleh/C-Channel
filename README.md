# Multithreaded Producer-Consumer Application

A C implementation of a many-producer, many-consumer (mpmc) channel using mutexes and semaphores

## Prerequisites

- C compiler (e.g., gcc)
- POSIX Threads library (e.g., `-pthread` option)

## Building

Compile the application with:
```bash
gcc -o main main.c channel.c utils.c deque.c priorityQueue.c -pthread -Wall -Wextra -pedantic -Wno-strict-prototypes
```

## Usage

Run the program with sample parameters:
```bash
./main 10 5 3
```
Where:
- `10` represents the number of producer threads
- `5` represents the number of consumer theads
- `3` represents the buffer size

### Expected Output

The program will display something like the following:

```text
Using:
10 producers
5 consumers
3 buffer slots
-----------------
Producer #0 has priority 4.
Producer #1 has priority 4.
Producer #2 has priority 2.
Producer #6 has priority 2.
[Producer #1] sent 10
[Producer #1] sent 11
[Producer #1] sent 10
[Producer #0] sent 12
[Consumer #0] received 10
[Consumer #1] received 11
[Producer #2] sent 11
[Consumer #2] received 12
...
Producer #1 finished. Items sent: all numbers between 10 and 19
...
Consumer #0 finished. Items recieved: 10, 13, 18, 38
...
```

## Testing

Compile and run the tests with:
```bash
gcc -o test tests.c channel.c utils.c deque.c -pthread -Wall -Wextra -pedantic -Wno-strict-prototypes
./test
```
