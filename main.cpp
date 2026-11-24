
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <limits>

using namespace std;

class Book {
public:
    int id;
    string title;
    string author;
    int totalCopies;
    int availableCopies;

    Book(int i, string t, string a, int copies) {
        id = i;
        title = t;
        author = a;
        totalCopies = copies;
        availableCopies = copies;
    }
};

class Member {
public:
    int id;
    string name;

    Member(int i, string n) {
        id = i;
        name = n;
    }
};

class BorrowRecord {
public:
    int bookID;
    int memberID;
    bool returned;

    BorrowRecord(int b, int m, bool r = false) {
        bookID = b;
        memberID = m;
        returned = r;
    }
};

class Library {
private:
    vector<Book> books;
    vector<Member> members;
    vector<BorrowRecord> records;

    int findBookIndex(int id) {
        for (int i = 0; i < (int)books.size(); i++) {
            if (books[i].id == id)
                return i;
        }
        return -1;
    }

    int findMemberIndex(int id) {
        for (int i = 0; i < (int)members.size(); i++) {
            if (members[i].id == id)
                return i;
        }
        return -1;
    }

    string toLowerCase(string text) {
        transform(text.begin(), text.end(), text.begin(),
                  [](unsigned char c) { return (char)tolower(c); });
        return text;
    }

    void saveBooks() {
        ofstream file("books.txt");
        for (const Book &b : books) {
            file << b.id << "|" << b.title << "|" << b.author
                 << "|" << b.totalCopies << "|" << b.availableCopies << "\n";
        }
    }

    void saveMembers() {
        ofstream file("members.txt");
        for (const Member &m : members) {
            file << m.id << "|" << m.name << "\n";
        }
    }

    void saveRecords() {
        ofstream file("borrow_records.txt");
        for (const BorrowRecord &r : records) {
            file << r.bookID << "|" << r.memberID << "|"
                 << r.returned << "\n";
        }
    }

