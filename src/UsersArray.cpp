#include <fstream>
#include "UsersArray.h"

//HELPERS
void UsersArray::resize() {
    capacity *= 2;
    try {
        User* newUsers = new User[capacity];
        for (int i = 0; i < size; ++i) {
            newUsers[i] = users[i];
        }
        deallocate();
        users = newUsers;
    } catch (std::bad_alloc& e) {
        std::cerr << "Unable to resize the users array." << std::endl;
    }
}

void UsersArray::copy(const UsersArray& other) {
    try {
        users = new User[other.capacity];
        for (int i = 0; i < other.size; ++i) {
            users[i] = other.users[i];
        }

        size = other.size;
        capacity = other.capacity;
    } catch (std::bad_alloc& e) {
        std::cerr << "Unable to allocate memory for the users array." << std::endl;
    }
}

void UsersArray::deallocate() {
    delete[] users;
}


//CONSTRUCTORS
UsersArray::UsersArray() : size(0), capacity(1) {
    try {
        users = new User[capacity];
    } catch (std::bad_alloc& e) {
        std::cerr << "Unable to allocate memory for the users array." << std::endl;
    }
}

UsersArray::UsersArray(const UsersArray& other) {
    copy(other);
}

UsersArray& UsersArray::operator=(const UsersArray& other) {
    if (this == &other)
        return *this;
    deallocate();
    copy(other);
    return *this;
}

UsersArray::~UsersArray() {
    deallocate();
}


//READ AND WRITE
void UsersArray::writeUsers(const char* fileName) {
    std::ofstream file(fileName, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Unable to open the file for writing.");
    }

    file.write(reinterpret_cast<const char*>(&size), sizeof(size_t));

    for (size_t i = 0; i < size; ++i) {
        users[i].write(file);
    }

    file.close();
}

void UsersArray::readUsers(const char* fileName) {
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Unable to open the file for reading.");
    }

    size_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size_t));

    for (int i = 0; i < size; ++i) {
        User user;
        user.read(file);
        push(user);
    }

    file.close();
}


//METHODS
void UsersArray::push(const User& user) {
    if (size >= capacity)
        resize();

    if (strcmp(user.getName(), "UNKNOWN") == 0)
        return;
    else
        users[size++] = user;
}

bool UsersArray::removeUser(const char* name) {
    size_t toRemoveIndex = findUserIndexByName(name);
    if (toRemoveIndex != -1) {
        for (int i = toRemoveIndex; i < size - 1; ++i) {
            users[i] = users[i + 1];
        }
        size--;
        return true;
    }
    return false;
}

size_t UsersArray::findUserIndexByName(const char* name) {
    for (int i = 0; i < size; ++i) {
        if (strcmp(users[i].getName(), name) == 0)
            return i;
    }
    return -1;
}


//GETTERS
size_t UsersArray::getSize() const { return size; }


//OPERATORS
const User& UsersArray::operator[](unsigned index) const {
    if (index > size - 1)
        throw std::out_of_range("Invalid index!");
    return users[index];
}

User& UsersArray::operator[](unsigned index) {
    if (index > size - 1)
        throw std::out_of_range("Invalid index!");
    return users[index];
}