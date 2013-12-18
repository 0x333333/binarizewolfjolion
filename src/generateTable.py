#!/usr/bin/python

########################################
#
# This is a fca input file generator
#
########################################

########################################
# Get target image and destination images

import os, sys
from PIL import Image

inFile1 = ''
inFile2 = ''
inFile3 = ''
inFile4 = ''

if len(sys.argv) != 5:
	print 'Input format error!'
else:
	inFile1 = sys.argv[1]
	inFile2 = sys.argv[2]
	inFile3 = sys.argv[3]
	inFile4 = sys.argv[4]

########################################
# Analyze image

im = Image.open(inFile1)

multilist = [[0 for col in range(4)] for row in range(im.size[0]*im.size[1])]

for i in range(im.size[0]):
	for j in range(im.size[1]):
		if im.getpixel((i,j)) == 0:
			multilist[i*im.size[1]+j][0] = 1

im = Image.open(inFile2)

for i in range(im.size[0]):
	for j in range(im.size[1]):
		if im.getpixel((i,j)) == 0:
			multilist[i*im.size[1]+j][1] = 1

im = Image.open(inFile3)

for i in range(im.size[0]):
	for j in range(im.size[1]):
		if im.getpixel((i,j)) == 0:
			multilist[i*im.size[1]+j][2] = 1

im = Image.open(inFile4)

for i in range(im.size[0]):
	for j in range(im.size[1]):
		if im.getpixel((i,j)) == 0:
			multilist[i*im.size[1]+j][3] = 1

########################################
# Construct slf file for Galicia or Lattice Miner

f = file('fca/testBin.slf','w')

f.write('[Lattice]'+'\n')
f.write(str(len(multilist))+'\n')
f.write('8'+'\n')

f.write('[Objects]'+'\n')
for i in range(im.size[0]):
	for j in range(im.size[1]):
		f.write('('+str(i)+','+str(j)+')'+'\n')

f.write('[Attributes]'+'\n')
# f.write('p1'+'\n')
# f.write('p2'+'\n')
# f.write('w1'+'\n')
# f.write('w2'+'\n')
# f.write('n1'+'\n')
# f.write('n2'+'\n')
# f.write('s1'+'\n')
# f.write('s2'+'\n')

f.write('0'+'\n')
f.write('1'+'\n')
f.write('2'+'\n')
f.write('3'+'\n')
f.write('4'+'\n')
f.write('5'+'\n')
f.write('6'+'\n')
f.write('7'+'\n')

f.write('[relation]'+'\n')
for i in range(0,len(multilist)):
	for j in range(0,len(multilist[i])):
		f.write(str(multilist[i][j]) + ' ')
		f.write(str(1-multilist[i][j]) + ' ')
	f.write('\n')

f.close()