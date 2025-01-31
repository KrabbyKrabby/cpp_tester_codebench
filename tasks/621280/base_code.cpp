#include <iostream>
#include <vector>
#include <string>
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

class Company {
public:
    std::vector<Employee> employees;

    // Adds a new employee to the company.
    void addEmployee(const std::string& name, int age, const std::string& department, double salary) {
        employees.push_back(Employee(name, age, department, salary));
    }

    // Removes an employee by name. Returns true if the employee was found and removed.
    bool removeEmployee(const std::string& name) {
        for (auto it = employees.begin(); it != employees.end(); ++it) {
            if (it->name == name) {
                employees.erase(it);
                return true;
            }
        }
        return false;
    }

    // Prints basic employee information to the console.
    void printEmployees() const {
        for (const auto& employee : employees) {
            std::cout << "Name: " << employee.name
                      << ", Age: " << employee.age
                      << ", Department: " << employee.department
                      << ", Salary: " << employee.salary << "\n";
        }
    }

    // Gives a raise by percentage to all employees in a given department.
    void giveRaiseByDepartment(const std::string& department, double percentage) const {
        for (auto& employee : employees) {
            if (employee.department == department) {
                employee.salary += employee.salary * percentage / 100.0;
            }
        }
    }

    // Computes and returns the average salary among all employees.
    double getAverageSalary() {
        if (employees.empty()) {
            return 0.0;
        }
        double total = 0.0;
        for (const auto& employee : employees) {
            total += employee.salary;
        }
        return total / employees.size();
    }

    // Returns a list of employees in a specific department.
    std::vector<Employee> getEmployeesInDepartment(const std::string& department) {
        std::vector<Employee> result;
        for (const auto& employee : employees) {
            if (employee.department == department) {
                result.push_back(employee);
            }
        }
        return result;
    }
};