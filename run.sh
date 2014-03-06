############
#
# Don't run this script directly!
# As there is two branches, for Galicia and Fcbo.
#
############



############
# Preparation of images

# Generate ppm with simple binarization algorithm
# python [source code] [input image] [output image]
python src/binarizezhipeng.py img_src/test.jpg img_bin/test_p.ppm

# Generate ppm with algorithm of Niblack (1986)
# ./[source code] [option of algorithm] [input image] [output image]
./src/binarizewolfjolion n img_src/test.jpg img_bin/test_n.ppm

# Generate ppm with algorithm of Wolf et al. (2001)
# ./[source code] [option of algorithm] [input image] [output image]
./src/binarizewolfjolion w img_src/test.jpg img_bin/test_w.ppm

# Generate ppm with algorithm of Sauvola et al. (1997)
# ./[source code] [option of algorithm] [input image] [output image]
./src/binarizewolfjolion s img_src/test.jpg img_bin/test_s.ppm



############
# Next there are two branches, one is for Galicia, one is for Fcbo.
############


############
# Galicia

# construct format input for Galicia, etc.
# python [source code] [input image 1] [input image 2] [input image 3] [input image 4]
python src/generateTableGalica.py img_bin/test_p.ppm img_bin/test_w.ppm img_bin/test_n.ppm img_bin/test_s.ppm

# After this step, testBin.slf will be generated in fca/, this is the input file for Calicia. 
# Once imported into Galicia, save lattice image as lattice.lat.xml. 

# Generate different images according to different lattice concepts
# python [source code] [lattice data]
python src/generateImage.py fca/lattice.lat.xml



############
# Fcbo

# construct format input for Fcbo.
# python [source code] [input image 1] [input image 2] [input image 3] [input image 4]
# After this step, testBinFcbo.dat will be generated in fca/, this is the input file for Fcbo. 
python src/generateTableFcbo.py img_bin/test_p.ppm img_bin/test_w.ppm img_bin/test_n.ppm img_bin/test_s.ppm

# Generate result data with Fcbo
# ./[source code] [input data] [output data]
./tools/fcbo-ins/fcbo fca/InputDataFcbo.dat fca/resFcbo.dat 



############
# Fcbo + GT(Ground Truth)

# construct format input for Fcbo
# python [source code] [folder name of csv files]
# After this step, InputDataFcboGT[1-10].dat will be generated in fca/, this is the input file for Fcbo.
python src/generateTableFcboGT.py res_gt/

# Generate result data with Fcbo
# ./[source code] [input data] [output data]
./tools/fcbo-ins/fcbo fca/InputDataFcboGT1.dat fca/resFcbo1.dat
./tools/fcbo-ins/fcbo fca/InputDataFcboGT2.dat fca/resFcbo2.dat
./tools/fcbo-ins/fcbo fca/InputDataFcboGT3.dat fca/resFcbo3.dat
# ...
