#!/usr/bin/env bash
set -e

echo "db,input_size,ops" > ops.csv
echo "db,input_size,peak_kb" > memory.csv

for db in 1 1L 1S 2 2L 2S 3; do
  for n in 100 1000 10000 100000; do
    echo "----------------- DB=$db  N=$n -----------------"

    # time benchmark
    ./build/benchmark --db=$db --input=./dataset/${n}.csv --mode=time

    # memory benchmark
    /usr/bin/time -v ./build/benchmark --db=$db --input=./dataset/${n}.csv --mode=memory 2>/dev/null
    # 2>/dev/null hides stdout of time
  done
done

echo "Done."
