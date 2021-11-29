#!/bin/bash

# Paulo Gil
# $1 disk path
# call script -> source test_traverse.sh; cd bin; test_traverse /tmp/dsk;

function test_traverse()
{
	echo -e "ade\n0\naaaa\n33\n" | ./testtool -g -q1 $1
	echo -e "ade\n0\nbbbb\n44\n" | ./testtool -g -q1 $1

	echo -e "tp\n/aaaa\n" | ./testtool -g -q1 $1
	echo -e "tp\n/bbbb\n" | ./testtool -g -q1 $1
}
