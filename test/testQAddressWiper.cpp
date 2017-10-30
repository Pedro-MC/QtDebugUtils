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
#include "QAddressWiper.h"

void QDebugUtilsTest::testQAddressWiper() {
    QFETCH(const QString, test);
    QFETCH(const QString, expected);
    QCOMPARE(QStringAddressWiper::copywipe(test), expected);
    QCOMPARE(QByteArrayAddressWiper::copywipe(test.toLocal8Bit()), expected.toLocal8Bit());
}

#if QT_POINTER_SIZE == 4
void QDebugUtilsTest::testQAddressWiper_data() {
    QTest::addColumn<QString>("test");
    QTest::addColumn<QString>("expected");

    QTest::newRow("0")
            << QStringLiteral("0x01234567")
            << QStringLiteral("0xffffffff");

    QTest::newRow("1")
            << QStringLiteral("0xx1234567")
            << QStringLiteral("0xx1234567");

    QTest::newRow("2")
            << QStringLiteral("abc 0x01234567")
            << QStringLiteral("abc 0xffffffff");

    QTest::newRow("3")
            << QStringLiteral("abc 0x01234567 xyz")
            << QStringLiteral("abc 0xffffffff xyz");

    QTest::newRow("4")
            << QStringLiteral("0x00000000 0x01234567")
            << QStringLiteral("0xffffffff 0xffffffff");

    QTest::newRow("5")
            << QStringLiteral("0xf0e1d2c3 abc 0x01234567")
            << QStringLiteral("0xffffffff abc 0xffffffff");

    QTest::newRow("6")
            << QStringLiteral("123 0xf0e1d2c3 abc 0x01234567")
            << QStringLiteral("123 0xffffffff abc 0xffffffff");

    QTest::newRow("7")
            << QStringLiteral("123 0xf0e1d2c3 abc 0x01234567 xyz")
            << QStringLiteral("123 0xffffffff abc 0xffffffff xyz");
}
#elif QT_POINTER_SIZE == 8
void QDebugUtilsTest::testQAddressWiper_data() {
    QTest::addColumn<QString>("test");
    QTest::addColumn<QString>("expected");

    QTest::newRow("0x0123456789abcdef")
            << QStringLiteral("0x0123456789abcdef")
            << QStringLiteral("0xffffffffffffffff");

    QTest::newRow("0xx123456789abcdef")
            << QStringLiteral("0xx123456789abcdef")
            << QStringLiteral("0xx123456789abcdef");

    QTest::newRow("abc 0x0123456789abcdef")
            << QStringLiteral("abc 0x0123456789abcdef")
            << QStringLiteral("abc 0xffffffffffffffff");

    QTest::newRow("abc 0x0123456789abcdef xyz")
            << QStringLiteral("abc 0x0123456789abcdef xyz")
            << QStringLiteral("abc 0xffffffffffffffff xyz");

    QTest::newRow("0x0000000000000000 0x0123456789abcdef")
            << QStringLiteral("0x0000000000000000 0x0123456789abcdef")
            << QStringLiteral("0xffffffffffffffff 0xffffffffffffffff");

    QTest::newRow("0xf0e1d2c3b4a59687 abc 0x0123456789abcdef")
            << QStringLiteral("0xf0e1d2c3b4a59687 abc 0x0123456789abcdef")
            << QStringLiteral("0xffffffffffffffff abc 0xffffffffffffffff");

    QTest::newRow("123 0xf0e1d2c3b4a59687 abc 0x0123456789abcdef")
            << QStringLiteral("123 0xf0e1d2c3b4a59687 abc 0x0123456789abcdef")
            << QStringLiteral("123 0xffffffffffffffff abc 0xffffffffffffffff");

    QTest::newRow("123 0xf0e1d2c3b4a59687 abc 0x0123456789abcdef xyz")
            << QStringLiteral("123 0xf0e1d2c3b4a59687 abc 0x0123456789abcdef xyz")
            << QStringLiteral("123 0xffffffffffffffff abc 0xffffffffffffffff xyz");

    QTest::newRow("123 0x0e1d2c3b4a59687 abc 0x0123456789abcdef xyz")
            << QStringLiteral("123 0x0e1d2c3b4a59687 abc 0x0123456789abcdef xyz")
            << QStringLiteral("123 0x0e1d2c3b4a59687 abc 0xffffffffffffffff xyz");

    QTest::newRow("123 0xff0e1d2c3b4a59687 abc 0x0123456789abcdef xyz")
            << QStringLiteral("123 0xff0e1d2c3b4a59687 abc 0x0123456789abcdef xyz")
            << QStringLiteral("123 0xffffffffffffffff7 abc 0xffffffffffffffff xyz");
}
#else
#error "Must define QDebugUtilsTest::testQAddressWiper_data() for target architecture."
#endif
