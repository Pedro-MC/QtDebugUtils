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
#include "QValueStringifier.h"

#include <QMultiHash>
#include <QString>
#include <QUrl>
#include <QUuid>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QPoint>
#include <QSize>
#include <QRect>
#include <QLine>

static const QLatin1Char singleQuote       = QLatin1Char('\'');
static const QLatin1Char doubleQuote       = QLatin1Char('"');
static const QLatin1Char openParenthesis   = QLatin1Char('(');
static const QLatin1Char closeParenthesis  = QLatin1Char(')');
static const QLatin1Char openCurlyBracket  = QLatin1Char('{');
static const QLatin1Char closeCurlyBracket = QLatin1Char('}');
static const QLatin1Char asterisk          = QLatin1Char('*');
static const QLatin1Char comma             = QLatin1Char(',');

static QMultiHash<QMetaType::Type, const QValueStringifier*>& getStringifiers() {
    static QMultiHash<QMetaType::Type, const QValueStringifier*> stringifiers;
    return stringifiers;
}

QValueStringifier::QValueStringifier(QMetaType::Type type)
    : _type(type) {
    getStringifiers().insertMulti(_type, this);
}

QValueStringifier::~QValueStringifier() {
    getStringifiers().remove(_type, this);
}

QMetaType::Type QValueStringifier::getType() const {
    return _type;
}

void QValueStringifier::stringify(const QVariant& var, QString& buffer
                                  , bool withType) {
    const auto& stringifiers = getStringifiers();
    auto iter = stringifiers.find(static_cast<QMetaType::Type>(var.userType()));
    if(iter == stringifiers.constEnd()) {
        iter = stringifiers.find(QMetaType::UnknownType);
    }
    if(withType) {
        buffer.append(QLatin1String(var.typeName()));
        buffer.append(openParenthesis);
    }
    iter.value()->doStringify(var, buffer);
    if(withType) {
        buffer.append(closeParenthesis);
    }
}

static void stringifyChar(const QChar& c, QString& buffer) {
    static const char* const escapeTable[] = {
        "\\x00", "\\x01", "\\x02", "\\x03", "\\x04", "\\x05", "\\x06", "\\a"  ,
        "\\b"  , "\\t"  , "\\n"  , "\\v"  , "\\f"  , "\\r"  , "\\x0e", "\\x0f",
        "\\x10", "\\x11", "\\x12", "\\x13", "\\x14", "\\x15", "\\x16", "\\x17",
        "\\x18", "\\x19", "\\x1a", "\\x1b", "\\x1c", "\\x1d", "\\x1e", "\\x1f",
        " "    , "!"    , "\\\"" , "#"    , "$"    , "%"    , "&"    , "\\\'" ,
        "("    , ")"    , "*"    , "+"    , ","    , "-"    , "."    , "/"    ,
        "0"    , "1"    , "2"    , "3"    , "4"    , "5"    , "6"    , "7"    ,
        "8"    , "9"    , ":"    , ";"    , "<"    , "="    , ">"    , "?"    ,
        "@"    , "A"    , "B"    , "C"    , "D"    , "E"    , "F"    , "G"    ,
        "H"    , "I"    , "J"    , "K"    , "L"    , "M"    , "N"    , "O"    ,
        "P"    , "Q"    , "R"    , "S"    , "T"    , "U"    , "V"    , "W"    ,
        "X"    , "Y"    , "Z"    , "["    , "\\\\"
    };
    const ushort unicode = c.unicode();
    if(unicode < sizeof(escapeTable) / sizeof(escapeTable[0])) {
        buffer.append(QLatin1String(escapeTable[unicode]));
    } else {
        buffer.append(c);
    }
}

static void stringifyChar(char c, QString& buffer) {
    stringifyChar(QChar::fromLatin1(c), buffer);
}

static void stringifyPointer(const void* pointer, QString& buffer) {
    if(pointer) {
        buffer.append(QLatin1Literal("0x"));
        buffer.append(QString::number(reinterpret_cast<const quintptr>(pointer))
                      .rightJustified(sizeof(void*) * 2, QLatin1Char('0')));
    } else {
        buffer.append(QLatin1Literal("nullptr"));
    }
}

template<typename TYPE>
static void stringifyNumber(TYPE num, QString& buffer) {
    buffer.append(QString::number(num));
}

template<typename TYPE>
static void stringifyString(TYPE string, QString& buffer) {
    buffer.append(doubleQuote);
    for(const auto c : string) {
        stringifyChar(c, buffer);
    }
    buffer.append(doubleQuote);
}

