#!/bin/bash

fio benchmarks/random_read.fio
fio benchmarks/random_write.fio
fio benchmarks/sequential_read.fio
fio benchmarks/sequential_write.fio
fio benchmarks/random_write_steady.fio
fio benchmarks/sequential_write_steady.fio
