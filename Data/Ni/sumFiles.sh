#!/bin/bash

suffix=`printf %04d $1`
fileName="result$suffix.root"
hadd -f Cylinder/$fileName Data01/$fileName Data02/$fileName Data03/$fileName Data04/$fileName Data05/$fileName Data06/$fileName Data07/$fileName Data08/$fileName Data09/$fileName Data10/$fileName
