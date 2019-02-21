#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>


using namespace std;


struct person{
    char Name[50];
    float Balance;
};

void * clearStream(){
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
/*char * readData(fstream& rw){
    //clear flags
    rw.clear();
    //set position to the end
    rw.seekg(0, rw.end);
    //call end position as an int to get length
    int length = rw.tellg();
    //reset position to beginning
    rw.seekg(0, rw.beg);

    //char to read the file into with + 1 length
    //because read does not append null terminator
    char * buffer = new char [length + 1];
    rw.read(buffer, length);

    //append null terminator
    buffer[length + 1] = '\0';
    return buffer;
}*/


void * fileToStruct(person p[], fstream& rw){
    rw.clear();
    rw.seekg(0, rw.beg);
    //int peeker = rw.peek();
    int i = 0;
    while (!rw.eof()){
        string name, lname;
        float blnc;
        rw >> name >> lname >> blnc;
        name = name + ' ' + lname;
        strcpy(p[i].Name, name.c_str());
        p[i].Balance = blnc;
        i++;
    }
}


void display(person p[], int n){
    //easy string for spacing
    string spacer = string(12, ' ');

    cout << spacer << "Name" << spacer << "Balance" << endl;
    cout << string(47, '-') << endl;
    for (int i = 0; i < n; i++){
        cout << spacer << p[i].Name << " " << p[i].Balance << endl;
    }
    cout << string(47, '-') << endl;
}


void menu(){
    cout << "1. Find the Highest Balance" << endl;
    cout << "2. Make a Deposit" << endl;
    cout << "3. Exit" << endl;
}


void FindRichest(person p[], int n){
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


void * Deposit(string yourName, person p[], int n){
    double amount;
    //int to hold the index of the matched customer
    int indexOfCust = -1;
    for (int i = 0; i < n; i++){
        //convert cstring to string
        string pNameStr = p[i].Name;
        //compare strings
        if (yourName == pNameStr){
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
            //add amount to customer balance
            p[indexOfCust].Balance += amount;
            cout << p[indexOfCust].Name << " your new balance is " << p[indexOfCust].Balance << endl;
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
void NewCopy(string filename, person p[], int n){
    //open an fstream and truncate the file
    fstream ww(filename, std::fstream::out | std::fstream::trunc);
    for (int i = 0; i < n; i++){
        //copy the array into the file in the same format
        ww << p[i].Name << " " << p[i].Balance << endl;
    }
    ww.close();


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
    //find number of people listed in file
    int n = count / 3;
    //initialize struct array with size n
    person p [n];
    //read file contents to the struct
    fileToStruct(p, rw);
    //close the file for now
    rw.close();
    //display the data once
    //display(p, n);


    //create a loop that displays account balances, a menu, and asks for input
    char k;   
    while (k != '3'){
        //Display all accounts
        display(p, n);
        //Display the menu
        menu();
        //Ask user to pick a menu item
        cout << "Please choose an option by typing its number: ";
        cin >> k;
        clearStream();
        if(k == '1'){
            //Find the highest balance
            FindRichest(p, n);
        }
        else if(k == '2'){
            //Ask for name first
            string yourName;
            cout << "Enter your full name to deposit money: ";
            getline(cin, yourName);
            //Deposit Flow
            Deposit(yourName, p, n);
            //Update the txt file
            NewCopy(filename, p, n);
        }
        else if(k == '3'){
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