template<typename TYPE>
static void stringifyNumberPair(TYPE n1, TYPE n2, QString& buffer) {
    buffer.append(openCurlyBracket);
    stringifyNumber(n1, buffer);
    buffer.append(comma);
    stringifyNumber(n2, buffer);
    buffer.append(closeCurlyBracket);
}

template<typename TYPE>
static void stringifyNumberQuad(TYPE n1, TYPE n2, TYPE n3, TYPE n4, QString& buffer) {
    buffer.append(openCurlyBracket);
    stringifyNumber(n1, buffer);
    buffer.append(comma);
    stringifyNumber(n2, buffer);
    buffer.append(comma);
    stringifyNumber(n3, buffer);
    buffer.append(comma);
    stringifyNumber(n4, buffer);
    buffer.append(closeCurlyBracket);
}

#define IMPLEMENT_VALUE_STRINGIFIER_QMETATYPE(ID) \
    DECLARE_VALUE_STRINGIFIER(ID##ValueStringifier, QMetaType:: ID); \
    static ID##ValueStringifier _##ID##ValueStringifier; \
    DEFINE_VALUE_STRINGIFIER(ID##ValueStringifier) \

#define ASSERT_TYPE() \
    Q_ASSERT(var.userType() == static_cast<int>(getType()))

#define IMPLEMENT_NUMBER_STRINGIFIER_QMETATYPE(ID, TYPE) \
    IMPLEMENT_VALUE_STRINGIFIER_QMETATYPE(ID) { \
    ASSERT_TYPE(); \
    stringifyNumber(var.value<TYPE>(), buffer); \
    }

#define IMPLEMENT_CHAR_STRINGIFIER_QMETATYPE(ID, TYPE) \
    IMPLEMENT_VALUE_STRINGIFIER_QMETATYPE(ID) { \
    ASSERT_TYPE(); \
    buffer.append(singleQuote); \
    stringifyChar(var.value<TYPE>(), buffer); \
    buffer.append(singleQuote); \
    }

#define IMPLEMENT_LIST_STRINGIFIER_QMETATYPE(TYPE, CODE) \
    IMPLEMENT_VALUE_STRINGIFIER_QMETATYPE(TYPE) { \
    ASSERT_TYPE(); \
    buffer.append(openCurlyBracket); \
    bool first = true; \
    for(const auto& item : var.value<TYPE>()) { \
    if(first) { \
    first = false; \
    } else { \
    buffer.append(QLatin1Literal(", ")); \
    } \
    CODE; \
    } \
    buffer.append(closeCurlyBracket); \
    }

IMPLEMENT_NUMBER_STRINGIFIER_QMETATYPE(Short, short)
IMPLEMENT_NUMBER_STRINGIFIER_QMETATYPE(Int, int)
IMPLEMENT_NUMBER_STRINGIFIER_QMETATYPE(Long, long)
IMPLEMENT_NUMBER_STRINGIFIER_QMETATYPE(LongLong, qlonglong)

IMPLEMENT_NUMBER_STRINGIFIER_QMETATYPE(UShort, ushort)
IMPLEMENT_NUMBER_STRINGIFIER_QMETATYPE(UInt, uint)
IMPLEMENT_NUMBER_STRINGIFIER_QMETATYPE(ULong, ulong)
IMPLEMENT_NUMBER_STRINGIFIER_QMETATYPE(ULongLong, qulonglong)

IMPLEMENT_NUMBER_STRINGIFIER_QMETATYPE(Float, float)
IMPLEMENT_NUMBER_STRINGIFIER_QMETATYPE(Double, double)

IMPLEMENT_CHAR_STRINGIFIER_QMETATYPE(Char, char)
IMPLEMENT_CHAR_STRINGIFIER_QMETATYPE(UChar, unsigned char)
IMPLEMENT_CHAR_STRINGIFIER_QMETATYPE(SChar, signed char)
IMPLEMENT_CHAR_STRINGIFIER_QMETATYPE(QChar, QChar)

IMPLEMENT_LIST_STRINGIFIER_QMETATYPE(QStringList, stringifyString(item, buffer))
IMPLEMENT_LIST_STRINGIFIER_QMETATYPE(QVariantList, stringify(item, buffer, true))

IMPLEMENT_VALUE_STRINGIFIER_QMETATYPE(UnknownType) {
    Q_UNUSED(var);
    buffer.append(QLatin1Literal("{?}"));
}

IMPLEMENT_VALUE_STRINGIFIER_QMETATYPE(Bool) {
    ASSERT_TYPE();
    buffer.append(var.value<bool>()
                  ? QLatin1Literal("true")
                  : QLatin1Literal("false"));
}

IMPLEMENT_VALUE_STRINGIFIER_QMETATYPE(QString) {
    ASSERT_TYPE();
    stringifyString(var.value<QString>(), buffer);
}

IMPLEMENT_VALUE_STRINGIFIER_QMETATYPE(QByteArray) {
    ASSERT_TYPE();
    stringifyString(var.value<QByteArray>(), buffer);
}

IMPLEMENT_VALUE_STRINGIFIER_QMETATYPE(QObjectStar) {
    ASSERT_TYPE();
    stringifyPointer(var.value<QObject*>(), buffer);
}

IMPLEMENT_VALUE_STRINGIFIER_QMETATYPE(VoidStar) {
    ASSERT_TYPE();
    stringifyPointer(var.value<void*>(), buffer);
}

IMPLEMENT_VALUE_STRINGIFIER_QMETATYPE(QUuid) {
    ASSERT_TYPE();
    buffer.append(var.value<QUuid>().toString());
}

IMPLEMENT_VALUE_STRINGIFIER_QMETATYPE(QDate) {
    ASSERT_TYPE();
    buffer.append(openCurlyBracket);
    buffer.append(var.value<QDate>().toString(QStringLiteral("yyyy-MM-dd")));
    buffer.append(closeCurlyBracket);
}

IMPLEMENT_VALUE_STRINGIFIER_QMETATYPE(QTime) {
    ASSERT_TYPE();
    buffer.append(openCurlyBracket);
    buffer.append(var.value<QTime>().toString(QStringLiteral("HH:mm:ss")));
    buffer.append(closeCurlyBracket);
}

IMPLEMENT_VALUE_STRINGIFIER_QMETATYPE(QDateTime) {
    ASSERT_TYPE();
    buffer.append(openCurlyBracket);
    buffer.append(var.value<QDateTime>().toString(QStringLiteral("yyyy-MM-dd HH:mm:ss")));
    buffer.append(closeCurlyBracket);
}

IMPLEMENT_VALUE_STRINGIFIER_QMETATYPE(QUrl) {
    ASSERT_TYPE();
    buffer.append(openCurlyBracket);
    buffer.append(var.value<QUrl>().toString());
    buffer.append(closeCurlyBracket);
}

IMPLEMENT_VALUE_STRINGIFIER_QMETATYPE(QPoint) {
    ASSERT_TYPE();
    const QPoint point = var.value<QPoint>();
    stringifyNumberPair(point.x(), point.y(), buffer);
}

IMPLEMENT_VALUE_STRINGIFIER_QMETATYPE(QPointF) {
    ASSERT_TYPE();
    const QPointF point = var.value<QPointF>();
    stringifyNumberPair(point.x(), point.y(), buffer);
}

IMPLEMENT_VALUE_STRINGIFIER_QMETATYPE(QSize) {
    ASSERT_TYPE();
    const QSize size = var.value<QSize>();
    stringifyNumberPair(size.width(), size.height(), buffer);
}

IMPLEMENT_VALUE_STRINGIFIER_QMETATYPE(QSizeF) {
    ASSERT_TYPE();
    const QSizeF size = var.value<QSizeF>();
    stringifyNumberPair(size.width(), size.height(), buffer);
}

IMPLEMENT_VALUE_STRINGIFIER_QMETATYPE(QRect) {
    ASSERT_TYPE();
    const QRect rect = var.value<QRect>();
    stringifyNumberQuad(rect.x(), rect.y(), rect.width(), rect.height(), buffer);
}

IMPLEMENT_VALUE_STRINGIFIER_QMETATYPE(QRectF) {
    ASSERT_TYPE();
    const QRectF rect = var.value<QRectF>();
    stringifyNumberQuad(rect.x(), rect.y(), rect.width(), rect.height(), buffer);
}

IMPLEMENT_VALUE_STRINGIFIER_QMETATYPE(QLine) {
    ASSERT_TYPE();
    const QLine line = var.value<QLine>();
    stringifyNumberQuad(line.x1(), line.y1(), line.x2(), line.y2(), buffer);
}

IMPLEMENT_VALUE_STRINGIFIER_QMETATYPE(QLineF) {
    ASSERT_TYPE();
    const QLineF line = var.value<QLineF>();
    stringifyNumberQuad(line.x1(), line.y1(), line.x2(), line.y2(), buffer);
}
