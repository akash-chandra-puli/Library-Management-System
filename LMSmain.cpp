#include <iostream>
#include <array>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <optional>
#include <algorithm>
using namespace std;

int newId = 101;
int totalfinespaidtoday = 0;// calculates total fine collected on todays date while advancing to the next date if any mmeber pays
int today = 1;
const int MAX_BOOKS = 1000;
const int MAX_MEMBERS = 200;
int membersc = 100;
int memberCount = 100;
struct BorrowRecord {
    int _memberId;
    int _startDate;
    int _expectedEndDate;
    optional<int> _endDate;
};

struct BookInfo {
    string _title;
    string _authorName;
    list<BorrowRecord> _borrowRecordList;

    bool isAvailable() const {
        return _borrowRecordList.empty() || _borrowRecordList.back()._endDate.has_value();
    }

    optional<BorrowRecord> currentBorrow() const {
        if (!isAvailable()) return _borrowRecordList.back();
        return nullopt;
    }
};

struct MemberInfo {
    int _id;
    string _name;
    string _address;
    string _phoneNo;
    vector<unsigned> _borrowedBookIds;
    bool _isActive = false;
    int _fine = 0;//a variable fine to increase fine for everyday delay by 1 rupee while extending date
};

array<BookInfo, MAX_BOOKS> loadSampleBooks() {
    array<BookInfo, MAX_BOOKS> books;

    for (int i = 0; i < MAX_BOOKS; ++i) {
        books[i]._title = "Book_" + to_string(i + 1);
        books[i]._authorName = "Author_" + to_string(i + 1);
    }

    return books;
}



map<int, MemberInfo> loadSampleMembers() {
    map<int, MemberInfo> members;
    int membersc = 100;
    int memberCount = 100;
    for (int i = 0; i < 100; ++i) {
        int id = i + 1;
        members[id] = { id, "Member_" + to_string(i + 1), "Address_" + to_string(i + 1), "1234567890", {}, true ,0 };
    }
    return members;
}

void insertBookBorrow(array<BookInfo, MAX_BOOKS>& books, map<int, MemberInfo>& members) {
    int bookId, memberId, startDate=today, endDate;
    cout << "Enter Book ID (1 to 1000): "; cin >> bookId;
    cout << "Enter Member ID (1 to " << membersc << "): "; cin >> memberId;
    cout << "Start Date: "<<startDate<<"\n";
    cout << "Expected End Date: "; cin >> endDate;

    if (bookId < 1 || bookId > MAX_BOOKS || !members.count(memberId)) {
        cout << "Invalid IDs.\n";
        return;
    }

    auto& book = books[bookId - 1];
    auto& member = members[memberId];

    if (!book.isAvailable()) {
        cout << "Book is currently borrowed.\n";
        return;
    }

    if (!member._isActive) {
        cout << "Member is deactivated.\n";
        return;
    }

    if (member._borrowedBookIds.size() >= 3) {
        cout << "Member has already borrowed 3 books.\n";
        return;
    }

    book._borrowRecordList.push_back({ memberId, startDate, endDate, nullopt });
    member._borrowedBookIds.push_back(bookId);

    cout << "Book borrowed successfully.\n";
}

void searchBookStatus(const array<BookInfo, MAX_BOOKS>& books, const map<int, MemberInfo>& members) {
    int bookId;
    cout << "Enter Book ID (1 to " << MAX_BOOKS << "): "; cin >> bookId;

    if (bookId < 1 || bookId > MAX_BOOKS) {
        cout << "Invalid Book ID.\n";
        return;
    }

    const auto& book = books[bookId - 1];
    if (book._title.empty()) {
        cout << "Book not found.\n";
        return;
    }

    if (book.isAvailable()) {
        cout << "Book is currently available in library.\n";
    }
    else {
        auto current = book._borrowRecordList.back();
        const auto& member = members.at(current._memberId);
        cout << "Book is borrowed by " << member._name << " (ID: " << member._id << ")\n";
        cout << "Borrow Period: " << current._startDate << " to " << current._expectedEndDate << "\n";
    }
}

void showBookBorrowHistory(const array<BookInfo, MAX_BOOKS>& books, const map<int, MemberInfo>& members) {
    int bookId;
    cout << "Enter Book ID (1 to " << MAX_BOOKS << "): "; cin >> bookId;

    if (bookId < 1 || bookId > MAX_BOOKS) {
        cout << "Invalid Book ID.\n";
        return;
    }

    const auto& book = books[bookId - 1];
    const auto& borrowList = book._borrowRecordList;

    if (book._title.empty()) {
        cout << "Book not found.\n";
        return;
    }

    if (borrowList.empty()) {
        cout << "No borrow history for this book.\n";
        return;
    }

    cout << "Borrow history for Book ID " << bookId << " (" << book._title << "):\n";
    for (const auto& record : borrowList) {
        const auto& member = members.at(record._memberId);
        cout << "Member ID: " << member._id << ", Name: " << member._name << ", Duration: " << record._startDate << " - " << record._expectedEndDate;
        if (record._endDate.has_value())
            cout << ", Returned On: " << record._endDate.value();
        else
            cout << ", Not returned yet.";
        cout << "\n";
    }
}

