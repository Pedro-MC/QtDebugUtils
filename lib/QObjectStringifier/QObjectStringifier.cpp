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

static QMultiHash<const QMetaObject*, QObjectStringifier*>& getStringifiers() {
    static QMultiHash<const QMetaObject*, QObjectStringifier*> stringifiers;
    return stringifiers;
}

QObjectStringifier::QObjectStringifier(const QMetaObject* metaObject)
    : _metaObject(metaObject) {
    getStringifiers().insertMulti(_metaObject, this);
}

QObjectStringifier::~QObjectStringifier() {
    getStringifiers().remove(_metaObject, this);
}

const QMetaObject* QObjectStringifier::getMetaObject() const {
    return _metaObject;
}

void QObjectStringifier::stringify(const QObject* object, QString& buffer) {
    const auto& stringifiers = getStringifiers();
    const QMetaObject* metaObject = object ? object->metaObject() : nullptr;
    decltype(stringifiers.find(metaObject)) iter;
    while((iter = stringifiers.find(metaObject)) == stringifiers.constEnd()) {
        metaObject = metaObject->superClass();
    }
    iter.value()->doStringify(object, buffer);
}

DECLARE_VALUE_STRINGIFIER(QObjectBaseValueStringifier, QMetaType::QObjectStar);
DEFINE_VALUE_STRINGIFIER(QObjectBaseValueStringifier) {
    QObjectStringifier::stringify(var.value<QObject*>(), buffer);
}
static QObjectBaseValueStringifier _QObjectBaseValueStringifier;

DECLARE_QOBJECT_STRINGIFIER(QObjectBaseStringifier, nullptr);
DEFINE_QOBJECT_STRINGIFIER(QObjectBaseStringifier) {
    QValueStringifier::stringify(QVariant::fromValue<void*>(
                                     const_cast<QObject*>(object)), buffer);
}
static QObjectBaseStringifier _QObjectBaseStringifier;
