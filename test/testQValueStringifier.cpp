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

#include "QDebugUtilsTest.h"
#include "QValueStringifier.h"
#include "QAddressWiper.h"

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

#if QT_POINTER_SIZE == 4
#define POINTER_MARK "0xffffffff"
#elif QT_POINTER_SIZE == 8
#define POINTER_MARK "0xffffffffffffffff"
#else
#error Must define POINTER_MARK for target architecture.
#endif

struct TestTypeA {
    int value;
};

Q_DECLARE_METATYPE(TestTypeA)

IMPLEMENT_VALUE_STRINGIFIER(TestTypeAValueStringifier, TestTypeA()) {
    Q_ASSERT(var.userType() == static_cast<int>(getType()));
    buffer.append(QLatin1Literal("{value="));
    stringify(QVariant::fromValue<int>(var.value<TestTypeA>().value), buffer);
    buffer.append(QLatin1Literal("}"));
}

struct TestTypeB {
    char value;
};

Q_DECLARE_METATYPE(TestTypeB)

IMPLEMENT_VALUE_STRINGIFIER(TestTypeBValueStringifier, TestTypeB()) {
    Q_ASSERT(var.userType() == static_cast<int>(getType()));
    buffer.append(QLatin1Literal("{value="));
    stringify(QVariant::fromValue<char>(var.value<TestTypeB>().value), buffer);
    buffer.append(QLatin1Literal("}"));
}

struct TestTypeC {
    float value;
};

Q_DECLARE_METATYPE(TestTypeC)

void QDebugUtilsTest::testQValueStringifier() {
    QFETCH(QVariant, testValue);
    QFETCH(QString, expectedWithoutType);
    QFETCH(QString, expectedWithType);

    QString bufferWithoutType;
    QString bufferWithType;

    QValueStringifier::stringify(testValue, bufferWithoutType);
    QValueStringifier::stringify(testValue, bufferWithType, true);

    QCOMPARE(QStringAddressWiper::copywipe(bufferWithoutType)
             , expectedWithoutType);

    QCOMPARE(QStringAddressWiper::copywipe(bufferWithType)
             , expectedWithType);
}

