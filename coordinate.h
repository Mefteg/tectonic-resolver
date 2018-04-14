#ifndef COORDINATE_H
#define COORDINATE_H

#include "iprintable.h"

#include <QtGlobal>

class Coordinate : IPrintable
{
public:
    quint32 m_x;
    quint32 m_y;

    Coordinate();
    Coordinate(quint32 x, quint32 y);

    virtual ~Coordinate();

    void print() const;
};

#endif // COORDINATE_H
