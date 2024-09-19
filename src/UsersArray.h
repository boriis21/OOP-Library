#ifndef LIBRARYFINAL_USERSARRAY_H
#define LIBRARYFINAL_USERSARRAY_H
#include "User.h"


class UsersArray {
public:
    //CONSTRUCTORS
    UsersArray();
    UsersArray(const UsersArray&);
    UsersArray& operator=(const UsersArray&);
    ~UsersArray();

    //READ AND WRITE
    void writeUsers(const char*);
    void readUsers(const char*);

    //METHODS
    void push(const User&);
    bool removeUser(const char*);
    size_t findUserIndexByName(const char*);

    //GETTERS
    size_t getSize() const;

    //OPERATORS
    const User& operator[](unsigned) const;
    User& operator[](unsigned);

private:
    User* users;
    size_t size;
    size_t capacity;

    //HELPERS
    void resize();
    void copy(const UsersArray&);
    void deallocate();
};


#endif //LIBRARYFINAL_USERSARRAY_H
