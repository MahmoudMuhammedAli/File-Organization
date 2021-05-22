#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include "../include/ClientData.hpp"
#include "ClientData.cpp"

enum Choices
{
    PRINT = 1,
    UPDATE,
    NEW,
    DELETE,
    BACKUP,
    RESTORE,
    END
};

Choices enterChoice();
void createTextFile(std::fstream &);
void updateRecord(std::fstream &);
void newRecord(std::fstream &);
void deleteRecord(std::fstream &);
void outputLine(std::ostream &, const ClientData &);
int getAccount(const char *const);
void backup(std::fstream &,std::fstream &);
void restore(std::fstream &,std::fstream &);

int main(int argc, char const *argv[])
{
    std::fstream inOutCredit("../credit.dat", std::ios::in | std::ios::out | std::ios::binary);

    if (!inOutCredit)
    {
        std::cerr << "File could not be opened." << std::endl;
        exit(EXIT_FAILURE);
        //TODO: if the file doesn't exist iniate one with 100 cells
    }
    std::fstream backUpFile("../backup.dat", std::ios::in | std::ios::out | std::ios::binary);

    if (!backUpFile)
    {
        std::cerr << "Backup File could not be opened." << std::endl;
        exit(EXIT_FAILURE);
         //TODO: if the file doesn't exist iniate one with 100 cells
    }
    Choices choice;

    while ((choice = enterChoice()) != Choices::END)
    {
        switch (choice)
        {
        case Choices::PRINT:
            createTextFile(inOutCredit);
            break;
        case Choices::UPDATE:
            updateRecord(inOutCredit);
            break;
        case Choices::NEW:
            newRecord(inOutCredit);
            break;
        case Choices::DELETE:
            deleteRecord(inOutCredit);
            break;
        case Choices::BACKUP:
            backup(inOutCredit,backUpFile);
            break;
        case Choices::RESTORE:
            backup(backUpFile,inOutCredit);
            break;    
        default:
            std::cerr << "Incorrect choice" << std::endl;
            break;
        }

        inOutCredit.clear();
    }

    return 0;
}

Choices enterChoice()
{
    std::cout << "\nEnter your choice:" << std::endl
              << "1- store a formatted text file of accounts" << std::endl
              << "   called \"print.txt\" for printing" << std::endl
              << "2- update an account" << std::endl
              << "3- add a new account" << std::endl
              << "4- delete an account" << std::endl
              << "5- Backup your account" << std::endl
              << "6- Restore your account" << std::endl
              << "7- end program\n? " << std::endl;
    int menuChoice;
    std::cin >> menuChoice;

    return static_cast<Choices>(menuChoice);
}
//BACKUP
void backup(std::fstream &inOutCredit,std::fstream &backUpFile){
   //ATTEMPT 1
   backUpFile.clear();
    std::string line;
            while(getline(inOutCredit,line)){
            backUpFile<< line << "\n";
        }
 
        std::cout << "Copy Finished \n";


    //ATTEMPT 2
    // if (inOutCredit.is_open() && backUpFile.is_open())
	// 	while (!inOutCredit.eof())
	// 		backUpFile.put(inOutCredit.get());
	// inOutCredit.close();
	// backUpFile.close();


    //ATTEMPT 3
	// while (!inOutCredit.eof())
    //     {
    //         auto c = inOutCredit.read( (char *) & ob, sizeof(ob));
	// 	    backUpFile.write((char *) & c, sizeof(int));
    //     }
	// inOutCredit.close();
	// backUpFile.close();


    //attempt4

   // std::ofstream("Backup.dat") << std::ifstream("credit.dat").rdbuf();

    //TODO: Fix backup and restore

}
void createTextFile(std::fstream &readFromFile)
{
    std::ofstream outPrintFile("../print.txt", std::ios::out);

    if (!outPrintFile)
    {
        std::cerr << "File could not be created." << std::endl;
        exit(EXIT_FAILURE);
    }

    outPrintFile << std::left << std::setw(10) << "Account" << std::setw(16)
                 << "Last Name" << std::setw(11) << "First Name" << std::right
                 << std::setw(10) << "Balance" << std::endl;

    readFromFile.seekg(0);

    ClientData client;
    readFromFile.read(reinterpret_cast<char *>(&client), sizeof(ClientData));

    while (!readFromFile.eof())
    {
        //das
        if (client.getAccountNumber() != 0)
            outputLine(outPrintFile, client);

        readFromFile.read(reinterpret_cast<char *>(&client), sizeof(ClientData));
    }
}

