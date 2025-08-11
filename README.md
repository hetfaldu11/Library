📚 Library Management System (C++)

📖 Overview
This is a Console-based Library Management System written in C++ that uses file handling to store data persistently.
It simulates a small-scale library system where Admins, Librarians, and Members can log in and perform different actions based on their roles.

The system supports:
- Adding and managing members with different roles
- Adding, searching, updating, and deleting books
- Issuing and returning books
- Viewing reports of issued/returned books
- Persistent storage using text files (members.txt, books.txt, transactions.txt)
- Fine calculation for late book returns

🛠 Features

🔑 User Roles
1. Admin
   - Add new members (Admin/Librarian/Member)
   - Manage books (Add, View, Update, Delete)
   - View reports of all transactions
   - Search books
   - View all members

2. Librarian
   - Manage books (Add, View, Update, Delete)
   - Issue and return books
   - View members
   - Search books

3. Member
   - View available books
   - View their own borrowed books
   - Search books

📂 File Storage
The program stores all data in plain text files:

- members.txt
  username
  password
  role
  ...

- books.txt
  id
  title
  author
  totalCopies availableCopies
  ...

- transactions.txt
  bookId
  memberName
  issueDate (format YYYY-MM-DD)
  returnDate (format YYYY-MM-DD or empty if not returned)
  ...

🚀 How to Run
1️⃣ Compile the Program
g++ main.cpp -o library

2️⃣ Run the Program
./library   # Linux / Mac
library.exe # Windows

📜 Menu Flow

Home Menu
[1] Login
[2] Create Account
[3] Exit

Admin Menu
[1] Add Member
[2] Add Book
[3] View Books
[4] Update Book
[5] Delete Book
[6] View Reports
[7] View Members
[8] Search Book
[9] Logout

Librarian Menu
[1] Add Book
[2] View Books
[3] Update Book
[4] Delete Book
[5] Issue Book
[6] Return Book
[7] View Members
[8] Search Book
[9] Logout

Member Menu
[1] View Books
[2] View My Borrowed Books
[3] Search Book
[4] Logout

📌 Key Functions
- addMember() → Adds a new user with a role.
- addBook() → Adds a new book to the system.
- updateBook() → Updates title, author, or copy count.
- deleteBook() → Deletes available copies of a book.
- issueBook() → Issues a book to a member and records current date as issue date.
- returnBook() → Returns a borrowed book, records current date as return date, and calculates fine if late.
- viewReports() → Shows all issued/returned transactions.
- searchBook() → Searches books by ID or title.
- viewBorrowedBooks() → Shows books borrowed by a member.

⚠ Fine System for Late Returns
- Books can be borrowed for up to 14 days without fine.
- On returning a book, the program calculates the difference between return date and issue date.
- If the book is returned after 14 days, a fine of 10 units per day late is charged.
- The fine amount and late days are displayed during the return process.
- Dates are stored and handled in "YYYY-MM-DD" format using C++ standard time functions.

⚠ Notes
- If no members exist, you must create the first account manually via the "Create Account" option.
- The program is case-insensitive for usernames and passwords.
- Only available copies can be deleted.
- Dates are handled properly now; previous placeholders ("today") are replaced with actual dates.

💡 Future Improvements
- Implement password hashing for better security.
- Support exporting reports to CSV format.
- Add notifications for overdue books.
- Implement more detailed member borrowing history.


🖊 Author
Your Name –  Het Faldu
