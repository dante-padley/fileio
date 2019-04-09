//Luccio Padley
//Section 3

#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>


using namespace std;

//---------------
//STRUCT GLOBAL
//---------------
struct person{
    char Name[50];
    float Balance;
};

//wipe the buffer up to newline
void * clearStream(){
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

//---------------
//READ FILE OUT TO STRUCT PERSON ARRAY AND RETURN POINTER TO IT
//---------------
person * readData(int & n);
person * readData(int & n){
    person * ptr = new person[n];
    string filename = "data.txt";
    fstream rw;
    rw.open(filename);
    rw.seekg(0, rw.beg);
    int i = 0;

    string fname, lname;
    float blnc;
    while (rw >> fname >> lname >> blnc){
         //first name needs to be 9 characters
        fname = fname.substr(0, 9);
        //last name needs to be 10 characters
        lname = lname.substr(0, 10);
        //fullname assignment/concatenation
        string name = fname + ' ' + lname;

        //copy fullname to struct name, which is char
        strcpy(ptr[i].Name, name.c_str());

        //assign the struct's balance variable
        ptr[i].Balance = blnc;
        i++;
    }
    rw.close();
    return ptr;
}

//---------------
//DISPLAY EACH ITEM IN STRUCT ARRAY AS CUSTOMER DATA
//---------------
void display(person * p, int n){
    //easy string for spacing
    string spacer = string(12, ' ');

    cout << spacer << "Name" << spacer << "Balance" << endl;
    cout << string(47, '-') << endl;
    for (int i = 0; i < n; i++){
        cout << spacer << p[i].Name << " " << p[i].Balance << endl;
    }
    cout << string(47, '-') << endl;
}

//displays options menu
void menu(){
    cout << "1. Display all accounts" << endl;
    cout << "2. Find the Highest Balance" << endl;
    cout << "3. Make a Deposit" << endl;
    cout << "4. Save to the file" << endl;
    cout << "5. Exit" << endl;
}

//---------------
//FIND THE ACCOUNT WITH HIGHEST BALANCE
//---------------
void FindRichest(person * p, int n){
    //value to hold the index of the current richest customer
    int richest = 0;
    for (int i = 0; i < n; i++){
        //if the current item is greater than the current set "richest", 
        if (p[i].Balance >= p[richest].Balance){
            //update richest index to the current item
            richest = i;
        }
    }
    cout << "\nThe customer with maximum balance is " << p[richest].Name << '\n' << '\n';
}

//---------------
//"DEPOSIT" MONEY TO AN ACCOUNT
//---------------
void Deposit(person * p, int n, string custName, double amount){

    int indexOfCust = -1;
    for (int i = 0; i < n; i++){
        //convert cstring to string
        string pNameStr = p[i].Name;
        //compare strings
        if (custName == pNameStr){
            indexOfCust = i;
        }
    }
    p[indexOfCust].Balance += amount;
    cout << p[indexOfCust].Name << " your new balance is " << p[indexOfCust].Balance << endl;    
}


//Helper to grab customer deposit information
void preDeposit(person * p, int n, string custName, double &amount, bool &isValid){
    int indexOfCust = -1;
    for (int i = 0; i < n; i++){
        //convert cstring to string
        string pNameStr = p[i].Name;
        //compare strings
        if (custName == pNameStr){
            indexOfCust = i;
        }
    }
    //if int was never changed, just return
    if (indexOfCust == -1){
        cout << "\nCould not find customer.\n" << endl;
    }
    //if correct name
    else {
        //grab the amount
        cout << "How much would you like to deposit? ";
        cin >> amount;
        //make sure input doesn't break everything
        if (!cin.fail()){
            isValid = true;
            clearStream();
        }
        //they put a letter or something for the amount
        else{
            cout << "Invalid!" << endl;
            cin.clear();
            clearStream();
            
        }
        
    }
}


//---------------
//WRITE NEW ARRAY DATA BACK TO FILE
//---------------
void NewCopy(string filename, person * p, int n){
    //open an fstream and truncate the file
    fstream ww(filename, std::fstream::out | std::fstream::trunc);
    for (int i = 0; i < n; i++){
        //copy the array into the file in the same format
        ww << p[i].Name << " " << p[i].Balance << endl;
    }
    ww.close();
    cout << "File Updated..." << endl;


}

int main(){
    
    string filename = "data.txt";
    fstream rw;
    rw.open(filename);
    //count words in the file (3 per line)
    int count = 0;
    string word;
    while (rw >> word){
        count++;
    }
    //close the file for now
    rw.close();
    //find number of people listed in file
    int n = count / 3;
    //read file data to an array of type struct and return a pointer to the array
    person * personPtr = readData(n);
    
    //create a loop that displays account balances, a menu, and asks for input
    char k;   
    while (k != '5'){
        
        //Display the menu
        menu();
        //Ask user to pick a menu item
        cout << "Please choose an option by typing its number: ";
        cin >> k;
        clearStream();
        if(k == '1'){
            //Display all accounts
            display(personPtr, n);
        }
        else if(k == '2'){
            //Find the highest balance
            FindRichest(personPtr, n);
        }
        else if(k == '3'){
            //Ask for name first
            string custName;
            double amount;
            bool isValid = false;
            cout << "Enter your full name to deposit money: ";
            getline(cin, custName);
            //Deposit Flow
            preDeposit(personPtr, n, custName, amount, isValid);
            //if name and amount are both valid
            if (isValid){
                //process the deposit
                Deposit(personPtr, n, custName, amount);
            }
            //Update the txt file
            //NewCopy(filename, personPtr, n);
        }
        else if(k == '4'){
            //Update the txt file
            NewCopy(filename, personPtr, n);
        }
        else if(k == '5'){
            //clear out the "new" person array we created earlier
            delete [] personPtr;
            //exit option
            cout << "Goodbye" << endl;   
        }
        else{
            //notice of wrong input
            cout << "Invalid Input" << endl;
        }
    }

    
    return 0;
}