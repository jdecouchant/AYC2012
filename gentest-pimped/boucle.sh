
rand() {
	echo $((`cat /dev/urandom|od -N1 -An -i` % $1))
}

while true
do

TIME=$(date -u +%s)

L_REF=$(rand 5000)
L_SEQ=$(rand 5000)
N_SEQ=$(rand 100)
N_FILES=$(rand 10)
CORES=$(rand 99)
L_CORES=$((`rand 500` + 4))


./datagen $L_REF $L_SEQ $N_SEQ $N_FILES dataset1 0.$CORES

../ayc-2/run 8 $L_CORES $(ls *.txt) > out1
../diago/run 8 $L_CORES $(ls *.txt) > out2

		if ! diff -q "out1" "out2" > /dev/null
		then
			echo ">>>> ERROR: out are differents !"
			echo ">> ../ayc-2/run 8 $L_CORES $(ls *.txt)"
			mkdir "bad_$TIME"
			mv *.txt out1 out2 "bad_$TIME"
		fi

rm *.txt

done