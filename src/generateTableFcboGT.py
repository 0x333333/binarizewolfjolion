#!/usr/bin/python

########################################
#
# This is a fca input file generator
#
########################################

import os, sys

inFile1 = ''

if len(sys.argv) != 2:
	print 'Input format error!'
else:
	inFile1 = sys.argv[1]

########################################
# Construct input file for Fcbo (GT)

fileNbr = 1

while fileNbr < 11:

	f = file('fca/InputDataFcboGT' + str(fileNbr) + '.dat', 'w')

	for line in open(inFile1 + 'total_gt_' + str(fileNbr) + '.csv'):
	    Radon, GFD, zernike, SC, ARG = line.split(",")
	    if Radon == '1':
	    	f.write(str(0) + ' ')
	    if GFD == '1':
	    	f.write(str(1) + ' ')
	    if zernike == '1':
	    	f.write(str(2) + ' ')
	    if SC == '1':
	    	f.write(str(3) + ' ')
	    if ARG == '1':
	    	f.write(str(4) + ' ')
	    f.write('\n')

	f.close()

	fileNbr += 1