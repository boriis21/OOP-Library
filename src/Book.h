#ifndef LIBRARYFINAL_BOOK_H
#define LIBRARYFINAL_BOOK_H

#include <cstdint>
#include "PrintEditions.h"

enum Characteristic {
    ROMANCE = 121,
    THRILLER,
    MYSTERY,
    FANTASY,
    ADVENTURE,
    HISTORICAL,
    HUMOR
};

enum Type {
    POETRY = 9,
    PROSE,
    DRAMA,
    ESSAY,
    BIOGRAPHY,
    PHILOSOPHY,
    HISTORY
};

enum Audience {
    CHILDREN = 12,
    TEENAGERS,
    ADOLESCENTS,
    ADULTS
};

uint16_t generateGenre();


class Book : virtual public PrintEditions {
public:
    //CONSTRUCTORS
    Book();
    Book(const char*, const char*, const char*, const char*, unsigned, unsigned);
    Book(const Book&);
    Book& operator=(const Book&);
    ~Book() override;

    //WRITE AND READ
    void write(std::ostream&) const override;
    void read(std::istream&) override;

    //GETTERS
    const char* getType() const override;
    const char* getAuthor() const;
    const char* getPublisher() const;
    unsigned short getGenre() const;
    unsigned getCount() const override;

    //SETTERS
    void setAuthor(const char*);
    void setPublisher(const char*);
    void setCount(unsigned) override;

    //OPERATORS
    friend std::ostream& operator<<(std::ostream&, const Book&);
    friend std::istream& operator>>(std::istream&, Book&);

protected:
    char* author;
    char* publisher;
    uint16_t genre;

    //HELPERS
    void copy(const Book&);
    void deallocate() override;
};


#endif //LIBRARYFINAL_BOOK_H