void QDebugUtilsTest::testQValueStringifier_data() {
    QTest::addColumn<QVariant>("testValue");
    QTest::addColumn<QString>("expectedWithoutType");
    QTest::addColumn<QString>("expectedWithType");

    QTest::newRow("unkown")
            << QVariant()
            << QStringLiteral("{?}")
            << QStringLiteral("({?})");

    QTest::newRow("bool(true)")
            << QVariant::fromValue(true)
            << QStringLiteral("true")
            << QStringLiteral("bool(true)");

    QTest::newRow("bool(false)")
            << QVariant::fromValue(false)
            << QStringLiteral("false")
            << QStringLiteral("bool(false)");

    QTest::newRow("short")
            << QVariant::fromValue(short(-123))
            << QStringLiteral("-123")
            << QStringLiteral("short(-123)");

    QTest::newRow("ushort")
            << QVariant::fromValue(ushort(123))
            << QStringLiteral("123")
            << QStringLiteral("ushort(123)");

    QTest::newRow("int")
            << QVariant::fromValue(int(-123))
            << QStringLiteral("-123")
            << QStringLiteral("int(-123)");

    QTest::newRow("uint")
            << QVariant::fromValue(uint(123))
            << QStringLiteral("123")
            << QStringLiteral("uint(123)");

    QTest::newRow("long")
            << QVariant::fromValue(long(-123))
            << QStringLiteral("-123")
            << QStringLiteral("long(-123)");

    QTest::newRow("ulong")
            << QVariant::fromValue(ulong(123456))
            << QStringLiteral("123456")
            << QStringLiteral("ulong(123456)");

    QTest::newRow("longlong")
            << QVariant::fromValue(qlonglong(-123456789))
            << QStringLiteral("-123456789")
            << QStringLiteral("qlonglong(-123456789)");

    QTest::newRow("ulonglong")
            << QVariant::fromValue(qulonglong(123456789))
            << QStringLiteral("123456789")
            << QStringLiteral("qulonglong(123456789)");

    QTest::newRow("float")
            << QVariant::fromValue(float(-1.23))
            << QStringLiteral("-1.23")
            << QStringLiteral("float(-1.23)");

    QTest::newRow("double")
            << QVariant::fromValue(double(1.23456))
            << QStringLiteral("1.23456")
            << QStringLiteral("double(1.23456)");

    QTest::newRow("(void*)nullptr")
            << QVariant::fromValue(static_cast<void*>(nullptr))
            << QStringLiteral("nullptr")
            << QStringLiteral("void*(nullptr)");

    QTest::newRow("(void*)" POINTER_MARK)
            << QVariant::fromValue(static_cast<void*>(this))
            << QStringLiteral(POINTER_MARK)
            << QStringLiteral("void*(" POINTER_MARK ")");

    QTest::newRow("(QObject*)nullptr")
            << QVariant::fromValue(static_cast<QObject*>(nullptr))
            << QStringLiteral("nullptr")
            << QStringLiteral("QObject*(nullptr)");

    QTest::newRow("(QObject*)" POINTER_MARK)
            << QVariant::fromValue(static_cast<QObject*>(this))
            << QStringLiteral(POINTER_MARK)
            << QStringLiteral("QObject*(" POINTER_MARK ")");

    QTest::newRow("(char)A")
            << QVariant::fromValue(char('A'))
            << QStringLiteral("'A'")
            << QStringLiteral("char('A')");

    QTest::newRow("(signed char)A")
            << QVariant::fromValue((signed char)('A'))
            << QStringLiteral("'A'")
            << QStringLiteral("signed char('A')");

    QTest::newRow("(uchar)A")
            << QVariant::fromValue(uchar('A'))
            << QStringLiteral("'A'")
            << QStringLiteral("uchar('A')");

    QTest::newRow("(QChar)A")
            << QVariant::fromValue(QChar::fromLatin1('A'))
            << QStringLiteral("'A'")
            << QStringLiteral("QChar('A')");

    QTest::newRow("QString:1")
            << QVariant::fromValue(QStringLiteral("A \a \f \" \' \\ \n \b \t \x01 \v \r çÇABC"))
            << QStringLiteral("\"A \\a \\f \\\" \\' \\\\ \\n \\b \\t \\x01 \\v \\r çÇABC\"")
            << QStringLiteral("QString(\"A \\a \\f \\\" \\' \\\\ \\n \\b \\t \\x01 \\v \\r çÇABC\")");

    QTest::newRow("QString:2")
            << QVariant::fromValue(
                   QStringLiteral(
                       "\x00\x01\x02\x03\x04\x05\x06\a\b\t\n\v\f\r\x0e\x0f"
                       "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f"
                       " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                       "[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
                       ))
            << QStringLiteral(
                   "\"\\x00\\x01\\x02\\x03\\x04\\x05\\x06\\a\\b\\t\\n\\v\\f\\r\\x0e\\x0f"
                   "\\x10\\x11\\x12\\x13\\x14\\x15\\x16\\x17\\x18\\x19\\x1a\\x1b\\x1c\\x1d\\x1e\\x1f"
                   " !\\\"#$%&\\\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                   "[\\\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\""
                   )
            << QStringLiteral(
                   "QString(\"\\x00\\x01\\x02\\x03\\x04\\x05\\x06\\a\\b\\t\\n\\v\\f\\r\\x0e\\x0f"
                   "\\x10\\x11\\x12\\x13\\x14\\x15\\x16\\x17\\x18\\x19\\x1a\\x1b\\x1c\\x1d\\x1e\\x1f"
                   " !\\\"#$%&\\\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                   "[\\\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\")"
                   );

    QTest::newRow("QByteArray")
            << QVariant::fromValue(QByteArrayLiteral("ABC \a \f \" \' \\ \n \b \t \x01 \v \r ABC"))
            << QStringLiteral("\"ABC \\a \\f \\\" \\' \\\\ \\n \\b \\t \\x01 \\v \\r ABC\"")
            << QStringLiteral("QByteArray(\"ABC \\a \\f \\\" \\' \\\\ \\n \\b \\t \\x01 \\v \\r ABC\")");

    QTest::newRow("QStringList")
            << QVariant::fromValue(QStringList()
                                   << QStringLiteral("A\r\n")
                                   << QStringLiteral("B\t\n"))
            << QStringLiteral("{\"A\\r\\n\", \"B\\t\\n\"}")
            << QStringLiteral("QStringList({\"A\\r\\n\", \"B\\t\\n\"})");

    QTest::newRow("QVariantList")
            << QVariant::fromValue(QVariantList()
                                   << QStringLiteral("A\r\n")
                                   << QVariant::fromValue(char('\n')))
            << QStringLiteral("{QString(\"A\\r\\n\"), char(\'\\n\')}")
            << QStringLiteral("QVariantList({QString(\"A\\r\\n\"), char(\'\\n\')})");

    QTest::newRow("QUuid")
            << QVariant::fromValue(
                   QUuid(QStringLiteral("{e3e3d988-899d-439a-8729-0711d7898702}")))
            << QStringLiteral("{e3e3d988-899d-439a-8729-0711d7898702}")
            << QStringLiteral("QUuid({e3e3d988-899d-439a-8729-0711d7898702})");

    QTest::newRow("QDate")
            << QVariant::fromValue(QDate(2345, 12, 31))
            << QStringLiteral("{2345-12-31}")
            << QStringLiteral("QDate({2345-12-31})");

    QTest::newRow("QTime")
            << QVariant::fromValue(QTime(23, 59, 58))
            << QStringLiteral("{23:59:58}")
            << QStringLiteral("QTime({23:59:58})");

    QTest::newRow("QDateTime")
            << QVariant::fromValue(QDateTime(QDate(2345, 12, 31)
                                             , QTime(23, 59, 58)))
            << QStringLiteral("{2345-12-31 23:59:58}")
            << QStringLiteral("QDateTime({2345-12-31 23:59:58})");

    QTest::newRow("QUrl")
            << QVariant::fromValue(QUrl(QStringLiteral("https://example.com/")))
            << QStringLiteral("{https://example.com/}")
            << QStringLiteral("QUrl({https://example.com/})");

    QTest::newRow("QPoint")
            << QVariant::fromValue(QPoint(1, 2))
            << QStringLiteral("{1,2}")
            << QStringLiteral("QPoint({1,2})");

    QTest::newRow("QPointF")
            << QVariant::fromValue(QPointF(1.1, 2.2))
            << QStringLiteral("{1.1,2.2}")
            << QStringLiteral("QPointF({1.1,2.2})");

    QTest::newRow("QSize")
            << QVariant::fromValue(QSize(1, 2))
            << QStringLiteral("{1,2}")
            << QStringLiteral("QSize({1,2})");

    QTest::newRow("QSizeF")
            << QVariant::fromValue(QSizeF(1, 2))
            << QStringLiteral("{1,2}")
            << QStringLiteral("QSizeF({1,2})");

    QTest::newRow("QRect")
            << QVariant::fromValue(QRect(1, 2, 3, 4))
            << QStringLiteral("{1,2,3,4}")
            << QStringLiteral("QRect({1,2,3,4})");

    QTest::newRow("QRectF")
            << QVariant::fromValue(QRectF(1.1, 2.2, 3.3, 4.4))
            << QStringLiteral("{1.1,2.2,3.3,4.4}")
            << QStringLiteral("QRectF({1.1,2.2,3.3,4.4})");

    QTest::newRow("QLine")
            << QVariant::fromValue(QLine(1, 2, 3, 4))
            << QStringLiteral("{1,2,3,4}")
            << QStringLiteral("QLine({1,2,3,4})");

    QTest::newRow("QLineF")
            << QVariant::fromValue(QLineF(1.1, 2.2, 3.3, 4.4))
            << QStringLiteral("{1.1,2.2,3.3,4.4}")
            << QStringLiteral("QLineF({1.1,2.2,3.3,4.4})");

    QTest::newRow("TestTypeA")
            << QVariant::fromValue(TestTypeA{1})
            << QStringLiteral("{value=1}")
            << QStringLiteral("TestTypeA({value=1})");

    QTest::newRow("TestTypeB")
            << QVariant::fromValue(TestTypeB{'\x01'})
            << QStringLiteral("{value='\\x01'}")
            << QStringLiteral("TestTypeB({value='\\x01'})");

    QTest::newRow("TestTypeC")
            << QVariant::fromValue(TestTypeC{1.1})
            << QStringLiteral("{?}")
            << QStringLiteral("TestTypeC({?})");
}
