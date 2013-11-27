# Document Image Binarization

## Project content

We propose to investigate formal data mining techniques to try and measure levels of agreement and disagreement between various algorithms. For example, this application domain will be in the area of Document Image Analysis.

We are going to assume that we have different algorithms trying to solve the same given interpretation problem. Although they are supposed to solve the same problem, they will not necessarily obtain the same results in all the input data. We will try and categorize the levels of agreement and disagreement between algorithms.

Four algorithms are used:

- Simple binarization algorithm
- Niblack
- Sauvola et al.
- Wolf et al.

There are two parts in this repo. The first part is written in python, which enable a simple binarization algorithm.

The second part uses an improved contrast maximization version of Niblack/Sauvola et al's method to binarize document images. It is also able to perform the more classical Niblack as well as Sauvola et al. methods. Details can be found in the [ICPR 2002 paper](http://liris.cnrs.fr/christian.wolf/publications/index.html#icpr2002v).

![example](http://liris.cnrs.fr/christian.wolf/software/binarize/ss_binarize_annotated.png)

More example can be found in my blog: 

[http://jesusjzp.github.io/blog/2013/10/04/document-binarization/](http://jesusjzp.github.io/blog/2013/10/04/document-binarization/)

## Usage:

### Folders

- fca/     : input file and out file for [Lattice Miner](http://sourceforge.net/projects/lattice-miner/).
- img/     : all images which corresponde to each point in Lattice tree.
- img_bin/ : all ppm image filess generated by binarization algorithms.
- img_src/ : test image.
- src/     : source code.
- tools/   : execution file of Lattice Miner.

### Execute step by step

Generate ppm files with *simple binarization algorithm*

	python src/binarizezhipeng.py img_src/test.jpg img_bin/test_p.ppm

Generate ppm with algorithm of *Niblack (1986)*

	./src/binarizewolfjolion n img_src/test.jpg img_bin/test_n.ppm

Generate ppm with algorithm of *Wolf et al. (2001)*

	./src/binarizewolfjolion w img_src/test.jpg img_bin/test_w.ppm

Generate ppm with algorithm of *Sauvola et al. (1997)*

	./src/binarizewolfjolion s img_src/test.jpg img_bin/test_s.ppm

Generate input file(.slf) for Lattice Miner.

	python src/generateTable.py img_bin/test_p.ppm img_bin/test_w.ppm img_bin/test_n.ppm img_bin/test_s.ppm

File 'testBin.slf' is the input file for [Calicia](http://www.iro.umontreal.ca/~galicia/) or [Lattice Miner](http://sourceforge.net/projects/lattice-miner/).

Open 'testBin.slf' from Lattice Miner and display the context:

![pic](http://media-cache-ec0.pinimg.com/originals/4a/9c/99/4a9c997588dea2e85e0bd8544dba3499.jpg)

Generate Lattice tree by *Ctrl+L*:

![pic](http://media-cache-ec0.pinimg.com/originals/b6/99/e1/b699e1a9fb823bc80187a7a5682b3902.jpg)

Save this Lattice tree to *.lat.xml* file by *Ctrl+S*.

Generate all images corresponding to each point in Lattice tree:

	python src/generateImage.py fca/lattice.lat.xml

## Limitations (To be completed)

1.  Input scale is not large enough for the image analysis.
    reference: [http://biblio.info.ucl.ac.be/2004/285670.pdf](http://biblio.info.ucl.ac.be/2004/285670.pdf)

## Licence

Permission is granted for anyone to copy, use, modify, or distribute this program and accompanying programs and documents for any purpose, provided this copyright notice is retained and prominently displayed, along with a note saying that the original programs are available from my web page.

The programs and documents are distributed without any warranty, express or implied. As the programs were written for research purposes only, they have not been tested to the degree that would be advisable in any important application.

All use of these programs is entirely at the user's own risk.