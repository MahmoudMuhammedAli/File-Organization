#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <array>
#include <fstream>
#include "../include/ClientData.hpp"
#include "ClientData.cpp"
#include "secondary.cpp"
#include "primary.cpp"

enum Choices
{
    END = 0,
    PRINT = 1,
    UPDATE,
    NEW,
    DELETE,
    BACKUP,
    RESTORE,
    CreatePrimaryIndex,
    CreateSecondaryIndex,
    TextForPrimary,
    TextForSecondary,
};

Choices enterChoice();
void createTextFile(std::fstream &);
void updateRecord(std::fstream &);
void newRecord(std::fstream &);
void deleteRecord(std::fstream &);
void outputLine(std::ostream &, const ClientData &);
int getAccount(const char *const);
void backup(std::fstream &, std::fstream &);
void restore(std::fstream &, std::fstream &);
void createPrimary(std::fstream &, std::fstream &);
void textForPrimary(std::fstream &);
void createsecondary(std::fstream &, std::fstream &);
void textForsecondary(std::fstream &);
static secondary arr[100];
using namespace std;
int main(int argc, char const *argv[])
{
    fstream inOutCredit;
    inOutCredit.open("../credit.dat", std::ios::in | std::ios::out | std::ios::binary);

    if (!inOutCredit)
    {
        inOutCredit.open("../credit.dat", ios::out | ios::binary);

        // exit program if ofstream could not open file
        if (!inOutCredit)
        {
            cerr << "inOutCredit File could not be opened." << endl;
            exit(EXIT_FAILURE);
        }                       // end if
        ClientData blankClient; // constructor zeros out each data member
        // output 100 blank records to file
        for (int i = 0; i < 100; ++i)
            inOutCredit.write(reinterpret_cast<const char *>(&blankClient),
                              sizeof(ClientData));
        inOutCredit.close();
    }

    fstream backUpFile;
    backUpFile.open("../backup.dat", std::ios::in | std::ios::out | std::ios::binary);

    if (!backUpFile)
    {
        backUpFile.open("../backup.dat", ios::out | ios::binary);

        // exit program if ofstream could not open file
        if (!backUpFile)
        {
            cerr << "backUpFile File could not be opened." << endl;
            exit(EXIT_FAILURE);
        } // end if
        ClientData blankClient;
        for (int i = 0; i < 100; ++i)
            backUpFile.write(reinterpret_cast<const char *>(&blankClient),
                             sizeof(ClientData));
        backUpFile.close();
    }
    fstream primaryIndex;
    primaryIndex.open("../primary.dat", std::ios::in | std::ios::out | std::ios::binary | std::ofstream::trunc);
    if (!primaryIndex)
    {
        primaryIndex.open("../primary.dat", std::ios::out | std::ios::binary);
        if (!primaryIndex)
        {
            cerr << "Primary File could not be opened." << endl;
            exit(EXIT_FAILURE);
        }
        Primary blankClient;
        for (int i = 0; i < 100; ++i)
            primaryIndex.write(reinterpret_cast<const char *>(&blankClient),
                               sizeof(Primary));
        primaryIndex.close();
    }
    fstream secondaryIndex;
    secondaryIndex.open("../secondary.dat", std::ios::in | std::ios::out | std::ios::binary);
    if (!secondaryIndex)
    {
        secondaryIndex.open("../secondary.dat", std::ios::out | std::ios::binary);
        if (!secondaryIndex)
        {
            cerr << "secondary File could not be opened." << endl;
            exit(EXIT_FAILURE);
        }
        secondary blankClient;
        for (int i = 0; i < 100; ++i)
            secondaryIndex.write(reinterpret_cast<const char *>(&blankClient),
                                 sizeof(secondary));
        secondaryIndex.close();
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
            backup(inOutCredit, backUpFile);
            break;
        case Choices::RESTORE:
            backup(inOutCredit, backUpFile);
            break;
        case Choices::CreatePrimaryIndex:
            createPrimary(primaryIndex, inOutCredit);
            break;
        case Choices::CreateSecondaryIndex:
            createPrimary(secondaryIndex, inOutCredit);
            break;
        case Choices::TextForPrimary:
            textForPrimary(primaryIndex);
            break;
        case Choices::TextForSecondary:
            textForsecondary(secondaryIndex);
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
              << "7- create primary index" << std::endl
              << "8- create secondary index" << std::endl
              << "9- convert primary to text" << std::endl
              << "10- convert secondary to text" << std::endl
              << "0- end program\n? " << std::endl;
    int menuChoice;
    std::cin >> menuChoice;

    return static_cast<Choices>(menuChoice);
}
//BACKUP
void backup(std::fstream &inOutCredit, std::fstream &backUpFile)
{

    backUpFile.clear();
    copy(istreambuf_iterator<char>(inOutCredit),
         istreambuf_iterator<char>(),
         ostreambuf_iterator<char>(backUpFile));
    cout << "Backup finished!" << endl;
}
void restore(std::fstream &inOutCredit, std::fstream &backUpFile)
{
    inOutCredit.open("../credit.dat" , std::ios::out | std::ios::trunc );
    copy(istreambuf_iterator<char>(backUpFile),
         istreambuf_iterator<char>(),
         ostreambuf_iterator<char>(inOutCredit));
    cout << "Restored!" << endl;
}
void createsecondary(std::fstream &secondaryIndex, std::fstream &inOutCredit)
{
    secondary index;
    inOutCredit.seekg(0);
    ClientData client;
    inOutCredit.read(reinterpret_cast<char *>(&client), sizeof(ClientData));

    int count = 0;
    for (int i = 0; i < 100; i++)
    {
        if (client.getAccountNumber() != 0)
        {
            secondaryIndex.seekg(i * sizeof(secondary));
            index.setLastName(client.getLastName());
            index.setAccountNumber(client.getAccountNumber());
            arr[i] = index;
            count++;
        }
    }

    secondary temp;
    for (int i = 0; i < count; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (arr[j].getLastName() < arr[i].getLastName())
            {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
  while (!inOutCredit.eof())
    {
        secondaryIndex.seekg(i * sizeof(secondary));
        index.setLastName(client.getLastName());
        secondaryIndex.write(reinterpret_cast<const char *>(&arr[i]), sizeof(secondary));
    }
}

void textForsecondary(std::fstream &readFromFile)
{
    std::ofstream outPrintFile("../secondaryIndex.txt", std::ios::out | std::ios::trunc);
    outPrintFile.clear();
    if (!outPrintFile)
    {
        std::cerr << "File could not be created." << std::endl;
        exit(EXIT_FAILURE);
    }
    outPrintFile.clear();
    outPrintFile << std::left << std::setw(14) << "Last Name" << std::setw(14) << "Account" << std::endl;

    readFromFile.seekg(0);

    secondary index;
    readFromFile.read(reinterpret_cast<char *>(&index), sizeof(secondary));

    while (!readFromFile.eof())
    {
        if (index.getAccountNumber() != 0)
            outPrintFile << std::left << std::setw(14) << index.getLastName()
                         << std::setw(14) << index.getAccountNumber() << std::endl;

        readFromFile.read(reinterpret_cast<char *>(&index), sizeof(secondary));
    }
}
void createPrimary(std::fstream &primaryIndex, std::fstream &inOutCredit)
{

    Primary index;
    inOutCredit.seekg(0);
    ClientData client;
    inOutCredit.read(reinterpret_cast<char *>(&client), sizeof(ClientData));

    while (!inOutCredit.eof())
    {
        index.setAccountNumber(client.getAccountNumber());
        index.setOffset((client.getAccountNumber() - 1) * sizeof(ClientData));
        primaryIndex.write(reinterpret_cast<const char *>(&index), sizeof(Primary));
        inOutCredit.read(reinterpret_cast<char *>(&client), sizeof(ClientData));
    }
}

void textForPrimary(std::fstream &readFromFile)
{
    std::ofstream outPrintFile("../primaryIndex.txt", std::ios::out | std::ios::trunc);
    outPrintFile.clear();
    if (!outPrintFile)
    {
        std::cerr << "File could not be created." << std::endl;
        exit(EXIT_FAILURE);
    }
    outPrintFile.clear();
    outPrintFile << std::left << std::setw(10) << "Account" << std::setw(10) << "offset" << std::endl;

    readFromFile.seekg(0);

    Primary index;
    readFromFile.read(reinterpret_cast<char *>(&index), sizeof(Primary));

    while (!readFromFile.eof())
    {
        if (index.getAccountNumber() != 0)
            outPrintFile << std::left << std::setw(10) << index.getAccountNumber()
                         << std::setw(10) << index.getOffset() << std::endl;

        readFromFile.read(reinterpret_cast<char *>(&index), sizeof(Primary));
    }
}

void createTextFile(std::fstream &readFromFile)
{
    std::ofstream outPrintFile("../print.txt", std::ios::out);

    if (!outPrintFile)
    {
        std::cerr << "File could not be created." << std::endl;
        exit(EXIT_FAILURE);
    }
    outPrintFile.clear();
    outPrintFile << std::left << std::setw(10) << "Account" << std::setw(16)
                 << "Last Name" << std::setw(11) << "First Name" << std::right
                 << std::setw(10) << "Balance" << std::setw(10) << "Branch" << std::setw(16) << std::endl;

    readFromFile.seekg(0);

    ClientData client;
    readFromFile.read(reinterpret_cast<char *>(&client), sizeof(ClientData));

    while (!readFromFile.eof())
    {
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
        int branchID;

        std::cout << "Enter lastname, firstname, balance , branchID\n? ";
        std::cin >> std::setw(15) >> lastName;
        std::cin >> std::setw(10) >> firstName;
        std::cin >> balance;
        cin >> branchID;

        client.setLastName(lastName);
        client.setFirstName(firstName);
        client.setBalance(balance);
        client.setAccountNumber(accountNumber);
        client.setBranchID(branchID);
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
           << std::showpoint << record.getBalance()
           << std::setw(10) << record.getBranchID() << std::endl;
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
} //ATTEMPT 1
  // backUpFile.clear();
  // std::string line;
  // while (getline(inOutCredit, line))
  // {
  //     backUpFile << line << "\n";
  // }

// std::cout << "Copy Finished \n";

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

//std::ofstream("Backup.dat") << std::ifstream("credit.dat").rdbuf();

//attempt 5

// backUpFile.clear();
// copy(istreambuf_iterator<char>(inOutCredit),
//      istreambuf_iterator<char>(),
//      ostreambuf_iterator<char>(backUpFile));
// cout << "COPY FINISHED!" << endl;
//attempt 6
    // ClientData newClient;
    // ClientData client;
    // inOutCredit.read(reinterpret_cast<char *>(&client), sizeof(ClientData));

    // while (!inOutCredit.eof())
    // {
    //     newClient.setAccountNumber(client.getAccountNumber());
    //     newClient.setLastName(client.getLastName());
    //     newClient.setFirstName(client.getFirstName());
    //     newClient.setBalance(client.getBalance());
    //     newClient.setBranchID(client.getBranchID());

    //     backUpFile.write(reinterpret_cast<const char *>(&newClient), sizeof(ClientData));
    //     inOutCredit.read(reinterpret_cast<char *>(&client), sizeof(ClientData));
    // }