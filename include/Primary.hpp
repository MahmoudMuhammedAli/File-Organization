#include <string>

class Primary
{
private:
    int accountNumber;
    int offset;


public:
    Primary(int = 0,int = 0);

    void setAccountNumber(int);
    int getAccountNumber() const;

    void setOffset(int);
    int getOffset() const;
};

