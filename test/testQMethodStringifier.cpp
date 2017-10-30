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
#include "QMethodStringifier.h"
#include "QTestSignaler.h"
#include "QTestSignalerD.h"
#include "QAddressWiper.h"

void QDebugUtilsTest::testQMethodStringifier() {
    QFETCH(bool, useDerived);
    QFETCH(QString, objectName);
    QFETCH(QByteArray, methodSignature);
    QFETCH(QVector<QVariant>, parameters);
    QFETCH(QString, expectedResultWithoutParameters);
    QFETCH(QString, expectedResultWithParameters);
    QFETCH(EmitFunctor, emitFunctor);

    static QTestSignaler testSignaler;
    static QTestSignalerD testSignalerD;
    QObject* object = useDerived ? &testSignalerD : &testSignaler;
    object->setObjectName(objectName);

    QMethodStringifier methodStringifier;
    const int methodIndex = object->metaObject()->indexOfMethod(methodSignature.data());
    const QMetaMethod metaMethod = object->metaObject()->method(methodIndex);

    emitFunctor(object, parameters);

    QCOMPARE(QStringAddressWiper::copywipe(
                 methodStringifier.stringify(object, methodIndex))
             , expectedResultWithoutParameters);

    QCOMPARE(QStringAddressWiper::copywipe(
                 methodStringifier.stringify(object, metaMethod))
             , expectedResultWithoutParameters);

    QCOMPARE(QStringAddressWiper::copywipe(
                 methodStringifier.stringify(object, methodIndex, parameters))
             , expectedResultWithParameters);

    QCOMPARE(QStringAddressWiper::copywipe(
                 methodStringifier.stringify(object, metaMethod, parameters))
             , expectedResultWithParameters);
}

void QDebugUtilsTest::testQMethodStringifier_data() {
    test_data();
}
