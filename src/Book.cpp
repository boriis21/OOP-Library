#include <iostream>
#include <cstring>
#include "Book.h"

uint16_t generateGenre() {
    char characteristicBuffer[20];
    uint8_t characteristic;
    std::cout << "Enter characteristic (romance, thriller, mystery, fantasy, adventure, historical, humor): ";
    std::cin >> characteristicBuffer;
    if (strcmp(characteristicBuffer, "romance") == 0) {
        characteristic = ROMANCE;
    } else if (strcmp(characteristicBuffer, "thriller") == 0) {
        characteristic = THRILLER;
    } else if (strcmp(characteristicBuffer, "mystery") == 0) {
        characteristic = MYSTERY;
    } else if (strcmp(characteristicBuffer, "fantasy") == 0) {
        characteristic = FANTASY;
    } else if (strcmp(characteristicBuffer, "adventure") == 0) {
        characteristic = ADVENTURE;
    } else if (strcmp(characteristicBuffer, "historical") == 0) {
        characteristic = HISTORICAL;
    } else if (strcmp(characteristicBuffer, "humor") == 0) {
        characteristic = HUMOR;
    } else {
        characteristic = 0;
    }

    char typeBuffer[20];
    uint8_t type;
    std::cout << "Enter type (poetry, prose, drama, essay, biography, philosophy, history): ";
    std::cin >> typeBuffer;
    if (strcmp(typeBuffer, "poetry") == 0) {
        type = POETRY;
    } else if (strcmp(typeBuffer, "prose") == 0) {
        type = PROSE;
    } else if (strcmp(typeBuffer, "drama") == 0) {
        type = DRAMA;
    } else if (strcmp(typeBuffer, "essay") == 0) {
        type = ESSAY;
    } else if (strcmp(typeBuffer, "biography") == 0) {
        type = BIOGRAPHY;
    } else if (strcmp(typeBuffer, "philosophy") == 0) {
        type = PHILOSOPHY;
    } else if (strcmp(typeBuffer, "history") == 0) {
        type = HISTORY;
    } else {
        type = 0;
    }

    char targetBuffer[20];
    uint8_t targetAudience;
    std::cout << "Enter target audience (children, teenagers, adolescents, adults): ";
    std::cin >> targetBuffer;
    if (strcmp(targetBuffer, "children") == 0) {
        targetAudience = CHILDREN;
    } else if (strcmp(targetBuffer, "teenagers") == 0) {
        targetAudience = TEENAGERS;
    } else if (strcmp(targetBuffer, "adolescents") == 0) {
        targetAudience = ADOLESCENTS;
    } else if (strcmp(targetBuffer, "adults") == 0) {
        targetAudience = ADULTS;
    } else {
        targetAudience = 0;
    }

    uint16_t genre = (characteristic & 0x7F) | ((type & 0xF) << 7) | ((targetAudience & 0xF) << 11);

    int count = 0;
    uint16_t temp = genre;
    while (temp) {
        count += temp & 1;
        temp >>= 1;
    }

    if (count % 2 != 0) {
        genre |= (1 << 15);
    }

    return genre;
}


//HELPERS
void Book::copy(const Book& other) {
    try {
        author = new char[strlen(other.author) + 1];
        strcpy(author, other.author);
    } catch (std::bad_alloc& e) {
        std::cerr << "Unable to allocate memory for the author.";
    }

    try {
        publisher = new char[strlen(other.publisher) + 1];
        strcpy(publisher, other.publisher);
    } catch (std::bad_alloc& e) {
        delete[] author;
        std::cerr << "Unable to allocate memory for the publisher.";
    }

    genre = other.genre;
}

void Book::deallocate() {
    if (strcmp(author, "UNKNOWN") != 0)
        delete[] author;
    if (strcmp(publisher, "UNKNOWN") != 0)
        delete[] publisher;
}


//CONSTRUCTORS
Book::Book() : PrintEditions(), author("UNKNOWN"), publisher("UNKNOWN"), genre(0) {}

Book::Book(const char* nAuthor, const char* nTitle, const char* nPublisher, const char* nDescription, unsigned nLibraryNumber, unsigned nYear) : PrintEditions(nTitle, nDescription, nLibraryNumber, nYear), author("UNKNOWN"), publisher("UNKNOWN"), genre(generateGenre()) {
    if (!isStringValid(nAuthor))
        throw std::invalid_argument("The author cannot be empty!");

    if (!isStringValid(nPublisher))
        throw std::invalid_argument("The publisher cannot be empty!");

    try {
        author = new char[strlen(nAuthor) + 1];
        strcpy(author, nAuthor);
    } catch (std::bad_alloc& e) {
        std::cerr << "Unable to allocate memory for the author.";
    }

    try {
        publisher = new char[strlen(nPublisher) + 1];
        strcpy(publisher, nPublisher);
    } catch (std::bad_alloc& e) {
        delete[] author;
        std::cerr << "Unable to allocate memory for the publisher.";
    }
}
Book::Book(const Book& other) : PrintEditions(other) {
    copy(other);
}

