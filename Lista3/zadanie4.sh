#!/bin/bash

http=$1
sleep_time=$2

if [ ! -e git ];
then
	echo 'CREATING CONNECTION WITH GIT'
	git clone git://github.com/matik98/website-history "git"
	echo ''
fi

cd git
lynx -dump $http >website.txt
git add website.txt >> /dev/null
git commit -m "Updated website"  >> /dev/null

while [ true ]
do
	sleep ${sleep_time}
	lynx -dump $http >new_website.txt
	diff website.txt new_website.txt >diff.txt
	var=$( sed -n 1p diff.txt)
	echo "ZMIANY:"
	if [ -z $var ];
	then 
		echo "Brak zmian"
	else
		cat diff.txt
	fi
	cat < new_website.txt > website.txt
	git add website.txt >> /dev/null
	git commit -m "Updated website"  >> /dev/null
	printf "\n__________________________________________________________\n"
done



