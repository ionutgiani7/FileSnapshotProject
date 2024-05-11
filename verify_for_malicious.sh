#!/bin/bash
if [ $# -ne 1 ]
then
     echo "Numar diferit de argumente"
     exit 1
fi
in="$1"
lines=$(wc -l "$1" | cut -f 1 -d ' ')
words=$(wc -w "$1" | cut -f 1 -d ' ')
characters=$(wc -c "$1" | cut -f 1 -d ' ')
per=0


if [ $lines -lt 3 ] && [ $words -gt 1000 ] && [ $characters -gt 2000 ]
then  
       while read -r line
       do
            if echo "$line" | grep -Pq "corrupted|risk|dangerous|attack|malware|[\x80-\xFF]"
            then
                   per=1
                   break
            fi
       done < "$in"
fi


if [ $per -eq 1 ]
then
     echo $(realpath "$in")
     mv -f "$1" $(realpath "izolated_space_dir")
else
     echo "SAFE"
fi