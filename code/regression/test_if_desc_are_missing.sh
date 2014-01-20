#!/bin/bash
A=$(ls * | cut -d. -f1 | sort | uniq -c | grep "1" | tail -n +4 |awk -F' ' '{ print $2".sh" }') 


#use this script to check if all .desc have been made for every .sh and what
#was the next person which have done modification on it.

#ls * | cut -d. -f1 | sort | uniq -c | grep "1" | tail -n +4 |awk -F' ' '{ print $2".sh" }' | xargs git log -1 | grep -i "Author" | cut -d" " -f2-
