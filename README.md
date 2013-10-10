# Document Image Binarization

* * *

Lab website: [http://liris.cnrs.fr/christian.wolf/software/binarize/](http://liris.cnrs.fr/christian.wolf/software/binarize/)

There are two parts in this repo. The first part is written in python, which enable a simple binarization algorithm.

The second part uses an improved contrast maximization version of Niblack/Sauvola et al's method to binarize document images. It is also able to perform the more classical Niblack as well as Sauvola et al. methods. Details can be found in the [ICPR 2002 paper](http://liris.cnrs.fr/christian.wolf/publications/index.html#icpr2002v).

![example](http://liris.cnrs.fr/christian.wolf/software/binarize/ss_binarize_annotated.png)

More example can be found in my blog: 

[http://jesusjzp.github.io/blog/2013/10/04/document-binarization/](http://jesusjzp.github.io/blog/2013/10/04/document-binarization/)

## Compilation

The code has been developed under Linux.

Compiler:

	./build_all.sh

## Usage:

### Execute step by step

Generate ppm with simple binarization algorithm

	python binarizezhipeng.py test.jpg test_p.ppm

Generate ppm with algorithm of Niblack (1986)

	./binarizewolfjolion n test.jpg test_n.ppm

Generate ppm with algorithm of Wolf et al. (2001)

	./binarizewolfjolion w test.jpg test_w.ppm

Generate ppm with algorithm of Sauvola et al. (1997)

	./binarizewolfjolion s test.jpg test_s.ppm

Construct format input for Galicia, etc.

	python generateTable.py test_p.ppm test_w.ppm test_n.ppm test_s.ppm

### Execute by default

I have integrated all those operation into a shell script file:

	./run.sh

## Licence

Permission is granted for anyone to copy, use, modify, or distribute this program and accompanying programs and documents for any purpose, provided this copyright notice is retained and prominently displayed, along with a note saying that the original programs are available from my web page.

The programs and documents are distributed without any warranty, express or implied. As the programs were written for research purposes only, they have not been tested to the degree that would be advisable in any important application.

All use of these programs is entirely at the user's own risk.
