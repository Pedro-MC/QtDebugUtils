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
#include "QSignalSlotMonitor.h"
#include "QTestSignaler.h"
#include "QTestSignalerD.h"
#include "QTestSignalSlotMonitor.h"

void QDebugUtilsTest::testQSignalSlotMonitor() {
    QFETCH(bool, useDerived);
    QFETCH(QString, objectName);
    QFETCH(QByteArray, methodSignature);
    QFETCH(QVector<QVariant>, parameters);
    QFETCH(QString, expectedResultWithoutParameters);
    QFETCH(QString, expectedResultWithParameters);
    QFETCH(EmitFunctor, emitFunctor);
    QFETCH(ConnectFunctor, connectFunctor);

    static const int slot0AIndex = QTestSignaler::staticMetaObject.indexOfSlot(SIG_SLOT_0A);
    static const int slotEmit0AIndex = QTestSignaler::staticMetaObject.indexOfSlot(SIG_SLOT_EMIT_0A);
    static const int signal0AIndex = QTestSignaler::staticMetaObject.indexOfSignal(SIG_SIGNAL_0A);
    static const QMetaMethod slot0AMethod = QTestSignaler::staticMetaObject.method(slot0AIndex);
    static const QMetaMethod slotEmit0AMethod = QTestSignaler::staticMetaObject.method(slotEmit0AIndex);
    static const QMetaMethod signal0AMethod = QTestSignaler::staticMetaObject.method(signal0AIndex);
    static const QVector<QVariant> noParameters;
    static QTestSignaler testSignaler;
    static QTestSignalerD testSignalerD;

    QObject* const signaler = useDerived ? &testSignalerD : &testSignaler;
    const int signalMethodIndex = signaler->metaObject()->indexOfSignal(methodSignature);
    const QMetaMethod signalMetaMethod = signaler->metaObject()->method(signalMethodIndex);

    // Only test signals.
    if(signalMethodIndex == -1) {
        return;
    }

    // Check a signal/slot call.
    auto checkCall = []
            ( const QTestSignalSlotMonitor& _monitor
            , int beginIndex, int endIndex
            , const QObject* _signaler
            , const QMetaMethod& _signalMetaMethod
            , const QVector<QVariant>& _parameters
            , const QObject* _receiver = nullptr
            , const QMetaMethod& _slotMetaMethod = QMetaMethod()) {
        const auto r = _monitor.callRecords;
        for(int I : {beginIndex, endIndex}) {
            QVERIFY(I < r.count());
            auto rI = r.at(I);
            QCOMPARE(rI.begin, I == beginIndex);
            QCOMPARE(rI.signaler, _signaler);
            QCOMPARE(rI.receiver, _receiver);
            QCOMPARE(rI.signalMethodIndex, _signalMetaMethod.methodIndex());
            QCOMPARE(rI.slotMethodIndex, _slotMetaMethod.methodIndex());
            QCOMPARE(rI.signalMetaMethod, _signalMetaMethod);
            QCOMPARE(rI.slotMetaMethod, _slotMetaMethod);
            QCOMPARE(rI.parameters.count(), _parameters.count());
            QCOMPARE(rI.parameters, _parameters);
        }
    };

    QTestSignalSlotMonitor monitorA;
    QTestSignalSlotMonitor monitorB;
    auto& callRecordsA = monitorA.callRecords;
    auto& callRecordsB = monitorB.callRecords;

    // With monitors A and B disabled.
    // Check signal with no slot connected.
    emitFunctor(signaler, parameters);
    QCOMPARE(callRecordsA.count(), 0);
    QCOMPARE(callRecordsB.count(), 0);

    // With monitor A enabled, monitor B disabled.
    // Check signal with no slot connected.
    monitorA.enableMonitor();
    emitFunctor(signaler, parameters);
    monitorA.disableMonitor();
    QCOMPARE(callRecordsA.count(), 2);
    QCOMPARE(callRecordsB.count(), 0);
    checkCall(monitorA, 0, 1, signaler, signalMetaMethod, parameters);
    callRecordsA.clear();

    // With monitor A disabled, monitor B enabled.
    // Check signal with no slot connected.
    monitorB.enableMonitor();
    emitFunctor(signaler, parameters);
    monitorB.disableMonitor();
    QCOMPARE(callRecordsA.count(), 0);
    QCOMPARE(callRecordsB.count(), 2);
    checkCall(monitorB, 0, 1, signaler, signalMetaMethod, parameters);
    callRecordsB.clear();

    // Check monitors A and B enabled.
    // Check signal with no slot connected.
    monitorA.enableMonitor();
    monitorB.enableMonitor();
    emitFunctor(signaler, parameters);
    monitorA.disableMonitor();
    monitorB.disableMonitor();
    QCOMPARE(callRecordsA.count(), 2);
    QCOMPARE(callRecordsB.count(), 2);
    QCOMPARE(monitorA.callRecords, monitorB.callRecords);
    checkCall(monitorA, 0, 1, signaler, signalMetaMethod, parameters);
    callRecordsA.clear();
    callRecordsB.clear();

    // With monitors A and B enabled.
    // Check with signal connected to slot_0A.
    const QMetaObject::Connection connectionB
            = connect(signaler, signalMetaMethod, &testSignalerD, slot0AMethod);
    monitorA.enableMonitor();
    monitorB.enableMonitor();
    emitFunctor(signaler, parameters);
    monitorA.disableMonitor();
    monitorB.disableMonitor();
    signaler->disconnect(connectionB);
    QCOMPARE(callRecordsA.count(), 4);
    QCOMPARE(callRecordsB.count(), 4);
    QCOMPARE(monitorA.callRecords, monitorB.callRecords);
    checkCall(monitorA, 0, 3, signaler, signalMetaMethod, parameters);
    checkCall(monitorA, 1 , 2, signaler, signalMetaMethod, parameters, &testSignalerD, slot0AMethod);
    callRecordsA.clear();
    callRecordsB.clear();

    if(methodSignature != SIG_SIGNAL_0A) {
        // With monitors A and B enabled.
        // Check with signal connected to slotEmit_0A
        // that will emit signal_0A that is connected to slot0A.
        const QMetaObject::Connection connectionC
                = connect(signaler, signalMetaMethod, &testSignalerD, slotEmit0AMethod);
        const QMetaObject::Connection connectionD
                = connect(&testSignalerD, signal0AMethod, signaler, slot0AMethod);
        monitorA.enableMonitor();
        monitorB.enableMonitor();
        emitFunctor(signaler, parameters);
        monitorA.disableMonitor();
        monitorB.disableMonitor();
        signaler->disconnect(connectionC);
        signaler->disconnect(connectionD);
        QCOMPARE(callRecordsA.count(), 8);
        QCOMPARE(callRecordsB.count(), 8);
        QCOMPARE(monitorA.callRecords, monitorB.callRecords);
        checkCall(monitorA, 0, 7, signaler, signalMetaMethod, parameters);
        checkCall(monitorA, 1 , 6, signaler, signalMetaMethod, parameters, &testSignalerD, slotEmit0AMethod);
        checkCall(monitorA, 2 , 5, &testSignalerD, signal0AMethod, noParameters);
        checkCall(monitorA, 3 , 4, &testSignalerD, signal0AMethod, noParameters, signaler, slot0AMethod);
        callRecordsA.clear();
        callRecordsB.clear();
    }
}

