#include "employee.h"
#include <string>

Employee::Employee() {
    this->name = "";
    this->mac_address = "";
}

Employee::Employee(const std::string& name, const std::string& mac_address) {
    this->name = name;
    this->mac_address = mac_address;
}

Employee::~Employee() {
    // Destructor
}

std::string Employee::getMacAddress() {
    return this->mac_address;
}

std::string Employee::getName() {
    return this->name;
}

void Employee::setMacAddress(const std::string& mac_address) {
    this->mac_address = mac_address;
}

void Employee::setName(const std::string& name) {
    this->name = name;
}