#include <QtGlobal>
#include <QVector>
#include <QString>

class IPrintable
{
public:
    virtual void print() const = 0;
};

class Coordinate : IPrintable
{
public:
    quint32 m_x;
    quint32 m_y;

    Coordinate()
        : m_x(0)
        , m_y(0)
    {

    }

    Coordinate(quint32 x, quint32 y)
        : m_x(x)
        , m_y(y)
    {

    }

    virtual ~Coordinate()
    {

    }

    void print() const
    {
        qDebug("(%i, %i", m_x, m_y);
    }
};

class Cell : IPrintable
{
public:
    Cell()
        : m_values()
        , m_members()
    {

    }

    Cell(const QVector<quint32>& values, const QVector<quint32>& members)
        : m_values(values)
        , m_members(members)
    {

    }

    virtual ~Cell()
    {

    }

    const QVector<quint32>& getValues() const { return m_values; }
    const QVector<quint32>& getMembers() const { return m_members; }

    void removeValue(quint32 value)
    {
        m_values.removeOne(value);
    }

    /*!
     * \brief Clear values and store only the specified one.
     * \param valueToKeep The only value to keep.
     */
    void removeAllValuesExcept(quint32 valueToKeep)
    {
        m_values.clear();
        m_values.push_back(valueToKeep);
    }

    void print() const
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

private:
    QVector<quint32> m_values;
    QVector<quint32> m_members;
};

class Grid : IPrintable
{
public:
    Grid()
        : m_width(0)
        , m_height(0)
        , m_cells(0)
    {

    }

    Grid(quint32 width, quint32 height)
        : m_width(width)
        , m_height(height)
        , m_cells(width * height)
    {

    }

    virtual ~Grid()
    {

    }

    const Cell& getCell(quint32 x, quint32 y) const
    {
        quint32 cellIndex = getCellIndex(x, y);
        return m_cells[cellIndex];
    }

    void setCell(quint32 x, quint32 y, const Cell& cell)
    {
        quint32 cellIndex = getCellIndex(x, y);
        m_cells[cellIndex] = cell;
    }

    /*!
     * \brief Apply each rules once on each cell. Stop as soon as a rule changed at least one cell.
     * \return True as soon as a rule has successfully changed at least one cell, false otherwise.
     */
    bool resolve()
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

    /*!
     * \brief Apply rule(s) on the specified cell.
     * \param x Cell's column.
     * \param y Cell's line.
     * \param ruleToApply Allow you to chose the rule to apply (1-4).0 to apply all rules. Handy for debug purpose.
     * \return True as soon as a rule has successfully changed at least one cell, false otherwise.
     */
    bool resolveCell(quint32 x, quint32 y, quint8 ruleToApply=0)
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

    void print() const
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

private:
    quint32 m_width;
    quint32 m_height;
    QVector<Cell> m_cells;

private:
    quint32 getCellIndex(quint32 x, quint32 y) const
    {
        return y * m_width + x;
    }

    Coordinate getCellCoordinate(quint32 cellIndex) const
    {
        return Coordinate(cellIndex % m_width, cellIndex / m_width);
    }

    /*!
     * \brief Neighbors are the surrounding cells that are not members.
     * \param x Cell's column.
     * \param y Cell's line.
     * \return Cell's neighbor indices.
     */
    QVector<quint32> getNeighbors(quint32 x, quint32 y) const
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

    /*!
     * \brief Get members that have the specified value.
     * \param members Members to check.
     * \param value Value that be the same between returned members.
     * \return Members that have the specified value in their values.
     */
    QVector<quint32> getMembersWithSameValue(const QVector<quint32>& members, quint32 value)
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

    /*!
     * \brief Get neighbors that are the same.
     * \param neighbors1 First neighbor vector.
     * \param neighbors2 Second neighbor vector.
     * \return A vector that is the intersection between given neighbor vectors.
     */
    QVector<quint32> getSameNeighbors(const QVector<quint32>& neighbors1, const QVector<quint32>& neighbors2)
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

    /*!
     * \brief Look for a member with only one value and remove this value from the cell.
     * \param x Cell's column.
     * \param y Cell's line.
     * \return True if a value has been removed, false otherwise.
     */
    bool applyRuleCompareWithMembers(quint32 x, quint32 y)
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

