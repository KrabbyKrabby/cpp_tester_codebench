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
    std::vector<Employee> employees;
    mutable std::shared_mutex mutex;
    double totalSalary = 0.0;

    // Adds a new employee to the company.
    void addEmployee(const std::string& name, int age, const std::string& department, double salary) {
        std::unique_lock<std::shared_mutex> lock(mutex);
        employees.push_back(Employee(name, age, department, salary));
        totalSalary += salary;
    }

    // Removes an employee by name. Returns true if the employee was found and removed.
    bool removeEmployee(const std::string& name) {
        std::unique_lock<std::shared_mutex> lock(mutex);
        for (auto it = employees.begin(); it != employees.end(); ++it) {
            if (it->name == name) {
                totalSalary -= it->salary;
                employees.erase(it);
                return true;
            }
        }
        return false;
    }

    // Prints basic employee information to the console.
    void printEmployees() const {
        std::shared_lock<std::shared_mutex> lock(mutex);
        for (const auto& employee : employees) {
            std::cout << "Name: " << employee.name
                      << ", Age: " << employee.age
                      << ", Department: " << employee.department
                      << ", Salary: " << employee.salary << "\n";
        }
    }

    // Gives a raise by percentage to all employees in a given department.
    void giveRaiseByDepartment(const std::string& department, double percentage) {
        std::unique_lock<std::shared_mutex> lock(mutex);
        for (auto& employee : employees) {
            if (employee.department == department) {
                totalSalary -= employee.salary;
                employee.salary += employee.salary * percentage / 100.0;
                totalSalary += employee.salary;
            }
        }
    }

    // Computes and returns the average salary among all employees.
    double getAverageSalary() const {
        std::shared_lock<std::shared_mutex> lock(mutex);
        if (employees.empty()) {
            return 0.0;
        }
        return totalSalary / employees.size();
    }

    // Returns a list of employees in a specific department.
    std::vector<Employee> getEmployeesInDepartment(const std::string& department) const {
        std::shared_lock<std::shared_mutex> lock(mutex);
        std::vector<Employee> result;
        for (const auto& employee : employees) {
            if (employee.department == department) {
                result.push_back(employee);
            }
        }
        return result;
    }
};

class Company {
public:
    EmployeeManager employeeManager;

    // Adds a new employee to the company.
    void addEmployee(const std::string& name, int age, const std::string& department, double salary) {
        employeeManager.addEmployee(name, age, department, salary);
    }

    // Removes an employee by name. Returns true if the employee was found and removed.
    bool removeEmployee(const std::string& name) {
        return employeeManager.removeEmployee(name);
    }

    // Prints basic employee information to the console.
    void printEmployees() const {
        employeeManager.printEmployees();
    }

    // Gives a raise by percentage to all employees in a given department.
    void giveRaiseByDepartment(const std::string& department, double percentage) const {
        employeeManager.giveRaiseByDepartment(department, percentage);
    }

    // Computes and returns the average salary among all employees.
    double getAverageSalary() {
        return employeeManager.getAverageSalary();
    }

    // Returns a list of employees in a specific department.
    std::vector<Employee> getEmployeesInDepartment(const std::string& department) {
        return employeeManager.getEmployeesInDepartment(department);
    }
};