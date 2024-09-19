#ifndef LIBRARYFINAL_PRINTEDITIONS_H
#define LIBRARYFINAL_PRINTEDITIONS_H
#include <iostream>
#include <cstring>

bool isStringValid(const char*);

enum Period {
    WEEKLY,
    MONTHLY,
    YEARLY,
    UNKNOWN
};

class PrintEditions {
public:
    //CONSTRUCTORS
    PrintEditions();
    PrintEditions(const char*, const char*, unsigned, unsigned);
    PrintEditions(const PrintEditions&);
    PrintEditions& operator=(const PrintEditions&);
    virtual ~PrintEditions();

    //READ AND WRITE
    virtual void write(std::ostream&) const = 0;
    virtual void read(std::istream&) = 0;

    //GETTERS
    virtual const char* getType() const = 0;
    const char* getTitle() const;
    const char* getDescription() const;
    virtual unsigned getCount() const = 0;
    unsigned getLibraryNumber() const;
    unsigned getYear() const;

    //SETTERS
    void setTitle(const char*);
    void setDescription(const char*);
    virtual void setCount(unsigned) = 0;
    void setLibraryNumber(unsigned);
    void setYear(unsigned);

    //OPERATORS
    friend bool operator<(const PrintEditions&, const PrintEditions&);
    friend bool operator<=(const PrintEditions&, const PrintEditions&);
    friend bool operator>(const PrintEditions&, const PrintEditions&);
    friend bool operator>=(const PrintEditions&, const PrintEditions&);

protected:
    char* title;
    char* description;
    unsigned libraryNumber;
    unsigned year;

    //HELPERS
    void copy(const PrintEditions&);
    virtual void deallocate();
};


#endif //LIBRARYFINAL_PRINTEDITIONS_H
