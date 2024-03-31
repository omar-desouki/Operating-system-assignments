#!/bin/bash
function convert {

if [ "$1" = 2 ]; then   #decimal to binary

decimal=$2
binary=""

	while [ $decimal -gt 0 ]; do
	remainder=$((decimal % 2))
	binary="$remainder$binary"
	decimal=$((decimal / 2))
	done

echo "$binary"


else                      #binary to decimal


number=$2

if [ -z "$2" ]; then		#condition en el binary to decimal hwa el default
number=$1
fi

number_str="$number" #btstore el binary number ka string
decimal=0
count=0

for ((i=${#number_str}; i>0; i--)); do 	#btloop 3ala rakm rakm
	number_on="${number_str:i-1:1}"

	if [ "$number_on" = 1 ]; then #lw el rakm 1 bazwe 2^tartebo
	decimal=$((decimal + 2 ** count))
	fi
count=$((count+1))

done
echo "$decimal"

fi

}
