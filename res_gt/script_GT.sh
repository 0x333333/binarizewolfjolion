#!/bin/bash

for method in *_text ; do 
    for query in $method/query* ; do 

        queryNb=`echo $query | sed "s|$method/query:||"`
        gtFile=ground-truth/$queryNb.txt
	gtFileLines=`wc -l $gtFile | cut -d\  -f 1`

        for i in `cat raw_documents.txt` ; do 
            head -$gtFileLines "$query" | grep -q  $i  ;
            x=$? ;
            if [ "$x" = "0" ] ; then 
		echo 1 >> "$query"_full; 
            else 
		echo 0 >> "$query"_full; 
	    fi  
        done ;
    done;
done

for i in *_text ; do echo $i | sed s/_text// > $i/total_gt ; cat $i/*full >> $i/total_gt ; done

paste -d , `find . -name total_gt` 
