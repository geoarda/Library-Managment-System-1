#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <fstream>

using namespace std;

// Book structure
struct Book {
    int id;
    string title;
    string author;
    string isbn;
    bool isAvailable;
    string borrower;
    string borrowDate;
    
    Book(int i, string t, string a, string is) 
        : id(i), title(t), author(a), isbn(is), isAvailable(true), borrower(""), borrowDate("") {}
};

// Library Management System Class
class LibrarySystem {
private:
    vector<Book> books;
    int nextId;
    
    string getCurrentDate() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        return to_string(ltm->tm_mday) + "/" + 
               to_string(1 + ltm->tm_mon) + "/" + 
               to_string(1900 + ltm->tm_year);
    }
    
    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
    
    void printHeader(string title) {
        cout << "\n";
        cout << "╔════════════════════════════════════════════════════════════════╗\n";
        cout << "║" << setw(35 + title.length()/2) << title 
             << setw(33 - title.length()/2) << "║\n";
        cout << "╚════════════════════════════════════════════════════════════════╝\n\n";
    }
    
    void printLine() {
        cout << "────────────────────────────────────────────────────────────────\n";
    }

public:
    LibrarySystem() : nextId(1) {
        // Add some sample books
        addBook("The Great Gatsby", "F. Scott Fitzgerald", "978-0-7432-7356-5");
        addBook("1984", "George Orwell", "978-0-452-28423-4");
        addBook("To Kill a Mockingbird", "Harper Lee", "978-0-06-112008-4");
        addBook("Pride and Prejudice", "Jane Austen", "978-0-14-143951-8");
        addBook("The Catcher in the Rye", "J.D. Salinger", "978-0-316-76948-0");
    }
    
    void addBook(string title, string author, string isbn) {
        books.push_back(Book(nextId++, title, author, isbn));
    }
    
    void displayAllBooks() {
        clearScreen();
        printHeader("📚 ALL BOOKS IN LIBRARY");
        
        if (books.empty()) {
            cout << "❌ No books in the library yet.\n";
            return;
        }
        
        cout << left << setw(5) << "ID" 
             << setw(30) << "Title" 
             << setw(25) << "Author" 
             << setw(10) << "Status\n";
        printLine();
        
        for (const auto& book : books) {
            cout << left << setw(5) << book.id
                 << setw(30) << book.title.substr(0, 28)
                 << setw(25) << book.author.substr(0, 23)
                 << (book.isAvailable ? "✅ Available" : "❌ Borrowed") << "\n";
        }
        cout << "\n";
    }
    
    void displayAvailableBooks() {
        clearScreen();
        printHeader("✅ AVAILABLE BOOKS");
        
        vector<Book> available;
        for (const auto& book : books) {
            if (book.isAvailable) available.push_back(book);
        }
        
        if (available.empty()) {
            cout << "❌ No books available at the moment.\n";
            return;
        }
        
        cout << left << setw(5) << "ID" 
             << setw(30) << "Title" 
             << setw(25) << "Author" 
             << setw(20) << "ISBN\n";
        printLine();
        
        for (const auto& book : available) {
            cout << left << setw(5) << book.id
                 << setw(30) << book.title.substr(0, 28)
                 << setw(25) << book.author.substr(0, 23)
                 << book.isbn << "\n";
        }
        cout << "\n";
    }
    
    void borrowBook() {
        clearScreen();
        printHeader("📖 BORROW A BOOK");
        displayAvailableBooks();
        
        int bookId;
        string borrowerName;
        
        cout << "Enter Book ID to borrow (0 to cancel): ";
        cin >> bookId;
        
        if (bookId == 0) return;
        
        auto it = find_if(books.begin(), books.end(), 
                         [bookId](const Book& b) { return b.id == bookId; });
        
        if (it == books.end()) {
            cout << "\n❌ Book not found!\n";
            return;
        }
        
        if (!it->isAvailable) {
            cout << "\n❌ This book is already borrowed!\n";
            return;
        }
        
        cin.ignore();
        cout << "Enter your name: ";
        getline(cin, borrowerName);
        
        it->isAvailable = false;
        it->borrower = borrowerName;
        it->borrowDate = getCurrentDate();
        
        cout << "\n✅ Book borrowed successfully!\n";
        cout << "📅 Borrow Date: " << it->borrowDate << "\n";
        cout << "📚 Book: " << it->title << "\n";
        cout << "👤 Borrower: " << borrowerName << "\n";
    }
    
    void returnBook() {
        clearScreen();
        printHeader("📥 RETURN A BOOK");
        
        // Display borrowed books
        vector<Book*> borrowed;
        for (auto& book : books) {
            if (!book.isAvailable) borrowed.push_back(&book);
        }
        
        if (borrowed.empty()) {
            cout << "❌ No books are currently borrowed.\n";
            return;
        }
        
        cout << left << setw(5) << "ID" 
             << setw(30) << "Title" 
             << setw(20) << "Borrower" 
             << setw(15) << "Borrow Date\n";
        printLine();
        
        for (const auto& book : borrowed) {
            cout << left << setw(5) << book->id
                 << setw(30) << book->title.substr(0, 28)
                 << setw(20) << book->borrower.substr(0, 18)
                 << book->borrowDate << "\n";
        }
        
        int bookId;
        cout << "\nEnter Book ID to return (0 to cancel): ";
        cin >> bookId;
        
        if (bookId == 0) return;
        
        auto it = find_if(books.begin(), books.end(), 
                         [bookId](const Book& b) { return b.id == bookId; });
        
        if (it == books.end()) {
            cout << "\n❌ Book not found!\n";
            return;
        }
        
        if (it->isAvailable) {
            cout << "\n❌ This book is not borrowed!\n";
            return;
        }
        
        string borrower = it->borrower;
        it->isAvailable = true;
        it->borrower = "";
        it->borrowDate = "";
        
        cout << "\n✅ Book returned successfully!\n";
        cout << "📚 Book: " << it->title << "\n";
        cout << "👤 Returned by: " << borrower << "\n";
    }
    
    void searchBooks() {
        clearScreen();
        printHeader("🔍 SEARCH BOOKS");
        
        cout << "Search by:\n";
        cout << "1. Title\n";
        cout << "2. Author\n";
        cout << "3. ISBN\n";
        cout << "Choice: ";
        
        int choice;
        cin >> choice;
        cin.ignore();
        
        string searchTerm;
        cout << "Enter search term: ";
        getline(cin, searchTerm);
        
        // Convert to lowercase for case-insensitive search
        transform(searchTerm.begin(), searchTerm.end(), searchTerm.begin(), ::tolower);
        
        vector<Book> results;
        for (const auto& book : books) {
            string compare;
            switch(choice) {
                case 1: compare = book.title; break;
                case 2: compare = book.author; break;
                case 3: compare = book.isbn; break;
                default: continue;
            }
            
            transform(compare.begin(), compare.end(), compare.begin(), ::tolower);
            if (compare.find(searchTerm) != string::npos) {
                results.push_back(book);
            }
        }
        
        if (results.empty()) {
            cout << "\n❌ No books found matching your search.\n";
            return;
        }
        
        cout << "\n📋 Search Results (" << results.size() << " found):\n";
        printLine();
        cout << left << setw(5) << "ID" 
             << setw(30) << "Title" 
             << setw(25) << "Author" 
             << setw(10) << "Status\n";
        printLine();
        
        for (const auto& book : results) {
            cout << left << setw(5) << book.id
                 << setw(30) << book.title.substr(0, 28)
                 << setw(25) << book.author.substr(0, 23)
                 << (book.isAvailable ? "✅ Available" : "❌ Borrowed") << "\n";
        }
        cout << "\n";
    }
    
    void addNewBook() {
        clearScreen();
        printHeader("➕ ADD NEW BOOK");
        
        string title, author, isbn;
        
        cin.ignore();
        cout << "Enter book title: ";
        getline(cin, title);
        
        cout << "Enter author name: ";
        getline(cin, author);
        
        cout << "Enter ISBN: ";
        getline(cin, isbn);
        
        addBook(title, author, isbn);
        
        cout << "\n✅ Book added successfully!\n";
        cout << "📚 Title: " << title << "\n";
        cout << "✍️  Author: " << author << "\n";
        cout << "🔢 ISBN: " << isbn << "\n";
    }
    
    void showStatistics() {
        clearScreen();
        printHeader("📊 LIBRARY STATISTICS");
        
        int totalBooks = books.size();
        int availableBooks = 0;
        int borrowedBooks = 0;
        
        for (const auto& book : books) {
            if (book.isAvailable) availableBooks++;
            else borrowedBooks++;
        }
        
        cout << "📚 Total Books: " << totalBooks << "\n";
        cout << "✅ Available Books: " << availableBooks << "\n";
        cout << "❌ Borrowed Books: " << borrowedBooks << "\n";
        
        if (totalBooks > 0) {
            double availabilityRate = (double)availableBooks / totalBooks * 100;
            cout << "📈 Availability Rate: " << fixed << setprecision(1) 
                 << availabilityRate << "%\n";
        }
        
        cout << "\n";
    }
    
    void displayMenu() {
        clearScreen();
        cout << "\n";
        cout << "╔════════════════════════════════════════════════════════════════╗\n";
        cout << "║                  📚 LIBRARY MANAGEMENT SYSTEM 📚               ║\n";
        cout << "║                        By Arda - 2025                          ║\n";
        cout << "╚════════════════════════════════════════════════════════════════╝\n\n";
        
        cout << "  1. 📋 Display All Books\n";
        cout << "  2. ✅ Display Available Books\n";
        cout << "  3. 📖 Borrow a Book\n";
        cout << "  4. 📥 Return a Book\n";
        cout << "  5. 🔍 Search Books\n";
        cout << "  6. ➕ Add New Book\n";
        cout << "  7. 📊 Show Statistics\n";
        cout << "  8. 🚪 Exit\n\n";
        
        printLine();
        cout << "Enter your choice: ";
    }
    
    void run() {
        int choice;
        
        do {
            displayMenu();
            cin >> choice;
            
            switch(choice) {
                case 1:
                    displayAllBooks();
                    break;
                case 2:
                    displayAvailableBooks();
                    break;
                case 3:
                    borrowBook();
                    break;
                case 4:
                    returnBook();
                    break;
                case 5:
                    searchBooks();
                    break;
                case 6:
                    addNewBook();
                    break;
                case 7:
                    showStatistics();
                    break;
                case 8:
                    clearScreen();
                    cout << "\n";
                    cout << "╔════════════════════════════════════════════════════════════════╗\n";
                    cout << "║              Thanks for using Library System! 📚               ║\n";
                    cout << "║                  Developed by Arda - 2025                      ║\n";
                    cout << "╚════════════════════════════════════════════════════════════════╝\n\n";
                    return;
                default:
                    cout << "\n❌ Invalid choice! Please try again.\n";
            }
            
            if (choice >= 1 && choice <= 7) {
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
            }
            
        } while(choice != 8);
    }
};

int main() {
    LibrarySystem library;
    library.run();
    return 0;
}
