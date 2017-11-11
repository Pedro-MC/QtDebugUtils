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

#include "QObjectStringifier.h"
#include "QValueStringifier.h"

#include <QMultiHash>
#include <QMutex>

class QObjectStringifierData {

public:

    typedef QObjectStringifier::StringifierFunc StringifierFunc;
    typedef QMultiHash<const QMetaObject*, StringifierFunc> Stringifiers;

    QObjectStringifierData()
        : _useMutex(_threadSafeAccess) {
        if(_useMutex) {
            _mutex.lock();
        }
    }

    ~QObjectStringifierData() {
        if(_useMutex) {
            _mutex.unlock();
        }
    }

    inline Stringifiers& getStringifiers() {
        return _stringifiers;
    }

    inline void enableStringifier(const QMetaObject* metaObject
                                  , StringifierFunc stringifierFunc) {
        if(! _stringifiers.contains(metaObject, stringifierFunc)) {
            _stringifiers.insertMulti(metaObject, stringifierFunc);
        }
    }

    inline void disableStringifier(const QMetaObject* metaObject
                                   , StringifierFunc stringifierFunc) {
        _stringifiers.remove(metaObject, stringifierFunc);
    }

    static inline void enableThreadSafeAccess() {
        _threadSafeAccess = true;
    }

    static inline void disableThreadSafeAccess() {
        _threadSafeAccess = false;
    }

private:

    const bool _useMutex;

    static Stringifiers _stringifiers;
    static QMutex _mutex;
    static bool _threadSafeAccess;

};

QObjectStringifierData::Stringifiers QObjectStringifierData::_stringifiers;
QMutex QObjectStringifierData::_mutex;
bool QObjectStringifierData::_threadSafeAccess = false;

QObjectStringifier::QObjectStringifier(const QMetaObject* metaObject
                                       , StringifierFunc stringifierFunc
                                       , bool enableAtConstruction)
    : _metaObject(metaObject)
    , _stringifierFunc(stringifierFunc) {
    Q_ASSERT(_metaObject);
    Q_ASSERT(_stringifierFunc);
    if(enableAtConstruction) {
        enable();
    }
}

QObjectStringifier::~QObjectStringifier() {
    disable();
}

const QMetaObject* QObjectStringifier::getMetaObject() const {
    return _metaObject;
}

QObjectStringifier::StringifierFunc QObjectStringifier::getStringifierFunc() const {
    return _stringifierFunc;
}

void QObjectStringifier::enable() {
    QObjectStringifierData().enableStringifier(_metaObject, _stringifierFunc);
}

void QObjectStringifier::disable() {
    QObjectStringifierData().disableStringifier(_metaObject, _stringifierFunc);
}

void QObjectStringifier::enableThreadSafe() {
    QObjectStringifierData::enableThreadSafeAccess();
}

void QObjectStringifier::disableThreadSafe() {
    QObjectStringifierData::disableThreadSafeAccess();
}

void QObjectStringifier::stringify(const QObject* object, QString& buffer) {
    QObjectStringifierData data;
    const auto& stringifiers = data.getStringifiers();
    const QMetaObject* metaObject = object ? object->metaObject() : &QObject::staticMetaObject;
    decltype(stringifiers.find(metaObject)) iter;
    while((iter = stringifiers.find(metaObject)) == stringifiers.constEnd()) {
        metaObject = metaObject->superClass();
    }
    iter.value()(object, buffer);
}

typedef QObject* qobjectstar;
IMPLEMENT_VALUE_STRINGIFIER(qobjectstar) {
    QObjectStringifier::stringify(var.value<QObject*>(), buffer);
}

IMPLEMENT_QOBJECT_STRINGIFIER(QObject) {
    QVariant const value = QVariant::fromValue<void*>(const_cast<QObject*>(object));
    QValueStringifier::stringify(value, buffer);
}
