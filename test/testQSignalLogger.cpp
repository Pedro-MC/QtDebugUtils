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
#include "QSignalLogger.h"
#include "QTestSignaler.h"
#include "QTestSignalerD.h"

void QDebugUtilsTest::testQSignalLogger() {
    QFETCH(bool, useDerived);
    QFETCH(QString, objectName);
    QFETCH(QByteArray, methodSignature);
    QFETCH(QVector<QVariant>, parameters);
    QFETCH(EmitFunctor, emitFunctor);

    static QTestSignaler testSignaler;
    static QTestSignalerD testSignalerD;
    QObject* const signaler = useDerived ? &testSignalerD : &testSignaler;
    const int methodIndex = signaler->metaObject()->indexOfSignal(methodSignature);

    // Only test signals.
    if(methodIndex == -1) {
        return;
    }

    const QMetaMethod metaMethod = signaler->metaObject()->method(methodIndex);
    signaler->setObjectName(objectName);

    // Prepare logger.
    QSignalLogger logger;
    logger.connectSignaler(signaler);
    QVERIFY(logger.getLog().isEmpty());

    for(int I = 1 ; I <= 16 ; ++I) {
        // Emit signal.
        emitFunctor(signaler, parameters);

        // Check log size.
        QCOMPARE(logger.getLog().size(), I);

        // Check last log entry.
        const QSignalLogger::Entry& lastEntry = logger.getLog().constLast();
        QCOMPARE(lastEntry.getSignaler().data(), signaler);
        QCOMPARE(lastEntry.getSignalerAddress(), signaler);
        QCOMPARE(lastEntry.getSignalerObjectName(), objectName);
        QCOMPARE(lastEntry.getSignalerMetaObject(), signaler->metaObject());
        QCOMPARE(lastEntry.getSignalMetaMethod(), metaMethod);
        QCOMPARE(lastEntry.getParameters(), parameters);
        QCOMPARE(lastEntry.getSignalMetaMethod().parameterCount()
                 , lastEntry.getParameters().count());
        for(int J = 0 ; J < parameters.count() ; ++J) {
            QByteArray parameterName
                    = lastEntry.getSignalMetaMethod().parameterNames().at(J);
            QByteArray expectedParameterName =
                    QByteArray("p").append(QByteArray::number(J + 1));
            QCOMPARE(parameterName, expectedParameterName);
            QCOMPARE(lastEntry.getParameterByName(parameterName), parameters.at(J));
        }
    }

    // Test log clear.
    logger.clear();
    QVERIFY(logger.getLog().isEmpty());
}

void QDebugUtilsTest::testQSignalLogger_data() {
    test_data();
}
