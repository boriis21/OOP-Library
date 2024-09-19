#include "PrintEditions.h"

bool isStringValid(const char* str) {
    return (str != nullptr && strlen(str) != 0 && strcmp(str, " ") != 0 && str[0] != '\0');
}

//HELPERS
void PrintEditions::copy(const PrintEditions& other) {
    try {
        title = new char[strlen(other.title) + 1];
        strcpy(title, other.title);
    } catch (std::bad_alloc& e) {
        std::cerr << "Unable to allocate memory for the title.";
    }

    try {
        description = new char[strlen(other.description) + 1];
        strcpy(description, other.description);
    } catch (std::bad_alloc& e) {
        delete[] title;
        std::cerr << "Unable to allocate memory for the description.";
    }

    libraryNumber = other.libraryNumber;
    year = other.year;
}

void PrintEditions::deallocate() {
    if (strcmp(title, "UNKNOWN") != 0)
        delete[] title;
    if (strcmp(description, "UNKNOWN") != 0)
        delete[] description;
}


//CONSTRUCTORS
PrintEditions::PrintEditions() : title("UNKNOWN"), description("UNKNOWN"), libraryNumber(0), year(1600) {}

PrintEditions::PrintEditions(const char* nTitle, const char* nDescription, unsigned nLibraryNumber, unsigned nYear) : title("UNKNOWN"), description("UNKNOWN"), libraryNumber(0), year(1600) {
    if (!isStringValid(nTitle))
        throw std::invalid_argument("The title cannot be empty!");
    if (!isStringValid(nDescription))
        throw std::invalid_argument("The description cannot be empty!");
    if (nYear <= 1600 || nYear > 2023)
        throw std::invalid_argument("Invalid year.");

    try {
        title = new char[strlen(nTitle) + 1];
        strcpy(title, nTitle);
    } catch (std::bad_alloc& e) {
        std::cerr << "Unable to allocate memory for the title.";
    }

    try {
        description = new char[strlen(nDescription) + 1];
        strcpy(description, nDescription);
    } catch (std::bad_alloc& e) {
        delete[] title;
        std::cerr << "Unable to allocate memory for the description.";
    }

    libraryNumber = nLibraryNumber;
    year = nYear;
}

PrintEditions::PrintEditions(const PrintEditions& other) {
    copy(other);
}

PrintEditions& PrintEditions::operator=(const PrintEditions& other) {
    if (this == & other)
        return *this;
    deallocate();
    copy(other);
    return *this;
}

PrintEditions::~PrintEditions() {
    deallocate();
}


//GETTERS
const char* PrintEditions::getTitle() const { return title; }
const char* PrintEditions::getDescription() const { return description; }
unsigned PrintEditions::getLibraryNumber() const { return libraryNumber; }
unsigned PrintEditions::getYear() const { return year; }


//SETTERS
void PrintEditions::setTitle(const char* nTitle) {
    if (!isStringValid(nTitle))
        throw std::invalid_argument("The title cannot be empty!");

    try {
        title = new char[strlen(nTitle) + 1];
        strcpy(title, nTitle);
    } catch (std::bad_alloc& e) {
        std::cerr << "Unable to allocate memory for the title.";
    }
}

void PrintEditions::setDescription(const char* nDescription) {
    if (!isStringValid(nDescription))
        throw std::invalid_argument("The description cannot be empty!");

    try {
        description = new char[strlen(nDescription) + 1];
        strcpy(description, nDescription);
    } catch (std::bad_alloc& e) {
        delete[] title;
        std::cerr << "Unable to allocate memory for the description.";
    }
}

void PrintEditions::setLibraryNumber(unsigned nLibraryNumber) {
    if (libraryNumber < 0)
        throw std::invalid_argument("The library number should be a positive number.");

    libraryNumber = nLibraryNumber;
}

void PrintEditions::setYear(unsigned nYear) {
    if (nYear <= 1600 || nYear > 2023)
        throw std::invalid_argument("Invalid year.");

    year = nYear;
}


//OPERATORS
bool operator<(const PrintEditions& lhs, const PrintEditions& rhs) {
    if (lhs.getYear() != rhs.getYear())
        return lhs.getYear() < rhs.getYear();

    if (strcmp(lhs.getTitle(), rhs.getTitle()) != 0)
        return strcmp(lhs.getTitle(), rhs.getTitle()) < 0;

    if ((strcmp(lhs.getType(), "magazine") == 0 && strcmp(rhs.getType(), "magazine") == 0) || (strcmp(lhs.getType(), "comics") == 0 && strcmp(rhs.getType(), "comics") == 0)) {
        if (strcmp(lhs.getTitle(), rhs.getTitle()) == 0) {
            return lhs.getCount() < rhs.getCount();
        }
    }

    return false;
}

bool operator<=(const PrintEditions& lhs, const PrintEditions& rhs) {
    return !(lhs > rhs);
}

bool operator>(const PrintEditions& lhs, const PrintEditions& rhs) {
    if (lhs.getYear() != rhs.getYear())
        return lhs.getYear() > rhs.getYear();

    if (strcmp(lhs.getTitle(), rhs.getTitle()) != 0)
        return strcmp(lhs.getTitle(), rhs.getTitle()) > 0;

    if ((strcmp(lhs.getType(), "magazine") == 0 && strcmp(rhs.getType(), "magazine") == 0) || (strcmp(lhs.getType(), "comics") == 0 && strcmp(rhs.getType(), "comics") == 0)) {
        if (strcmp(lhs.getTitle(), rhs.getTitle()) == 0) {
            return lhs.getCount() > rhs.getCount();
        }
    }

    return false;
}

bool operator>=(const PrintEditions& lhs, const PrintEditions& rhs) {
    return !(lhs < rhs);
}