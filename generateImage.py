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
from PIL import Image


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

objList=[]
attList=[]
resList=[]

for child in root[1]:
	# print child.tag, child.get('id'), child.text
	objList.append(child.text)

for child in root[2]:
	attList.append(child.text)

for child in root[3]:
	if child.get('id') == '0':
		continue
	obj = child[0].findall('OBJ')
	res = []
	for objChild in obj:
		res.append(objChild.get('id'))
	resList.append(res)

print len(objList)
print len(attList)
print resList