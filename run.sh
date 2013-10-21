#!/bin/sh

# Generate ppm with simple binarization algorithm
python binarizezhipeng.py test.jpg test_p.ppm

# Generate ppm with algorithm of Niblack (1986)
./binarizewolfjolion n test.jpg test_n.ppm

# Generate ppm with algorithm of Wolf et al. (2001)
./binarizewolfjolion w test.jpg test_w.ppm

# Generate ppm with algorithm of Sauvola et al. (1997)
./binarizewolfjolion s test.jpg test_s.ppm

# construct format input for Galicia, etc.
python generateTable.py test_p.ppm test_w.ppm test_n.ppm test_s.ppm

# Generate different images according to different lattice concepts
# python generateImage.py lattice.lat.xml
