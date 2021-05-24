#include "../include/Primary.hpp"
#include <string>
// welcome
Primary::Primary(int accountNumberValue, int offsetValue)
    : accountNumber(accountNumberValue),offset(offsetValue)
{}

void Primary::setAccountNumber(int accountNumberValue)
{
    accountNumber = accountNumberValue;
}

int Primary::getAccountNumber() const
{
    return accountNumber;
}

int Primary::getOffset() const
{
    return offset;
}
void Primary::setOffset(int offsetValue)
{
    offset = offsetValue;
}
