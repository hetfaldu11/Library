#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <limits>
#include <iomanip>

using namespace std;

// ===== Helpers =====
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

tm parseDate(const string& dateStr) {
    tm date = {};
    istringstream ss(dateStr);
    ss >> get_time(&date, "%Y-%m-%d");
    return date;
}

// Get current date as string "YYYY-MM-DD"
string currentDate() {
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", ltm);
    return string(buffer);
}

// Calculate days between two dates (date2 - date1)
int daysBetween(const string& date1, const string& date2) {
    tm d1 = parseDate(date1);
    tm d2 = parseDate(date2);
    time_t time1 = mktime(&d1);
    time_t time2 = mktime(&d2);
    double seconds = difftime(time2, time1);
    return static_cast<int>(seconds / (60 * 60 * 24));
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    // consume leftover newline then wait for Enter
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string toLowerCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

string getNonEmptyLine(const string &prompt) {
    string input;
    do {
        cout << prompt;
        getline(cin, input);
        if (input.empty()) {
            cout << "Input cannot be empty. Try again.\n";
        }
    } while (input.empty());
    return input;
}

int getValidatedInt(const string &prompt) {
    int num;
    while (true) {
        cout << prompt;
        if (cin >> num) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return num;
        }
        cout << "Invalid number. Try again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// ===== Menu / UI helpers =====
void printCenteredLine(const string &text, int width = 48) {
    int len = (int)text.length();
    if (len >= width) {
        cout << text << "\n";
        return;
    }
    int left = (width - len) / 2;
    int right = width - len - left;
    cout << string(left, ' ') << text << string(right, ' ') << "\n";
}

void printBanner() {
    clearScreen();
    cout << string(48, '=') << "\n";
    printCenteredLine("📚  WELCOME TO LIBRARY SYSTEM  📚", 48);
    cout << string(48, '=') << "\n\n";
}

void displayMenuDecorated(const string &title, const vector<string> &options) {
    // width used for header centering
    const int width = 60;
    cout << string(width, '=') << "\n";
    printCenteredLine(" " + title + " ", width);
    cout << string(width, '=') << "\n\n";

    // If many options (>5) show in two columns
    if ((int)options.size() > 5) {
        int half = (options.size() + 1) / 2;
        size_t leftColWidth = 35;
        for (int i = 0; i < half; ++i) {
            string left = options[i];
            string leftFmt = "  [" + to_string(i+1) + "] " + left;
            string rightFmt = "";
            if (i + half < (int)options.size()) {
                string right = options[i + half];
                rightFmt = "  [" + to_string(i + half + 1) + "] " + right;
            }
            cout << leftFmt;
            // padding
            if (leftFmt.length() < leftColWidth) cout << string(leftColWidth - leftFmt.length(), ' ');
            cout << rightFmt << "\n";
        }
    } else {
        // single column
        for (size_t i = 0; i < options.size(); ++i) {
            cout << "  [" << (i+1) << "] " << options[i] << "\n";
        }
    }
    cout << "\n" << string(60, '-') << "\n";
}

// ===== Data structures =====
struct Member {
    string name;
    string password;
    string role;
};

struct Book {
    int id;
    string title;
    string author;
    int totalCopies;
    int availableCopies;
};

struct Transaction {
    int bookId;
    string memberName;
    string issueDate;
    string returnDate;
};

// ===== Library class =====
class Library {
private:
    vector<Member> members;
    vector<Book> books;
    vector<Transaction> transactions;

    string membersFile = "members.txt";
    string booksFile = "books.txt";
    string transactionsFile = "transactions.txt";

public:
    Library() {
        loadMembers();
        loadBooks();
        loadTransactions();
    }

    // ---- File I/O ----
    void loadMembers() {
        members.clear();
        ifstream file(membersFile);
        if (!file) return;
        Member m;
        while (getline(file, m.name) && getline(file, m.password) && getline(file, m.role)) {
            members.push_back(m);
        }
    }

    void saveMembers() {
        ofstream file(membersFile);
        for (auto &m : members) {
            file << m.name << "\n" << m.password << "\n" << m.role << "\n";
        }
    }

    void loadBooks() {
        books.clear();
        ifstream file(booksFile);
        if (!file) return;
        Book b;
        while (file >> b.id) {
            file.ignore();
            getline(file, b.title);
            getline(file, b.author);
            file >> b.totalCopies >> b.availableCopies;
            file.ignore();
            books.push_back(b);
        }
    }

    void saveBooks() {
        ofstream file(booksFile);
        for (auto &b : books) {
            file << b.id << "\n" << b.title << "\n" << b.author << "\n"
                 << b.totalCopies << " " << b.availableCopies << "\n";
        }
    }

    void loadTransactions() {
        transactions.clear();
        ifstream file(transactionsFile);
        if (!file) return;
        Transaction t;
        while (file >> t.bookId) {
            file.ignore();
            getline(file, t.memberName);
            getline(file, t.issueDate);
            getline(file, t.returnDate);
            transactions.push_back(t);
        }
    }

    void saveTransactions() {
        ofstream file(transactionsFile);
        for (auto &t : transactions) {
            file << t.bookId << "\n" << t.memberName << "\n" << t.issueDate << "\n" << t.returnDate << "\n";
        }
    }

    // // ---- Utility ----
    // void ensureFirstUser() {
    //     if (members.empty()) {
    //         cout << "No members found. Please create your first account.\n";
    //         addMember();
    //         pauseScreen();
    //     }
    // }

    Member* login() {
        while (true) {
            string name = getNonEmptyLine("Username: ");
            string pass = getNonEmptyLine("Password: ");

            for (auto &m : members) {
                if (toLowerCase(m.name) == toLowerCase(name) &&
                    toLowerCase(m.password) == toLowerCase(pass)) {
                    cout << "Login successful (" << m.role << ").\n";
                    pauseScreen();
                    return &m;
                }
            }
            cout << "Invalid credentials. Try again.\n";
        }
    }

    // ---- Menus (decorative) ----
    void homeMenu() {
        
        int choice;
        do {
            clearScreen();
            vector<string> opts = { "Login", "Create Account", "Exit" };
            displayMenuDecorated("LIBRARY MAIN MENU", opts);
            choice = getValidatedInt("Choose an option: ");
            switch (choice) {
                case 1: {
                    Member* current = login();
                    // Show program banner once after login
                    printBanner();
                    if (current->role == "admin") adminMenu();
                    else if (current->role == "librarian") librarianMenu();
                    else memberMenu(current);
                    break;
                }
                case 2:
                    addMember();
                    pauseScreen();
                    break;
                case 0:
                case 3: // in case user chooses 3 for Exit when printed as option 3
                    cout << "Exiting... Goodbye!\n";
                    pauseScreen();
                    exit(0);
                default:
                    cout << "Invalid option. Try again.\n";
                    pauseScreen();
            }
        } while (true);
    }

    void adminMenu() {
        int choice;
        vector<string> opts = {
            "Add Member", "Add Book", "View Books", "Update Book",
            "Delete Book", "View Reports", "View Members", "Search Book", "Logout"
        };
        do {
            clearScreen();
            displayMenuDecorated("ADMIN MENU", opts);
            choice = getValidatedInt("Choose: ");
            switch (choice) {
                case 1: addMember(); pauseScreen(); break;
                case 2: addBook(); pauseScreen(); break;
                case 3: viewBooks(); pauseScreen(); break;
                case 4: updateBook(); pauseScreen(); break;
                case 5: deleteBook(); pauseScreen(); break;
                case 6: viewReports(); pauseScreen(); break;
                case 7: viewMembers(); pauseScreen(); break;
                case 8: searchBook(); pauseScreen(); break;
                case 9:
                case 0:
                    cout << "Logging out...\n";
                    pauseScreen();
                    return;
                default: cout << "Invalid choice.\n"; pauseScreen();
            }
        } while (true);
    }

    void librarianMenu() {
        int choice;
        vector<string> opts = {
            "Add Book", "View Books", "Update Book", "Delete Book",
            "Issue Book", "Return Book", "View Members", "Search Book", "Logout"
        };
        do {
            clearScreen();
            displayMenuDecorated("LIBRARIAN MENU", opts);
            choice = getValidatedInt("Choose: ");
            switch (choice) {
                case 1: addBook(); pauseScreen(); break;
                case 2: viewBooks(); pauseScreen(); break;
                case 3: updateBook(); pauseScreen(); break;
                case 4: deleteBook(); pauseScreen(); break;
                case 5: issueBook(); pauseScreen(); break;
                case 6: returnBook(); pauseScreen(); break;
                case 7: viewMembers(); pauseScreen(); break;
                case 8: searchBook(); pauseScreen(); break;
                case 9:
                case 0:
                    cout << "Logging out...\n";
                    pauseScreen();
                    return;
                default: cout << "Invalid choice.\n"; pauseScreen();
            }
        } while (true);
    }

    void memberMenu(Member* m) {
        int choice;
        vector<string> opts = {
            "View Books", "View My Borrowed Books", "Search Book", "Logout"
        };
        do {
            clearScreen();
            displayMenuDecorated("MEMBER MENU", opts);
            choice = getValidatedInt("Choose: ");
            switch (choice) {
                case 1: viewBooks(); pauseScreen(); break;
                case 2: viewBorrowedBooks(m->name); pauseScreen(); break;
                case 3: searchBook(); pauseScreen(); break;
                case 4:
                case 0:
                    cout << "Logging out...\n";
                    pauseScreen();
                    return;
                default: cout << "Invalid choice.\n"; pauseScreen();
            }
        } while (true);
    }

    // ---- Member ops ----
    void addMember() {
        clearScreen();
        string name = getNonEmptyLine("Enter username: ");
        for (auto &m : members) {
            if (toLowerCase(m.name) == toLowerCase(name)) {
                cout << "Username already exists.\n";
                return;
            }
        }
        string pass = getNonEmptyLine("Enter password: ");
        string role;
        while (true) {
            role = getNonEmptyLine("Enter role (admin/librarian/member): ");
            string roleLower = toLowerCase(role);
            if (roleLower == "admin" || roleLower == "librarian" || roleLower == "member") {
                break;
            }
            cout << "Invalid role. Try again.\n";
        }
        members.push_back({name, pass, toLowerCase(role)});
        saveMembers();
        cout << "Member added.\n";
    }

    // ---- Book ops ----
    void addBook() {
        clearScreen();
        int id = getValidatedInt("Enter book ID: ");
        for (auto &b : books) {
            if (b.id == id) {
                cout << "Book ID already exists.\n";
                return;
            }
        }
        string title = getNonEmptyLine("Enter title: ");
        string author = getNonEmptyLine("Enter author: ");
        int copies = getValidatedInt("Enter number of copies: ");
        if(copies <= 0) {
            cout << "Copies must be at least 1.\n";
            return;
        }
        books.push_back({id, title, author, copies, copies});
        saveBooks();
        cout << "Book added.\n";
    }

    void viewBooks() {
        clearScreen();
        cout << "\nBooks List:\n";
        cout << left << setw(6) << "ID" << setw(30) << "Title" << setw(25) << "Author"
             << setw(12) << "Available" << setw(12) << "Total" << setw(12) << "Issued" << "\n";
        cout << string(97, '=') << "\n";
        for (auto &b : books) {
            int issued = b.totalCopies - b.availableCopies;
            cout << left << setw(6) << b.id
                 << setw(30) << b.title
                 << setw(25) << b.author
                 << setw(12) << b.availableCopies
                 << setw(12) << b.totalCopies
                 << setw(12) << issued << "\n";
        }
    }

    void searchBook() {
        clearScreen();
        cout << "\n--- Search Book ---\n";
        string keyword = getNonEmptyLine("Enter Book ID or Title: ");
        bool found = false;

        bool isNumber = all_of(keyword.begin(), keyword.end(), ::isdigit);
        cout << left << setw(6) << "ID" << setw(30) << "Title" << setw(25) << "Author"
             << setw(12) << "Available" << setw(12) << "Total" << setw(12) << "Issued" << "\n";
        cout << string(97, '=') << "\n";

        for (auto &b : books) {
            if ((isNumber && to_string(b.id) == keyword) ||
                (!isNumber && toLowerCase(b.title).find(toLowerCase(keyword)) != string::npos)) {
                int issued = b.totalCopies - b.availableCopies;
                cout << left << setw(6) << b.id
                     << setw(30) << b.title
                     << setw(25) << b.author
                     << setw(12) << b.availableCopies
                     << setw(12) << b.totalCopies
                     << setw(12) << issued << "\n";
                found = true;
            }
        }
        if (!found) {
            cout << "No book found matching '" << keyword << "'.\n";
        }
    }

    void updateBook() {
        clearScreen();
        int id = getValidatedInt("Enter book ID to update: ");
        for (auto &b : books) {
            if (b.id == id) {
                string newTitle = getNonEmptyLine("Enter new title (leave blank to keep current): ");
                if (!newTitle.empty()) b.title = newTitle;
                string newAuthor = getNonEmptyLine("Enter new author (leave blank to keep current): ");
                if (!newAuthor.empty()) b.author = newAuthor;
                int newCopies = getValidatedInt("Enter new total copies: ");
                if (newCopies >= 0) {
                    int issuedCopies = b.totalCopies - b.availableCopies;
                    if (newCopies < issuedCopies) {
                        cout << "Cannot set total copies less than issued copies (" << issuedCopies << ").\n";
                        return;
                    }
                    b.totalCopies = newCopies;
                    b.availableCopies = newCopies - issuedCopies;
                }
                saveBooks();
                cout << "Book updated.\n";
                return;
            }
        }
        cout << "Book not found.\n";
    }

    // ---- Fixed deleteBook (only delete available copies) ----
    void deleteBook() {
        viewBooks();
        int id = getValidatedInt("Enter Book ID to delete copies from: ");

        auto it = find_if(books.begin(), books.end(),
                          [id](const Book &b) { return b.id == id; });

        if (it != books.end()) {
            int issuedCopies = it->totalCopies - it->availableCopies;
            cout << "Book found: " << it->title << " by " << it->author << "\n";
            cout << "Total copies: " << it->totalCopies
                 << ", Available copies: " << it->availableCopies
                 << ", Issued copies: " << issuedCopies << "\n";

            int toDelete = getValidatedInt("How many copies should be deleted? ");
            if (toDelete <= 0) {
                cout << "Invalid number. No copies deleted.\n";
                return;
            }

            // Only allow deleting from available copies
            if (toDelete > it->availableCopies) {
                cout << "Cannot delete " << toDelete << " copies.\n";
                cout << "Only " << it->availableCopies << " copies are available to delete. "
                     << issuedCopies << " copies are currently issued.\n";
                return;
            }

            it->totalCopies -= toDelete;
            it->availableCopies -= toDelete;

            if (it->totalCopies == 0) {
                // If total is zero (which implies issuedCopies was 0 because toDelete <= available),
                // remove the record.
                books.erase(it);
                cout << "All copies removed. Book deleted from library.\n";
            } else {
                cout << "Deleted " << toDelete << " copies. "
                     << "Remaining total: " << it->totalCopies << "\n";
            }

            saveBooks();
        } else {
            cout << "Book not found.\n";
        }
    }

void issueBook() {
    clearScreen();
    int id = getValidatedInt("Enter book ID to issue: ");
    for (auto &b : books) {
        if (b.id == id) {
            if (b.availableCopies <= 0) {
                cout << "No copies available to issue.\n";
                return;
            }
            string member = getNonEmptyLine("Enter member username to issue book to: ");
            bool memberExists = false;
            for (auto &m : members) {
                if (toLowerCase(m.name) == toLowerCase(member)) {
                    memberExists = true;
                    break;
                }
            }
            if (!memberExists) {
                cout << "Member not found.\n";
                return;
            }
            b.availableCopies--;
            string issue_date = currentDate();
            transactions.push_back({id, member, issue_date, ""});
            saveBooks();
            saveTransactions();
            cout << "Book issued on " << issue_date << ".\n";
            return;
        }
    }
    cout << "Book not found.\n";
}

    void returnBook() {
    clearScreen();
    int id = getValidatedInt("Enter book ID to return: ");
    string member = getNonEmptyLine("Enter member username returning the book: ");
    for (auto &b : books) {
        if (b.id == id) {
            bool found = false;
            for (auto &t : transactions) {
                if (t.bookId == id && toLowerCase(t.memberName) == toLowerCase(member) && t.returnDate.empty()) {
                    string return_date = currentDate();
                    t.returnDate = return_date;
                    b.availableCopies++;

                    // Calculate fine
                    const int allowed_days = 14;  // 14 days allowed borrowing period
                    const int fine_per_day = 10;  // fine amount per late day
                    int diff = daysBetween(t.issueDate, return_date);
                    int late_days = diff - allowed_days;
                    if (late_days > 0) {
                        int fine = late_days * fine_per_day;
                        cout << "Book returned late by " << late_days << " days.\n";
                        cout << "Fine to be paid: " << fine << " units.\n";
                    } else {
                        cout << "Book returned on time. No fine.\n";
                    }

                    saveBooks();
                    saveTransactions();
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "No outstanding issue record found for this book and member.\n";
            }
            return;
        }
    }
    cout << "Book not found.\n";
}

    void viewReports() {
        clearScreen();
        cout << "\nTransactions:\n";
        cout << left << setw(8) << "BookID" << setw(20) << "Member" << setw(15) << "Issued" << setw(15) << "Returned" << "\n";
        cout << string(58, '=') << "\n";
        for (auto &t : transactions) {
            cout << left << setw(8) << t.bookId
                 << setw(20) << t.memberName
                 << setw(15) << t.issueDate
                 << setw(15) << (t.returnDate.empty() ? "-" : t.returnDate) << "\n";
        }
    }

    void viewBorrowedBooks(string memberName) {
        clearScreen();
        cout << "\nBorrowed Books for " << memberName << ":\n";
        bool found = false;
        cout << left << setw(8) << "BookID" << setw(30) << "Title" << setw(15) << "Issued Date" << "\n";
        cout << string(53, '=') << "\n";
        for (auto &t : transactions) {
            if (toLowerCase(t.memberName) == toLowerCase(memberName) && t.returnDate.empty()) {
                string title = "Unknown";
                for (auto &b : books) {
                    if (b.id == t.bookId) {
                        title = b.title;
                        break;
                    }
                }
                cout << left << setw(8) << t.bookId << setw(30) << title << setw(15) << t.issueDate << "\n";
                found = true;
            }
        }
        if (!found) {
            cout << "No borrowed books.\n";
        }
    }

    void viewMembers() {
        clearScreen();
        cout << "\nMembers List:\n";
        cout << left << setw(20) << "Username" << setw(15) << "Role" << "\n";
        cout << string(35, '=') << "\n";
        for (auto &m : members) {
            cout << left << setw(20) << m.name << setw(15) << m.role << "\n";
        }
    }
};

// ===== main =====
int main() {
    Library lib;
    lib.homeMenu();
    return 0;
}
