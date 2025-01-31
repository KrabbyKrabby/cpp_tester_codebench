#include <iostream>
#include <vector>
#include <string>
#include <shared_mutex>
#include <mutex>
#include <optional>
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
    mutable std::optional<double> cached_average_salary;

    void invalidate_cache() {
        cached_average_salary.reset();
    }

    void update_average_salary() const {
        if (employees.empty()) {
            cached_average_salary = 0.0;
            return;
        }
        double total = 0.0;
        for (const auto& employee : employees) {
            total += employee.salary;
        }
        cached_average_salary = total / employees.size();
    }

public:
    void add_employee(const std::string& name, int age, const std::string& department, double salary) {
        std::unique_lock lock(mutex);
        employees.push_back(Employee(name, age, department, salary));
        invalidate_cache();
    }

    bool remove_employee(const std::string& name) {
        std::unique_lock lock(mutex);
        for (auto it = employees.begin(); it != employees.end(); ++it) {
            if (it->name == name) {
                employees.erase(it);
                invalidate_cache();
                return true;
            }
        }
        return false;
    }

    std::vector<Employee> get_all_employees() const {
        std::shared_lock lock(mutex);
        return employees;
    }

    void give_raise_by_department(const std::string& department, double percentage) {
        std::unique_lock lock(mutex);
        for (auto& employee : employees) {
            if (employee.department == department) {
                employee.salary += employee.salary * percentage / 100.0;
            }
        }
        invalidate_cache();
    }

    double get_average_salary() const {
        std::shared_lock lock(mutex);
        if (!cached_average_salary) {
            update_average_salary();
        }
        return *cached_average_salary;
    }

    std::vector<Employee> get_employees_in_department(const std::string& department) const {
        std::shared_lock lock(mutex);
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
private:
    EmployeeManager employee_manager;

public:
    // Preserved public interface
    void addEmployee(const std::string& name, int age, const std::string& department, double salary) {
        employee_manager.add_employee(name, age, department, salary);
    }

    bool removeEmployee(const std::string& name) {
        return employee_manager.remove_employee(name);
    }

    void printEmployees() const {
        for (const auto& employee : employee_manager.get_all_employees()) {
            std::cout << "Name: " << employee.name
                      << ", Age: " << employee.age
                      << ", Department: " << employee.department
                      << ", Salary: " << employee.salary << "\n";
        }
    }

    void giveRaiseByDepartment(const std::string& department, double percentage) {
        employee_manager.give_raise_by_department(department, percentage);
    }

    double getAverageSalary() {
        return employee_manager.get_average_salary();
    }

    std::vector<Employee> getEmployeesInDepartment(const std::string& department) {
        return employee_manager.get_employees_in_department(department);
    }
};
