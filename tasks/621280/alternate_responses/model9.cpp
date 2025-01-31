#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <mutex>
#include <memory>
#include <unordered_map>

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
public:
    EmployeeManager() = default;

    void addEmployee(const std::string& name, int age, const std::string& department, double salary) {
        std::lock_guard<std::mutex> lock(mutex_);
        employees_.push_back(std::make_unique<Employee>(name, age, department, salary));
        updateSalaryCache(salary);
    }

    bool removeEmployee(const std::string& name) {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto it = employees_.begin(); it != employees_.end(); ++it) {
            if ((*it)->name == name) {
                updateSalaryCache(-(*it)->salary);
                employees_.erase(it);
                return true;
            }
        }
        return false;
    }

    void printEmployees() const {
        std::lock_guard<std::mutex> lock(mutex_);
        for (const auto& employee : employees_) {
            std::cout << "Name: " << employee->name
                      << ", Age: " << employee->age
                      << ", Department: " << employee->department
                      << ", Salary: " << employee->salary << "\n";
        }
    }

    void giveRaiseByDepartment(const std::string& department, double percentage) {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto& employee : employees_) {
            if (employee->department == department) {
                double oldSalary = employee->salary;
                employee->salary += employee->salary * percentage / 100.0;
                updateSalaryCache(employee->salary - oldSalary);
            }
        }
    }

    double getAverageSalary() const {
        std::lock_guard<std::mutex> lock(mutex_);
        if (employees_.empty()) {
            return 0.0;
        }
        return totalSalary_ / employees_.size();
    }

    std::vector<std::unique_ptr<Employee>> getEmployeesInDepartment(const std::string& department) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<std::unique_ptr<Employee>> result;
        for (const auto& employee : employees_) {
            if (employee->department == department) {
                result.push_back(std::make_unique<Employee>(*employee));
            }
        }
        return result;
    }

private:
    void updateSalaryCache(double delta) {
        totalSalary_ += delta;
    }

    std::vector<std::unique_ptr<Employee>> employees_;
    std::mutex mutex_;
    double totalSalary_ = 0.0;
};

class Company {
public:
    Company() = default;

    void addEmployee(const std::string& name, int age, const std::string& department, double salary) {
        employeeManager_.addEmployee(name, age, department, salary);
    }

    bool removeEmployee(const std::string& name) {
        return employeeManager_.removeEmployee(name);
    }

    void printEmployees() const {
        employeeManager_.printEmployees();
    }

    void giveRaiseByDepartment(const std::string& department, double percentage) {
        employeeManager_.giveRaiseByDepartment(department, percentage);
    }

    double getAverageSalary() {
        return employeeManager_.getAverageSalary();
    }

    std::vector<Employee> getEmployeesInDepartment(const std::string& department) {
        auto employees = employeeManager_.getEmployeesInDepartment(department);
        std::vector<Employee> result;
        for (const auto& employee : employees) {
            result.push_back(*employee);
        }
        return result;
    }

private:
    EmployeeManager employeeManager_;
};
