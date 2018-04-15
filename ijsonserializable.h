#ifndef IJSONSERIALIZER_H
#define IJSONSERIALIZER_H

#include <QJsonDocument>

class IJsonSerializable
{
public:
    virtual QJsonObject encode() const = 0;
    virtual void decode(const QJsonObject& json) = 0;
};

#endif // IJSONSERIALIZER_H
