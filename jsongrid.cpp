#include "jsongrid.h"

#include <QJsonObject>
#include <QJsonArray>

JsonGrid::JsonGrid()
    : Grid()
{

}

JsonGrid::JsonGrid(quint32 width, quint32 height)
    : Grid(width, height)
{

}

QJsonObject JsonGrid::encode() const
{
    return QJsonObject();
}

void JsonGrid::decode(const QJsonObject& json)
{
    m_width = json["width"].toInt();
    m_height = json["height"].toInt();

    m_cells.resize(m_width * m_height);

    QJsonArray areas = json["areas"].toArray();
    decodeAreas(areas);

    resetCellsValues();

    QJsonArray cells = json["cells"].toArray();
    decodeCells(cells);
}

void JsonGrid::decodeAreas(const QJsonArray& areas)
{
    foreach (const QJsonValue& areaValue, areas)
    {
        QJsonArray area = areaValue.toArray();
        decodeArea(area);
    }
}

void JsonGrid::decodeArea(const QJsonArray& area)
{
    const int memberCount = area.size();
    int members[memberCount];
    for (int i=0; i<memberCount; ++i)
    {
        members[i] = area[i].toInt();
    }

    for (int i=0; i<memberCount; ++i)
    {
        decodeCellMembers(members[i], members, memberCount);
    }
}

void JsonGrid::decodeCellMembers(int cellIndex, const int areaMembers[], int memberCount)
{
    int cellMembers[memberCount - 1];
    int memberIndex = 0;
    for (int i=0; i<memberCount; ++i)
    {
        if (cellIndex != areaMembers[i])
        {
            cellMembers[memberIndex] = areaMembers[i];
            ++memberIndex;
        }
    }

    m_cells[cellIndex].setMembers(cellMembers, memberCount - 1);
}

void JsonGrid::resetCellsValues()
{
    for (int i=0; i<m_cells.size(); ++i)
    {
        m_cells[i].resetValues();
    }
}

void JsonGrid::decodeCells(const QJsonArray& cells)
{
    for (int i=0; i<cells.size(); ++i)
    {
        QJsonObject cell = cells[i].toObject();
        decodeCell(cell);
    }
}

void JsonGrid::decodeCell(const QJsonObject& cell)
{
    int cellIndex = cell["index"].toInt();
    int value = cell["value"].toInt();

    Cell& decodedCell = m_cells[cellIndex];
    decodedCell.removeAllValuesExcept(value);
}
