#include <cstring>
#include <fstream>
#include "Magazine.h"

//CONSTRUCTORS
Magazine::Magazine() : PrintEditions(), period(UNKNOWN), count(0) {}

Magazine::Magazine(const char* nTitle, Period nPeriod, const char* nDescription, unsigned nCount, unsigned nLibraryNumber, unsigned nYear) : PrintEditions(nTitle, nDescription, nLibraryNumber, nYear), period(UNKNOWN), count(0) {
    period = nPeriod;
    count = nCount;
}

Magazine::Magazine(Period nPeriod, unsigned nCount) : period(UNKNOWN), count(0) {
    period = nPeriod;
    count = nCount;
}

Magazine& Magazine::operator=(const Magazine& other) {
    if (this == &other)
        return *this;
    PrintEditions::operator=(other);
    period = other.period;
    count = other.count;
    return *this;
}


//WRITE AND READ
void Magazine::write(std::ostream& file) const {
    size_t titleLength = strlen(title);
    file.write(reinterpret_cast<const char*>(&titleLength), sizeof(size_t));
    file.write(title, titleLength);

    file.write(reinterpret_cast<const char*>(&period), sizeof(Period));

    size_t descriptionLength = strlen(description);
    file.write(reinterpret_cast<const char*>(&descriptionLength), sizeof(size_t));
    file.write(description, descriptionLength);

    file.write(reinterpret_cast<const char*>(&count), sizeof(unsigned));

    file.write(reinterpret_cast<const char*>(&libraryNumber), sizeof(unsigned));

    file.write(reinterpret_cast<const char*>(&year), sizeof(unsigned));
}

void Magazine::read(std::istream& file) {
    size_t titleLength;
    file.read(reinterpret_cast<char*>(&titleLength), sizeof(size_t));
    try {
        char *titleBuffer = new char[titleLength + 1];
        file.read(titleBuffer, titleLength);
        titleBuffer[titleLength] = '\0';
        setTitle(titleBuffer);
        delete[] titleBuffer;
    } catch (std::bad_alloc& e) {
        std::cerr << "Unable to allocate memory for the title." << std::endl;
        return;
    } catch (std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }

    Period periodBuffer;
    file.read(reinterpret_cast<char*>(&periodBuffer), sizeof(Period));
    setPeriod(periodBuffer);

    size_t descriptionLength;
    file.read(reinterpret_cast<char*>(&descriptionLength), sizeof(size_t));
    try {
        char *descriptionBuffer = new char[descriptionLength + 1];
        file.read(descriptionBuffer, descriptionLength);
        descriptionBuffer[descriptionLength] = '\0';
        setDescription(descriptionBuffer);
        delete[] descriptionBuffer;
    } catch (std::bad_alloc& e) {
        std::cerr << "Unable to allocate memory for the description." << std::endl;
        return;
    } catch (std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }

    unsigned countBuffer;
    file.read(reinterpret_cast<char*>(&countBuffer), sizeof(unsigned));
    setCount(countBuffer);

    unsigned libNumBuffer;
    file.read(reinterpret_cast<char*>(&libNumBuffer), sizeof(unsigned));
    setLibraryNumber(libNumBuffer);

    unsigned yearBuffer;
    file.read(reinterpret_cast<char*>(&yearBuffer), sizeof(unsigned));
    try {
        setYear(yearBuffer);
    } catch (std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }
}

//GETTERS
const char* Magazine::getType() const { return "magazine"; }
Period Magazine::getPeriod() const { return period; }
unsigned Magazine::getCount() const { return count; }


//SETTERS
void Magazine::setPeriod(const Period nPeriod) {
    period = nPeriod;
}

void Magazine::setCount(unsigned nCount) {
    count = nCount;
}


//OPERATORS
std::ostream& operator<<(std::ostream& out, const Magazine& magazine) {
    out << "Title: " << magazine.title << std::endl;
    out << "Period: ";
    switch (magazine.period) {
        case WEEKLY:
            out << "WEEKLY" << std::endl;
            break;
        case MONTHLY:
            out << "MONTHLY" << std::endl;
            break;
        case YEARLY:
            out << "YEARLY" << std::endl;
            break;
        default:
            out << "UNKNOWN" << std::endl;
            break;
    }
    out << "Description: " << magazine.description << std::endl;
    out << "Count: " << magazine.count << std::endl;
    out << "Library Number: " << magazine.libraryNumber << std::endl;
    out << "Year: " << magazine.year << std::endl;
    return out;
}

std::istream& operator>>(std::istream& in, Magazine& magazine) {
    char buffer[256];

    std::cout << "Enter the title of the magazine: ";
    std::cin.ignore(sizeof(buffer), '\n');
    in.getline(buffer, sizeof(buffer));
    try {
        magazine.setTitle(buffer);
    } catch (std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
        return in;
    }

    std::cout << "How often is a new edition released (WEEKLY, MONTHLY, YEARLY) : ";
    in.getline(buffer, sizeof(buffer));
    if (strcmp(buffer, "WEEKLY") == 0)
        magazine.setPeriod(WEEKLY);
    else if (strcmp(buffer, "MONTHLY") == 0)
        magazine.setPeriod(MONTHLY);
    else if (strcmp(buffer, "YEARLY") == 0)
        magazine.setPeriod(YEARLY);
    else
        magazine.setPeriod(UNKNOWN);

    std::cout << "What is the description of the magazine: ";
    in.getline(buffer, sizeof(buffer));
    try {
        magazine.setDescription(buffer);
    } catch (std::invalid_argument& e) {
        delete[] magazine.title;
        magazine.setTitle("UNKNOWN");
        magazine.setPeriod(UNKNOWN);
        std::cerr << e.what() << std::endl;
        return in;
    }

    unsigned countBuffer;
    std::cout << "How many copies of this magazine are there: ";
    in >> countBuffer;
    magazine.setCount(countBuffer);

    unsigned libNumBuffer;
    std::cout << "Enter the library number of the magazine: ";
    in >> libNumBuffer;
    magazine.setLibraryNumber(libNumBuffer);

    unsigned yearBuffer;
    std::cout << "Enter the year the magazine was published: ";
    in >> yearBuffer;
    try {
        magazine.setYear(yearBuffer);
    } catch (std::invalid_argument& e) {
        delete[] magazine.title;
        magazine.setTitle("UNKNOWN");
        delete[] magazine.description;
        magazine.setDescription("UNKNOWN");
        magazine.setPeriod(UNKNOWN);
        std::cerr << e.what() << std::endl;
        return in;
    }

    return in;
}