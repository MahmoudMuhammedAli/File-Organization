#include "../include/ClientData.hpp"
#include <string>
// welcome
ClientData::ClientData(int accountNumberValue, const std::string &lastName,
                       const std::string &firstName, double balanceValue,int branchID)
    : accountNumber(accountNumberValue), balance(balanceValue)
{
    setFirstName(firstName);
    setLastName(lastName);
}

void ClientData::setAccountNumber(int accountNumberValue)
{
    accountNumber = accountNumberValue;
}

int ClientData::getAccountNumber() const
{
    return accountNumber;
}

void ClientData::setLastName(const std::string &lastNameString)
{
    size_t length = lastNameString.size();
    length = (length < 15) ? length : 14;
    lastNameString.copy(lastName, length);
    lastName[length] = '\0';
}

std::string ClientData::getLastName() const
{
    return lastName;
}

void ClientData::setFirstName(const std::string &firstNameString)
{
    size_t lenght = firstNameString.size();
    lenght = (lenght < 10) ? lenght : 9;
    firstNameString.copy(firstName, lenght);
    firstName[lenght] = '\0';
}

std::string ClientData::getFirstName() const
{
    return firstName;
}

void ClientData::setBalance(double balanceValue)
{
    balance = balanceValue;
}

double ClientData::getBalance() const
{
    return balance;
}


int ClientData::getBranchID() const
{
    return branchID;
}
void ClientData::setBranchID(int branchIDValue)
{
    branchID = branchIDValue;
}