void deactivateMember(map<int, MemberInfo>& members, const array<BookInfo, MAX_BOOKS>& books) {
    int memberId;
    cout << "Enter Member ID to deactivate (1 to " << membersc << " ): "; cin >> memberId;

    auto it = members.find(memberId);//returns data if memberId is available, else assigns members.end() if not found
    if (it == members.end()) {
        cout << "Member not found.\n";
        return;
    }

    MemberInfo& member = it->second;// var "it" is key value pair --first means key and -->second means data
    if (!member._borrowedBookIds.empty()) {
        cout << "Cannot deactivate: Member has borrowed books.\n";
        for (unsigned id : member._borrowedBookIds) {
            int index = id - 1;
            if (index >= 0 && index < MAX_BOOKS)
                cout << "  - Book ID: " << id << ", Title: " << books[index]._title << "\n";
        }
        return;
    }

    member._isActive = false;
    cout << "Member deactivated successfully.\n";
}

void viewMemberDetails(const map<int, MemberInfo>& members, const array<BookInfo, MAX_BOOKS>& books) {
    int memberId;
    cout << "Enter Member ID ( 1 to " << membersc << " ): "; cin >> memberId;

    if (members.find(memberId) != members.end()) {
        const auto& member = members.at(memberId);
        cout << "Member ID: " << member._id << "\n";
        cout << "Name: " << member._name << "\n";
        cout << "Address: " << member._address << "\n";
        cout << "Phone: " << member._phoneNo << "\n";
        cout << "Status: " << (member._isActive ? "Active" : "Inactive") << "\n";
        cout << "Fine amount is: Rs." << member._fine << "/-" "\n";
        cout << "Borrowed Books:\n";
        if (member._borrowedBookIds.empty()) {
            cout << "  None\n";
        }
        else {
            for (unsigned id : member._borrowedBookIds) {
                int index = id - 1;
                if (index >= 0 && index < MAX_BOOKS)
                    cout << "  - Book ID: " << id << ", Title: " << books[index]._title << "\n";
            }
        }
    }
    else {
        cout << "Member not found.\n";
    }
}

void returnBook(array<BookInfo, MAX_BOOKS>& books, map<int, MemberInfo>& members) {
    int bookId, memberId, returnDate;
    cout << "Enter Book ID to return (1 to " << MAX_BOOKS << "): "; cin >> bookId;
    cout << "Enter Member ID (1 to "<<newId-1<<"): "; cin >> memberId;
    cout << "Enter Return Date: "; cin >> returnDate;

    if (bookId < 1 || bookId > MAX_BOOKS) {
        cout << "Invalid Book ID.\n";
        return;
    }

    auto& book = books[bookId - 1];
    if (book._borrowRecordList.empty()) {
        cout << "No borrow records found for this book.\n";
        return;
    }

    auto& borrowRecord = book._borrowRecordList.back();
    if (borrowRecord._endDate.has_value()) {
        cout << "This book has already been returned.\n";
        return;
    }

    if (borrowRecord._memberId != memberId) {
        cout << "This book is not borrowed by member ID " << memberId << ".\n";
        return;
    }

    borrowRecord._endDate = returnDate;
    cout << "Book returned successfully.\n";

    auto& member = members.at(memberId);
    auto it =std:: find(member._borrowedBookIds.begin(), member._borrowedBookIds.end(), static_cast<unsigned>(bookId));
    if (it != member._borrowedBookIds.end()) {
        member._borrowedBookIds.erase(it);
    }
}

void insertNewMember(map<int, MemberInfo>& members) {
    while (members.count(newId)) newId++;

    if (members.size() >= MAX_MEMBERS) {
        cout << "Member limit reached. Cannot add more members.\n";
        return;
    }
    membersc = newId;
    string name, address, phone;
    cout << "New Member ID: " << newId << "\n";
    cout << "Enter Name: ";cin.ignore(); getline(cin, name);
    cout << "Enter Address: "; getline(cin, address);
    cout << "Enter Phone Number: "; getline(cin, phone);

    members[newId] = { newId, name, address, phone, {}, true ,0 };
    cout << "Member added successfully with ID " << newId << " and name is \"" << name << "\".\n";
    newId++;
}

