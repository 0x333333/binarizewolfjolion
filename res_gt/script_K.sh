#!/bin/bash

K=$1;

shift;

for method in *_text ; do 
    for query in $method/query* ; do 

        queryNb=`echo $query | sed "s|$method/query:||"`
#        gtFile=ground-truth/$queryNb.txt
#	gtFileLines=`wc -l $gtFile | cut -d\  -f 1`
	sort -n "$query" > "$method/sorted_query:$queryNb"

        for i in `cat raw_documents.txt` ; do 
            head -$K "$method/sorted_query:$queryNb" | grep -q  $i  ;
            x=$? ;
            if [ "$x" = "0" ] ; then 
		echo 1 >> "$query"_full_$K; 
            else 
		echo 0 >> "$query"_full_$K; 
	    fi  
        done ;
    done;
done

for i in *_text ; do echo $i | sed s/_text// > $i/total_gt_$K ; cat $i/*full_$K >> $i/total_gt_$K ; \rm $i/*full_$K ; done

paste -d , `find . -name total_gt_$K` > total_gt_$K.csv
