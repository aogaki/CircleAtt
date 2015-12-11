#!/bin/bash

#for t in `cat thickness`
for t in `cat testT`
do
    for((i=0;i<75;i++))
    do
	root -l -q "MakeSingleAtt.cpp+O($i,$t)" 
    done
done
