#include "coordinate.h"

Coordinate::Coordinate()
    : m_x(0)
    , m_y(0)
{

}

Coordinate::Coordinate(quint32 x, quint32 y)
    : m_x(x)
    , m_y(y)
{

}

Coordinate::~Coordinate()
{

}

void Coordinate::print() const
{
    qDebug("(%i, %i", m_x, m_y);
}
