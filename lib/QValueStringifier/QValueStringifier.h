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
#ifndef QVALUESTRINGIFIER_H
#define QVALUESTRINGIFIER_H

#include <QString>
#include <QVariant>
#include <QMetaType>

class QValueStringifier {

public:

    virtual ~QValueStringifier();

    QMetaType::Type getType() const;

    static void stringify(const QVariant& value, QString& buffer
                          , bool withType = false);

protected:

    explicit QValueStringifier(QMetaType::Type type);

private:

    virtual void doStringify(const QVariant& value, QString& buffer) const = 0;

    const QMetaType::Type _type;

};

#define DECLARE_VALUE_STRINGIFIER(CLASS, QMETATYPE_ID) \
    class CLASS : public QValueStringifier { \
    public: \
    CLASS() : QValueStringifier(QMETATYPE_ID) {} \
    private: \
    virtual void doStringify(const QVariant& var, QString& buffer) const override; \
    }

#define DEFINE_VALUE_STRINGIFIER(CLASS) \
    void CLASS::doStringify(const QVariant& var, QString& buffer) const

#define IMPLEMENT_VALUE_STRINGIFIER(CLASS, VALUE) \
    DECLARE_VALUE_STRINGIFIER(CLASS, \
    static_cast<QMetaType::Type>(QVariant::fromValue(VALUE).userType())); \
    static CLASS _##CLASS; \
    DEFINE_VALUE_STRINGIFIER(CLASS)

#endif // QVALUESTRINGIFIER_H
