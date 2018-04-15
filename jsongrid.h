#ifndef JSONGRID_H
#define JSONGRID_H

#include "grid.h"
#include "ijsonserializable.h"

class JsonGrid : public Grid, IJsonSerializable
{
public:
    JsonGrid();
    JsonGrid(quint32 width, quint32 height);

    QJsonObject encode() const;
    void decode(const QJsonObject& json);

private:
    void decodeAreas(const QJsonArray& areas);
    void decodeArea(const QJsonArray& area);
    void decodeCellMembers(int cellIndex, const int areaMembers[], int memberCount);

    void decodeCells(const QJsonArray& cells);
    void decodeCell(const QJsonObject& cell);

    void resetCellsValues();
};

#endif // JSONGRID_H
