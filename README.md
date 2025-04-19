# Multithreaded Producer-Consumer Application

Short description of what the project does and its main features.

## Prerequisites

- C compiler (e.g., gcc)
- POSIX Threads library (e.g., `-pthread` option)

## Building

Compile the application with:
```bash
gcc -o main main.c channel.c utils.c deque.c -pthread -Wall -Wextra -pedantic -Wno-strict-prototypes
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

The program will display and return:
- Some calculation based on the input parameters
- Logs or statuses indicating progress

## Testing

Compile and run the tests with:
```bash
gcc -o test tests.c channel.c utils.c deque.c -pthread -Wall -Wextra -pedantic -Wno-strict-prototypes
./test
```
