#include "grid.h"

Grid::Grid()
    : m_width(0)
    , m_height(0)
    , m_cells(0)
{

}

Grid::Grid(quint32 width, quint32 height)
    : m_width(width)
    , m_height(height)
    , m_cells(width * height)
{

}

Grid::~Grid()
{

}

const Cell& Grid::getCell(quint32 x, quint32 y) const
{
    quint32 cellIndex = getCellIndex(x, y);
    return m_cells[cellIndex];
}

void Grid::setCell(quint32 x, quint32 y, const Cell& cell)
{
    quint32 cellIndex = getCellIndex(x, y);
    m_cells[cellIndex] = cell;
}

bool Grid::resolve()
{
    for (quint32 y=0; y<m_height; ++y)
    {
        for (quint32 x=0; x<m_width; ++x)
        {
            if (resolveCell(x, y) == true)
            {
                return true;
            }
        }
    }

    return false;
}

bool Grid::resolveCell(quint32 x, quint32 y, quint8 ruleToApply)
{
    if ((ruleToApply == 0 || ruleToApply == 1) && applyRuleCompareWithMembers(x, y) == true)
    {
        return true;
    }

    if ((ruleToApply == 0 || ruleToApply == 2) && applyRuleAloneAmongMembers(x, y) == true)
    {
        return true;
    }

    if ((ruleToApply == 0 || ruleToApply == 3) && applyRuleCheckNeighbors(x, y) == true)
    {
        return true;
    }

    if ((ruleToApply == 0 || ruleToApply == 4) && applyRuleExcludeMemberIntersections(x, y) == true)
    {
        return true;
    }

    return false;
}

void Grid::print() const
{
    QString output = "";
    for (quint32 y=0; y<m_height; ++y)
    {
        output += "";
        for (quint32 x=0; x<m_width; ++x)
        {
            quint32 index = getCellIndex(x, y);
            const Cell& cell = m_cells[index];
            output += cell.getValues().count() == 1 ? QString::number(cell.getValues().first()) : " ";
        }
        output += "\n";
    }

    qDebug("%s", output.toStdString().c_str());
}

quint32 Grid::getCellIndex(quint32 x, quint32 y) const
{
    return y * m_width + x;
}

Coordinate Grid::getCellCoordinate(quint32 cellIndex) const
{
    return Coordinate(cellIndex % m_width, cellIndex / m_width);
}

QVector<quint32> Grid::getNeighbors(quint32 x, quint32 y) const
{
    QVector<quint32> neighbors;
    const Cell& cell = getCell(x, y);
    const QVector<quint32>& members = cell.getMembers();

    for (qint32 j=-1; j<2; ++j)
    {
        quint32 line = y + j;
        if (line >= m_height)
        {
            continue;
        }

        for (qint32 i=-1; i<2; ++i)
        {
            if (j == 0 && i == 0)
            {
                continue;
            }

            quint32 column = x + i;
            if (column >= m_width)
            {
                continue;
            }

            quint32 neighborIndex = getCellIndex(column, line);
            if (members.indexOf(neighborIndex) > -1)
            {
                continue;
            }

            neighbors.push_back(neighborIndex);
        }
    }

    return neighbors;
}

QVector<quint32> Grid::getMembersWithSameValue(const QVector<quint32>& members, quint32 value) const
{
    QVector<quint32> membersWithValue;

    foreach (auto memberIndex, members)
    {
        const Cell& member = m_cells[memberIndex];
        if (member.getValues().indexOf(value) > -1)
        {
            membersWithValue.push_back(memberIndex);
        }
    }

    return membersWithValue;
}

QVector<quint32> Grid::getSameNeighbors(const QVector<quint32>& neighbors1, const QVector<quint32>& neighbors2) const
{
    QVector<quint32> sameNeighbors;

    foreach (auto neighbor1, neighbors1)
    {
        if (neighbors2.indexOf(neighbor1) > -1)
        {
            sameNeighbors.push_back(neighbor1);
        }
    }

    return sameNeighbors;
}

