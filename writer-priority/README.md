# Writer's Priority

This program is built using different threads for reader and writer, and uses semaphores for critical access of the file.

To run the program, first build the c file and then run it

```sh
gcc main.c -lpthread -o main.out
./main.out
```