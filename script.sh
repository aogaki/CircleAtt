#!/bin/bash

events="50000000"
material="G4_Al"
T="1000"

echo "/run/beamOn $events" > tmp.mac

for((plate=0;plate<=75;plate++))
do
    for thick in `cat thickness`
    do
	root -l -q "MakeAtt.cpp($plate, $thick)"
	./BI -m tmp.mac
	mv result.root p"$plate"t"$thick".root
    done
done
