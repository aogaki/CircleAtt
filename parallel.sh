#!/bin/bash

kEvents="100000000"
echo "/run/beamOn $kEvents" > test.mac

cat threads | parallel -j 8 ./wrapper.sh
outName="result"`cat simInfo`".root"
hadd -f $outName tmp*.root
