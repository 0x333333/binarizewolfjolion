#!/bin/bash

    for query in ground-truth/*.txt ; do 

        for i in `cat raw_documents.txt` ; do 
            grep -q  $i $query ;
            x=$? ;
            if [ "$x" = "0" ] ; then 
		echo 1 >> "$query"_gt; 
            else 
		echo 0 >> "$query"_gt; 
	    fi  
        done ;
    done;

#for i in *_text ; do echo $i | sed s/_text// > $i/total_gt ; cat $i/*full >> $i/total_gt ; done

#paste -d , `find . -name total_gt` 
