#!/bin/bash

tmpName="tmp_t$1.root"
seed=`od -vAn -N3 -tu4 < /dev/urandom`
./BI -o $tmpName -m test.mac
#./BI -o $tmpName -m test.mac -s $seed
