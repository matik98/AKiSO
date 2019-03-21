#!/bin/bash

cd /proc

printf "____________________________________________________________\n\n"
printf "NAME               | STATE          | PID   | PPID  | NOF\n"
printf "____________________________________________________________\n"
for process in *
do
	if [ $process -eq $process 2> /dev/null ]
	then
		cd  /proc/${process}
		
		name=$(sed -n 1p status)
		state=$(sed -n 3p status)
		pid=$(sed -n 6p status)
		ppid=$(sed -n 7p status)
		licznik=$(sudo ls fd | wc -l)
		
		name=${name#'Name:	'}
	      	state=${state#'State:	'}
	      	pid=${pid#'Pid:	'}
	        ppid=${ppid#'PPid:	'}

		printf "$name"
	   	for j in $(seq 0 $((20-${#name})))
		do
			printf ' '
		done
		printf "$state"
		for j in $(seq 0 $((16-${#state})))
		do
			printf ' '
		done
		printf "$pid"
		for j in $(seq 0 $((7-${#pid})))
		do
			printf ' '
		done
		printf "$ppid"
		for j in $(seq 0 $((7-${#ppid})))
		do
			printf ' '
		done
		printf "$licznik\n"

	fi
done

