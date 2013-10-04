#!/usr/bin/python

import os, sys
from PIL import Image

# for infile in sys.argv[1:]:
#     f, e = os.path.splitext(infile)
#     outfile = f + ".jpg"
#     if infile != outfile:
#         try:
#             Image.open(infile).save(outfile)
#         except IOError:
#             print "cannot convert", infile

# im = Image.open("sample.jpg")
# print im.size
# print im.getpixel((0,0))

#im.show()

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

im.show()
im.save(outFile)