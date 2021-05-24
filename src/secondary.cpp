#include "../include/secondary.hpp"
#include <string>

secondary::secondary( const std::string &lastName,int accountNumberValue, int offsetValue)
    : accountNumber(accountNumberValue)
{
    setLastName(lastName);
}

void secondary::setLastName(const std::string &lastNameString)
{
    size_t length = lastNameString.size();
    length = (length < 15) ? length : 14;
    lastNameString.copy(lastName, length);
    lastName[length] = '\0';
}

std::string secondary::getLastName() const
{
    return lastName;
}

void secondary::setAccountNumber(int accountNumberValue)
{
    accountNumber = accountNumberValue;
}

int secondary::getAccountNumber() const
{
    return accountNumber;
}

