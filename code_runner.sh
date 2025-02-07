#!/bin/bash

# Function to remove logs.txt if it exists inside the task folder
remove_logs() {
    task_id=$1
    if [ -f "tasks/$task_id/logs.txt" ]; then
        echo "Deleting old logs.txt in task $task_id..."
        rm "tasks/$task_id/logs.txt"
    fi
}

# Function to remove the main function from a given file
remove_main_from_file() {
    cpp_file=$1

    if [ -f "$cpp_file" ]; then
        echo "Removing main function from $cpp_file..."
        awk '
BEGIN { in_main = 0; braces = 0 }
/^[ \t]*int[ \t]+main[ \t]*\(.*\)/ { in_main = 1; braces = 0 }
/^[ \t]*{/ { if (in_main) braces++ }
/^[ \t]*}/ { if (in_main && --braces == 0) { in_main = 0; next } }
!in_main { print }
' "$cpp_file" > "${cpp_file}.tmp" && mv "${cpp_file}.tmp" "$cpp_file"
    fi
}


# Function to remove the main function from all .cpp files in the alternate_responses directory
remove_main_from_alternate_responses() {
    task_id=$1
    alt_dir="tasks/$task_id/alternate_responses"

    if [ -d "$alt_dir" ]; then
        echo "Processing alternate_responses directory for task $task_id to remove main functions..."
        for cpp_file in "$alt_dir"/*.cpp; do
            if [ -f "$cpp_file" ]; then
                echo "Removing main function from $cpp_file..."
                awk '
BEGIN { in_main = 0; braces = 0 }
/^[ \t]*int[ \t]+main[ \t]*\(.*\)/ { in_main = 1; braces = 0 }
/^[ \t]*{/ { if (in_main) braces++ }
/^[ \t]*}/ { if (in_main && --braces == 0) { in_main = 0; next } }
!in_main { print }
' "$cpp_file" > "${cpp_file}.tmp" && mv "${cpp_file}.tmp" "$cpp_file"
            fi
        done
    else
        echo "No alternate_responses directory found for task $task_id."
    fi
}

# Function to replace #include "solution.cpp" with #include WRAPPER_FILE in test_model.cpp
replace_solution_include() {
    task_id=$1
    test_file="tasks/$task_id/test.cpp"

    if [ -f "$test_file" ]; then
        echo "Replacing #include \"solution.cpp\" with #include WRAPPER_FILE in $test_file..."
        sed -i.bak -E 's|#include[[:space:]]*"solution.cpp"|#include WRAPPER_FILE|' "$test_file"
    else
        echo "Error: test.cpp not found for task $task_id!"
        exit 1
    fi
}

# Function to restore #include WRAPPER_FILE back to #include "solution.cpp"
restore_solution_include() {
    task_id=$1
    test_file="tasks/$task_id/test.cpp"

    if [ -f "$test_file.bak" ]; then
        echo "Restoring original test.cpp..."
        mv "$test_file.bak" "$test_file"
    fi
}

run_task() {
    task_id=$1

    if [ ! -d "tasks/$task_id" ]; then
        echo "Task $task_id does not exist!"
        return 1
    fi

    echo "Running task $task_id..."

    # Replace #include "solution.cpp" with #include WRAPPER_FILE
    replace_solution_include "$task_id"

    # Remove all main functions from the alternate_responses directory
    remove_main_from_alternate_responses "$task_id"

    # Create build directory if it doesn't exist
    if [ ! -d "tasks/$task_id/build" ]; then
        echo "Build directory not found. Creating..."
        mkdir -p tasks/$task_id/build
    fi

    # Remove old logs.txt if it exists inside the task folder
    remove_logs $task_id

    # Clean up old files from the build directory
    echo "Cleaning up old files in build directory..."
    rm -rf tasks/$task_id/build/* && rm -rf tasks/$task_id/CMakeFiles tasks/$task_id/CMakeCache.txt

    pushd tasks/$task_id/build > /dev/null

    echo "Running cmake to generate build files..."
    cmake ..  # This assumes each task folder contains its own CMakeLists.txt

    echo "Building the project..."
    make -k

    echo "Running tests with verbose output..."
    {
        ctest -V --timeout 5 | sed -E '/^\/Users\/ittehad\/Documents\/Coding\/Python\/Turing\//d'
        ctest --timeout 5
    } >> "../logs.txt" 2>&1

    echo "Tests have been run for task $task_id. Check tasks/$task_id/logs.txt for the output."

    popd > /dev/null

    # Restore #include WRAPPER_FILE back to #include "solution.cpp"
    restore_solution_include "$task_id"
}

# Function to run tests for solution.cpp without CMake
run_solution() {
    task_id=$1
    echo "Running tests for solution.cpp in task $task_id..."

    test_file="tasks/$task_id/test.cpp"

    if [ ! -f "$test_file" ]; then
        echo "Error: test.cpp not found in task $task_id!"
        exit 1
    fi

    echo "Compiling and running solution.cpp..."
    # g++ -std=c++17 tasks/$task_id/test.cpp -lgtest -lgtest_main -pthread -o temp_test && ./temp_test && rm temp_test
    GCC_COMPILER=$(which g++-14)
    "$GCC_COMPILER" -std=c++17 "tasks/$task_id/test.cpp" -lgtest -lgtest_main -pthread -o temp_test && ./temp_test && rm temp_test

}

run_base() {
    task_id=$1
    echo "Running tests for base_code.cpp in task $task_id..."

    test_file="tasks/$task_id/test.cpp"

    if [ ! -f "$test_file" ]; then
        echo "Error: test.cpp not found in task $task_id!"
        exit 1
    fi

    # Replace #include "solution.cpp" with #include "base_code.cpp"
    echo "Replacing #include \"solution.cpp\" with #include \"base_code.cpp\" in $test_file..."
    sed -i.bak -E 's|#include[[:space:]]*"solution.cpp"|#include "base_code.cpp"|' "$test_file"

    echo "Compiling and running base.cpp..."
    # g++ -std=c++17 tasks/$task_id/test.cpp -lgtest -lgtest_main -pthread -o temp_test && ./temp_test && rm temp_test
    GCC_COMPILER=$(which g++-14)
    "$GCC_COMPILER" -std=c++17 "tasks/$task_id/test.cpp" -lgtest -lgtest_main -pthread -o temp_test && ./temp_test && rm temp_test


    # Restore original include
    echo "Restoring original test.cpp..."
    mv "$test_file.bak" "$test_file"
}

run_all_solutions() {
    log_file="all_solution_logs.txt"
    echo "Running solution.cpp for all tasks and storing results in $log_file..."
    > "$log_file"  # Clear the log file before writing

    for task_dir in tasks/*; do
        if [ -d "$task_dir" ]; then
            task_id=$(basename "$task_dir")
            echo "Running solution.cpp for Task $task_id..." | tee -a "$log_file"

            test_file="tasks/$task_id/test.cpp"

            if [ -f "$test_file" ]; then
                echo "Compiling and running solution.cpp for Task $task_id..." | tee -a "$log_file"
                # g++ -std=c++17 "$test_file" -lgtest -lgtest_main -pthread -o temp_test 2>>"$log_file"
                GCC_COMPILER=$(which g++-14)
                "$GCC_COMPILER" -std=c++17 "$test_file" -lgtest -lgtest_main -pthread -o temp_test 2>>"$log_file"


                if [ $? -eq 0 ]; then
                    ./temp_test >> "$log_file" 2>&1
                    echo "Completed Task $task_id" | tee -a "$log_file"
                else
                    echo "Compilation failed for Task $task_id" | tee -a "$log_file"
                fi

                rm -f temp_test
            else
                echo "Skipping Task $task_id: test.cpp not found!" | tee -a "$log_file"
            fi

            echo "----------------------" | tee -a "$log_file"
        fi
    done

    echo "All tasks processed. Logs saved in $log_file."
}

run_all_alternates() {
    echo "Running all alternate responses for all tasks..."

    for task_dir in tasks/*; do
        if [ -d "$task_dir" ]; then
            task_id=$(basename "$task_dir")
            echo "Running alternate responses for Task $task_id..."
            ./code_runner.sh "$task_id" alternate
        fi
    done

    echo "All alternate responses have been tested."
}

run_all_base() {
    log_file="all_base_logs.txt"
    echo "Running base_code.cpp for all tasks and storing results in $log_file..."
    > "$log_file"  # Clear the log file before writing

    for task_dir in tasks/*; do
        if [ -d "$task_dir" ]; then
            task_id=$(basename "$task_dir")
            test_file="tasks/$task_id/test.cpp"

            if [ -f "$test_file" ]; then
                echo "Compiling and running base_code.cpp for Task $task_id..." | tee -a "$log_file"
                sed -i.bak -E 's|#include[[:space:]]*"solution.cpp"|#include "base_code.cpp"|' "$test_file"

                GCC_COMPILER=$(which g++-14)
                "$GCC_COMPILER" -std=c++17 "$test_file" -lgtest -lgtest_main -pthread -o temp_test 2>>"$log_file"

                if [ $? -eq 0 ]; then
                    ./temp_test >> "$log_file" 2>&1
                    echo "Completed Task $task_id" | tee -a "$log_file"
                else
                    echo "Compilation failed for Task $task_id" | tee -a "$log_file"
                fi

                rm -f temp_test
                mv "$test_file.bak" "$test_file"
            else
                echo "Skipping Task $task_id: test.cpp not found!" | tee -a "$log_file"
            fi

            echo "----------------------" | tee -a "$log_file"
        fi
    done

    echo "All base_code.cpp tests processed. Logs saved in $log_file."
}



# Main script logic
if [ -z "$1" ]; then
    echo "Usage: $0 <task_id> [solution|alternate|base] OR $0 run_all"
    exit 1

elif [ "$1" == "run_all_solutions" ]; then
    run_all_solutions
    exit 0
# Modify the main script logic to include the new option
elif [ "$1" == "run_all_alternate" ]; then
    run_all_alternates
    exit 0
elif [ "$1" == "run_all_base" ]; then
    run_all_base
    exit 0
else
    task_id=$1
    mode=$2

    if [ ! -d "tasks/$task_id" ]; then
        echo "Task $task_id does not exist!"
        exit 1
    fi

    if [ "$mode" == "solution" ]; then
        run_solution "$task_id"
    elif [ "$mode" == "alternate" ]; then
        run_task "$task_id"
    elif [ "$mode" == "base" ]; then
        run_base "$task_id"
    elif [ "$mode" == "run_all" ]; then
        run_all_solutions
    else
        echo "Invalid mode! Use 'solution' to test solution.cpp or 'alternate' to test all alternate_responses/*.cpp."
        exit 1
    fi
fi