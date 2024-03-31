#!/bin/bash

function checkpal {

	str=$1
	left=0
	right=$((${#str} - 1))
	right1=$((right))

	lower_str="${str,,}"

	flag=1  #lw 1 yb2o byt3kso 0 la

	for ((i=0; i<right1; i++)); do

		#compare the first and last letter and loop
		if [ ${lower_str:$left:1} != ${lower_str:$right:1} ]; then
			flag=0
		fi

		#change the letter
		left=$((left + 1))
		right=$((right - 1))
	done

	echo "$flag"

}
