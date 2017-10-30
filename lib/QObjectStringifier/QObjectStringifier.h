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
#ifndef QOBJECTSTRINGIFIER_H
#define QOBJECTSTRINGIFIER_H

#include <QVariant>
#include <QObject>
#include <QMetaObject>

class QObjectStringifier {

public:

    virtual ~QObjectStringifier();

    const QMetaObject* getMetaObject() const;

    static void stringify(const QObject* object, QString& buffer);

protected:

    QObjectStringifier(const QMetaObject* metaObject);

    virtual void doStringify(const QObject* object, QString& buffer) const = 0;

    const QMetaObject* _metaObject;

};

#define DECLARE_QOBJECT_STRINGIFIER(CLASS, METACLASS_POINTER) \
    class CLASS : public QObjectStringifier { \
    public: \
    CLASS() : QObjectStringifier(METACLASS_POINTER) {} \
    private: \
    virtual void doStringify(const QObject* object, QString& buffer) const override; \
    }

#define DEFINE_QOBJECT_STRINGIFIER(CLASS) \
    void CLASS::doStringify(const QObject* object, QString& buffer) const

#define IMPLEMENT_QOBJECT_STRINGIFIER(CLASS) \
    DECLARE_QOBJECT_STRINGIFIER(CLASS##Stringifier, &CLASS::staticMetaObject) \
    static CLASS##Stringifier _##CLASS##Stringifier; \
    DEFINE_QOBJECT_STRINGIFIER(CLASS##Stringifier)

#endif // QOBJECTSTRINGIFIER_H