    /*!
     * \brief Check if the cell is the only one storing a value among members and remove other values if it's the case.
     * \param x Cell's column.
     * \param y Cell's line.
     * \return True if values have been removed, false otherwise.
     */
    bool applyRuleAloneAmongMembers(quint32 x, quint32 y)
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

    /*!
     * \brief Look for a neighbor with only one value and remove this value from the cell.
     * \param x Cell's column.
     * \param y Cell's line.
     * \return True if a value has been removed, false otherwise.
     */
    bool applyRuleCheckNeighbors(quint32 x, quint32 y)
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

    /*!
     * \brief Remove values of common neighbors of members with same values.
     * \param x Cell's column.
     * \param y Cell's line.
     * \return True if at least a value has been removed, false otherwise.
     */
    bool applyRuleExcludeMemberIntersections(quint32 x, quint32 y)
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
};

int main(int argc, char *argv[])
{
    Grid grid(4, 5);

    // Area 0
    grid.setCell(0, 0, Cell(QVector<quint32>({1, 2}), QVector<quint32>({1})));
    grid.setCell(1, 0, Cell(QVector<quint32>({1, 2}), QVector<quint32>({0})));

    // Area 1
    grid.setCell(2, 0, Cell(QVector<quint32>({1, 2, 3, 4, 5}), QVector<quint32>({4, 5, 6, 10})));
    grid.setCell(0, 1, Cell(QVector<quint32>({1, 2, 3, 4, 5}), QVector<quint32>({2, 5, 6, 10})));
    grid.setCell(1, 1, Cell(QVector<quint32>({4}), QVector<quint32>({2, 4, 6, 10})));
    grid.setCell(2, 1, Cell(QVector<quint32>({1, 2, 3, 4, 5}), QVector<quint32>({2, 4, 5, 10})));
    grid.setCell(2, 2, Cell(QVector<quint32>({1, 2, 3, 4, 5}), QVector<quint32>({2, 4, 5, 6})));

    // Area 2
    grid.setCell(3, 0, Cell(QVector<quint32>({1, 2, 3, 4}), QVector<quint32>({7, 11, 15})));
    grid.setCell(3, 1, Cell(QVector<quint32>({1, 2, 3, 4}), QVector<quint32>({3, 11, 15})));
    grid.setCell(3, 2, Cell(QVector<quint32>({3}), QVector<quint32>({3, 7, 15})));
    grid.setCell(3, 3, Cell(QVector<quint32>({1, 2, 3, 4}), QVector<quint32>({3, 7, 11})));

    // Area 3
    grid.setCell(0, 2, Cell(QVector<quint32>({1, 2, 3}), QVector<quint32>({12, 16})));
    grid.setCell(0, 3, Cell(QVector<quint32>({1, 2, 3}), QVector<quint32>({8, 16})));
    grid.setCell(0, 4, Cell(QVector<quint32>({1, 2, 3}), QVector<quint32>({8, 12})));

    // Area 4
    grid.setCell(1, 2, Cell(QVector<quint32>({1, 2, 3, 4, 5}), QVector<quint32>({13, 14, 17, 18})));
    grid.setCell(1, 3, Cell(QVector<quint32>({1, 2, 3, 4, 5}), QVector<quint32>({9, 14, 17, 18})));
    grid.setCell(2, 3, Cell(QVector<quint32>({5}), QVector<quint32>({9, 13, 17, 18})));
    grid.setCell(1, 4, Cell(QVector<quint32>({1, 2, 3, 4, 5}), QVector<quint32>({9, 13, 14, 18})));
    grid.setCell(2, 4, Cell(QVector<quint32>({1, 2, 3, 4, 5}), QVector<quint32>({9, 13, 14, 17})));

    // Area 5
    grid.setCell(3, 4, Cell(QVector<quint32>({1}), QVector<quint32>({})));

    grid.print();

    quint32 maxLoopCount = 10000;
    quint32 loopCount = 0;
    while (loopCount < maxLoopCount && grid.resolve() == true)
    {
        // Keep resolving...
        ++loopCount;
    }
    qDebug("Loop count: %i", loopCount);

    grid.print();

    return 0;
}
