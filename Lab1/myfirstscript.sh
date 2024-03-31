#!/bin/bash

#2
rm  -r lab1/

#3
mkdir lab1

#4
cp words.txt lab1/words.txt
cp numbers.txt lab1/numbers.txt

#5
paste lab1/words.txt lab1/numbers.txt > lab1/MergedContent.txt

#6
head -n 3 lab1/MergedContent.txt

#7
sort lab1/MergedContent.txt > lab1/SortedMergedContent.txt

#8
echo "The sorted file is:"

#9
cat lab1/SortedMergedContent.txt

#10
chmod a-r lab1/SortedMergedContent.txt

#11
uniq lab1/SortedMergedContent.txt

#12
cat lab1/SortedMergedContent.txt | tr[a-z] [A-Z] > lab1/CapitalSortedMergedContent.txt

#13
echo "we took the content of sortedmergedcontent.txt and passed it to tr to change small letters to capital letters but there was an error because we removed the permission"

#14
chmod a+r lab1/SortedMergedContent.txt
#then repeat #12

#15
grep -n "^w.*[0-9]$" lab1/MergedContent.txt

#16
cat lab1/MergedContent.txt | tri o > lab1/NewMergedContent

#17
paste lab1/MergedContent.txt lab1/NewMergedContent.txt












