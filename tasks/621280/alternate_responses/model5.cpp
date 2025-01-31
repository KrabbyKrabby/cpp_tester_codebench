#include <iostream>
#include <vector>
#include <string>
#include <shared_mutex>
#include <memory>
#include <algorithm>
#include <numeric>

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
    double cachedAverageSalary{0.0};
    bool isAverageSalaryCacheValid{false};

    void updateAverageSalaryCache() {
        if (employees.empty()) {
            cachedAverageSalary = 0.0;
        } else {
            double total = std::accumulate(employees.begin(), employees.end(), 0.0,
                [](double sum, const Employee& emp) { return sum + emp.salary; });
            cachedAverageSalary = total / employees.size();
        }
        isAverageSalaryCacheValid = true;
    }

public:
    void addEmployee(const std::string& name, int age, const std::string& department, double salary) {
        std::unique_lock lock(mutex);
        employees.push_back(Employee(name, age, department, salary));
        isAverageSalaryCacheValid = false;
    }

    bool removeEmployee(const std::string& name) {
        std::unique_lock lock(mutex);
        auto it = std::find_if(employees.begin(), employees.end(),
            [&name](const Employee& emp) { return emp.name == name; });
        if (it != employees.end()) {
            employees.erase(it);
            isAverageSalaryCacheValid = false;
            return true;
        }
        return false;
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

    void giveRaiseByDepartment(const std::string& department, double percentage) {
        std::unique_lock lock(mutex);
        for (auto& employee : employees) {
            if (employee.department == department) {
                employee.salary += employee.salary * percentage / 100.0;
            }
        }
        isAverageSalaryCacheValid = false;
    }

    double getAverageSalary() {
        std::unique_lock lock(mutex);
        if (!isAverageSalaryCacheValid) {
            updateAverageSalaryCache();
        }
        return cachedAverageSalary;
    }

    std::vector<Employee> getEmployeesInDepartment(const std::string& department) {
        std::shared_lock lock(mutex);
        std::vector<Employee> result;
        std::copy_if(employees.begin(), employees.end(), std::back_inserter(result),
            [&department](const Employee& emp) { return emp.department == department; });
        return result;
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

    void giveRaiseByDepartment(const std::string& department, double percentage) const {
        employeeManager->giveRaiseByDepartment(department, percentage);
    }

    double getAverageSalary() {
        return employeeManager->getAverageSalary();
    }

    std::vector<Employee> getEmployeesInDepartment(const std::string& department) {
        return employeeManager->getEmployeesInDepartment(department);
    }
};