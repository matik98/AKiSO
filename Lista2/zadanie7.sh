#! /bin/bash

# $1 - lokalizacja folderu docelowego

if [ "$1" != '' ]
then
	cd "$1"
fi

for FILE in ./*
do
	nazwa=`echo $FILE | tr 'A-Z' 'a-z' | tr 'ĄĆĘŁŃÓŚŻŹ' 'ąćęłńóśżź'`
	if [ "./$nazwa" != "./$FILE" ]
	then
		mv "./$FILE" "$nazwa"
	fi
done
