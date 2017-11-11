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

#include <QMutex>
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

class QValueStringifierData {

public:

    typedef QValueStringifier::StringifierFunc StringifierFunc;
    typedef QMultiHash<QMetaType::Type, StringifierFunc> Stringifiers;

    QValueStringifierData()
        : _useMutex(_threadSafeAccess) {
        if(_useMutex) {
            _mutex.lock();
        }
    }

    ~QValueStringifierData() {
        if(_useMutex) {
            _mutex.unlock();
        }
    }

    inline Stringifiers& getStringifiers() {
        return _stringifiers;
    }

    inline void enableStringifier(QMetaType::Type typeId
                                  , StringifierFunc stringifierFunc) {
        if(! _stringifiers.contains(typeId, stringifierFunc)) {
            _stringifiers.insertMulti(typeId, stringifierFunc);
        }
    }

    inline void disableStringifier(QMetaType::Type typeId
                                   , StringifierFunc stringifierFunc) {
        _stringifiers.remove(typeId, stringifierFunc);
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

QValueStringifierData::Stringifiers QValueStringifierData::_stringifiers;
QMutex QValueStringifierData::_mutex;
bool QValueStringifierData::_threadSafeAccess = false;

QValueStringifier::QValueStringifier(QMetaType::Type typeId
                                     , StringifierFunc stringifierFunc
                                     , bool enableAtConstruction)
    : _typeId(typeId)
    , _stringifierFunc(stringifierFunc) {
    Q_ASSERT(_stringifierFunc);
    if(enableAtConstruction) {
        enable();
    }
}

QValueStringifier::~QValueStringifier() {
    disable();
}

QMetaType::Type QValueStringifier::getTypeId() const {
    return _typeId;
}

QValueStringifier::StringifierFunc QValueStringifier::getStringifierFunc() const {
    return _stringifierFunc;
}

void QValueStringifier::enable() {
    QValueStringifierData().enableStringifier(_typeId, _stringifierFunc);
}

void QValueStringifier::disable() {
    QValueStringifierData().disableStringifier(_typeId, _stringifierFunc);
}

void QValueStringifier::enableThreadSafe() {
    QValueStringifierData::enableThreadSafeAccess();
}

void QValueStringifier::disableThreadSafe() {
    QValueStringifierData::disableThreadSafeAccess();
}

void QValueStringifier::stringify(const QVariant& var, QString& buffer
                                  , bool withType) {
    QValueStringifierData data;
    const auto& stringifiers = data.getStringifiers();
    auto iter = stringifiers.find(static_cast<QMetaType::Type>(var.userType()));
    if(iter == stringifiers.constEnd()) {
        iter = stringifiers.find(QMetaType::UnknownType);
    }
    if(withType) {
        buffer.append(QLatin1String(var.typeName()));
        buffer.append(openParenthesis);
    }
    iter.value()(var, buffer);
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

#define ASSERT_TYPE(TYPE) \
    Q_ASSERT(var.userType() == qMetaTypeId<TYPE>())

#define IMPLEMENT_NUMBER_STRINGIFIER(TYPE) \
    IMPLEMENT_VALUE_STRINGIFIER(TYPE) { \
    ASSERT_TYPE(TYPE); \
    stringifyNumber(var.value<TYPE>(), buffer); \
    }

#define IMPLEMENT_CHAR_STRINGIFIER(TYPE) \
    IMPLEMENT_VALUE_STRINGIFIER(TYPE) { \
    ASSERT_TYPE(TYPE); \
    buffer.append(singleQuote); \
    stringifyChar(var.value<TYPE>(), buffer); \
    buffer.append(singleQuote); \
    }

#define IMPLEMENT_LIST_STRINGIFIER(TYPE, CODE) \
    IMPLEMENT_VALUE_STRINGIFIER(TYPE) { \
    ASSERT_TYPE(TYPE); \
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

IMPLEMENT_NUMBER_STRINGIFIER(short)
IMPLEMENT_NUMBER_STRINGIFIER(int)
IMPLEMENT_NUMBER_STRINGIFIER(long)
IMPLEMENT_NUMBER_STRINGIFIER(qlonglong)

IMPLEMENT_NUMBER_STRINGIFIER(ushort)
IMPLEMENT_NUMBER_STRINGIFIER(uint)
IMPLEMENT_NUMBER_STRINGIFIER(ulong)
IMPLEMENT_NUMBER_STRINGIFIER(qulonglong)

IMPLEMENT_NUMBER_STRINGIFIER(float)
IMPLEMENT_NUMBER_STRINGIFIER(double)

typedef signed char schar;
IMPLEMENT_CHAR_STRINGIFIER(char)
IMPLEMENT_CHAR_STRINGIFIER(uchar)
IMPLEMENT_CHAR_STRINGIFIER(schar)
IMPLEMENT_CHAR_STRINGIFIER(QChar)

IMPLEMENT_LIST_STRINGIFIER(QStringList, stringifyString(item, buffer))
IMPLEMENT_LIST_STRINGIFIER(QVariantList, QValueStringifier::stringify(item, buffer, true))

DECLARE_VALUE_STRINGIFIER_FUNC(unknown) {
    Q_UNUSED(var);
    buffer.append(QLatin1Literal("{?}"));
}
static QValueStringifier unknownStringifierFuncRegister
    (QMetaType::UnknownType, &unknownStringifierFunc);

IMPLEMENT_VALUE_STRINGIFIER(bool) {
    ASSERT_TYPE(bool);
    buffer.append(var.value<bool>()
                  ? QLatin1Literal("true")
                  : QLatin1Literal("false"));
}

IMPLEMENT_VALUE_STRINGIFIER(QString) {
    ASSERT_TYPE(QString);
    stringifyString(var.value<QString>(), buffer);
}

IMPLEMENT_VALUE_STRINGIFIER(QByteArray) {
    ASSERT_TYPE(QByteArray);
    stringifyString(var.value<QByteArray>(), buffer);
}

typedef QObject* qobjectstar;
IMPLEMENT_VALUE_STRINGIFIER(qobjectstar) {
    ASSERT_TYPE(QObject*);
    stringifyPointer(var.value<QObject*>(), buffer);
}

typedef void* voidstar;
IMPLEMENT_VALUE_STRINGIFIER(voidstar) {
    ASSERT_TYPE(void*);
    stringifyPointer(var.value<void*>(), buffer);
}

IMPLEMENT_VALUE_STRINGIFIER(QUuid) {
    ASSERT_TYPE(QUuid);
    buffer.append(var.value<QUuid>().toString());
}

IMPLEMENT_VALUE_STRINGIFIER(QDate) {
    ASSERT_TYPE(QDate);
    buffer.append(openCurlyBracket);
    buffer.append(var.value<QDate>().toString(QStringLiteral("yyyy-MM-dd")));
    buffer.append(closeCurlyBracket);
}

IMPLEMENT_VALUE_STRINGIFIER(QTime) {
    ASSERT_TYPE(QTime);
    buffer.append(openCurlyBracket);
    buffer.append(var.value<QTime>().toString(QStringLiteral("HH:mm:ss")));
    buffer.append(closeCurlyBracket);
}

IMPLEMENT_VALUE_STRINGIFIER(QDateTime) {
    ASSERT_TYPE(QDateTime);
    buffer.append(openCurlyBracket);
    buffer.append(var.value<QDateTime>().toString(QStringLiteral("yyyy-MM-dd HH:mm:ss")));
    buffer.append(closeCurlyBracket);
}

IMPLEMENT_VALUE_STRINGIFIER(QUrl) {
    ASSERT_TYPE(QUrl);
    buffer.append(openCurlyBracket);
    buffer.append(var.value<QUrl>().toString());
    buffer.append(closeCurlyBracket);
}

IMPLEMENT_VALUE_STRINGIFIER(QPoint) {
    ASSERT_TYPE(QPoint);
    const QPoint point = var.value<QPoint>();
    stringifyNumberPair(point.x(), point.y(), buffer);
}

IMPLEMENT_VALUE_STRINGIFIER(QPointF) {
    ASSERT_TYPE(QPointF);
    const QPointF point = var.value<QPointF>();
    stringifyNumberPair(point.x(), point.y(), buffer);
}

IMPLEMENT_VALUE_STRINGIFIER(QSize) {
    ASSERT_TYPE(QSize);
    const QSize size = var.value<QSize>();
    stringifyNumberPair(size.width(), size.height(), buffer);
}

IMPLEMENT_VALUE_STRINGIFIER(QSizeF) {
    ASSERT_TYPE(QSizeF);
    const QSizeF size = var.value<QSizeF>();
    stringifyNumberPair(size.width(), size.height(), buffer);
}

IMPLEMENT_VALUE_STRINGIFIER(QRect) {
    ASSERT_TYPE(QRect);
    const QRect rect = var.value<QRect>();
    stringifyNumberQuad(rect.x(), rect.y(), rect.width(), rect.height(), buffer);
}

IMPLEMENT_VALUE_STRINGIFIER(QRectF) {
    ASSERT_TYPE(QRectF);
    const QRectF rect = var.value<QRectF>();
    stringifyNumberQuad(rect.x(), rect.y(), rect.width(), rect.height(), buffer);
}

IMPLEMENT_VALUE_STRINGIFIER(QLine) {
    ASSERT_TYPE(QLine);
    const QLine line = var.value<QLine>();
    stringifyNumberQuad(line.x1(), line.y1(), line.x2(), line.y2(), buffer);
}

IMPLEMENT_VALUE_STRINGIFIER(QLineF) {
    ASSERT_TYPE(QLineF);
    const QLineF line = var.value<QLineF>();
    stringifyNumberQuad(line.x1(), line.y1(), line.x2(), line.y2(), buffer);
}
