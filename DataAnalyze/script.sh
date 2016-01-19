#!/bin/bash

for((i=0;i<650;i++))
do
    root -l -q "MakeGraph.cpp+O($i)"
done
