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
#ifndef QDEBUGUTILSTEST_H
#define QDEBUGUTILSTEST_H

#include <QObject>
#include <QtTest>
#include <functional>

//#include "QSignalSlotMonitor.h"

// Functor for invoking a method.
typedef std::function<void(QObject*, const QVector<QVariant>&)> EmitFunctor;
Q_DECLARE_METATYPE(EmitFunctor);

// Functor for connecting a signal.
typedef std::function<QMetaObject::Connection(class QUniversalSlot&, QObject*)> ConnectFunctor;
Q_DECLARE_METATYPE(ConnectFunctor);

class QDebugUtilsTest : public QObject {
    Q_OBJECT

public:

    QDebugUtilsTest();

private slots:

    void initTestCase();
    void cleanupTestCase();

    void testQAddressWiper();
    void testQAddressWiper_data();

    void testQValueStringifier();
    void testQValueStringifier_data();

    void testQObjectStringifier();
    void testQObjectStringifier_data();

    void testQMethodStringifier();
    void testQMethodStringifier_data();

    void testQSignalSlotMonitor();
    void testQSignalSlotMonitor_Lambda();
    void testQSignalSlotMonitor_SignalNoMonitorBenchmark();
    void testQSignalSlotMonitor_SignalWithMonitorBenchmark();
    void testQSignalSlotMonitor_SignalSlotNoMonitorBenchmark();
    void testQSignalSlotMonitor_SignalSlotWithMonitorBenchmark();
    void testQSignalSlotMonitor_data();

    void testQUniversalSlot();
    void testQUniversalSlot_Benchmark();
    void testQUniversalSlot_EverythingBenchmark();
    void testQUniversalSlot_data();

    void testQSignalLogger();
    void testQSignalLogger_data();

    void testQSignalDumper();
    void testQSignalDumper_data();

private:

    void test_data();

};

#endif // QDEBUGUTILSTEST_H
