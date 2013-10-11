#!/usr/bin/python

########################################
#
# This is a image generator
#
########################################

########################################
# cElementTree is much more faster 
# then ElementTree
try:
    import xml.etree.cElementTree as ET
except ImportError:
    import xml.etree.ElementTree as ET

import os, sys
from PIL import Image


########################################
# Get target image and destination images

inFile = ''
outFile = ''

if len(sys.argv) != 3:
	print 'Input format error!'
else:
	inFile = sys.argv[1]
	outFile = sys.argv[2]

tree = ET.ElementTree(file='')
