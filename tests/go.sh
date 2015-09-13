
# afficher en rouge le premier parametre
echoinred() {
	echo -e '\e[0;31m'$1
	tput sgr0
}
# afficher en bleu le premier parametre
echoinblue() {
	echo -e '\e[0;34m'$1
	tput sgr0
}


if [ "X$1" == "X" ]
then
	echo "usage : ./go.sh RUN_PROGRAMME"
	exit
fi

RUN=$1

OuputInFile=$2

RUN_reference="../ayc-2/run"

temp=$(uname -p | md5sum)
temp=${temp:0:6}
MACHINE=${temp//[-._ ()@\/\\\"\'\`\~-]/}

TIME=$(date -u +%s)

TIMEPGM=$(which time)

format="
Elapsed real time                  %E
CPU-seconds spent in kernel        %S
CPU-seconds spent in user          %U
Percentage of the CPU got          %P
Maximum resident set size          %M Kbytes.
Average resident set size          %t Kbytes.
data+stack+text memory use         %K Kbytes.
Average size unshared data area    %D Kbytes.
Average unshared stack space       %p Kbytes.
Average size shared text space     %X Kbytes.
Number of major page faults        %F
Number of recoverable page faults  %R
Number of context-switch           %c
Number of waits                    %w
Number of file system inputs       %I
Number of file system outputs      %O"

IFS=$'\n'
for t in `cat test_list`
do
	IFS=$' '
	if [ ${t:0:1} != '#' ]
	then
		echo ">>>>" $RUN $t
		set -- $t
		f_comp="$2_$3_$4"
		
		ref_out="ref/x_ref_${f_comp}.out"
		ref_perf="ref/x_ref_${MACHINE}_${f_comp}.perf"
		out="results/result_${MACHINE}_${f_comp}.out"
		perf="results/result_${MACHINE}_${f_comp}.perf"
		
		#get reference
		if [ -f $ref_out ]
		then
			echo ""
		else
			echo ">> this test have never be run, run the reference !"
			
			$TIMEPGM -f "$format" -o $ref_perf $RUN_reference $t &> $ref_out
			
			echo ">> reference finished !"
			echo ">> ref PERF: $ref_perf"
			echo ">> ref OUT: $ref_out"
			echo ">> now it's serious buisness !"
		fi
		
		$TIMEPGM -f "$format" -o "$perf" $RUN $t &> "$out"
		
		echo ">> PERF: $out"
		echo ">> OUT: $perf"

		if ! diff -q "$ref_out" "$out" > /dev/null
		then
			echoinred ">> ERROR: out are differents ! See the file $ref_out and:"
			cat "$out"
		else
			echo ">> OK: out are equals !"
# 			cat "$perf"
		fi
		
	fi
	IFS=$'\n'
done
