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
#include "QTestUniversalSlot.h"
#include "QTestSignaler.h"
#include "QTestSignalerD.h"

void QDebugUtilsTest::testQUniversalSlot_Benchmark() {
    DECLARE_TEST_VALUES_D;
    auto unislotFunc = [] (QObject*, const QMetaMethod&, const QVector<QVariant>&) {};
    QTestUniversalSlot unislot(unislotFunc);
    QTestSignaler testSignaler;
    QTestSignalerD testSignalerD;
    unislot.connectSignaler(&testSignaler);
    unislot.connectSignaler(&testSignalerD);
    QBENCHMARK {
        EMIT_ALL_SIGNALS(testSignaler);
        EMIT_ALL_SIGNALS_D(testSignalerD);
    }
}

void QDebugUtilsTest::testQUniversalSlot_EverythingBenchmark() {
    DECLARE_TEST_VALUES_D;
    auto unislotFunc = [] (QObject*, const QMetaMethod&, const QVector<QVariant>&) {};
    QTestUniversalSlot unislot(unislotFunc);
    QTestSignaler testSignaler;
    QTestSignalerD testSignalerD;
    unislot.connectEverything();
    QBENCHMARK {
        EMIT_ALL_SIGNALS(testSignaler);
        EMIT_ALL_SIGNALS_D(testSignalerD);
    }
}

void QDebugUtilsTest::testQUniversalSlot() {
    QFETCH(bool, useDerived);
    QFETCH(QString, objectName);
    QFETCH(QByteArray, methodSignature);
    QFETCH(QVector<QVariant>, parameters);
    QFETCH(EmitFunctor, emitFunctor);
    QFETCH(ConnectFunctor, connectFunctor);

    QTestSignaler testSignaler;
    QTestSignalerD testSignalerD;
    QObject* const signaler = useDerived ? &testSignalerD : &testSignaler;

    const int methodIndex = signaler->metaObject()->indexOfSignal(methodSignature);

    // Only test signals.
    if(methodIndex == -1) {
        return;
    }

    const QMetaMethod metaMethod = signaler->metaObject()->method(methodIndex);

    uint count = 0;
    QByteArray signalSignature(QByteArray("2").append(methodSignature));

    auto unislotFunc = [&] (QObject* _signaler, const QMetaMethod& _metaMethod
            , const QVector<QVariant>& _parameters) {
        QCOMPARE(signaler, _signaler);
        QCOMPARE(metaMethod, _metaMethod);
        QCOMPARE(parameters, _parameters);
        ++count;
    };

    QTestUniversalSlot unislot(unislotFunc);

    // Basically emits the signal and counts the number of times it is received.
    auto testConnection = [&] (uint expectedCount) {
        count = 0;
        emitFunctor(signaler, parameters);
        QCOMPARE(count, expectedCount);
    };

    // Test disconnected.
    testConnection(0);

    // Connect the signal using the signal signature.
    auto connectionA = unislot.connect(signaler, signalSignature.constData());
    QVERIFY(connectionA);
    // Test connected.
    testConnection(1);
    // Disconnect.
    unislot.disconnect(connectionA);
    // Test disconnected.
    testConnection(0);

    // Connect the signal using the signal signature.
    auto connectionB = connectFunctor(unislot, signaler);
    QVERIFY(connectionB);
    // Test connected.
    testConnection(1);
    // Disconnect.
    unislot.disconnect(connectionB);
    // Test disconnected.
    testConnection(0);

    // Connect all signal from the signaler.
    unislot.connectSignaler(signaler);
    // Test connected.
    testConnection(1);
    // Disconnect all signals from the signaler.
    unislot.disconnectSignaler(signaler);
    // Test disconnected.
    testConnection(0);

    // Connect all signals from all objects.
    unislot.connectEverything();
    // Test connected.
    testConnection(1);
    // Disconnect all signals.
    unislot.disconnectEverything();
    // Test disconnected.
    testConnection(0);

    // Test all at the same time.
    // Connect the signal using the signal signature.
    connectionA = unislot.connect(signaler, signalSignature.constData());
    QVERIFY(connectionA);
    // Test connected.
    testConnection(1);
    // Connect the signal again using the connect functor.
    connectionB = connectFunctor(unislot, signaler);
    QVERIFY(connectionB);
    // Test connected.
    testConnection(2);
    // Connect all signal from the signaler.
    unislot.connectSignaler(signaler);
    // Test connected.
    testConnection(3);
    // Connect all signals from all objects.
    unislot.connectEverything();
    // Test connected.
    testConnection(4);
    // Disconnect.
    unislot.disconnect(connectionA);
    // Test connected.
    testConnection(3);
    // Disconnect.
    unislot.disconnect(connectionB);
    // Test connected.
    testConnection(2);
    // Disconnect all signals from the signaler.
    unislot.disconnectSignaler(signaler);
    // Test connected.
    testConnection(1);
    // Disconnect all signals.
    unislot.disconnectEverything();
    // Test disconnected.
    testConnection(0);
}

void QDebugUtilsTest::testQUniversalSlot_data() {
    test_data();
}
