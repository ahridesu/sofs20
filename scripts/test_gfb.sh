#!/bin/bash

# Paulo Gil
# $1 disk path
# call script -> source test_gfb.sh; cd bin; test_gfb /tmp/dsk;

function test_gfb()
{
	echo -e "gfb\n0\n0\n" | ./testtool -g -q1 $1

	echo -e "gfb\n0\n520\n" | ./testtool -g -q1 $1
	
	echo -e "gfb\n0\n6000\n" | ./testtool -g -q1 $1
}
