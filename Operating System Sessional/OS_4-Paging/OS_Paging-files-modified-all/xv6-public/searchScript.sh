#!/bin/bash
clear;
oldIfs=$IFS;
IFS=$'\n';
thingToSearch="$1";
#for file in `ls -f` 
for file in `find . -maxdepth 1 -type f` 
do
	
	x=`grep -wc "$thingToSearch" "$file"`
	if [[ $x -gt 0 ]]; then
		echo "=========>>>>> In file : $file"	
		grep -n "$thingToSearch" "$file"
		echo; echo; echo;
	fi
done 
IFS=$oldIfs
