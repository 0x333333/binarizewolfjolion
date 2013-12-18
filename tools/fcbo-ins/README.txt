FCBO(7)                         FCA Algorithms                         FCBO(7)



NAME
       fcbo - computes formal concepts and maximal frequent itemsets

SYNOPSIS
       fcbo [OPTION]... [INPUT-FILE] [OUTPUT-FILE]

DESCRIPTION
       This  program  computes  intents  of  all formal concepts in an object-
       attribute data set (a formal context), i.e. the algorithm computes  all
       maximal submatrices of a boolean matrix which are full of 1's. The pro-
       gram implements FCbO, a fast algorithm based on  Kuznetsov's  CbO  with
       improved canonicity test.

       The  INPUT-FILE  is  in  the  usual FIMI format: each line represents a
       transaction or an object and it contains of a list  of  attributes/fea-
       tures/items.  If  the INPUT-FILE is omitted or if it equals to `-', the
       program reads the input form the stdin.  The OUTPUT-FILE has a  similar
       format,  each line represents one intent (itemset), where numbers indi-
       cate attributes in the intent (itemset). If the OUTPUT-FILE is  omitted
       or if it equals to `-', the program writes the input to the stdout.

   Optional arguments
       -index,
              sets the initial index of the first attribute. The default value
              is 0, meaning that attributes are numbered from 0 upwards. If  a
              data  set  uses  attributes  numbered from 1, you should use the
              `-1' switch, and so on.

       -Smin-support,
              the minimal  support  considered  is  set  to  min-support.  The
              default  value is 0, meaning that the support is disregarded and
              all intents (itemsets) are written to the output. If min-support
              is set to a positive value, only itemsets having extents with at
              least min-support are written to the output.

       -Vlevel,
              sets the verbosity level to a specified value. Permitted  values
              are  numbers  from  0 up to 3. The default value is 1. Verbosity
              level 0 (no output) just computes the intents  and  produces  no
              output. Verbosity level 1 produces lists of intents with no aux-
              iliary output. Verbosity levels 2 and  higher  write  additional
              information to stderr.

EXAMPLES
       fcbo -1 mushroom.dat

       Computes all intents in the file named mushroom.dat where 1 denotes the
       first attribute in mushroom.dat. The output is written to the  standard
       output.

       fcbo -S200 foo.dat output-intents.dat

       Computes  all  intents  in  foo.dat  with  extents  having at least 200
       objects, writing the output to output-intents.dat.

AUTHORS
       Written by Jan Outrata and Vilem Vychodil.

REPORTING BUGS
       Report bugs to <fcalgs-bugs@lists.sourceforge.net>.

COPYRIGHT
       GNU GPL 2  (http://www.gnu.org/licenses/gpl-2.0.html).   This  is  free
       software:  you  are  free  to  change and redistribute it.  There is NO
       WARRANTY, to the extent permitted by law.



http://fcalgs.sourceforge.net   September 2010                         FCBO(7)
