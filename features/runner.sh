#!/bin/bash

echo "runner = $1"
echo "test = $2"

if [ ! -f $2 ]; then
    echo "feature file $2 not found!"
    exit 1
fi

$1 & cucumber $2