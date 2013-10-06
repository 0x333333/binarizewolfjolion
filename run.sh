#!/bin/sh

# generate ppm with simple algorithm
python binarizezhipeng.py test.jpg test_p.ppm

# generate ppm with simple algorithm
./binarizewolfjolion n test.jpg test_n.ppm

# generate ppm with simple algorithm
./binarizewolfjolion w test.jpg test_w.ppm

# generate ppm with simple algorithm
./binarizewolfjolion s test.jpg test_s.ppm

# construct format input for Galicia, etc.
python generateTable.py test_p.ppm test_w.ppm test_n.ppm test_s.ppm
