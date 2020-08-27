#!/bin/bash

gcc main.c -o prog `pkg-config fuse --cflags --libs`
./prog -f myfs/
