#include <string>

class secondary
{
private:
    char lastName[15];
    int accountNumber;



public:
    secondary(const std::string & = "",int = 0,int = 0);

    void setLastName(const std::string &);
    std::string getLastName() const;

    void setAccountNumber(int);
    int getAccountNumber() const;

};

