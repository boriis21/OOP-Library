#ifndef LIBRARYFINAL_MAGAZINE_H
#define LIBRARYFINAL_MAGAZINE_H
#include "PrintEditions.h"


class Magazine : virtual public PrintEditions {
public:
    //CONSTRUCTORS
    Magazine();
    Magazine(const char*, Period, const char*, unsigned, unsigned, unsigned);
    Magazine(Period, unsigned);
    Magazine& operator=(const Magazine&);

    //WRITE AND READ
    void write(std::ostream&) const override;
    void read(std::istream&) override;

    //GETTERS
    const char* getType() const override;
    Period getPeriod() const;
    unsigned getCount() const override;

    //SETTERS
    void setPeriod(const Period);
    void setCount(unsigned) override;

    //OPERATORS
    friend std::ostream& operator<<(std::ostream&, const Magazine&);
    friend std::istream& operator>>(std::istream&, Magazine&);

protected:
    Period period;
    unsigned count;
};


#endif //LIBRARYFINAL_MAGAZINE_H
