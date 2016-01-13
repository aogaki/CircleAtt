#!/bin/bash

suffix=`printf %04d $1`
fileName="result$suffix.root"
hadd -f Pipe/$fileName tmp/$fileName tmp2/$fileName tmp3/$fileName tmp4/$fileName
