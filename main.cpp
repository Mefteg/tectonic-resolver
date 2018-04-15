#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include "jsongrid.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 0;
    }

    QString jsonGridFilePath = argv[1];

    QFile gridFile(jsonGridFilePath);
    if (gridFile.exists() == false || gridFile.open(QIODevice::ReadOnly | QIODevice::Text) == false)
    {
        qDebug("Can not open the file");
        return -1;
    }

    QByteArray gridRawData = gridFile.readAll();

    gridFile.close();

    QJsonParseError parseError;
    QJsonDocument gridJsonDocument = QJsonDocument::fromJson(gridRawData, &parseError);
    if (parseError.error != QJsonParseError::ParseError::NoError)
    {
        qDebug("Parse JSON issue: \n%s", parseError.errorString().toStdString().c_str());
        return -1;
    }

    JsonGrid grid1;
    grid1.decode(gridJsonDocument.object());
    grid1.print();

    int maxLoopCount = 10000;
    int loopCount = 0;
    while (loopCount < maxLoopCount && grid1.resolve() == true)
    {
        // Keep resolving...
        ++loopCount;
    }
    qDebug("Loop count: %i\n", loopCount);

    grid1.print();

    return 0;
}
