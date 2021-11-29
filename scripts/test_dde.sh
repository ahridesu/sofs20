#!/bin/bash

# Paulo Gil
# $1 disk path
# call script -> source dde_datablock.sh; cd bin; test_dde /tmp/dsk;

function test_dde()
{
	for i in {1..9}
	do
		echo -e "ade\n0\n$i$i$i$i\n$i" | ./testtool -g -q1 $1
		((i++))
	done
	
	echo -e "sb\nd\n5" | ./testtool -g -q1 $1
	
	for i in {1..3}
	do
		echo -e "dde\n0\n$i$i$i$i" | ./testtool -g -q1 $1
	done

	echo -e "sb\nd\n5" | ./testtool -g -q1 $1
}
