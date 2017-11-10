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
#include "QObjectStringifier.h"
#include "QAddressWiper.h"
#include "QTestSignaler.h"
#include "QTestSignalerD.h"

#include <QScopedPointer>

#if QT_POINTER_SIZE == 4
#define POINTER_MARK "0xffffffff"
#elif QT_POINTER_SIZE == 8
#define POINTER_MARK "0xffffffffffffffff"
#else
#error Must define POINTER_MARK for target architecture.
#endif

#define DECLARE_TEST_STRINGIFIER_FUNC(CLASS) \
    DECLARE_QOBJECT_STRINGIFIER_FUNC(CLASS) { \
        Q_ASSERT(qobject_cast<const CLASS*>(object)); \
        buffer.append(QLatin1Literal("<" #CLASS ">")); \
    }

DECLARE_TEST_STRINGIFIER_FUNC(QTestSignaler)
DECLARE_TEST_STRINGIFIER_FUNC(QTestSignalerD)

void QDebugUtilsTest::testQObjectStringifier() {
    QFETCH(bool, enableQTestSignalerStringifier);
    QFETCH(bool, enableQTestSignalerDStringifier);
    QFETCH(QVariant, testValue);
    QFETCH(QString, expectedWithoutType);
    QFETCH(QString, expectedWithType);

    QScopedPointer<const QObjectStringifier> testSignalerStringifier(
                enableQTestSignalerStringifier
                ? new QObjectStringifier(&QTestSignaler::staticMetaObject
                                         , &QTestSignalerStringifierFunc)
                : nullptr);
    QScopedPointer<const QObjectStringifier> testSignalerDStringifier(
                enableQTestSignalerDStringifier
                ? new QObjectStringifier(&QTestSignalerD::staticMetaObject
                                         , &QTestSignalerDStringifierFunc)
                : nullptr);

    QString bufferWithoutType;
    QString bufferWithType;

    QValueStringifier::stringify(testValue, bufferWithoutType);
    QValueStringifier::stringify(testValue, bufferWithType, true);

    QCOMPARE(QStringAddressWiper::copywipe(bufferWithoutType)
             , expectedWithoutType);

    QCOMPARE(QStringAddressWiper::copywipe(bufferWithType)
             , expectedWithType);
}

void QDebugUtilsTest::testQObjectStringifier_data() {
    static QTestSignaler testSignaler;
    static QTestSignalerD testSignalerD;

    QTest::addColumn<bool>("enableQTestSignalerStringifier");
    QTest::addColumn<bool>("enableQTestSignalerDStringifier");
    QTest::addColumn<QVariant>("testValue");
    QTest::addColumn<QString>("expectedWithoutType");
    QTest::addColumn<QString>("expectedWithType");

    QTest::newRow("nullptr F/F")
            << false
            << false
            << QVariant::fromValue<QObject*>(nullptr)
            << QStringLiteral("nullptr")
            << QStringLiteral("QObject*(nullptr)");

    QTest::newRow("nullptr T/T")
            << true
            << true
            << QVariant::fromValue<QObject*>(nullptr)
            << QStringLiteral("nullptr")
            << QStringLiteral("QObject*(nullptr)");

    QTest::newRow("QTestSignaler F/F")
            << false
            << false
            << QVariant::fromValue<QObject*>(&testSignaler)
            << QStringLiteral(POINTER_MARK)
            << QStringLiteral("QObject*(" POINTER_MARK ")");

    QTest::newRow("QTestSignaler F/T")
            << false
            << true
            << QVariant::fromValue<QObject*>(&testSignaler)
            << QStringLiteral(POINTER_MARK)
            << QStringLiteral("QObject*(" POINTER_MARK ")");

    QTest::newRow("QTestSignaler T/F")
            << true
            << false
            << QVariant::fromValue<QObject*>(&testSignaler)
            << QStringLiteral("<QTestSignaler>")
            << QStringLiteral("QObject*(<QTestSignaler>)");

    QTest::newRow("QTestSignaler T/T")
            << true
            << true
            << QVariant::fromValue<QObject*>(&testSignaler)
            << QStringLiteral("<QTestSignaler>")
            << QStringLiteral("QObject*(<QTestSignaler>)");

    QTest::newRow("QTestSignalerD F/F")
            << false
            << false
            << QVariant::fromValue<QObject*>(&testSignalerD)
            << QStringLiteral(POINTER_MARK)
            << QStringLiteral("QObject*(" POINTER_MARK ")");

    QTest::newRow("QTestSignalerD T/F")
            << true
            << false
            << QVariant::fromValue<QObject*>(&testSignalerD)
            << QStringLiteral("<QTestSignaler>")
            << QStringLiteral("QObject*(<QTestSignaler>)");

    QTest::newRow("QTestSignalerD T/T")
            << false
            << true
            << QVariant::fromValue<QObject*>(&testSignalerD)
            << QStringLiteral("<QTestSignalerD>")
            << QStringLiteral("QObject*(<QTestSignalerD>)");

    QTest::newRow("QTestSignalerD T/T")
            << true
            << true
            << QVariant::fromValue<QObject*>(&testSignalerD)
            << QStringLiteral("<QTestSignalerD>")
            << QStringLiteral("QObject*(<QTestSignalerD>)");
}
