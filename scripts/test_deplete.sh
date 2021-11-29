#!/bin/bash

# Paulo Gil
# $1 disk path
# call script -> source test_deplete.sh; cd bin; test_deplete /tmp/dsk;

function test_deplete()
{
	for i in {1..90}
	do
		echo -e "adb\n" | ./testtool -g -q1 $1
		((i++))
	done

	for i in {10..80}
	do
		echo -e "fdb\n$i\n" | ./testtool -g -q1 $1
		((i++))
	done

	echo -e "sb\ns\n0\n" | ./testtool -g -q1 $1
}
