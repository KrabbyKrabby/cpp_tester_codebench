# **Tester_cpp_codebench**

## **Project Structure**
```
/Tester_cpp_codebench/       # Root directory
│── code_runner.sh           # The script to run tests
│── README.md                # Documentation file
│── tasks/                   # Contains individual tasks
│   │── <task_id>/           # Task-specific directory (e.g., 303992)
│   │   │── test.cpp         # Unit tests for this task
│   │   │── base.cpp         # base code
│   │   │── solution.cpp     # The main solution file for this task
│   │   │── logs.txt         # Log file for test output (generated dynamically)
│   │   │── CMakeLists.txt   # CMake file for building alternate responses
│   │   │── build/           # Build directory for CMake (generated dynamically)
│   │   │── alternate_responses/  # Directory for alternative implementations
│   │   │   │── A.cpp
│   │   │   │── B.cpp
│   │   │   │── C.cpp
│   │   │   │── ...
│   │   │   │── incorrect.cpp
│   │── <task_id>/           # Another task (same structure)
│   │── <task_id>/           # Another task (same structure)
```

---

## **Setup Instructions**

### **1. Install Required Dependencies**
Ensure that your system has a C++ compiler (GCC), CMake, and GoogleTest installed.

#### **Install GCC, CMake, and GoogleTest (Linux)**
```bash
sudo apt update
sudo apt install -y build-essential cmake libgtest-dev
```

#### **Manually Build GoogleTest (If Needed)**
If `libgtest-dev` is installed but not compiled, run:
```bash
cd /usr/src/gtest
sudo cmake .
sudo make
sudo cp libgtest*.a /usr/lib/
cd -
```

### **2. Clone the Repository**
```bash
git clone <repo_url>
cd Tester_cpp_codebench
```

---

## **How to Use**

### **Run Tests for `solution.cpp`**
#### **Command:**
```bash
./code_runner.sh <task_id> solution
```
#### **Description:**
This compiles `solution.cpp` directly and runs tests from `test_model.cpp` for a specific task.
### **Run Tests for `base.cpp`**
#### **Command:**
```bash
./code_runner.sh <task_id> base
```
#### **Description:**
This runs `base.cpp` in the same way as `solution.cpp` for a specific task.
#### **Give execute permission to the script:**
```bash
chmod +x code_runner.sh
```
#### **Run this command from the project root (`Tester_cpp_codebench/`):**
```bash
./code_runner.sh <task_id> solution
```
This compiles `solution.cpp` directly and runs tests from `test_model.cpp`.
```bash
./code_runner.sh <task_id> base
```
This runs `base.cpp` in the same way as `solution.cpp`.

### **Run Tests for All Alternate Responses**
#### **Command:**
```bash
./code_runner.sh <task_id> alternate
```
#### **Description:**
This compiles and tests all implementations inside `alternate_responses/` using CMake for a specific task.
### **Run Tests for All Tasks**
#### **Command:**
```bash
./code_runner.sh run_all
```
#### **Description:**
This runs `solution.cpp` for all tasks inside `tasks/` and stores the results in `all_solution_logs.txt`.

---