void QDebugUtilsTest::testQSignalSlotMonitor_data() {
    test_data();
}

void QDebugUtilsTest::testQSignalSlotMonitor_Lambda() {
    // Check that lambda function are NOT treated as slots.
    QTestSignaler signaler;
    QTestSignalSlotMonitor monitor;
    connect(&signaler, &QObject::objectNameChanged, [] () {});
    monitor.enableMonitor();
    signaler.setObjectName(QStringLiteral("Test Signaler"));
    QCOMPARE(monitor.callRecords.count(), 2);
}

void QDebugUtilsTest::testQSignalSlotMonitor_SignalNoMonitorBenchmark() {
    QTestSignaler signaler;

    QBENCHMARK {
        emit signaler.signal_0A();
    }
}

void QDebugUtilsTest::testQSignalSlotMonitor_SignalWithMonitorBenchmark() {
    QTestSignaler signaler;
    QSignalSlotMonitor monitor;
    monitor.enableMonitor();

    QBENCHMARK {
        emit signaler.signal_0A();
    }
}

void QDebugUtilsTest::testQSignalSlotMonitor_SignalSlotNoMonitorBenchmark() {
    QTestSignaler signaler;
    connect(&signaler, SIGNAL(signal_0A()), &signaler, SLOT(slot_0A()));

    QBENCHMARK {
        emit signaler.signal_0A();
    }
}

void QDebugUtilsTest::testQSignalSlotMonitor_SignalSlotWithMonitorBenchmark() {
    QTestSignaler signaler;
    QSignalSlotMonitor monitor;
    monitor.enableMonitor();
    connect(&signaler, SIGNAL(signal_0A()), &signaler, SLOT(slot_0A()));

    QBENCHMARK {
        emit signaler.signal_0A();
    }
}
