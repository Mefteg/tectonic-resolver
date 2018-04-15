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

void Cell::setMembers(int members[], int memberCount)
{
    m_members.clear();
    m_members.reserve(memberCount);
    for (int i=0; i<memberCount; ++i)
    {
        m_members.append(members[i]);
    }
}

void Cell::resetValues()
{
    m_values.clear();
    m_values.reserve(m_members.size() + 1);
    for (int i=0; i<m_members.size() + 1; ++i)
    {
        m_values.append(i + 1);
    }
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
