#include <iostream>
#include <vector>
#include <string>
#include <shared_mutex>
#include <mutex>
#include <memory>
#include <algorithm>
#include <cassert>

class Employee {
public:
    std::string name;
    int age;
    std::string department;
    double salary;

    Employee(const std::string& name, int age, const std::string& department, double salary)
        : name(name), age(age), department(department), salary(salary) {}
};

class EmployeeManager {
private:
    std::vector<Employee> employees;
    mutable std::shared_mutex mutex;
    double totalSalary{0.0};  // Cache for total salary

public:
    void addEmployee(const std::string& name, int age, const std::string& department, double salary) {
        std::unique_lock lock(mutex);
        employees.push_back(Employee(name, age, department, salary));
        totalSalary += salary;
    }

    bool removeEmployee(const std::string& name) {
        std::unique_lock lock(mutex);
        auto it = std::find_if(employees.begin(), employees.end(),
            [&name](const Employee& emp) { return emp.name == name; });
        
        if (it != employees.end()) {
            totalSalary -= it->salary;
            employees.erase(it);
            return true;
        }
        return false;
    }

    void giveRaiseByDepartment(const std::string& department, double percentage) {
        std::unique_lock lock(mutex);
        for (auto& employee : employees) {
            if (employee.department == department) {
                double raise = employee.salary * percentage / 100.0;
                employee.salary += raise;
                totalSalary += raise;
            }
        }
    }

    double getAverageSalary() const {
        std::shared_lock lock(mutex);
        return employees.empty() ? 0.0 : totalSalary / employees.size();
    }

    std::vector<Employee> getEmployeesInDepartment(const std::string& department) const {
        std::shared_lock lock(mutex);
        std::vector<Employee> result;
        std::copy_if(employees.begin(), employees.end(), std::back_inserter(result),
            [&department](const Employee& emp) { return emp.department == department; });
        return result;
    }

    void printEmployees() const {
        std::shared_lock lock(mutex);
        for (const auto& employee : employees) {
            std::cout << "Name: " << employee.name
                      << ", Age: " << employee.age
                      << ", Department: " << employee.department
                      << ", Salary: " << employee.salary << "\n";
        }
    }
};

class Company {
private:
    std::unique_ptr<EmployeeManager> employeeManager;

public:
    Company() : employeeManager(std::make_unique<EmployeeManager>()) {}

    void addEmployee(const std::string& name, int age, const std::string& department, double salary) {
        employeeManager->addEmployee(name, age, department, salary);
    }

    bool removeEmployee(const std::string& name) {
        return employeeManager->removeEmployee(name);
    }

    void printEmployees() const {
        employeeManager->printEmployees();
    }

    void giveRaiseByDepartment(const std::string& department, double percentage) {
        employeeManager->giveRaiseByDepartment(department, percentage);
    }

    double getAverageSalary() {
        return employeeManager->getAverageSalary();
    }

    std::vector<Employee> getEmployeesInDepartment(const std::string& department) {
        return employeeManager->getEmployeesInDepartment(department);
    }
};
