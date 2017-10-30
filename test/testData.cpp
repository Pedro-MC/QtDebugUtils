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
#include "QTestSignaler.h"
#include "QTestSignalerD.h"
#include "QUniversalSlot.h"
#include "QValueStringifier.h"

#if QT_POINTER_SIZE == 4
#define POINTER_MARK "0xffffffff"
#elif QT_POINTER_SIZE == 8
#define POINTER_MARK "0xffffffffffffffff"
#else
#error Must define POINTER_MARK for target architecture.
#endif

#define EMIT_FUNCTOR(NUM, TYPE, METHOD, PARAMETERS) \
    EmitFunctor([] (QObject* s, const QVector<QVariant>& p) { \
        TYPE* const sender = qobject_cast<TYPE*>(s); \
        if(sender && p.count() == (NUM)) sender->METHOD PARAMETERS; \
    })

#define EMIT_FUNCTOR_0(TYPE, METHOD) \
    EMIT_FUNCTOR(0, TYPE, METHOD, ())

#define EMIT_FUNCTOR_1(TYPE, METHOD, TP0) \
    EMIT_FUNCTOR(1, TYPE, METHOD, ( \
    p.at(0).value<TP0>()))

#define EMIT_FUNCTOR_2(TYPE, METHOD, TP0, TP1) \
    EMIT_FUNCTOR(2, TYPE, METHOD, ( \
    p.at(0).value<TP0>(), \
    p.at(1).value<TP1>()))

#define EMIT_FUNCTOR_3(TYPE, METHOD, TP0, TP1, TP2) \
    EMIT_FUNCTOR(3, TYPE, METHOD, ( \
    p.at(0).value<TP0>(), \
    p.at(1).value<TP1>(), \
    p.at(2).value<TP2>()))

#define EMIT_FUNCTOR_4(TYPE, METHOD, TP0, TP1, TP2, TP3) \
    EMIT_FUNCTOR(4, TYPE, METHOD, ( \
    p.at(0).value<TP0>(), \
    p.at(1).value<TP1>(), \
    p.at(2).value<TP2>(), \
    p.at(3).value<TP3>()))

#define EMIT_FUNCTOR_9(TYPE, METHOD, TP0, TP1, TP2, TP3, TP4, TP5, TP6, TP7, TP8) \
    EMIT_FUNCTOR(9, TYPE, METHOD, ( \
    p.at(0).value<TP0>(), \
    p.at(1).value<TP1>(), \
    p.at(2).value<TP2>(), \
    p.at(3).value<TP3>(), \
    p.at(4).value<TP4>(), \
    p.at(5).value<TP5>(), \
    p.at(6).value<TP6>(), \
    p.at(7).value<TP7>(), \
    p.at(8).value<TP8>()))

#define CONNECT_FUNCTOR_NOT_SIGNAL() \
    ConnectFunctor([] (QUniversalSlot&, QObject*) { \
        return QMetaObject::Connection(); \
    })

#define CONNECT_FUNCTOR(TYPE, FUNC) \
    ConnectFunctor([] (QUniversalSlot& unislot, QObject* signaler) { \
        return unislot.connect(signaler, & TYPE :: FUNC); \
    })

struct TestType {
    int value;
};

Q_DECLARE_METATYPE(TestType)

IMPLEMENT_VALUE_STRINGIFIER(TestTypeValueStringifier, TestType()) {
    Q_ASSERT(var.userType() == static_cast<int>(getType()));
    buffer.append(QLatin1Literal("{value="));
    stringify(QVariant::fromValue<int>(var.value<TestType>().value), buffer);
    buffer.append(QLatin1Literal("}"));
}

