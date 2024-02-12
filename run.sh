#!/bin/bash
# Function to run tests and log results
run_tests() {
    local exper="$1"
    local start_index="$2"
    local end_index="$3"
    local LOG_FILE="TDD_cir_2_$exper.txt"
    local program="test_$exper"

    # Empty the log file at the start
    > "$LOG_FILE"

    for i in $(seq $start_index $end_index); do
        # Run your program with the current number as argument
        ./build/test/$program $i >> "$LOG_FILE"

        # Check if the program ran successfully
        if [ $? -eq 0 ]; then
            echo "Execution complete with argument $i"
        else
            echo "Program failed with argument $i"
        fi
    done

    echo "All executions completed. Check $LOG_FILE for details."
}

# Run tests for each experiment with specific ranges
# run_tests "clt" 1 19
# run_tests "cl" 1 19
run_tests "qft" 2 17
# run_tests "bv" 2 21
