#include <fstream>
#include "Comics.h"

//CONSTRUCTORS
Comics::Comics() : Book(), Magazine() {}

Comics::Comics(const char* nAuthor, const char* nTitle, const char* nPublisher, const char* nDescription, Period nPeriod, unsigned nCount, unsigned nLibraryNumber, unsigned nYear)
                : Book(nAuthor, nTitle, nPublisher, nDescription, nLibraryNumber, nYear), Magazine(nPeriod, nCount) {}

Comics& Comics::operator=(const Comics& other) {
    if (this == &other)
        return *this;
    Book::operator=(other);
    Magazine::operator=(other);
    return *this;
}


//WRITE AND READ
void Comics::write(std::ostream& file) const {
    size_t authorLength = strlen(author);
    file.write(reinterpret_cast<const char*>(&authorLength), sizeof(size_t));
    file.write(author, authorLength);

    size_t titleLength = strlen(title);
    file.write(reinterpret_cast<const char*>(&titleLength), sizeof(size_t));
    file.write(title, titleLength);

    size_t publisherLength = strlen(publisher);
    file.write(reinterpret_cast<const char*>(&publisherLength), sizeof(size_t));
    file.write(publisher, publisherLength);

    file.write(reinterpret_cast<const char*>(&genre), sizeof(uint16_t));

    size_t descriptionLength = strlen(description);
    file.write(reinterpret_cast<const char*>(&descriptionLength), sizeof(size_t));
    file.write(description, descriptionLength);

    file.write(reinterpret_cast<const char*>(&period), sizeof(Period));

    file.write(reinterpret_cast<const char*>(&count), sizeof(unsigned));

    file.write(reinterpret_cast<const char*>(&libraryNumber), sizeof(unsigned));

    file.write(reinterpret_cast<const char*>(&year), sizeof(unsigned));
}

void Comics::read(std::istream& file) {
    size_t authorLength;
    file.read(reinterpret_cast<char*>(&authorLength), sizeof(size_t));
    try {
        char *authorBuffer = new char[authorLength + 1];
        file.read(authorBuffer, authorLength);
        authorBuffer[authorLength] = '\0';
        setAuthor(authorBuffer);
        delete[] authorBuffer;
    } catch (std::bad_alloc& e) {
        std::cerr << "Unable to allocate memory for the author." << std::endl;
        return;
    } catch (std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }

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

    size_t publisherLength;
    file.read(reinterpret_cast<char*>(&publisherLength), sizeof(size_t));
    try {
        char *publisherBuffer = new char[publisherLength + 1];
        file.read(publisherBuffer, publisherLength);
        publisherBuffer[publisherLength] = '\0';
        setPublisher(publisherBuffer);
        delete[] publisherBuffer;
    } catch (std::bad_alloc& e) {
        std::cerr << "Unable to allocate memory for the publisher." << std::endl;
        return;
    } catch (std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }

    uint16_t genreBuffer;
    file.read(reinterpret_cast<char*>(&genreBuffer), sizeof(uint16_t));
    genre = genreBuffer;

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

    Period periodBuffer;
    file.read(reinterpret_cast<char*>(&periodBuffer), sizeof(Period));
    setPeriod(periodBuffer);

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
const char* Comics::getType() const { return "comics"; }
unsigned Comics::getCount() const { return count; }
void Comics::setCount(unsigned nCount) {
    count = nCount;
}


//OPERATORS
std::ostream& operator<<(std::ostream& out, const Comics& comics) {
    out << "Author: " << comics.author << std::endl;
    out << "Title: " << comics.title << std::endl;
    out << "Publisher: " << comics.publisher << std::endl;
    out << "Genre: " << comics.genre << std::endl;
    out << "Description: " << comics.description << std::endl;
    out << "Period: ";
    switch (comics.period) {
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
    out << "Count: " << comics.count << std::endl;
    out << "Library Number: " << comics.libraryNumber << std::endl;
    out << "Year: " << comics.year << std::endl;
    return out;
}

std::istream& operator>>(std::istream& in, Comics& comics) {
    char buffer[256];

    std::cout << "Enter the name of the author: ";
    in.ignore(sizeof(buffer), '\n');
    in.getline(buffer, sizeof(buffer));
    try {
        comics.setAuthor(buffer);
    } catch (std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
        return in;
    }

    std::cout << "Enter the title of the comics: ";
    in.getline(buffer, sizeof(buffer));
    try {
        comics.setTitle(buffer);
    } catch (std::invalid_argument& e) {
        delete[] comics.author;
        comics.setAuthor("UNKNOWN");
        std::cerr << e.what() << std::endl;
        return in;
    }

    std::cout << "Enter the name of the publisher: ";
    in.getline(buffer, sizeof(buffer));
    try {
        comics.setPublisher(buffer);
    } catch (std::invalid_argument& e) {
        delete[] comics.author;
        comics.setAuthor("UNKNOWN");
        delete[] comics.title;
        comics.setTitle("UNKNOWN");
        std::cerr << e.what() << std::endl;
        return in;
    }

    comics.genre = generateGenre();

    std::cout << "What is the description of the comics: ";
    in.ignore(sizeof(buffer), '\n');
    in.getline(buffer, sizeof(buffer));
    try {
        comics.setDescription(buffer);
    } catch (std::invalid_argument& e) {
        delete[] comics.author;
        comics.setAuthor("UNKNOWN");
        delete[] comics.title;
        comics.setTitle("UNKNOWN");
        delete[] comics.publisher;
        comics.setPublisher("UNKnOWN");
        std::cerr << e.what() << std::endl;
        return in;
    }

    std::cout << "How often is a new edition released (WEEKLY, MONTHLY, YEARLY) : ";
    in.getline(buffer, sizeof(buffer));
    if (strcmp(buffer, "WEEKLY") == 0)
        comics.setPeriod(WEEKLY);
    else if (strcmp(buffer, "MONTHLY") == 0)
        comics.setPeriod(MONTHLY);
    else if (strcmp(buffer, "YEARLY") == 0)
        comics.setPeriod(YEARLY);
    else
        comics.setPeriod(UNKNOWN);

    unsigned countBuffer;
    std::cout << "How many copies of this comics are there: ";
    in >> countBuffer;
    comics.setCount(countBuffer);

    unsigned libNumBuffer;
    std::cout << "Enter the library number of the comics: ";
    in >> libNumBuffer;
    comics.setLibraryNumber(libNumBuffer);

    unsigned yearBuffer;
    std::cout << "Enter the year the comics was written: ";
    in >> yearBuffer;
    try {
        comics.setYear(yearBuffer);
    } catch (std::invalid_argument& e) {
        delete[] comics.author;
        comics.setAuthor("UNKNOWN");
        delete[] comics.title;
        comics.setTitle("UNKNOWN");
        delete[] comics.publisher;
        comics.setPublisher("UNKnOWN");
        delete[] comics.description;
        comics.setDescription("UNKNOWN");
        std::cerr << e.what() << std::endl;
        return in;
    }

    return in;
}