#!/bin/bash
gcc init.c -lpthread -o init.out
gcc reader.c -lpthread -o reader.out
gcc writer.c -lpthread -o writer.out
gcc semaphore_reader.c -lpthread -o semaphore_reader.out