void listBooksDueToday(array<BookInfo, MAX_BOOKS>& books, map<int, MemberInfo>& members) {
    bool found = false;
    for (int i = 0; i < MAX_BOOKS; ++i) {
        auto& book = books[i];
        if (!book._borrowRecordList.empty()) {
            auto& lastRecord = book._borrowRecordList.back();
            if (!lastRecord._endDate.has_value() && lastRecord._expectedEndDate == today) {
                auto& member = members.at(lastRecord._memberId);

                // If overdue (still not returned), update fine & extend due date
                if (today < lastRecord._expectedEndDate) continue; // Not due yet
                if (today > lastRecord._expectedEndDate) {
                    member._fine += 1;
                    lastRecord._expectedEndDate += 1;
                    continue; // Not shown in today list anymore
                }

                cout << "Book ID: " << i + 1 << ", Title: " << book._title
                    << ", Borrowed by: " << member._name << " (ID: " << member._id
                    << ") | Mobile No: " << member._phoneNo << "\n";
                found = true;
            }
        }
    }
    if (!found) {
        cout << "No books are due today.\n";
    }
}

void advanceDateAndApplyFines(array<BookInfo, MAX_BOOKS>& books, map<int, MemberInfo>& members, int& today) {
    bool anyDue = false;
    for (auto& book : books) {
        if (!book._borrowRecordList.empty()) {
            auto& record = book._borrowRecordList.back();
            if (!record._endDate.has_value() && record._expectedEndDate <= today) {
                members[record._memberId]._fine += 1;//adda fine by every one day extension by one rupee
                record._expectedEndDate += 1;  // extend due date
                anyDue = true;
            }
        }
    }
    today++;
    if (anyDue)
        cout << "Books were due today. Fines applied. Today's Date and expected date of each book, incremented to " << today << ".\n" << "Fines paid by members today: Rs." << totalfinespaidtoday << "/-\n";
    else
        cout << "No dues today. Date incremented to " << today << ".\n";
    totalfinespaidtoday = 0;
}

void payFine(map<int, MemberInfo>& members) {
    //calculates total fine collected on todays date while advancing to the next date if any mmeber pays
    int memberId;
    cout << "Enter Member ID to pay fine: ";
    cin >> memberId;

    auto it = members.find(memberId);
    if (it == members.end()) {
        cout << "Member not found.\n";
        return;
    }

    MemberInfo& member = it->second;
    totalfinespaidtoday = totalfinespaidtoday + member._fine;
    if (member._fine == 0) {
        cout << "No fine is due for this member.\n";
        return;
    }

    cout << "Current fine: Rs. " << member._fine << "/-\n";
    char confirm;
    cout << "Do you want to pay the fine and reset to 0? (y/n): ";
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        member._fine = 0;
        cout << "Fine cleared. Member now has no dues.\n";
    }
    else {
        cout << "Payment cancelled.\n";
    }
}


void activateMember(map<int, MemberInfo>& members) {
    int memberId;
    cout << "Enter Member ID to activate (1 to " << membersc << "): ";
    cin >> memberId;

    auto it = members.find(memberId);//returns data if memberId is available, else assigns members.end() if not found
    if (it == members.end()) {
        cout << "Member not found.\n";
        return;
    }

    MemberInfo& member = it->second;
    // var "it" is a key value where key is called as first and value is called second it value of member is given to member var
    if (member._isActive) {
        cout << "Member is already active.\n";
    }
    else {
        member._isActive = true;
        cout << "Member activated successfully.\n";
    }
}


int main() {
    array<BookInfo, MAX_BOOKS> books = loadSampleBooks();
    map<int, MemberInfo> members = loadSampleMembers();

    int choice;
    do {
        cout << "\nLibrary Management Menu (Today: " << today << ")\n";
        cout << "1. Borrow / Check Book Status\n";
        cout << "2. Deactivate / Activate Member\n";
        cout << "3. Show Book Borrow History\n";
        cout << "4. View Member Details\n";
        cout << "5. Return Book\n";
        cout << "6. Insert New Member\n";
        cout << "7. View Due Books Today\n";
        cout << "8. Advance Date If No Dues\n";
        cout << "9. Pay Fine\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
        case 1: {
            int subchoice;
            cout << "1. Borrow Book\n2. Check Book Status\nEnter option: ";
            cin >> subchoice;
            if (subchoice == 1)
                insertBookBorrow(books, members);
            else if (subchoice == 2)
                searchBookStatus(books, members);
            else
                cout << "Invalid option.\n";
            break;
        }
        case 2: {
            int subchoice;
            cout << "1. Activate Member\n2. Deactivate Member\nEnter option: ";
            cin >> subchoice;
            if (subchoice == 1)
                activateMember(members);
            else if (subchoice == 2)
                deactivateMember(members, books);
            else
                cout << "Invalid option.\n";
            break;
        }
        case 3: showBookBorrowHistory(books, members); break;
        case 4: viewMemberDetails(members, books); break;
        case 5: returnBook(books, members); break;
        case 6: insertNewMember(members); break;
        case 7: listBooksDueToday(books, members); break;
        case 8: advanceDateAndApplyFines(books, members, today);break;
        case 9: payFine(members); break;
        case 0: cout << "Exiting...\n"; break;
        default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
