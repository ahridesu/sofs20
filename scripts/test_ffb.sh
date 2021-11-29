#!/bin/bash

# Dário Notas:
# arg1 -> path até ao disco;
# usar -> source alloc_inode.sh; cd bin/; alloc_inode dsk;

function free_file_blocks()
{
	for i in {1..1500}
	do
		echo -e "afb\n0\n${i}\n" | ./testtool -g -q1 $1
		((i++))
	done

	echo 0
}