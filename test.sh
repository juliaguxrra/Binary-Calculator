#!/bin/bash

RED='\033[0;31m'
NC='\033[0m'

# Make sure file exists
if [ ! -f "binarycalc" ]; then
    echo "Run make to compile your code"
    exit 1
fi

# Run a test
run_test () {
    # Parameters
    FORMULA=$1 # Formula to run
    EXPECTED_A=`echo -e $2` 
    EXPECTED_B=`echo -e $3` 

    echo "*****************************************************"
    printf "%-40s%s" "$FORMULA"

    # Run binarycalc
    OUTFILE=$(mktemp /tmp/binarycalc_test.XXXXXX)
    ./binarycalc "$FORMULA" > $OUTFILE
    ACTUAL_A=`head -n 1 $OUTFILE | tr -d '\0'`
    ACTUAL_B=`tail -n +2 $OUTFILE | tr -d '\0'`

    grep -Pa "\x00" $OUTFILE > /dev/null
    if [ $? -eq 0 ]; then
	echo -e "FAILED: EXTRA NULL CHARS\n"
	grep -Pa "\x00" $OUTFILE | sed -e 's/\x00/\\0/g'
    elif [ "$ACTUAL_A" != "$EXPECTED_A" ]; then
        echo -e "PART A FAILED"
        echo -e "\nExpected output for part A:"
        echo "$EXPECTED_A"
        echo -e "\nActual output for part A:"
        echo "$ACTUAL_A"
    elif [ "$ACTUAL_B" != "$EXPECTED_B" ]; then 
        echo -e "PART B FAILED"
        echo -e "\nExpected output for part B:"
        echo "$EXPECTED_B"
        echo -e "\nActual output for part B:"
        echo "$ACTUAL_B"
    else
        echo -e "CORRECT"
    fi
    echo ""
    rm $OUTFILE
}

# Valid formulas
run_test "1+0" "00000001+00000000" "00000001"
run_test "101|10" "00000101|00000010" "00000111"
run_test "111&000" "00000111&00000000" "00000000"
run_test "10101010-01010101" "10101010-01010101" "01010101\nOverflow"

# Invalid formulas
run_test "1+ 0" "Invalid binary formula" ""
run_test "111" "Invalid binary formula" ""
run_test "1*1" "Invalid binary formula" ""
run_test "10+11&01" "Invalid binary formula" ""