#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <mutex>
#include <memory>

// Employee class remains unchanged
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
        updateAverageSalary(salary, true);
    }

    bool removeEmployee(const std::string& name) {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto it = employees_.begin(); it != employees_.end(); ++it) {
            if (it->name == name) {
                updateAverageSalary(it->salary, false);
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
                double oldSalary = employee.salary;
                employee.salary += employee.salary * percentage / 100.0;
                updateAverageSalary(oldSalary, false);
                updateAverageSalary(employee.salary, true);
            }
        }
    }

    double getAverageSalary() {
        std::lock_guard<std::mutex> lock(mutex_);
        return averageSalary_;
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
    void updateAverageSalary(double salary, bool add) {
        if (add) {
            totalSalary_ += salary;
            if (employees_.size() == 1) {
                averageSalary_ = salary;
            } else {
                averageSalary_ = totalSalary_ / employees_.size();
            }
        } else {
            totalSalary_ -= salary;
            if (employees_.empty()) {
                averageSalary_ = 0.0;
            } else {
                averageSalary_ = totalSalary_ / employees_.size();
            }
        }
    }

    std::vector<Employee> employees_;
    double totalSalary_ = 0.0;
    double averageSalary_ = 0.0;
    mutable std::mutex mutex_;
};

class Company {
public:
    // Delegates to EmployeeManager
    void addEmployee(const std::string& name, int age, const std::string& department, double salary) {
        employeeManager_->addEmployee(name, age, department, salary);
    }

    bool removeEmployee(const std::string& name) {
        return employeeManager_->removeEmployee(name);
    }

    void printEmployees() const {
        employeeManager_->printEmployees();
    }

    void giveRaiseByDepartment(const std::string& department, double percentage) {
        employeeManager_->giveRaiseByDepartment(department, percentage);
    }

    double getAverageSalary() {
        return employeeManager_->getAverageSalary();
    }

    std::vector<Employee> getEmployeesInDepartment(const std::string& department) {
        return employeeManager_->getEmployeesInDepartment(department);
    }

private:
    std::unique_ptr<EmployeeManager> employeeManager_ = std::make_unique<EmployeeManager>();
};