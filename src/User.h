#ifndef LIBRARYFINAL_USER_H
#define LIBRARYFINAL_USER_H
#include "EditionsArray.h"


class User {
public:
    //CONSTRUCTORS
    User();
    User(const char*);
    User(const User&);
    User& operator=(const User&);
    ~User();

    //READ AND WRITE
    void write(std::ostream&) const;
    void read(std::istream&);

    //GETTERS
    const char* getName() const;
    const EditionsArray& getTakenEditions() const;
    const EditionsArray& getReadEditions() const;
    size_t getTakenSize() const;
    size_t getReadSize() const;

    //SETTERS
    void setName(const char*);

    //METHODS
    void removeFromTaken(const PrintEditions&);
    void removeFromRead(const PrintEditions&);

    //OPERATORS
    friend std::ostream& operator<<(std::ostream&, const User&);
    friend std::istream& operator>>(std::istream&, User&);
    friend bool operator==(const User&, const User&);
    friend bool operator!=(const User&, const User&);
    friend bool operator<(const User&, const User&);
    friend bool operator<=(const User&, const User&);
    friend bool operator>(const User&, const User&);
    friend bool operator>=(const User&, const User&);
    User& operator+=(const PrintEditions&);
    User& operator-=(const PrintEditions&);
    friend User operator+(User, const PrintEditions&);
    friend User operator-(User, const PrintEditions&);
    const char* operator[](unsigned) const;

private:
    char* name;
    EditionsArray takenEditions;
    EditionsArray readEditions;

    //HELPERS
    void copy(const User&);
    void deallocate();
};


#endif //LIBRARYFINAL_USER_H
