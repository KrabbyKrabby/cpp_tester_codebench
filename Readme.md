# **Tester_cpp_codebench**

## **Project Structure**
```
/Tester_cpp_codebench/       # Root directory
│── code_runner.sh           # The script to run tests
│── README.md                # Documentation file
│── tasks/                   # Contains individual tasks
│   │── <task_id>/           # Task-specific directory (e.g., 303992)
│   │   │── test_model.cpp   # Unit tests for this task
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
│   │   │   │── base.cpp
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

### **Run Tests for `solution.cpp`** (Without CMake)
#### **Give execute permission to the script:**
```bash
chmod +x code_runner.sh
```
#### **Run this command from the project root (`Tester_cpp_codebench/`):
#### **Run this command from the project root (`Tester_cpp_codebench/`):**
```bash
./code_runner.sh <task_id> solution
```
This compiles `solution.cpp` directly and runs tests from `test_model.cpp`.

### **Run Tests for All Alternate Responses** (Using CMake)
#### **Give execute permission to the script:**
```bash
chmod +x code_runner.sh
```
#### **Run this command from the project root (`Tester_cpp_codebench/`):
#### **Run this command from the project root (`Tester_cpp_codebench/`):**
```bash
./code_runner.sh <task_id> alternate
```
This compiles and tests all implementations inside `alternate_responses/` using CMake.

---

## **Testing Process**

### **1. Solution File (`solution.cpp`)**
- The script directly compiles `solution.cpp` and runs tests against `test_model.cpp`.

### **2. Alternate Responses (`alternate_responses/` Directory)**
- CMake is used to build and run tests for each `.cpp` file inside `alternate_responses/`.
- The script replaces `#include "solution.cpp"` with `#include WRAPPER_FILE` in `test_model.cpp` before running.
- After tests, the original include statement is restored.

# meta_codebench_cpp_tester_new
