#!/usr/bin/env bash
set -e

echo "db,input_size,ops" > ops.csv
echo "db,input_size,peak_kb" > memory.csv

for db in 1 1L 2 2L 3; do
  for n in 100 1000 10000 100000; do
    echo "*** DB=$db  N=$n ***"

    # time benchmark
    ./benchmark --db=$db --input=${n}.csv --mode=time

    # memory benchmark
    /usr/bin/time -v ./benchmark --db=$db --input=${n}.csv --mode=memory 2>/dev/null
    # 2>/dev/null hides stdout of time
  done
done

echo "Done."
