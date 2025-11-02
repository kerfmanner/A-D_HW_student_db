#!/usr/bin/env bash
set -e

echo "method,input_size,time_ms" > sort.csv

for m in builtin radix; do
  for n in 100 1000 10000 100000; do
    ./build/srt_bench --method=$m --input=./dataset/${n}.csv
  done
done

echo "Done"
