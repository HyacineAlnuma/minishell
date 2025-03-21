#!/bin/bash

# Colors
END="\033[0m"
GREY="\033[0;30m"
RED="\033[0;31m"
GREEN="\033[0;32m"
YELLOW="\033[0;33m"
BLUE="\033[0;34m"
PURPLE="\033[0;35m"
CYAN="\033[0;36m"
BOLD="\e[1m"

MINISHELL="./minishell"

run_test() {
	command=$1
	expected_output=$2
	expected_error=$3

	echo -e "${YELLOW}Running: $command${END}"
	($MINISHELL <<EOF
$command
EOF
	) &> output.txt

	if [ "$(sed -n 15p output.txt)" == "$expected_output" ]; then
		echo -e "${GREEN}Test passed!${END}"
	else
		echo -e "${RED}Test failed!${END}"
		echo -e "	${BOLD}Expected:${END} $expected_output"
		echo -e "	${BOLD}Actual:${END} $(sed -n 15p output.txt)"
	fi
}

# Test Case 1:
run_test "echo Hello, World!" "Hello, World!"

# Test Case 2:
run_test "fakecmd" "fakecmd: command not found."

# Test Case 3:
run_test "env | grep CODE | wc -l" "5"

# Test Case 4:
run_test "env | grep SHLVL" "SHLVL=2"

# Clean up
rm output.txt
