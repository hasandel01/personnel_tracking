#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>

class Employee {
    private:
        std::string name;
        std::string mac_address;
    public:
        Employee();
        Employee(const std::string& name, const std::string& mac_address);
        ~Employee();

        std::string getName();
        std::string getMacAddress();
        void setName(const std::string& name);
        void setMacAddress(const std::string& mac_address);
};

#endif // EMPLOYEE_H