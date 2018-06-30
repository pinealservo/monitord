# Task Description

Implement a linux daemon in modern C++ which will be running on background and will be listening on TCP port 5001.

## Daemon

The daemon should recognize the following two commands sent to that port:

+ cpu : Returns current load of the CPU. The load should be read from /proc/stat and computed as described here:  http://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux/ 

+ mem : Returns memory usage in kilobytes. Read the data from /proc/meminfo and compute the usage like:  MemTotal - MemFree - Buffers - Cached

Both commands ends with '\n'.

## Client

A part of the tasks is to implement also a client for the described daemon. 

+ Client should accept commands, send them to the server and print out the output. 

+ Clients should not block each other.

## Libraries

It's possible to use pthread, boost (asio), libevent and standard template library.

## Other Requirements

Make sure that clients do not block each other. Comment your code so we can see your thinking. 
Append a Makefile or any script which is needed for compiling the code.
Put the code to some public github repository and send us a link.
