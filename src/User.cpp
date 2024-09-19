#include "User.h"

//HELPERS
void User::copy(const User& other) {
    try {
        name = new char[strlen(other.name) + 1];
        strcpy(name, other.name);
    } catch (std::bad_alloc& e) {
        std::cerr << "Unable to allocate new memory for the name of the user." << std::endl;
    }

    takenEditions = other.takenEditions;
    readEditions = other.readEditions;
}

void User::deallocate() {
    if (strcmp(name, "UNKNOWN") != 0)
        delete[] name;
}


//CONSTRUCTORS
User::User() : name("UNKNOWN"), takenEditions(), readEditions() {}

User::User(const char* nName) : name("UNKNOWN"), takenEditions(), readEditions() {
    if (!isStringValid(nName))
        throw std::invalid_argument("The name of the user cannot be empty.");

    try {
        name = new char[strlen(nName) + 1];
        strcpy(name, nName);
    } catch (std::bad_alloc& e) {
        std::cerr << "Unable to allocate new memory for the name of the user." << std::endl;
    }
}

User::User(const User& other) {
    copy(other);
}

User& User::operator=(const User& other) {
    if (this == &other)
        return *this;
    deallocate();
    copy(other);
    return *this;
}

User::~User() {
   deallocate();
}


//READ AND WRITE
void User::write(std::ostream& file) const {
    size_t nameLength = strlen(name);
    file.write(reinterpret_cast<const char*>(&nameLength), sizeof(size_t));
    file.write(name, nameLength);

    takenEditions.writeEdition(file);
    readEditions.writeEdition(file);
}

void User::read(std::istream& file) {
    size_t nameLength;
    file.read(reinterpret_cast<char*>(&nameLength), sizeof(size_t));
    try {
        char* nameBuffer = new char[nameLength + 1];
        file.read(nameBuffer, nameLength);
        nameBuffer[nameLength] = '\0';
        setName(nameBuffer);
        delete[] nameBuffer;
    } catch (std::bad_alloc& e) {
        std::cerr << "Unable to allocate memory for the name." << std::endl;
        return;
    }

    takenEditions.readEdition(file);
    readEditions.readEdition(file);
}


//GETTERS
const char* User::getName() const { return name; }
const EditionsArray& User::getTakenEditions() const { return takenEditions; }
const EditionsArray& User::getReadEditions() const { return readEditions; }
size_t User::getTakenSize() const { return takenEditions.getSize(); }
size_t User::getReadSize() const { return readEditions.getSize(); }


//SETTERS
void User::setName(const char* nName) {
    if (!isStringValid(nName))
        throw std::invalid_argument("The name of the user cannot be empty.");

    try {
        name = new char[strlen(nName) + 1];
        strcpy(name, nName);
    } catch (std::bad_alloc& e) {
        std::cerr << "Unable to allocate new memory for the name of the user." << std::endl;
    }
}


//METHODS
void User::removeFromTaken(const PrintEditions& edition) {
    takenEditions.remove(edition.getLibraryNumber());
}

void User::removeFromRead(const PrintEditions& edition) {
    readEditions.remove(edition.getLibraryNumber());
}


//OPERATORS
std::ostream& operator<<(std::ostream& out, const User& user) {
    out << "Name: " << user.getName() << std::endl;
    out << "Taken prints: " << std::endl;
    for (int i = 0; i < user.getTakenSize(); ++i) {
        out << user.takenEditions[i].getLibraryNumber() << ". " << user.takenEditions[i].getTitle() << std::endl;
    }

    out << "Read prints: " << std::endl;
    for (int i = 0; i < user.getReadSize(); ++i) {
        out << user.readEditions[i].getLibraryNumber() << ". " << user.readEditions[i].getTitle() << std::endl;
    }

    return out;
}

std::istream& operator>>(std::istream& in, User& user) {
    char nameBuffer[256];
    std::cout << "Enter the name of the user: ";
    std::cin.ignore(sizeof(nameBuffer), '\n');
    in.getline(nameBuffer, sizeof(nameBuffer));
    try {
        user.setName(nameBuffer);
    } catch (std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
        return in;
    }

    return in;
}

bool operator==(const User& lhs, const User& rhs) {
    if (strcmp(lhs.getName(), rhs.getName()) == 0)
        return true;

    return false;
}

bool operator!=(const User& lhs, const User& rhs) {
    return !(lhs == rhs);
}

bool operator<(const User& lhs, const User& rhs) {
    return lhs.readEditions.getSize() < rhs.readEditions.getSize();
}

bool operator<=(const User& lhs, const User& rhs) {
    return !(lhs > rhs);
}

bool operator>(const User& lhs, const User& rhs) {
    return lhs.readEditions.getSize() > rhs.readEditions.getSize();
}

bool operator>=(const User& lhs, const User& rhs) {
    return !(lhs < rhs);
}

User& User::operator+=(const PrintEditions& edition) {
    if (strcmp(edition.getType(), "book") == 0) {
        const Book* book = dynamic_cast<const Book*>(&edition);
        try {
            takenEditions.push(new Book(*book));
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }
    } else if (strcmp(edition.getType(), "magazine") == 0) {
        const Magazine* magazine = dynamic_cast<const Magazine*>(&edition);
        try {
            takenEditions.push(new Magazine(*magazine));
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }
    } else if (strcmp(edition.getType(), "comics") == 0) {
        const Comics* comics = dynamic_cast<const Comics*>(&edition);
        try {
            takenEditions.push(new Comics(*comics));
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    return *this;
}

User& User::operator-=(const PrintEditions& edition) {
    takenEditions.remove(edition.getLibraryNumber());

    if (strcmp(edition.getType(), "book") == 0) {
        const Book* book = dynamic_cast<const Book*>(&edition);
        try {
            readEditions.push(new Book(*book));
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }
    } else if (strcmp(edition.getType(), "magazine") == 0) {
        const Magazine* magazine = dynamic_cast<const Magazine*>(&edition);
        try {
            readEditions.push(new Magazine(*magazine));
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }
    } else if (strcmp(edition.getType(), "comics") == 0) {
        const Comics* comics = dynamic_cast<const Comics*>(&edition);
        try {
            readEditions.push(new Comics(*comics));
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    return *this;
}

User operator+(User user, const PrintEditions& print) {
    user += print;
    return user;
}

User operator-(User user, const PrintEditions& print) {
    user -= print;
    return user;
}

const char* User::operator[](unsigned libraryNumber) const {
    bool hasRead = readEditions.contains(libraryNumber);
    bool isTaken = takenEditions.contains(libraryNumber);

    if (hasRead)
        return "Already read.";
    else if (isTaken)
        return "Currently taken.";
    else
        return "Never taken.";
}