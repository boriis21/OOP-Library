#ifndef LIBRARYFINAL_EDITIONSARRAY_H
#define LIBRARYFINAL_EDITIONSARRAY_H
#include "Comics.h"


class EditionsArray {
public:
    //CONSTRUCTORS
    EditionsArray();
    EditionsArray(const EditionsArray&);
    EditionsArray& operator=(const EditionsArray&);
    ~EditionsArray();

    //READ AND WRITE
    void writeEdition(const char*) const;
    void readEdition(const char*);
    void writeEdition(std::ostream&) const;
    void readEdition(std::istream&);

    //GETTER
    size_t getSize() const;

    //METHODS
    size_t findEditionIndexByNum(unsigned) const;
    void push(PrintEditions*);
    bool contains(unsigned) const;
    bool remove(unsigned);

    //OPERATORS
    const PrintEditions& operator[](unsigned) const;
    PrintEditions& operator[](unsigned);

private:
    PrintEditions** editions;
    size_t capacity;
    size_t size;

    //HELPERS
    void resize();
    void copy(const EditionsArray&);
    void deallocate();
};


#endif //LIBRARYFINAL_EDITIONSARRAY_H
