#include <QtGlobal>
#include <QVector>
#include <QString>

#include "grid.h"

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
