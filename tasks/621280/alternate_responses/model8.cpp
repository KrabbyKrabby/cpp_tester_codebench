#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <mutex>
#include <shared_mutex>

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
        employees_.push_back(Employee(name, age, department, salary));
        totalSalary_ += salary;
    }

    bool removeEmployee(const std::string& name) {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto it = employees_.begin(); it != employees_.end(); ++it) {
            if (it->name == name) {
                totalSalary_ -= it->salary;
                employees_.erase(it);
                return true;
            }
        }
        return false;
    }

    void printEmployees() const {
        std::shared_lock<std::shared_mutex> lock(sharedMutex_);
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
                totalSalary_ += raise;
            }
        }
    }

    double getAverageSalary() const {
        std::shared_lock<std::shared_mutex> lock(sharedMutex_);
        return employees_.empty() ? 0.0 : totalSalary_ / employees_.size();
    }

    std::vector<Employee> getEmployeesInDepartment(const std::string& department) {
        std::shared_lock<std::shared_mutex> lock(sharedMutex_);
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
    double totalSalary_ = 0.0;
    mutable std::shared_mutex sharedMutex_;
    std::mutex mutex_;
};

class Company {
public:
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
        return employeeManager_.getEmployeesInDepartment(department);
    }

private:
    EmployeeManager employeeManager_;
};