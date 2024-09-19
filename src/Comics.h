#ifndef LIBRARYFINAL_COMICS_H
#define LIBRARYFINAL_COMICS_H
#include "Magazine.h"
#include "Book.h"


class Comics : public Book, public Magazine {
public:
    //CONSTRUCTORS
    Comics();
    Comics(const char*, const char*, const char*, const char*, Period, unsigned, unsigned, unsigned);
    Comics& operator=(const Comics&);

    //WRITE AND READ
    void write(std::ostream&) const override;
    void read(std::istream&) override;

    //GETTERS
    const char* getType() const override;
    unsigned getCount() const override;
    void setCount(unsigned) override;

    //OPERATORS
    friend std::ostream& operator<<(std::ostream&, const Comics&);
    friend std::istream& operator>>(std::istream&, Comics&);
};


#endif //LIBRARYFINAL_COMICS_H
