#ifndef CELL_H
#define CELL_H

#include "iprintable.h"

#include <QtGlobal>
#include <QVector>

class Cell : IPrintable
{
public:
    Cell();
    Cell(const QVector<quint32>& values, const QVector<quint32>& members);

    virtual ~Cell();

    const QVector<quint32>& getValues() const;
    const QVector<quint32>& getMembers() const;

    void setMembers(int members[], int memberCount);

    void resetValues();
    void removeValue(quint32 value);

    /*!
     * \brief Clear values and store only the specified one.
     * \param valueToKeep The only value to keep.
     */
    void removeAllValuesExcept(quint32 valueToKeep);

    void print() const;

private:
    QVector<quint32> m_values;
    QVector<quint32> m_members;
};

#endif // CELL_H
