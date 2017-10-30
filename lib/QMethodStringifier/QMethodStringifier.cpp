/*******************************************************************************
** Copyright © 2017 Pedro Miguel Carvalho <PedroMC@pmc.com.pt>
**
** This file is part of QtDebugUtils.
**
** QtDebugUtils is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** any later version.
**
** QtDebugUtils is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with QtDebugUtils.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/
#include "QMethodStringifier.h"
#include "QValueStringifier.h"

#define GUARD(TEST, RETURN, MESSAGE) \
    if(! (TEST)) { \
    qWarning("QMethodStringifier::%s : %s", __func__, (MESSAGE)); \
    return (RETURN); \
    }

QMethodStringifier::QMethodStringifier()
    : _buffer() {
    _buffer.reserve(4096);
}

QString QMethodStringifier::stringify(const QObject* object, int methodIndex
                                      , const QVector<QVariant>& parameters) {
    GUARD(object, QString(), "Invalid/null object.");
    return stringify(object, object->objectName(), object->metaObject()
                     , object->metaObject()->method(methodIndex), parameters);
}

QString QMethodStringifier::stringify(const QObject* object
                                      , const QMetaMethod& metaMethod
                                      , const QVector<QVariant>& parameters) {
    GUARD(object, QString(), "Invalid/null object.");
    return stringify(object, object->objectName(), object->metaObject()
                     , metaMethod, parameters);
}

QString QMethodStringifier::stringify(const void* objectAddress
                                      , const QString& objectName
                                      , const QMetaObject* metaObject
                                      , const QMetaMethod& metaMethod
                                      , const QVector<QVariant>& parameters) {
    GUARD(metaObject, QString(), "metaObject pointer must be non null.");
    GUARD(metaMethod.isValid(), QString(), "metamethod must be valid.");
    const QList<QByteArray> parameterTypes = metaMethod.parameterTypes();
    const QList<QByteArray> parameterNames = metaMethod.parameterNames();
    GUARD(parameters.count() >= parameterTypes.count() || parameters.count() == 0
          , QString(), "parameters count must be zero or be equal or greater"
                       " to the method's parameter count.");
    _buffer.append(QLatin1String(metaObject->className()));
    _buffer.append(QLatin1Literal("*("));
    QValueStringifier::stringify(QVariant::fromValue<void*>(
                                     const_cast<void*>(objectAddress)), _buffer);
    if(! objectName.isEmpty()) {
        _buffer.append(QLatin1Char(';'));
        QValueStringifier::stringify(objectName, _buffer);
    }
    _buffer.append(QLatin1Literal(")->"));
    _buffer.append(QLatin1String(metaMethod.name()));
    _buffer.append(QLatin1Char('('));
    for(int I = 0; I < parameterTypes.count(); ++I) {
        if(I > 0) _buffer.append(QLatin1Literal(", "));
        _buffer.append(QLatin1String(parameterTypes.at(I)));
        _buffer.append(QLatin1Char(' '));
        _buffer.append(QLatin1String(parameterNames.at(I)));
        if(parameters.count() > 0) {
            const bool isVariant = metaMethod.parameterType(I) == QMetaType::QVariant;
            GUARD(isVariant
                  || metaMethod.parameterType(I) == static_cast<int>(parameters.at(I).type())
                  , QString(), "parameters types must match the method's parameters types.");
            _buffer.append(QLatin1Char('='));
            QValueStringifier::stringify(parameters.at(I), _buffer, isVariant);
        }
    }
    _buffer.append(QLatin1Char(')'));
    QString result(_buffer);
    result.detach();
    _buffer.truncate(0);
    return result;
}
