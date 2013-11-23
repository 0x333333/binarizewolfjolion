#!/usr/bin/python

########################################
# This is a jpg->ppm conventor
# By using simple binarization algorithm
########################################

import os, sys
from PIL import Image

inFile = ''
outFile = ''

if len(sys.argv) != 3:
	print 'Input format error!'
else:
	inFile = sys.argv[1]
	outFile = sys.argv[2]

im = Image.open(inFile).convert('L')

for i in range(im.size[0]):
	for j in range(im.size[1]):
		if im.getpixel((i,j)) > 127:
			im.putpixel((i,j), 255)
		else:
			im.putpixel((i,j), 0)

im.save(outFile)
