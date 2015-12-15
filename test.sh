#!/bin/bash


kEvents="100000000"

for((i=0;i<75;i++)); do
    for t in `cat thickness`; do
	suffix="$i""T$t"
	attFile="att$suffix.dat"
	
	rm -f att.dat
	ln -sf $attFile att.dat
	echo "/run/beamOn $kEvents" > test.mac
	./BI -m test.mac -o result$suffix.root
    done
done
