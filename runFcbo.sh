#!/bin/sh

# Generate ppm with simple binarization algorithm
python src/binarizezhipeng.py img_src/test.jpg img_bin/test_p.ppm

# Generate ppm with algorithm of Niblack (1986)
./src/binarizewolfjolion n img_src/test.jpg img_bin/test_n.ppm

# Generate ppm with algorithm of Wolf et al. (2001)
./src/binarizewolfjolion w img_src/test.jpg img_bin/test_w.ppm

# Generate ppm with algorithm of Sauvola et al. (1997)
./src/binarizewolfjolion s img_src/test.jpg img_bin/test_s.ppm

# construct format input for Galicia, etc.
python src/generateTableFcbo.py img_bin/test_p.ppm img_bin/test_w.ppm img_bin/test_n.ppm img_bin/test_s.ppm

# Generate different images according to different lattice concepts
# python src/generateImage.py fca/lattice.lat.xml
