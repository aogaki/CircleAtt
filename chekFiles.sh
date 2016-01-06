#!/bin/bash


dir=$1

for((i=1;i<=3750;i++))
do
    suffix=`printf %04d $i`
    fileName="result$suffix.root"
    if [ ! -e Data/Ni/$dir/$fileName ]
    then
	attName="att$suffix.dat"
	ln -sf tools/$attName ./att.dat
	./BI -m test.mac -o $fileName
	mv $fileName Data/Ni/$dir/
    fi
done
