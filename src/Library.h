#ifndef LIBRARYFINAL_LIBRARY_H
#define LIBRARYFINAL_LIBRARY_H
#include "UsersArray.h"


class Library {
public:
    //CONSTRUCTORS
    Library(const Library&) = delete;
    Library& operator=(const Library&) = delete;
    static Library& getInstance();

    //RUN
    void run();

private:
    Library() {};

    //COMMANDS
    void addEdition();
    void removeEdition();
    void takeEdition();
    void returnEdition();
    void printPublications();
    void addUser();
    void removeUser();
    void printUsersWhoTook();
    void printAllUsers();
    void help();

    //HELPER
    bool compareEditions(const PrintEditions*, const PrintEditions*);

    EditionsArray editions;
    UsersArray users;
};


#endif //LIBRARYFINAL_LIBRARY_H
