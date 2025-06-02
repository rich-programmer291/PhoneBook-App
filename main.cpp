#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

class Contact
{
public:
    string name, phone, email;

    // Constructors
    Contact() {}
    Contact(string n, string p, string e) : name(n), phone(p), email(e) {}

    string serialize()
    {
        return name + "," + phone + "," + email;
    }

    Contact deserialize(string data)
    {
        size_t pos1 = data.find(",");
        size_t pos2 = data.find(",", pos1 + 1);

        name = data.substr(0, pos1);
        phone = data.substr(pos1 + 1, pos2 - pos1 - 1);
        email = data.substr(pos2 + 1);

        return Contact(name, phone, email);
    }

    void display()
    {
        cout << left << setw(20) << name << setw(15) << phone << email << endl;
    }
};

class Phonebook
{
private:
    const string filename = "contacts.txt";

public:
    void addContact()
    {
        cout << "\n\n";
        Contact c;
        string phone;
        cout << "Enter Name: ";
        getline(cin, c.name);
        cout << "Enter Phone: ";
        getline(cin, phone);
        while (phone.length() != 10 && phone!="0")
        {
            cout << "Phone number must be 10 digits long." << endl;
            cout << "Re-enter the Phone number(Press 0 if number is not known): ";
            getline(cin, phone);
            
        }
        if(phone == "0")
        {
            phone = "Not Provided";
        }
        c.phone = phone;
        cout << "Enter Email: ";
        getline(cin, c.email);

        ofstream file;
        file.open(filename, ios::app);
        if (file.is_open())
        {
            file << c.serialize() << endl;
            file.close();
            cout << "\n\nContact added successfully!" << endl;
        }
        else
        {
            cout << "\n\nError opening file!" << endl;
        }
    }

    void viewContacts()
    {
        cout << "\n\n";
        ifstream file;
        file.open(filename, ios::in);
        if (!file.is_open())
        {
            cout << "\n\nError opening file!" << endl;
            return;
        }

        string line;
        cout << left << setw(20) << "Name " << setw(15) << "Phone " << "Email" << endl;
        cout << string(60, '-') << endl;

        while (getline(file, line))
        {
            Contact c;
            c.deserialize(line).display();
        }
        file.close();
    }

    void searchContactbyName()
    {
        cout << "\n\n";
        string name;
        cout << "Enter Name to search: ";
        getline(cin, name);

        ifstream file(filename);
        if (!file.is_open())
        {
            cout << "\n\nError opening file!" << endl;
            return;
        }

        string line;
        bool flag = false;
        while (getline(file, line))
        {
            Contact c;
            c.deserialize(line);
            if (c.name == name)
            {
                cout << left << setw(20) << c.name << setw(15) << c.phone << c.email << endl;
                flag = true;
            }
        }

        if (!flag)
        {
            cout << "\n\nContact not found!" << endl;
        }
        file.close();
    }

    void searchContactbyPhone()
    {
        cout << "\n\n";
        string phone;
        cout << "Enter Phone to search: ";
        getline(cin, phone);

        ifstream file(filename);
        if (!file.is_open())
        {
            cout << "\n\nError opening file!" << endl;
            return;
        }

        string line;
        bool flag = false;
        while (getline(file, line))
        {
            Contact c;
            c.deserialize(line);
            if (c.phone == phone)
            {
                cout << left << setw(20) << c.name << setw(15) << c.phone << c.email << endl;
                flag = true;
            }
        }

        if (!flag)
        {
            cout << "\n\nContact not found!" << endl;
        }
        file.close();
    }

    void updateContact()
    {
        cout << "\n\n";
        string name;
        cout << "Enter Name of the contact to update: ";
        getline(cin, name);

        ifstream file;
        file.open(filename, ios::in);
        if (!file.is_open())
        {
            cout << "\n\nError opening file!" << endl;
            return;
        }

        ofstream temp;
        temp.open("temp.txt", ios::out);
        if (!temp.is_open())
        {
            cout << "\n\nError creating temp file!" << endl;
            file.close();
            return;
        }

        string line;
        bool found = false;
        while (getline(file, line))
        {
            Contact c;
            c.deserialize(line);
            if (c.name == name)
            {
                found = true;
                cout << "\n\nCurrent details: \n\n";
                c.display();

                cout << "Enter new Phone: ";
                getline(cin, c.phone);
                while (c.phone.length() != 10 && c.phone != "n" && c.phone != "N")
                {
                    cout << "Phone number must be 10 digits long." << endl;
                    cout << "Re-enter the Phone number(Press 'n' or 'N' if number is not known): ";
                    getline(cin, c.phone);
                }
                if (c.phone == "n" || c.phone == "N")
                {
                    c.phone = "Not Provided";
                }
                char choice;
                cout << "Want to update Email? (y/n): ";
                cin >> choice;
                if (choice == 'y' || choice == 'Y')
                {
                    cout << "Enter new Email: ";
                    cin.ignore();
                    getline(cin, c.email);
                }
                temp << c.serialize() << endl;
                cout << "Contact updated successfully!" << endl;
            }
            else
            {
                temp << line << endl;
            }
        }

        file.close();
        temp.close();

        if (found)
        {
            remove(filename.c_str());             // delete original
            rename("temp.txt", filename.c_str()); // rename temp to original
        }
        else
        {
            remove("temp.txt"); // discard temp if no match
            cout << "\n\nContact not found!" << endl;
        }
    }

    void deleteContact()
    {
        cout << "\n\n";
        ifstream file(filename);
        ofstream temp("temp.txt");
        string keyword;
        cout << "Enter name to delete: ";
        getline(cin >> ws, keyword);
        string line;
        bool deleted = false;
        while (getline(file, line))
        {
            if (line.find(keyword) != string::npos)
            {
                deleted = true;
                continue;
            }
            temp << line << "\n";
        }
        file.close();
        temp.close();
        remove(filename.c_str());
        rename("temp.txt", filename.c_str());
        if (deleted)
            cout << "\n\nContact deleted.\n";
        else
            cout << "\n\nNo such contact found.\n";
    }
};

void displayMenu()
{
    cout << "\n\n";
    cout << "Phonebook Application" << endl;
    cout << "======================" << endl;
    cout << "1. Add Contact" << endl;
    cout << "2. View Contacts" << endl;
    cout << "3. Search Contact by Name" << endl;
    cout << "4. Search Contact by Phone" << endl;
    cout << "5. Update Contact" << endl;
    cout << "6. Delete Contact" << endl;
    cout << "7. Exit" << endl;
    cout << "\nEnter your choice: ";
}

int main()
{
    Phonebook pb;
    int choice;

    do
    {
        displayMenu();
        cin >> choice;
        cin.ignore();
        switch (choice)
        {
        case 1:
            pb.addContact();
            break;
        case 2:
            pb.viewContacts();
            break;
        case 3:
            pb.searchContactbyName();
            break;
        case 4:
            pb.searchContactbyPhone();
            break;
        case 5:
            pb.updateContact();
            break;
        case 6:
            pb.deleteContact();
            break;
        case 7:
            cout << "\n\nThank you for using PhoneBook." << endl;
            break;
        default:
            cout << "\n\nInvalid choice! Please try again." << endl;
        }

    } while (choice != 7);
}