#!/bin/bash

for method in *_text ; do for query in $method/query* ; do for i in `cat raw_documents.txt` ; do head -20 "$query" | grep -q  $i  ; x=$? ; if [ "$x" = "0" ] ; then echo 1 >> "$query"_full; else echo 0 >> "$query"_full; fi  done ; done; done

for i in *_text ; do echo $i | sed s/_text// > $i/total ; cat $i/*full >> $i/total ; done

paste -d , `find . -name total` 
