#!/bin/bash

set -u

# A quick script to build a day's code.
# Example: ./build.sh -d day1
# Flags:
#   1) -d Specify the day you want to compile
#   2) -c CleanAll
#   3) -r Run

CC=gcc
CFLAGS="-std=c99 -pedantic -Wall"
DAY=day1
OUT=out

# Make sure working directory is the right day:
if [ ! -f "build.sh" ] || [ ! -d $DAY ]
then
	echo "Exiting: Please change to the directory with the build.sh script"
	exit 0
fi

# Compile/run/clean:
if [ $? -eq 0 ]
then
	while getopts d:cr flag
	do
		case "${flag}" in
			c) for d in */; do cd $d; rm ${OUT}; cd ..; done;
				exit 0;;
			d) DAY="${OPTARG}";
				cd $DAY;
				echo "Attempting to compile '${DAY}'...";
				$CC $CFLAGS ${DAY}.c -o ${OUT};
				cd ..;
				echo "Finished compilation";;
			r) echo "Attempting to run '${DAY}'...";
				cd $DAY;
				./${OUT};
				cd ..;;
		esac
	done
fi

echo "Done"
