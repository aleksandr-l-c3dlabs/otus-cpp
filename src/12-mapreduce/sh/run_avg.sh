#!/bin/bash

BINARY_HOME=/usr/lib/mapreduce
DEFAULT_FILE="/var/lib/mapreduce/AB_NYC_2019.csv"

if [ $# -eq 0 ]; then
    INPUT="$DEFAULT_FILE"
else
    INPUT="$1"
fi

if [ ! -f "$INPUT" ]; then
    echo "Error: file '$INPUT' not found!" >&2
    exit 1
fi

cat ${INPUT} | ${BINARY_HOME}/mapper | sort -k1 | ${BINARY_HOME}/average/reducer