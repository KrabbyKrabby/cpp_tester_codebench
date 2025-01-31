#include <iostream>
#include <vector>
#include <string>
#include <shared_mutex>
#include <memory>
#include <unordered_map>
#include <numeric>
#include <optional>

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
    std::vector<Employee> employees_;
    mutable std::shared_mutex mutex_;
    double total_salary_{0.0};
    size_t employee_count_{0};

    void updateAverageSalaryAdd(double salary) {
        total_salary_ += salary;
        employee_count_++;
    }

    void updateAverageSalaryRemove(double salary) {
        total_salary_ -= salary;
        employee_count_--;
    }

public:
    void addEmployee(const std::string& name, int age, const std::string& department, double salary) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        employees_.emplace_back(name, age, department, salary);
        updateAverageSalaryAdd(salary);
    }

    bool removeEmployee(const std::string& name) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        for (auto it = employees_.begin(); it != employees_.end(); ++it) {
            if (it->name == name) {
                updateAverageSalaryRemove(it->salary);
                employees_.erase(it);
                return true;
            }
        }
        return false;
    }

    void giveRaiseByDepartment(const std::string& department, double percentage) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        for (auto& employee : employees_) {
            if (employee.department == department) {
                double raise = employee.salary * percentage / 100.0;
                employee.salary += raise;
                total_salary_ += raise;
            }
        }
    }

    double getAverageSalary() const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return employee_count_ > 0 ? total_salary_ / employee_count_ : 0.0;
    }

    std::vector<Employee> getEmployeesInDepartment(const std::string& department) const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        std::vector<Employee> result;
        std::copy_if(employees_.begin(), employees_.end(), 
                     std::back_inserter(result),
                     [&department](const Employee& emp) { 
                         return emp.department == department; 
                     });
        return result;
    }

    void printEmployees() const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        for (const auto& employee : employees_) {
            std::cout << "Name: " << employee.name
                      << ", Age: " << employee.age
                      << ", Department: " << employee.department
                      << ", Salary: " << employee.salary << "\n";
        }
    }
};

class Company {
private:
    std::unique_ptr<EmployeeManager> employee_manager_;

public:
    Company() : employee_manager_(std::make_unique<EmployeeManager>()) {}

    void addEmployee(const std::string& name, int age, const std::string& department, double salary) {
        employee_manager_->addEmployee(name, age, department, salary);
    }

    bool removeEmployee(const std::string& name) {
        return employee_manager_->removeEmployee(name);
    }

    void printEmployees() const {
        employee_manager_->printEmployees();
    }

    void giveRaiseByDepartment(const std::string& department, double percentage) {
        employee_manager_->giveRaiseByDepartment(department, percentage);
    }

    double getAverageSalary() {
        return employee_manager_->getAverageSalary();
    }

    std::vector<Employee> getEmployeesInDepartment(const std::string& department) {
        return employee_manager_->getEmployeesInDepartment(department);
    }
};