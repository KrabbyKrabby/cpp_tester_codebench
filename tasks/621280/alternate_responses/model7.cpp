#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <mutex>

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
    void addEmployee(const std::string& name, int age, const std::string& department, double salary) {
        std::lock_guard<std::mutex> lock(mutex_);
        employees_.emplace_back(name, age, department, salary);
        total_salary_ += salary;
    }

    bool removeEmployee(const std::string& name) {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto it = employees_.begin(); it != employees_.end(); ++it) {
            if (it->name == name) {
                total_salary_ -= it->salary;
                employees_.erase(it);
                return true;
            }
        }
        return false;
    }

    void printEmployees() const {
        std::lock_guard<std::mutex> lock(mutex_);
        for (const auto& employee : employees_) {
            std::cout << "Name: " << employee.name
                      << ", Age: " << employee.age
                      << ", Department: " << employee.department
                      << ", Salary: " << employee.salary << "\n";
        }
    }

    void giveRaiseByDepartment(const std::string& department, double percentage) {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto& employee : employees_) {
            if (employee.department == department) {
                double raise = employee.salary * percentage / 100.0;
                employee.salary += raise;
                total_salary_ += raise;
            }
        }
    }

    double getAverageSalary() const {
        std::lock_guard<std::mutex> lock(mutex_);
        if (employees_.empty()) {
            return 0.0;
        }
        return total_salary_ / employees_.size();
    }

    std::vector<Employee> getEmployeesInDepartment(const std::string& department) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<Employee> result;
        for (const auto& employee : employees_) {
            if (employee.department == department) {
                result.push_back(employee);
            }
        }
        return result;
    }

private:
    std::vector<Employee> employees_;
    double total_salary_ = 0.0;
    mutable std::mutex mutex_;
};

class Company {
public:
    void addEmployee(const std::string& name, int age, const std::string& department, double salary) {
        employee_manager_.addEmployee(name, age, department, salary);
    }

    bool removeEmployee(const std::string& name) {
        return employee_manager_.removeEmployee(name);
    }

    void printEmployees() const {
        employee_manager_.printEmployees();
    }

    void giveRaiseByDepartment(const std::string& department, double percentage) const {
        employee_manager_.giveRaiseByDepartment(department, percentage);
    }

    double getAverageSalary() {
        return employee_manager_.getAverageSalary();
    }

    std::vector<Employee> getEmployeesInDepartment(const std::string& department) {
        return employee_manager_.getEmployeesInDepartment(department);
    }

private:
    EmployeeManager employee_manager_;
};
