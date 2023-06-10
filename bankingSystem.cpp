#include <iostream>
#include <vector>
#include <map>
#include <set>

// Base class for Accounts
class Account{
    protected:
        // Base account information required for every account created
        std::string accountNumber;
        double ballance;

    public:
        // Constructor
        Account(std::string accNumber) : accountNumber(accNumber), ballance(0.0) {}

        // Methods
            // Withdrawing money from accounts
        virtual void withdraw(double amount) {
            if(amount <= ballance) {
                ballance -= amount;
            }else {
                std::cout << "Insufficient funds." << std::endl;
            }
        }
            // Depositing money into accounts
        virtual void deposit(double amount) {
            ballance += amount;
        }
            // Displaying Account information
        virtual void displayAccountInfo() const {
            std::cout << "Account Number: " << accountNumber << std::endl;
            std::cout << "Ballance: " << ballance << std::endl;
            std::cout << "--------------------";
        }
        // Add other methods
};


// Deposit account class  -  Child class of the Account class
class DepositAccount : public Account{
    public:
        // Constructor
        DepositAccount(std::string accNumber) : Account(accNumber) {}

        // Methods
            // Overriding main Display function
        void displayAccountInfo() const override {
            std::cout << "Deposit Account Information" << std::endl;
            Account::displayAccountInfo();
        }
        // Add other methods for deposit account
};


// Investment account class  -  Child class of the account class
class InvestmentAccount : public Account{
    public:
        // Constructor
        InvestmentAccount(std::string accNumber) : Account(accNumber) {}

        // Methods
            // Overriding main Display function
        void displayAccountInfo() const override {
            std::cout << "Investment Account Information" << std::endl;
            Account::displayAccountInfo();
        }
        // Add other methods for investment account
};


// Investment account types
// Checking Account  -  Child class of the Investment Account class
class CheckingAccount : public InvestmentAccount{
    private:
        static constexpr double transactionFee = 0.1;
        static constexpr double overdraftLimit = -500.0;
        int transactionCount;

        std::map<std::string, double> stocks;

    public:
        // Constructors
        CheckingAccount(std::string accNumber) : InvestmentAccount(accNumber), transactionCount(0) {}

        // Methods
            // Overriding main Withdraw function
        void withdraw(double amount) override {
            double fee = amount * transactionFee;
            amount += fee;


            if(ballance - amount >= overdraftLimit) {
                InvestmentAccount::withdraw(amount);

                transactionCount++;
            }else {
                std::cout << "Withdrawal amount exceeds the overdraft limit." << std::endl;
            }
        }
        
            // Overriding main Deposit function
        void deposit(double amount) override {
            // Apply transaction fee to deposit
            double fee = amount * transactionFee;
            amount -= fee;

            InvestmentAccount::deposit(amount);

            transactionCount++;
        }
            // Add stock
        void addStock(std::string symbol, double percent) {
            stocks[symbol] = percent;
        }

            // Remove stock
        void removeStock(std::string symbol) {
            stocks.erase(symbol);
        }

            // Overriding main Display function
        void displayAccountInfo() const override {
            std::cout << "Checking Account Information" << std::endl;
            std::cout << "Transaction count: " << transactionCount << std::endl;
            std::cout << "Stock holdings: " << std::endl;
            for(const auto& pair : stocks) {
                std::cout << "Stock: " << pair.first << ", Percentage: " << pair.second << "%" << std::endl;
            } 
            InvestmentAccount::displayAccountInfo();
        }
        // Add other methods
};


// Deposit account types
// Savings Account  -  Child class of the  Deposit Account class
class SavingsAccount : public DepositAccount{
    private:
        static constexpr double interestRate = 0.01;

    public:
        // Constructor
        SavingsAccount(std::string accNumber) : DepositAccount(accNumber) {}

        // Methods
            // Custom Interest function only for Savings Account class
        void applyInterest() {
            double interest = ballance * interestRate;
            ballance += interest;
        }

            // Overriding main Display function
        void displayAccountInfo() const override {
            std::cout << "Savings Account Information" << std::endl;
            DepositAccount::displayAccountInfo();
        }
        // Add other methods
};

// CD Account  -  Child class of the Deposit Account class
class CDAccount : public DepositAccount{
    private:
        double interestRate;
    
    public:
        // Constructor
        CDAccount(std::string accNumber, double interest) : DepositAccount(accNumber), interestRate(interest) {}

