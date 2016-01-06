#!/bin/bash


dir=$1

for((i=1;i<=3750;i++))
do
    suffix=`printf %04d $i`
    fileName="result$suffix.root"
    if [ ! -e $dir/$fileName ]
    then
	#echo $fileName
	echo $i
    fi
done