void QDebugUtilsTest::test_data() {
    QTest::addColumn<bool>("useDerived");
    QTest::addColumn<QString>("objectName");
    QTest::addColumn<QByteArray>("methodSignature");
    QTest::addColumn<QVector<QVariant> >("parameters");
    QTest::addColumn<QString>("expectedResultWithoutParameters");
    QTest::addColumn<QString>("expectedResultWithParameters");
    QTest::addColumn<EmitFunctor>("emitFunctor");
    QTest::addColumn<ConnectFunctor>("connectFunctor");

    QTest::newRow(SIG_METHOD_0A)
            << false
            << QString()
            << QByteArrayLiteral(SIG_METHOD_0A)
            << QVector<QVariant>()
            << QStringLiteral("QTestSignaler*(" POINTER_MARK ")->method_0A()")
            << QStringLiteral("QTestSignaler*(" POINTER_MARK ")->method_0A()")
            << EMIT_FUNCTOR_0(QTestSignaler, method_0A)
            << CONNECT_FUNCTOR_NOT_SIGNAL();

    QTest::newRow(SIG_METHOD_0B)
            << true
            << QString()
            << QByteArrayLiteral(SIG_METHOD_0B)
            << QVector<QVariant>()
            << QStringLiteral("QTestSignalerD*(" POINTER_MARK ")->method_0B()")
            << QStringLiteral("QTestSignalerD*(" POINTER_MARK ")->method_0B()")
            << EMIT_FUNCTOR_0(QTestSignalerD, method_0B)
            << CONNECT_FUNCTOR_NOT_SIGNAL();

    QTest::newRow(SIG_SLOT_0A)
            << true
            << QString()
            << QByteArrayLiteral(SIG_SLOT_0A)
            << QVector<QVariant>()
            << QStringLiteral("QTestSignalerD*(" POINTER_MARK ")->slot_0A()")
            << QStringLiteral("QTestSignalerD*(" POINTER_MARK ")->slot_0A()")
            << EMIT_FUNCTOR_0(QTestSignaler, slot_0A)
            << CONNECT_FUNCTOR_NOT_SIGNAL();

    QTest::newRow(SIG_SLOT_0B)
            << true
            << QString()
            << QByteArrayLiteral(SIG_SLOT_0B)
            << QVector<QVariant>()
            << QStringLiteral("QTestSignalerD*(" POINTER_MARK ")->slot_0B()")
            << QStringLiteral("QTestSignalerD*(" POINTER_MARK ")->slot_0B()")
            << EMIT_FUNCTOR_0(QTestSignalerD, slot_0B)
            << CONNECT_FUNCTOR_NOT_SIGNAL();

    QTest::newRow(SIG_SIGNAL_0A)
            << false
            << QStringLiteral("Test Signal 0A")
            << QByteArrayLiteral(SIG_SIGNAL_0A)
            << QVector<QVariant>()
            << QStringLiteral("QTestSignaler*("
                              POINTER_MARK
                              ";\"Test Signal 0A\")->signal_0A()")
            << QStringLiteral("QTestSignaler*("
                              POINTER_MARK
                              ";\"Test Signal 0A\")->signal_0A()")
            << EMIT_FUNCTOR_0(QTestSignaler, signal_0A)
            << CONNECT_FUNCTOR(QTestSignaler, signal_0A);

    QTest::newRow(SIG_SIGNAL_0B)
            << true
            << QStringLiteral("Test Signal\t0B")
            << QByteArrayLiteral(SIG_SIGNAL_0B)
            << QVector<QVariant>()
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal\\t0B\")->signal_0B()")
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal\\t0B\")->signal_0B()")
            << EMIT_FUNCTOR_0(QTestSignalerD, signal_0B)
            << CONNECT_FUNCTOR(QTestSignalerD, signal_0B);

    QTest::newRow(SIG_SIGNAL_1A)
            << false
            << QStringLiteral("Test Signal 1A\r\n")
            << QByteArrayLiteral(SIG_SIGNAL_1A)
            << (QVector<QVariant>() << int(-123))
            << QStringLiteral("QTestSignaler*("
                              POINTER_MARK
                              ";\"Test Signal 1A\\r\\n\")->signal_1A(int p1)")
            << QStringLiteral("QTestSignaler*("
                              POINTER_MARK
                              ";\"Test Signal 1A\\r\\n\")->signal_1A(int p1=-123)")
            << EMIT_FUNCTOR_1(QTestSignaler, signal_1A, int)
            << CONNECT_FUNCTOR(QTestSignaler, signal_1A);

    QTest::newRow(SIG_SIGNAL_1B)
            << true
            << QStringLiteral("Test Signal 1B \\ \a \b \v \x01 \x02")
            << QByteArrayLiteral(SIG_SIGNAL_1B)
            << (QVector<QVariant>() << QStringLiteral("abc"))
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 1B \\\\ \\a \\b \\v \\x01 \\x02\")"
                              "->signal_1B(QString p1)")
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 1B \\\\ \\a \\b \\v \\x01 \\x02\")"
                              "->signal_1B(QString p1=\"abc\")")
            << EMIT_FUNCTOR_1(QTestSignaler, signal_1B, QString)
            << CONNECT_FUNCTOR(QTestSignaler, signal_1B);

    QTest::newRow(SIG_SIGNAL_1C)
            << false
            << QStringLiteral("Test Signal 1C")
            << QByteArrayLiteral(SIG_SIGNAL_1C)
            << (QVector<QVariant>()
                << QVariant::fromValue<QObject*>(this))
            << QStringLiteral("QTestSignaler*("
                              POINTER_MARK
                              ";\"Test Signal 1C\")"
                              "->signal_1C(QObject* p1)")
            << QStringLiteral("QTestSignaler*("
                              POINTER_MARK
                              ";\"Test Signal 1C\")"
                              "->signal_1C(QObject* p1=" POINTER_MARK ")")
            << EMIT_FUNCTOR_1(QTestSignaler, signal_1C, QObject*)
            << CONNECT_FUNCTOR(QTestSignaler, signal_1C);

    QTest::newRow(SIG_SIGNAL_1D)
            << false
            << QStringLiteral("Test Signal 1D")
            << QByteArrayLiteral(SIG_SIGNAL_1D)
            << (QVector<QVariant>()
                << (QStringList()
                    << QStringLiteral("abc")
                    << QStringLiteral("xyz")))
            << QStringLiteral("QTestSignaler*("
                              POINTER_MARK
                              ";\"Test Signal 1D\")"
                              "->signal_1D(QStringList p1)")
            << QStringLiteral("QTestSignaler*("
                              POINTER_MARK
                              ";\"Test Signal 1D\")"
                              "->signal_1D(QStringList p1={\"abc\", \"xyz\"})")
            << EMIT_FUNCTOR_1(QTestSignaler, signal_1D, QStringList)
            << CONNECT_FUNCTOR(QTestSignaler, signal_1D);

    QTest::newRow(SIG_SIGNAL_1E ":1")
            << false
            << QStringLiteral("Test Signal 1E:1")
            << QByteArrayLiteral(SIG_SIGNAL_1E)
            << (QVector<QVariant>() << int(-123))
            << QStringLiteral("QTestSignaler*("
                              POINTER_MARK
                              ";\"Test Signal 1E:1\")"
                              "->signal_1E(QVariant p1)")
            << QStringLiteral("QTestSignaler*("
                              POINTER_MARK
                              ";\"Test Signal 1E:1\")"
                              "->signal_1E(QVariant p1=int(-123))")
            << EMIT_FUNCTOR_1(QTestSignaler, signal_1E, int)
            << CONNECT_FUNCTOR(QTestSignaler, signal_1E);

    QTest::newRow(SIG_SIGNAL_1E ":2")
            << false
            << QStringLiteral("Test Signal 1E:2")
            << QByteArrayLiteral(SIG_SIGNAL_1E)
            << (QVector<QVariant>() << QVariant::fromValue(TestType{1}))
            << QStringLiteral("QTestSignaler*("
                              POINTER_MARK
                              ";\"Test Signal 1E:2\")"
                              "->signal_1E(QVariant p1)")
            << QStringLiteral("QTestSignaler*("
                              POINTER_MARK
                              ";\"Test Signal 1E:2\")"
                              "->signal_1E(QVariant p1=TestType({value=1}))")
            << EMIT_FUNCTOR_1(QTestSignaler, signal_1E, QVariant)
            << CONNECT_FUNCTOR(QTestSignaler, signal_1E);

    QTest::newRow(SIG_SIGNAL_1E ":3")
            << false
            << QStringLiteral("Test Signal 1E:3")
            << QByteArrayLiteral(SIG_SIGNAL_1E)
            << (QVector<QVariant>() << QVariant::fromValue(TestType{-1}))
            << QStringLiteral("QTestSignaler*("
                              POINTER_MARK
                              ";\"Test Signal 1E:3\")"
                              "->signal_1E(QVariant p1)")
            << QStringLiteral("QTestSignaler*("
                              POINTER_MARK
                              ";\"Test Signal 1E:3\")"
                              "->signal_1E(QVariant p1=TestType({value=-1}))")
            << EMIT_FUNCTOR_1(QTestSignaler, signal_1E, QVariant)
            << CONNECT_FUNCTOR(QTestSignaler, signal_1E);

    QTest::newRow(SIG_SIGNAL_1F)
            << true
            << QStringLiteral("Test Signal 1F")
            << QByteArrayLiteral(SIG_SIGNAL_1F)
            << (QVector<QVariant>() << uint(123))
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 1F\")"
                              "->signal_1F(uint p1)")
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 1F\")"
                              "->signal_1F(uint p1=123)")
            << EMIT_FUNCTOR_1(QTestSignalerD, signal_1F, uint)
            << CONNECT_FUNCTOR(QTestSignalerD, signal_1F);

    QTest::newRow(SIG_SIGNAL_1G "<true>")
            << true
            << QStringLiteral("Test Signal 1G")
            << QByteArrayLiteral(SIG_SIGNAL_1G)
            << (QVector<QVariant>() << true)
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 1G\")"
                              "->signal_1G(bool p1)")
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 1G\")"
                              "->signal_1G(bool p1=true)")
            << EMIT_FUNCTOR_1(QTestSignalerD, signal_1G, bool)
            << CONNECT_FUNCTOR(QTestSignalerD, signal_1G);

    QTest::newRow(SIG_SIGNAL_1G "<false>")
            << true
            << QStringLiteral("Test Signal 1G")
            << QByteArrayLiteral(SIG_SIGNAL_1G)
            << (QVector<QVariant>() << false)
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 1G\")"
                              "->signal_1G(bool p1)")
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 1G\")"
                              "->signal_1G(bool p1=false)")
            << EMIT_FUNCTOR_1(QTestSignalerD, signal_1G, bool)
            << CONNECT_FUNCTOR(QTestSignalerD, signal_1G);

    QTest::newRow(SIG_SIGNAL_1H)
            << true
            << QStringLiteral("Test Signal 1H")
            << QByteArrayLiteral(SIG_SIGNAL_1H)
            << (QVector<QVariant>() << QUrl(QStringLiteral("https://example.com/")))
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 1H\")"
                              "->signal_1H(QUrl p1)")
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 1H\")"
                              "->signal_1H(QUrl p1={https://example.com/})")
            << EMIT_FUNCTOR_1(QTestSignalerD, signal_1H, QUrl)
            << CONNECT_FUNCTOR(QTestSignalerD, signal_1H);

    QTest::newRow(SIG_SIGNAL_1I)
            << true
            << QStringLiteral("Test Signal 1I")
            << QByteArrayLiteral(SIG_SIGNAL_1I)
            << (QVector<QVariant>() << QVariant::fromValue<QObject*>(nullptr))
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 1I\")"
                              "->signal_1I(QObject* p1)")
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 1I\")"
                              "->signal_1I(QObject* p1=nullptr)")
            << EMIT_FUNCTOR_1(QTestSignalerD, signal_1I, QObject*)
            << CONNECT_FUNCTOR(QTestSignalerD, signal_1I);

    QTest::newRow(SIG_SIGNAL_2A)
            << false
            << QStringLiteral("Test Signal 2A")
            << QByteArrayLiteral(SIG_SIGNAL_2A)
            << (QVector<QVariant>() << uint(123) << QStringLiteral("abc"))
            << QStringLiteral("QTestSignaler*("
                              POINTER_MARK
                              ";\"Test Signal 2A\")"
                              "->signal_2A(uint p1, QString p2)")
            << QStringLiteral("QTestSignaler*("
                              POINTER_MARK
                              ";\"Test Signal 2A\")"
                              "->signal_2A(uint p1=123, QString p2=\"abc\")")
            << EMIT_FUNCTOR_2(QTestSignaler, signal_2A, uint, QString)
            << CONNECT_FUNCTOR(QTestSignaler, signal_2A);

    QTest::newRow(SIG_SIGNAL_2B)
            << false
            << QStringLiteral("Test Signal 2B")
            << QByteArrayLiteral(SIG_SIGNAL_2B)
            << (QVector<QVariant>()
                << QUuid(QStringLiteral("{83458634-6f02-42a5-ae3a-fb4ddd3eec38}"))
                << qulonglong(123456))
            << QStringLiteral("QTestSignaler*("
                              POINTER_MARK
                              ";\"Test Signal 2B\")"
                              "->signal_2B(QVariant p1, qulonglong p2)")
            << QStringLiteral("QTestSignaler*("
                              POINTER_MARK
                              ";\"Test Signal 2B\")"
                              "->signal_2B("
                              "QVariant p1=QUuid({83458634-6f02-42a5-ae3a-fb4ddd3eec38})"
                              ", qulonglong p2=123456)")
            << EMIT_FUNCTOR_2(QTestSignaler, signal_2B, QUuid, qulonglong)
            << CONNECT_FUNCTOR(QTestSignaler, signal_2B);

    QTest::newRow(SIG_SIGNAL_2C)
            << true
            << QStringLiteral("Test Signal 2C")
            << QByteArrayLiteral(SIG_SIGNAL_2C)
            << (QVector<QVariant>()
                << QStringLiteral("abc \"")
                << QStringLiteral("xyz \'"))
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 2C\")"
                              "->signal_2C(QString p1, QString p2)")
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 2C\")"
                              "->signal_2C("
                              "QString p1=\"abc \\\"\", "
                              "QString p2=\"xyz \\\'\")")
            << EMIT_FUNCTOR_2(QTestSignalerD, signal_2C, QString, QString)
            << CONNECT_FUNCTOR(QTestSignalerD, signal_2C);

    QTest::newRow(SIG_SIGNAL_2D)
            << true
            << QStringLiteral("Test Signal 2D")
            << QByteArrayLiteral(SIG_SIGNAL_2D)
            << (QVector<QVariant>()
                << QChar::fromLatin1('\x01')
                << (QVariantList()
                    << QRect(1, 2, 3, 4)
                    << QRectF(1.1, 2.2, 3.3, 4.4)
                    << QLine(1, 2, 3, 4)
                    << QLineF(1.1, 2.2, 3.3, 4.4)))
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 2D\")"
                              "->signal_2D(QVariant p1, QVariant p2)")
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 2D\")"
                              "->signal_2D(QVariant p1=QChar('\\x01')"
                              ", QVariant p2=QVariantList({"
                              "QRect({1,2,3,4}), "
                              "QRectF({1.1,2.2,3.3,4.4}), "
                              "QLine({1,2,3,4}), "
                              "QLineF({1.1,2.2,3.3,4.4})"
                              "}))")
            << EMIT_FUNCTOR_2(QTestSignalerD, signal_2D, QChar, QVariantList)
            << CONNECT_FUNCTOR(QTestSignalerD, signal_2D);

    QTest::newRow(SIG_SIGNAL_2E)
            << true
            << QStringLiteral("Test Signal 2E")
            << QByteArrayLiteral(SIG_SIGNAL_2E)
            << (QVector<QVariant>() << double(-1.23) << float(1.23))
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 2E\")"
                              "->signal_2E(double p1, float p2)")
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 2E\")"
                              "->signal_2E(double p1=-1.23, float p2=1.23)")
            << EMIT_FUNCTOR_2(QTestSignalerD, signal_2E, double, float)
            << CONNECT_FUNCTOR(QTestSignalerD, signal_2E);

    QTest::newRow(SIG_SIGNAL_3A)
            << false
            << QStringLiteral("Test Signal 3A")
            << QByteArrayLiteral(SIG_SIGNAL_3A)
            << (QVector<QVariant>()
                << uint(123) << QString()
                << QVariant::fromValue<QObject*>(this))
            << QStringLiteral("QTestSignaler*("
                              POINTER_MARK
                              ";\"Test Signal 3A\")"
                              "->signal_3A(uint p1, QString p2, QObject* p3)")
            << QStringLiteral("QTestSignaler*("
                              POINTER_MARK
                              ";\"Test Signal 3A\")"
                              "->signal_3A(uint p1=123, QString p2=\"\", "
                              "QObject* p3=" POINTER_MARK ")")
            << EMIT_FUNCTOR_3(QTestSignaler, signal_3A, uint, QString, QObject*)
            << CONNECT_FUNCTOR(QTestSignaler, signal_3A);

    QTest::newRow(SIG_SIGNAL_3B)
            << true
            << QStringLiteral("Test Signal 3B")
            << QByteArrayLiteral(SIG_SIGNAL_3B)
            << (QVector<QVariant>() << QString() << int(-123) << qlonglong(-12345))
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 3B\")"
                              "->signal_3B(QString p1, int p2, QVariant p3)")
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 3B\")"
                              "->signal_3B(QString p1=\"\", int p2=-123"
                              ", QVariant p3=qlonglong(-12345))")
            << EMIT_FUNCTOR_3(QTestSignalerD, signal_3B, QString, int, qlonglong)
            << CONNECT_FUNCTOR(QTestSignalerD, signal_3B);

    QTest::newRow(SIG_SIGNAL_3C)
            << true
            << QStringLiteral("Test Signal 3C")
            << QByteArrayLiteral(SIG_SIGNAL_3C)
            << (QVector<QVariant>()
                << QDate(2001,12,31)
                << QTime(20, 21, 22)
                << QDateTime(QDate(2001,12,31), QTime(20, 21, 22)))
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 3C\")"
                              "->signal_3C(QDate p1, QTime p2, QDateTime p3)")
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 3C\")"
                              "->signal_3C(QDate p1={2001-12-31}"
                              ", QTime p2={20:21:22}"
                              ", QDateTime p3={2001-12-31 20:21:22})")
            << EMIT_FUNCTOR_3(QTestSignalerD, signal_3C, QDate, QTime, QDateTime)
            << CONNECT_FUNCTOR(QTestSignalerD, signal_3C);

    QTest::newRow(SIG_SIGNAL_3D)
            << true
            << QStringLiteral("Test Signal 3D")
            << QByteArrayLiteral(SIG_SIGNAL_3D)
            << (QVector<QVariant>()
                << QUuid(QStringLiteral("{83458634-6f02-42a5-ae3a-fb4ddd3eec38}"))
                << QByteArray("abc \a \b \n \r \t \v \f \x01 \x02")
                << QVariant::fromValue<void*>(nullptr))
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 3D\")"
                              "->signal_3D(QUuid p1, QByteArray p2, void* p3)")
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 3D\")"
                              "->signal_3D("
                              "QUuid p1={83458634-6f02-42a5-ae3a-fb4ddd3eec38}"
                              ", QByteArray p2=\"abc \\a \\b \\n \\r \\t \\v"
                              " \\f \\x01 \\x02\", void* p3=nullptr)")
            << EMIT_FUNCTOR_3(QTestSignalerD, signal_3D, QUuid, QByteArray, void*)
            << CONNECT_FUNCTOR(QTestSignalerD, signal_3D);

    QTest::newRow(SIG_SIGNAL_4A)
            << true
            << QStringLiteral("Test Signal 4A")
            << QByteArrayLiteral(SIG_SIGNAL_4A)
            << (QVector<QVariant>()
                << QSize(1,2)
                << QSizeF(1.1, 2.2)
                << QPoint(3, 4)
                << QPointF(3.3, 4.4))
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 4A\")"
                              "->signal_4A(QSize p1, QSizeF p2, QPoint p3, QPointF p4)")
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 4A\")"
                              "->signal_4A(QSize p1={1,2}, QSizeF p2={1.1,2.2}"
                              ", QPoint p3={3,4}, QPointF p4={3.3,4.4})")
            << EMIT_FUNCTOR_4(QTestSignalerD, signal_4A, QSize, QSizeF
                                , QPoint, QPointF)
            << CONNECT_FUNCTOR(QTestSignalerD, signal_4A);

    QTest::newRow(SIG_SIGNAL_9A)
            << false
            << QStringLiteral("Test Signal 9A")
            << QByteArrayLiteral(SIG_SIGNAL_9A)
            << (QVector<QVariant>() << int(-1) << int(-2) << int(-3) << int(-4)
                << int(-5) << int(-6) << int(-7) << int(-8) << int(-9))
            << QStringLiteral("QTestSignaler*("
                              POINTER_MARK
                              ";\"Test Signal 9A\")"
                              "->signal_9A(int p1, int p2, int p3, int p4"
                              ", int p5, int p6, int p7, int p8, int p9)")
            << QStringLiteral("QTestSignaler*("
                              POINTER_MARK
                              ";\"Test Signal 9A\")"
                              "->signal_9A(int p1=-1, int p2=-2, int p3=-3"
                              ", int p4=-4, int p5=-5, int p6=-6, int p7=-7"
                              ", int p8=-8, int p9=-9)")
            << EMIT_FUNCTOR_9(QTestSignaler, signal_9A
                                , int, int, int, int, int, int, int, int, int)
            << CONNECT_FUNCTOR(QTestSignaler, signal_9A);

    QTest::newRow(SIG_SIGNAL_9B)
            << true
            << QStringLiteral("Test Signal 9B")
            << QByteArrayLiteral(SIG_SIGNAL_9B)
            << (QVector<QVariant>() << uint(1) << uint(2) << uint(3) << uint(4)
                << uint(5) << uint(6) << uint(7) << uint(8) << uint(9))
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 9B\")"
                              "->signal_9B(uint p1, uint p2, uint p3, uint p4"
                              ", uint p5, uint p6, uint p7, uint p8, uint p9)")
            << QStringLiteral("QTestSignalerD*("
                              POINTER_MARK
                              ";\"Test Signal 9B\")"
                              "->signal_9B(uint p1=1, uint p2=2, uint p3=3"
                              ", uint p4=4, uint p5=5, uint p6=6, uint p7=7"
                              ", uint p8=8, uint p9=9)")
            << EMIT_FUNCTOR_9(QTestSignalerD, signal_9B
                                , uint, uint, uint, uint, uint, uint, uint, uint, uint)
            << CONNECT_FUNCTOR(QTestSignalerD, signal_9B);
}