bool Grid::applyRuleCompareWithMembers(quint32 x, quint32 y)
{
    quint32 cellIndex = getCellIndex(x, y);
    Cell& cell = m_cells[cellIndex];

    const QVector<quint32>& values = cell.getValues();
    if (values.count() == 1)
    {
        return false;
    }

    const QVector<quint32>& members = cell.getMembers();
    if (members.isEmpty() == true)
    {
        return false;
    }

    foreach (auto memberIndex, members)
    {
        const Cell& member = m_cells[memberIndex];

        if (member.getValues().count() > 1)
        {
            continue;
        }

        quint32 uniqueValue = member.getValues().first();
        bool hasValue = values.indexOf(uniqueValue) > -1;
        if (hasValue)
        {
            cell.removeValue(uniqueValue);
            return true;
        }
    }

    return false;
}

bool Grid::applyRuleAloneAmongMembers(quint32 x, quint32 y)
{
    quint32 cellIndex = getCellIndex(x, y);
    Cell& cell = m_cells[cellIndex];

    const QVector<quint32> values = cell.getValues();
    if (values.count() == 1)
    {
        return false;
    }

    const QVector<quint32>& members = cell.getMembers();
    if (members.isEmpty() == true)
    {
        return false;
    }

    foreach (auto value, values) {
        bool uniqueValue = true;
        foreach (auto memberIndex, members)
        {
            const Cell& member = m_cells[memberIndex];

            bool hasValue = member.getValues().indexOf(value) > -1;
            if (hasValue)
            {
                uniqueValue = false;
                break;
            }
        }

        if (uniqueValue == true)
        {
            cell.removeAllValuesExcept(value);
            return true;
        }
    }

    return false;
}

bool Grid::applyRuleCheckNeighbors(quint32 x, quint32 y)
{
    quint32 cellIndex = getCellIndex(x, y);
    Cell& cell = m_cells[cellIndex];

    const QVector<quint32> values = cell.getValues();
    if (values.count() == 1)
    {
        return false;
    }

    const QVector<quint32> neighbors = getNeighbors(x, y);

    foreach (auto neighborIndex, neighbors)
    {
        const Cell& member = m_cells[neighborIndex];
        const QVector<quint32>& memberValues = member.getValues();
        if (memberValues.count() > 1)
        {
            continue;
        }

        quint32 remainingValue = memberValues.first();
        bool hasValue = values.indexOf(remainingValue) > -1;
        if (hasValue)
        {
            cell.removeValue(remainingValue);
            return true;
        }
    }

    return false;
}

bool Grid::applyRuleExcludeMemberIntersections(quint32 x, quint32 y)
{
    quint32 cellIndex = getCellIndex(x, y);
    Cell& cell = m_cells[cellIndex];

    const QVector<quint32> values = cell.getValues();
    if (values.count() == 1)
    {
        return false;
    }

    foreach (auto value, values)
    {
        // Get all common neighbors of members with same value.
        QVector<quint32> membersWithSameValue = getMembersWithSameValue(cell.getMembers(), value);
        QVector<quint32> commonNeighbors = getNeighbors(x, y);
        foreach (auto memberIndex, membersWithSameValue)
        {
            Coordinate coord = getCellCoordinate(memberIndex);
            const QVector<quint32> memberNeighbors = getNeighbors(coord.m_x, coord.m_y);
            commonNeighbors = getSameNeighbors(commonNeighbors, memberNeighbors);
        }

        // If at least one neighbor is found
        if (commonNeighbors.empty() == false)
        {
            bool changed = false;
            // Remove the value from these neighbors.
            foreach (auto neighborIndex, commonNeighbors)
            {
                Cell& neighbor = m_cells[neighborIndex];
                if (neighbor.getValues().indexOf(value) > -1)
                {
                    neighbor.removeValue(value);
                    changed = true;
                }
            }

            // Stop the process with success when the first set of changes is done.
            if (changed == true)
            {
                return true;
            }
        }
    }

    return false;
}
