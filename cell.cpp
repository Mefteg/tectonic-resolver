#include "cell.h"

Cell::Cell()
    : m_values()
    , m_members()
{

}

Cell::Cell(const QVector<quint32>& values, const QVector<quint32>& members)
    : m_values(values)
    , m_members(members)
{

}

Cell::~Cell()
{

}

const QVector<quint32>& Cell::getValues() const
{
    return m_values;
}

const QVector<quint32>& Cell::getMembers() const
{
    return m_members;
}

void Cell::removeValue(quint32 value)
{
    m_values.removeOne(value);
}

void Cell::removeAllValuesExcept(quint32 valueToKeep)
{
    m_values.clear();
    m_values.push_back(valueToKeep);
}

void Cell::print() const
{
    QString output = "values: { ";
    foreach (auto value, m_values) {
        output += QString::number(value) + " ";
    }
    output += "} | members: { ";
    foreach (auto value, m_members) {
        output += QString::number(value) + " ";
    }
    output += "}";

    qDebug("%s", output.toStdString().c_str());
}
