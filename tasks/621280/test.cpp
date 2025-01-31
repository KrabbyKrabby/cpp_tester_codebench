
#include <gtest/gtest.h>
#include "solution.cpp"
#include <thread>
#include <vector>
#include <algorithm>

//------------------------------------------------------------------------------
// Helper function to find an Employee by name in a vector of Employee objects.
bool containsEmployee(const std::vector<Employee>& v, const std::string& name) {
    return std::any_of(v.begin(), v.end(),
        [&](const Employee& e) { return e.name == name; }
    );
}

//------------------------------------------------------------------------------
// Test 1: Basic add and average salary
//------------------------------------------------------------------------------
TEST(CompanyTest, AddAndAverageSalary) {
    Company company;
    company.addEmployee("Alice", 30, "Engineering", 70000);
    company.addEmployee("Bob", 25, "Engineering", 65000);
    company.addEmployee("Charlie", 40, "HR", 60000);

    double average = company.getAverageSalary();
    // Average = (70000 + 65000 + 60000) / 3 = 65000
    EXPECT_DOUBLE_EQ(average, 65000.0);
}

//------------------------------------------------------------------------------
// Test 2: Remove employees
//------------------------------------------------------------------------------
TEST(CompanyTest, RemoveEmployee) {
    Company company;
    company.addEmployee("David", 28, "Marketing", 50000);
    company.addEmployee("Eve", 35, "Marketing", 55000);

    // Remove existing employee
    bool removed = company.removeEmployee("David");
    EXPECT_TRUE(removed);

    // Attempt to remove non-existing employee
    removed = company.removeEmployee("NonExistent");
    EXPECT_FALSE(removed);

    // Check average salary now that only Eve remains
    double avg = company.getAverageSalary();
    EXPECT_DOUBLE_EQ(avg, 55000.0);
}

//------------------------------------------------------------------------------
// Test 3: Give raise by department
//------------------------------------------------------------------------------
TEST(CompanyTest, GiveRaiseByDepartment) {
    Company company;
    company.addEmployee("Frank", 45, "Engineering", 80000);
    company.addEmployee("Grace", 38, "HR", 50000);
    company.addEmployee("Heidi", 29, "Engineering", 75000);

    // Give a 10% raise to Engineering
    company.giveRaiseByDepartment("Engineering", 10);

    // Engineering salaries should be 10% higher
    // Frank: 88000, Heidi: 82500, Grace remains 50000
    double avg = company.getAverageSalary();
    // New sum = 88000 + 82500 + 50000 = 220500
    // Count = 3
    // Average = 220500 / 3 = 73500
    EXPECT_DOUBLE_EQ(avg, 73500.0);

    // Check that Grace did not get a raise (still 50000)
    auto hrEmployees = company.getEmployeesInDepartment("HR");
    EXPECT_EQ(hrEmployees.size(), 1u);
    EXPECT_EQ(hrEmployees.front().salary, 50000);
}

//------------------------------------------------------------------------------
// Test 4: Get employees in a department
//------------------------------------------------------------------------------
TEST(CompanyTest, GetEmployeesInDepartment) {
    Company company;
    company.addEmployee("Ivan", 32, "Engineering", 90000);
    company.addEmployee("Judy", 29, "HR", 60000);
    company.addEmployee("Karl", 41, "HR", 65000);
    company.addEmployee("Leo", 35, "Engineering", 95000);

    std::vector<Employee> eng = company.getEmployeesInDepartment("Engineering");
    std::vector<Employee> hr = company.getEmployeesInDepartment("HR");

    // Check counts
    EXPECT_EQ(eng.size(), 2u);
    EXPECT_EQ(hr.size(), 2u);

    // Ensure correct employees are in each list
    EXPECT_TRUE(containsEmployee(eng, "Ivan"));
    EXPECT_TRUE(containsEmployee(eng, "Leo"));
    EXPECT_TRUE(containsEmployee(hr, "Judy"));
    EXPECT_TRUE(containsEmployee(hr, "Karl"));
}

//------------------------------------------------------------------------------
// Test 5: Thread-safety check (basic concurrency test)
//------------------------------------------------------------------------------
TEST(CompanyTest, ThreadSafetyAddAndRemove) {
    Company company;

    // We'll spin up multiple threads that add employees concurrently
    auto addEmployees = [&](const std::string& prefix, int startIdx, int count) {
        for(int i = 0; i < count; ++i) {
            std::string name = prefix + std::to_string(startIdx + i);
            company.addEmployee(name, 30, "ConcurrentDept", 50000 + i);
        }
    };

    // Start multiple threads
    std::thread t1(addEmployees, "WorkerA", 0, 50);
    std::thread t2(addEmployees, "WorkerB", 50, 50);
    std::thread t3(addEmployees, "WorkerC", 100, 50);
    std::thread t4(addEmployees, "WorkerD", 150, 50);

    // Join threads
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    // We should have 200 employees in "ConcurrentDept"
    auto allInDept = company.getEmployeesInDepartment("ConcurrentDept");
    EXPECT_EQ(allInDept.size(), 200u);

    // Now remove some in parallel
    auto removeEmployees = [&](const std::string& prefix, int startIdx, int count) {
        for (int i = 0; i < count; ++i) {
            std::string name = prefix + std::to_string(startIdx + i);
            company.removeEmployee(name);
        }
    };

    std::thread t5(removeEmployees, "WorkerA", 0, 10);
    std::thread t6(removeEmployees, "WorkerB", 50, 10);
    std::thread t7(removeEmployees, "WorkerC", 100, 10);
    std::thread t8(removeEmployees, "WorkerD", 150, 10);

    t5.join();
    t6.join();
    t7.join();
    t8.join();

    // 40 employees removed in total
    allInDept = company.getEmployeesInDepartment("ConcurrentDept");
    EXPECT_EQ(allInDept.size(), 160u);

    // Let's do a quick average check; it's not strictly verifying concurrency
    // correctness but ensures the code is stable under concurrent changes.
    double avg = company.getAverageSalary();
    EXPECT_TRUE(avg > 0.0);
}

