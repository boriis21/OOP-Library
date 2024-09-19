#include <fstream>
#include "EditionsArray.h"

//HELPERS
void EditionsArray::resize() {
    capacity *= 2;
    try {
        PrintEditions** newEditions = new PrintEditions*[capacity];
        for (int i = 0; i < size; ++i) {
            newEditions[i] = editions[i];
        }
        delete[] editions;
        editions = newEditions;
    } catch (std::bad_alloc& e) {
        std::cerr << "Unable to resize the array." << std::endl;
    }
}

void EditionsArray::copy(const EditionsArray& other) {
    try {
        editions = new PrintEditions *[other.capacity];
        for (int i = 0; i < other.size; ++i) {
            if (typeid(*other.editions[i]) == typeid(Book)) {
                Book *tempPtr = dynamic_cast<Book *>(other.editions[i]);
                editions[i] = new Book(*tempPtr);
            }
            else if (typeid(*other.editions[i]) == typeid(Magazine)) {
                Magazine *tempPtr = dynamic_cast<Magazine *>(other.editions[i]);
                editions[i] = new Magazine(*tempPtr);
            }
            else if (typeid(*other.editions[i]) == typeid(Comics)) {
                Comics *tempPtr = dynamic_cast<Comics *>(other.editions[i]);
                editions[i] = new Comics(*tempPtr);
            }
        }
        size = other.size;
        capacity = other.capacity;
    } catch (std::bad_alloc& e) {
        std::cerr << "Unable to allocate new memory for the pointers array." << std::endl;
    }
}

void EditionsArray::deallocate() {
    for (int i = 0; i < size; ++i) {
        delete editions[i];
    }
    delete[] editions;
}


//CONSTRUCTORS
EditionsArray::EditionsArray() : size(0), capacity(1) {
    try {
        editions = new PrintEditions*[capacity];
    } catch (std::bad_alloc& e) {
        std::cerr << "Unable to allocate new memory for the pointers array." << std::endl;
    }
}

EditionsArray::EditionsArray(const EditionsArray& other) {
    copy(other);
}

EditionsArray& EditionsArray::operator=(const EditionsArray& other) {
    if (this == &other)
        return *this;
    deallocate();
    copy(other);
    return *this;
}

EditionsArray::~EditionsArray() {
    deallocate();
}


//READ AND WRITE
void EditionsArray::writeEdition(const char* fileName) const {
    std::ofstream file(fileName, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Unable to open the file for writing.");
    }

    writeEdition(file);
    file.close();
}

void EditionsArray::readEdition(const char* fileName) {
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Unable to open the file for reading.");
    }

    readEdition(file);
    file.close();
}

void EditionsArray::writeEdition(std::ostream& file) const {
    size_t size = getSize();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size_t));

    for (int i = 0; i < size; ++i) {
        const char* type = editions[i]->getType();
        size_t typeLength = strlen(type);
        file.write(reinterpret_cast<const char*>(&typeLength), sizeof(size_t));
        file.write(type, typeLength);

        editions[i]->write(file);
    }
}

void EditionsArray::readEdition(std::istream& file) {
    size_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size_t));

    for (int i = 0; i < size; ++i) {
        size_t typeLength;
        file.read(reinterpret_cast<char*>(&typeLength), sizeof(size_t));
        try {
            char* type = new char[typeLength + 1];
            file.read(type, typeLength);
            type[typeLength] = '\0';

            if (strcmp(type, "book") == 0) {
                try {
                    Book book;
                    book.read(file);
                    push(new Book(book));
                } catch (std::runtime_error& e) {
                    std::cerr << e.what() << std::endl;
                }
            } else if (strcmp(type, "magazine") == 0) {
                try {
                    Magazine magazine;
                    magazine.read(file);
                    push(new Magazine(magazine));
                } catch (std::runtime_error& e) {
                    std::cerr << e.what() << std::endl;
                }
            } else if (strcmp(type, "comics") == 0) {
                try {
                    Comics comics;
                    comics.read(file);
                    push(new Comics(comics));
                } catch (std::runtime_error& e) {
                    std::cerr << e.what() << std::endl;
                }
            }
            delete[] type;
        } catch (std::bad_alloc& e) {
            std::cerr << "Unable to allocate memory for the type." << std::endl;
        }
    }
}


//GETTER
size_t EditionsArray::getSize() const { return size; }


//METHODS
size_t EditionsArray::findEditionIndexByNum(unsigned libraryNumber) const {
    for (int i = 0; i < size; ++i) {
        try {
            if (editions[i]->getLibraryNumber() == libraryNumber)
                return i;
        } catch (std::out_of_range& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    return -1;
}

void EditionsArray::push(PrintEditions* edition) {
    if (size >= capacity)
        resize();

    if ((strcmp(edition->getTitle(), "UNKNOWN") == 0) || (strcmp(edition->getDescription(), "UNKNOWN") == 0))
        return;
    else {
        for (int i = 0; i < size; ++i) {
            if (editions[i]->getLibraryNumber() == edition->getLibraryNumber()) {
                throw std::runtime_error("You cannot add a printed edition with already existing library number!");
            }
        }
        editions[size++] = edition;
    }
}

bool EditionsArray::contains(unsigned libraryNumber) const {
    if (findEditionIndexByNum(libraryNumber) != -1)
        return true;
    return false;
}

bool EditionsArray::remove(unsigned libraryNumber) {
    size_t indexToBeRemoved = findEditionIndexByNum(libraryNumber);
    if (indexToBeRemoved != -1) {
        for (int i = indexToBeRemoved; i < size - 1; ++i) {
            editions[i] = editions[i + 1];
        }
        size--;
        return true;
    }

    return false;
}


//OPERATORS
const PrintEditions& EditionsArray::operator[](unsigned index) const {
    if (index > size - 1)
        throw std::out_of_range("Invalid index!");

    return *editions[index];
}

PrintEditions& EditionsArray::operator[](unsigned index) {
    if (index > size - 1)
        throw std::out_of_range("Invalid index!");

    return *editions[index];
}