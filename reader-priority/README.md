# Reader's Priority

This program is built using different processes for reader and writer, and uses semaphores are counter variable for the number of readers as semaphores are stored on the OS and are not program specific. 

To run the program, first build all the files using the bash script.

```sh
chmod +x build_all.sh
./build_all.sh
```

Run multiple readers and writers using the following commands in different terminals
```sh
./writer.out
./reader.out
```

To see the semaphores on your linux system run the following command

```sh
ls -al /dev/shm/sem.*|more
```