void updateRecord(std::fstream &updateFile)
{
    int accountNumber = getAccount("Enter account to update");

    updateFile.seekg((accountNumber - 1) * sizeof(ClientData));

    ClientData client;

    updateFile.read(reinterpret_cast<char *>(&client), sizeof(ClientData));

    if (client.getAccountNumber() != 0)
    {
        outputLine(std::cout, client);

        std::cout << "\nEnter charge (+) or payment (-) : ";
        double transaction;
        std::cin >> transaction;

        double oldBalance = client.getBalance();
        client.setBalance(oldBalance + transaction);
        outputLine(std::cout, client);

        updateFile.seekp((accountNumber - 1) * sizeof(ClientData));
        updateFile.write(reinterpret_cast<const char *>(&client), sizeof(ClientData));
    }
    else
    {
        std::cerr << "Account #" << accountNumber << " has no information." << std::endl;
    }
}

void newRecord(std::fstream &insertInFile)
{
    int accountNumber = getAccount("Enter new account number");

    insertInFile.seekg((accountNumber - 1) * sizeof(ClientData));

    ClientData client;
    insertInFile.read(reinterpret_cast<char *>(&client), sizeof(ClientData));

    if (client.getAccountNumber() == 0)
    {
        std::string lastName;
        std::string firstName;
        double balance;

        std::cout << "Enter lastname, firstname, balance\n? ";
        std::cin >> std::setw(15) >> lastName;
        std::cin >> std::setw(10) >> firstName;
        std::cin >> balance;

        client.setLastName(lastName);
        client.setFirstName(firstName);
        client.setBalance(balance);
        client.setAccountNumber(accountNumber);

        insertInFile.seekp((accountNumber - 1) * sizeof(ClientData));

        insertInFile.write(reinterpret_cast<const char *>(&client), sizeof(ClientData));
    }
    else
    {
        std::cerr << "Account #" << accountNumber
                  << " already contains information." << std::endl;
    }
}

void deleteRecord(std::fstream &deleteFromFile)
{
    int accountNumber = getAccount("Enter an account to delete");

    deleteFromFile.seekg((accountNumber - 1) * sizeof(ClientData));

    ClientData client;
    deleteFromFile.read(reinterpret_cast<char *>(&client), sizeof(ClientData));

    if (client.getAccountNumber() != 0)
    {
        ClientData blankClient;

        deleteFromFile.seekp((accountNumber - 1) * sizeof(ClientData));
        deleteFromFile.write(reinterpret_cast<const char *>(&blankClient), sizeof(ClientData));

        std::cout << "Account #" << accountNumber << " deleted.\n";
    }
    else
    {
        std::cerr << "Account #" << accountNumber << " is empty.\n";
    }
}

void outputLine(std::ostream &output, const ClientData &record)
{
    output << std::left << std::setw(10) << record.getAccountNumber()
           << std::setw(16) << record.getLastName()
           << std::setw(11) << record.getFirstName()
           << std::setw(10) << std::setprecision(2) << std::right << std::fixed
           << std::showpoint << record.getBalance() << std::endl;
}

int getAccount(const char *const prompt)
{
    int accountNumber;

    do
    {
        std::cout << prompt << " ( 1 -100 ): ";
        std::cin >> accountNumber;
    } while (accountNumber < 1 || accountNumber > 100);

    return accountNumber;
}