#ifndef GRID_H
#define GRID_H

#include "iprintable.h"

#include <QVector>

#include "coordinate.h"
#include "cell.h"

class Grid : IPrintable
{
public:
    Grid();
    Grid(quint32 width, quint32 height);

    virtual ~Grid();

    const Cell& getCell(quint32 x, quint32 y) const;

    void setCell(quint32 x, quint32 y, const Cell& cell);

    /*!
     * \brief Apply each rules once on each cell. Stop as soon as a rule changed at least one cell.
     * \return True as soon as a rule has successfully changed at least one cell, false otherwise.
     */
    bool resolve();

    /*!
     * \brief Apply rule(s) on the specified cell.
     * \param x Cell's column.
     * \param y Cell's line.
     * \param ruleToApply Allow you to chose the rule to apply (1-4).0 to apply all rules. Handy for debug purpose.
     * \return True as soon as a rule has successfully changed at least one cell, false otherwise.
     */
    bool resolveCell(quint32 x, quint32 y, quint8 ruleToApply=0);

    void print() const;

private:
    quint32 m_width;
    quint32 m_height;
    QVector<Cell> m_cells;

    quint32 getCellIndex(quint32 x, quint32 y) const;
    Coordinate getCellCoordinate(quint32 cellIndex) const;

    /*!
     * \brief Neighbors are the surrounding cells that are not members.
     * \param x Cell's column.
     * \param y Cell's line.
     * \return Cell's neighbor indices.
     */
    QVector<quint32> getNeighbors(quint32 x, quint32 y) const;

    /*!
     * \brief Get members that have the specified value.
     * \param members Members to check.
     * \param value Value that be the same between returned members.
     * \return Members that have the specified value in their values.
     */
    QVector<quint32> getMembersWithSameValue(const QVector<quint32>& members, quint32 value) const;

    /*!
     * \brief Get neighbors that are the same.
     * \param neighbors1 First neighbor vector.
     * \param neighbors2 Second neighbor vector.
     * \return A vector that is the intersection between given neighbor vectors.
     */
    QVector<quint32> getSameNeighbors(const QVector<quint32>& neighbors1, const QVector<quint32>& neighbors2) const;

    /*!
     * \brief Look for a member with only one value and remove this value from the cell.
     * \param x Cell's column.
     * \param y Cell's line.
     * \return True if a value has been removed, false otherwise.
     */
    bool applyRuleCompareWithMembers(quint32 x, quint32 y);

    /*!
     * \brief Check if the cell is the only one storing a value among members and remove other values if it's the case.
     * \param x Cell's column.
     * \param y Cell's line.
     * \return True if values have been removed, false otherwise.
     */
    bool applyRuleAloneAmongMembers(quint32 x, quint32 y);

    /*!
     * \brief Look for a neighbor with only one value and remove this value from the cell.
     * \param x Cell's column.
     * \param y Cell's line.
     * \return True if a value has been removed, false otherwise.
     */
    bool applyRuleCheckNeighbors(quint32 x, quint32 y);

    /*!
     * \brief Remove values of common neighbors of members with same values.
     * \param x Cell's column.
     * \param y Cell's line.
     * \return True if at least a value has been removed, false otherwise.
     */
    bool applyRuleExcludeMemberIntersections(quint32 x, quint32 y);
};

#endif // GRID_H
