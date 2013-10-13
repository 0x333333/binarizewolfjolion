#!/usr/bin/python

########################################
#
# This is a image generator
#
########################################

########################################
# XML example:
#
# <?xml version="1.0" encoding="UTF-8"?>
# <LAT Desc="testBin - lattice.slf - #OfNodes = 6" type="ConceptLattice">
#   <MINSUPP>0.0</MINSUPP>
#   <OBJS>
#     <OBJ id="">(0,0)</OBJ>
#     ...
#   </OBJS>
#   <ATTS>
#     <ATT id="">n</ATT>
#     ...
#   </ATTS>
#   <NODS>
#     <NOD id="">
#       <EXT>
#         <OBJ id="" />
#         ...
#       </EXT>
#       <INT>
#         <ATT id="" />
#       </INT>
#       <SUP_NOD>
#         <PARENT id="" />
#       </SUP_NOD>
#     </NOD>
#     ...
#   </NODS>
# </LAT>
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
from PIL import Image, ImageDraw


########################################
# Get target image and destination images

inFile = ''
outFile = ''

if len(sys.argv) > 3:
	print 'Input format error!'
else:
	inFile = sys.argv[1]
	outFile = sys.argv[2]

tree = ET.parse(inFile)
root = tree.getroot()

objList=[] # Save all objects
attList=[] # Save all attributes
resList=[] # Save objects for each lattice node
intList=[] # Save attributes for each lattice node

# Extract objects list
for child in root[1]:
	objList.append(child.text)

# Extract attributes list
for child in root[2]:
	attList.append(child.text)

# Extract objects and attributes for each lattice node 
for child in root[3]:
	if child.get('id') == '0':
		continue
	obj = child[0].findall('OBJ')
	att = child[1].findall('ATT')
	res = []
	for objChild in obj:
		res.append(objChild.get('id'))
	resList.append(res)
	res = []
	for attChild in att:
		res.append(attChild.get('id'))
	intList.append(res)

# Find the size of image
height = 0
width = 0
for i in objList:
	if i[1] == '0':
		height = height + 1
	else:
		break
width = int(objList[len(objList)-1][1]) + 1

# print height
# print width

# print objList
# print attList
# print resList
# print intList

# Create new images
for i in range(0,len(resList)):
	# Construct file name
	fileName = 'img_'
	for x in intList[i]:
		fileName = fileName + attList[int(x)]
	fileName = fileName + '.ppm'

	# Create new image and save it
	im = Image.new("RGB", (width, height), (255, 255, 255))

	for y in resList[i]:
		obj = objList[int(y)]
		im.putpixel((int(obj[1]), int(obj[3])), 0)
	im.save('img/'+fileName)