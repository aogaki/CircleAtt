#!/bin/bash

suffix=`printf %04d $1`
fileName="result$suffix.root"
hadd -f $fileName Pipe/$fileName tmp/$fileName