        // Methods
            // Custom Interest function only for CD Account class
        void applyInterest() {
            double interest = ballance * interestRate;
            ballance += interest;
        }
            // Overriding main Display function
        void displayAccountInfo() const override {
            std::cout << "CD Account Information" << std::endl;
            DepositAccount::displayAccountInfo();
        }
        // Add other methods
};



// Customer Class
class Customer{
    private:
        // Customer information
        std::string name;
        std::string gender;
        std::string citizenship;
        int age;
        std::string id;

        // Set to store multiple unique accounts
        std::set<Account*> accounts;

        // Map to check if other is owning account or not
        static std::map<Account*, Customer*> accountCustomerMap;

    public:
        // Constructor
        Customer(std::string custName, std::string custGender, std::string custCitizenship, int custAge, std::string custId) :
            name(custName), gender(custGender), citizenship(custCitizenship), age(custAge), id(custId) {}

        // Methods
            // Display function for Customer information
        void displayCustomerInfo() const {
            std::cout << "\n\nCustomer Information\n";
            std::cout << "Name: " << name << std::endl;
            std::cout << "Gender: " << gender << std::endl;
            std::cout << "Citizenship: " << citizenship << std::endl;
            std::cout << "Age: " << age << std::endl;
            std::cout << "Id: " << id << std::endl;
        }

            // Add accounts
        void addAccounts(Account* account) {
            // Checking if Account already belongs to other user
            if(accountCustomerMap.count(account) > 0) {
                // If it does we return following text and quit the function
                std::cout << "This Account Is Already Owned By Other Customer." << std::endl;
                return;
            }
            
            // If account doesn't belong to anyone else we add it to Customers accounts
            accounts.insert(account);
            // We also add it to the Map so that we know it now belongs to someone
            accountCustomerMap[account] = this;
        }
        
            // Getter to return stored accounts
        const std::set<Account*>& getAccounts() const {
            return accounts;
        }
        // Add other methods
};
// Initializing the static member variable outside the class definition
std::map<Account*, Customer*> Customer::accountCustomerMap;



int main(){

    // Create Deposit Account
    DepositAccount depositAccount("1234567");
    depositAccount.deposit(1000);
    DepositAccount testingDepositAccount("55555");
    testingDepositAccount.deposit(1);

    // Create Savings Account
    SavingsAccount savingsAccount("7654321");
    savingsAccount.deposit(2000);

    // Create CD Account
    CDAccount cdAccount("999", 0.05);
    cdAccount.deposit(3000);

    // Create Checking Account
    CheckingAccount checkingAccount("222");
    checkingAccount.deposit(5000);
    checkingAccount.withdraw(2000);

    // Add Stocks To The Map
    checkingAccount.addStock("Apple", 2.87);
    checkingAccount.addStock("Microsoft", 7.33);
    checkingAccount.addStock("Meta", 3.49);
    checkingAccount.addStock("Google", 1.37);

    // Apply Interest To Savings And CD Accounts
    savingsAccount.applyInterest();
    cdAccount.applyInterest();

    // Create Investment Account
    InvestmentAccount investmentAccount("888");
    investmentAccount.deposit(4000);

    // Create Join Account With Deposit, Investment And Checking Accounts
    std::set<Account*> jointAccount;
    jointAccount.insert(&depositAccount);
    jointAccount.insert(&investmentAccount);
    jointAccount.insert(&checkingAccount);
    jointAccount.insert(&testingDepositAccount);

    // Create Customer With Deposit Account
    Customer customer1("John Doe", "Male", "USA", 30, "ID123");
    customer1.addAccounts(&depositAccount);

    // Create Customer With Investment Account
    Customer customer2("Jane Smith", "Female", "Canada", 27, "ID456");
    customer2.addAccounts(&investmentAccount);

    // Create Customer With Joint Account
    Customer customer3("James Bond", "male", "UK", 45, "ID000");
    for(Account* account : jointAccount) {
        customer3.addAccounts(account); 
    }

    // Display Customer Information
        // Customer1
    customer1.displayCustomerInfo();
    std::cout << "Account Information:" << "\n\n";
    for (Account* account : customer1.getAccounts()) {
        account->displayAccountInfo();
        std::cout << std::endl;
    }

        // Customer2
    customer2.displayCustomerInfo();
    std::cout << "Account Information:" << "\n\n";
    for (Account* account : customer2.getAccounts()) {
        account->displayAccountInfo();
        std::cout << std::endl;
    }

        // Customer3
    customer3.displayCustomerInfo();
    std::cout << "Account Information:" << "\n\n";
    for (Account* account : customer3.getAccounts()) {
        account->displayAccountInfo();
        std::cout << std::endl;
    }


    return 0;
}