    void loadBooks() {
        ifstream file("books.txt");
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string idText, title, author, totalText, availableText;

            if (getline(ss, idText, '|') &&
                getline(ss, title, '|') &&
                getline(ss, author, '|') &&
                getline(ss, totalText, '|') &&
                getline(ss, availableText, '|')) {

                try {
                    Book b(stoi(idText), title, author, stoi(totalText));
                    b.availableCopies = stoi(availableText);
                    books.push_back(b);
                } catch (...) {
                    // Ignore incomplete or invalid records.
                }
            }
        }
    }

    void loadMembers() {
        ifstream file("members.txt");
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string idText, name;

            if (getline(ss, idText, '|') && getline(ss, name)) {
                try {
                    members.push_back(Member(stoi(idText), name));
                } catch (...) {
                    // Ignore incomplete or invalid records.
                }
            }
        }
    }

    void loadRecords() {
        ifstream file("borrow_records.txt");
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string bookText, memberText, returnedText;

            if (getline(ss, bookText, '|') &&
                getline(ss, memberText, '|') &&
                getline(ss, returnedText)) {
                try {
                    records.push_back(
                        BorrowRecord(stoi(bookText), stoi(memberText),
                                     stoi(returnedText) != 0)
                    );
                } catch (...) {
                    // Ignore incomplete or invalid records.
                }
            }
        }
    }

    int readNumber(string message) {
        int number;

        while (true) {
            cout << message;
            if (cin >> number) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return number;
            }

            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

public:
    Library() {
        loadBooks();
        loadMembers();
        loadRecords();
    }

    void addBook() {
        cout << "\n--- ADD NEW BOOK ---\n";

        int id = readNumber("Enter Book ID: ");

        if (findBookIndex(id) != -1) {
            cout << "A book with this ID already exists.\n";
            return;
        }

        string title, author;
        cout << "Enter Book Title: ";
        getline(cin, title);

        cout << "Enter Author Name: ";
        getline(cin, author);

        int copies = readNumber("Enter Number of Copies: ");

        if (title.empty() || author.empty() || copies <= 0) {
            cout << "Invalid book details. Please try again.\n";
            return;
        }

        if (title.find('|') != string::npos ||
            author.find('|') != string::npos) {
            cout << "The character | cannot be used in names or titles.\n";
            return;
        }

        books.push_back(Book(id, title, author, copies));
        saveBooks();

        cout << "Book added successfully!\n";
    }

    void registerMember() {
        cout << "\n--- REGISTER MEMBER ---\n";

        int id = readNumber("Enter Member ID: ");

        if (findMemberIndex(id) != -1) {
            cout << "A member with this ID already exists.\n";
            return;
        }

        string name;
        cout << "Enter Member Name: ";
        getline(cin, name);

        if (name.empty() || name.find('|') != string::npos) {
            cout << "Invalid member name.\n";
            return;
        }

        members.push_back(Member(id, name));
        saveMembers();

        cout << "Member registered successfully!\n";
    }

    void displayBooks() {
        cout << "\n--- ALL BOOKS ---\n";

        if (books.empty()) {
            cout << "No books are available in the library.\n";
            return;
        }

        cout << "\nID\tTitle\t\tAuthor\t\tAvailable/Total\n";
        cout << "----------------------------------------------------------\n";

        for (const Book &b : books) {
            cout << b.id << "\t" << b.title << "\t\t"
                 << b.author << "\t\t"
                 << b.availableCopies << "/" << b.totalCopies << "\n";
        }
    }

    void displayMembers() {
        cout << "\n--- REGISTERED MEMBERS ---\n";

        if (members.empty()) {
            cout << "No members registered yet.\n";
            return;
        }

        for (const Member &m : members) {
            cout << "Member ID: " << m.id
                 << " | Name: " << m.name << "\n";
        }
    }

    void searchBooks() {
        cout << "\n--- SEARCH BOOKS ---\n";

        string keyword;
        cout << "Enter book title or author name: ";
        getline(cin, keyword);

        string search = toLowerCase(keyword);
        bool found = false;

        for (const Book &b : books) {
            if (toLowerCase(b.title).find(search) != string::npos ||
                toLowerCase(b.author).find(search) != string::npos) {

                cout << "\nBook ID: " << b.id
                     << "\nTitle: " << b.title
                     << "\nAuthor: " << b.author
                     << "\nAvailable Copies: " << b.availableCopies
                     << "/" << b.totalCopies << "\n";

                found = true;
            }
        }

        if (!found)
            cout << "No matching books found.\n";
    }

    void issueBook() {
        cout << "\n--- ISSUE BOOK ---\n";

        int bookID = readNumber("Enter Book ID: ");
        int memberID = readNumber("Enter Member ID: ");

        int bookIndex = findBookIndex(bookID);
        int memberIndex = findMemberIndex(memberID);

        if (bookIndex == -1) {
            cout << "Book not found.\n";
            return;
        }

        if (memberIndex == -1) {
            cout << "Member not found. Register the member first.\n";
            return;
        }

        if (books[bookIndex].availableCopies <= 0) {
            cout << "Sorry! No copies of this book are currently available.\n";
            return;
        }

        books[bookIndex].availableCopies--;
        records.push_back(BorrowRecord(bookID, memberID));

        saveBooks();
        saveRecords();

        cout << "Book issued successfully to "
             << members[memberIndex].name << "!\n";
    }

    void returnBook() {
        cout << "\n--- RETURN BOOK ---\n";

        int bookID = readNumber("Enter Book ID: ");
        int memberID = readNumber("Enter Member ID: ");

        int bookIndex = findBookIndex(bookID);

        if (bookIndex == -1 || findMemberIndex(memberID) == -1) {
            cout << "Invalid Book ID or Member ID.\n";
            return;
        }

        for (int i = (int)records.size() - 1; i >= 0; i--) {
            if (records[i].bookID == bookID &&
                records[i].memberID == memberID &&
                !records[i].returned) {

                records[i].returned = true;
                books[bookIndex].availableCopies++;

                saveBooks();
                saveRecords();

                cout << "Book returned successfully!\n";
                return;
            }
        }

        cout << "No active issue record found for this member and book.\n";
    }

    void borrowingHistory() {
        cout << "\n--- BORROWING HISTORY ---\n";

        if (records.empty()) {
            cout << "No borrowing records found.\n";
            return;
        }

        for (const BorrowRecord &r : records) {
            int bi = findBookIndex(r.bookID);
            int mi = findMemberIndex(r.memberID);

            cout << "Book: "
                 << (bi == -1 ? "Unknown" : books[bi].title)
                 << " | Member: "
                 << (mi == -1 ? "Unknown" : members[mi].name)
                 << " | Status: "
                 << (r.returned ? "Returned" : "Issued")
                 << "\n";
        }
    }

    void menu() {
        int choice;

        do {
            cout << "\n====================================\n";
            cout << "       LIBRARY MANAGEMENT SYSTEM\n";
            cout << "====================================\n";
            cout << "1. Add New Book\n";
            cout << "2. Register Member\n";
            cout << "3. Display All Books\n";
            cout << "4. Display All Members\n";
            cout << "5. Search Book by Title or Author\n";
            cout << "6. Issue Book\n";
            cout << "7. Return Book\n";
            cout << "8. View Borrowing History\n";
            cout << "9. Exit\n";
            cout << "====================================\n";

            choice = readNumber("Enter your choice: ");

            switch (choice) {
                case 1: addBook(); break;
                case 2: registerMember(); break;
                case 3: displayBooks(); break;
                case 4: displayMembers(); break;
                case 5: searchBooks(); break;
                case 6: issueBook(); break;
                case 7: returnBook(); break;
                case 8: borrowingHistory(); break;
                case 9:
                    cout << "Thank you for using the Library Management System!\n";
                    break;
                default:
                    cout << "Invalid choice. Please select 1 to 9.\n";
            }

        } while (choice != 9);
    }
};

int main() {
    Library library;
    library.menu();
    return 0;
}