Book& Book::operator=(const Book& other) {
    if (this == &other)
        return *this;
    PrintEditions::operator=(other);
    deallocate();
    copy(other);
    return *this;
}

Book::~Book() {
    deallocate();
}


//WRITE AND READ
void Book::write(std::ostream& file) const {
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

    file.write(reinterpret_cast<const char*>(&libraryNumber), sizeof(unsigned));

    file.write(reinterpret_cast<const char*>(&year), sizeof(unsigned));
}

void Book::read(std::istream& file) {
    size_t authorLength;
    file.read(reinterpret_cast<char*>(&authorLength), sizeof(size_t));

    try {
        char* authorBuffer = new char[authorLength + 1];
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
        char* titleBuffer = new char[titleLength + 1];
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
        char* publisherBuffer = new char[publisherLength + 1];
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
        char* descriptionBuffer = new char[descriptionLength + 1];
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

    file.read(reinterpret_cast<char*>(&libraryNumber), sizeof(unsigned));

    file.read(reinterpret_cast<char*>(&year), sizeof(unsigned));
}


//GETTERS
const char* Book::getType() const { return "book"; }
const char* Book::getAuthor() const { return author; }
const char* Book::getPublisher() const { return publisher; }
unsigned short Book::getGenre() const { return genre; }
unsigned Book::getCount() const {}


//SETTERS
void Book::setAuthor(const char* nAuthor) {
    if (!isStringValid(nAuthor))
        throw std::invalid_argument("The author cannot be empty!");

    try {
        author = new char[strlen(nAuthor) + 1];
        strcpy(author, nAuthor);
    } catch (std::bad_alloc& e) {
        std::cerr << "Unable to allocate memory for the author.";
    }
}

void Book::setPublisher(const char* nPublisher) {
    if (!isStringValid(nPublisher))
        throw std::invalid_argument("The publisher cannot be empty!");

    try {
        publisher = new char[strlen(nPublisher) + 1];
        strcpy(publisher, nPublisher);
    } catch (std::bad_alloc& e) {
        std::cerr << "Unable to allocate memory for the publisher.";
    }
}

void Book::setCount(unsigned) {}


//OPERATORSs
std::ostream& operator<<(std::ostream& out, const Book& book) {
    out << "Author: " << book.author << std::endl;
    out << "Title: " << book.title << std::endl;
    out << "Publisher: " << book.publisher << std::endl;
    out << "Genre: " << book.genre << std::endl;
    out << "Description: " << book.description << std::endl;
    out << "Library Number: " << book.libraryNumber << std::endl;
    out << "Year: " << book.year << std::endl;
    return out;
}

std::istream& operator>>(std::istream& in, Book& book) {
    char buffer[256];

    std::cout << "Enter the name of the author: ";
    in.ignore(sizeof(buffer), '\n');
    in.getline(buffer, sizeof(buffer));
    try {
        book.setAuthor(buffer);
    } catch (std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
        return in;
    }

    std::cout << "Enter the title of the book: ";
    in.getline(buffer, sizeof(buffer));
    try {
        book.setTitle(buffer);
    } catch (std::invalid_argument& e) {
        delete[] book.author;
        book.setAuthor("UNKNOWN");
        std::cerr << e.what() << std::endl;
        return in;
    }

    std::cout << "Enter the name of the publisher: ";
    in.getline(buffer, sizeof(buffer));
    try {
        book.setPublisher(buffer);
    } catch (std::invalid_argument& e) {
        delete[] book.author;
        book.setAuthor("UNKNOWN");
        delete[] book.title;
        book.setTitle("UNKNOWN");
        std::cerr << e.what() << std::endl;
        return in;
    }

    book.genre = generateGenre();

    std::cout << "What is the description of the book: ";
    in.ignore(sizeof(buffer), '\n');
    in.getline(buffer, sizeof(buffer));
    try {
        book.setDescription(buffer);
    } catch (std::invalid_argument& e) {
        delete[] book.author;
        book.setAuthor("UNKNOWN");
        delete[] book.title;
        book.setTitle("UNKNOWN");
        delete[] book.publisher;
        book.setPublisher("UNKnOWN");
        std::cerr << e.what() << std::endl;
        return in;
    }

    unsigned libNumBuffer;
    std::cout << "Enter the library number of the book: ";
    in >> libNumBuffer;
    book.setLibraryNumber(libNumBuffer);

    unsigned yearBuffer;
    std::cout << "Enter the year the book was written: ";
    in >> yearBuffer;
    try {
        book.setYear(yearBuffer);
    } catch (std::invalid_argument& e) {
        delete[] book.author;
        book.setAuthor("UNKNOWN");
        delete[] book.title;
        book.setTitle("UNKNOWN");
        delete[] book.publisher;
        book.setPublisher("UNKnOWN");
        delete[] book.description;
        book.setDescription("UNKNOWN");
        std::cerr << e.what() << std::endl;
        return in;
    }

    return in;
}