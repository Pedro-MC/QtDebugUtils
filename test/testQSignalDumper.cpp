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
#include "QSignalDumper.h"
#include "QTestSignaler.h"
#include "QTestSignalerD.h"
#include "QAddressWiper.h"

static QByteArray bufferQD;

static void qDebugHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
    Q_UNUSED(context);
    switch (type) {
        case QtDebugMsg: bufferQD.append(msg.toLocal8Bit()).append('\n'); break;
        default: break;
    }
}

void QDebugUtilsTest::testQSignalDumper() {
    QFETCH(bool, useDerived);
    QFETCH(QString, objectName);
    QFETCH(QByteArray, methodSignature);
    QFETCH(QVector<QVariant>, parameters);
    QFETCH(QString, expectedResultWithoutParameters);
    QFETCH(QString, expectedResultWithParameters);
    QFETCH(EmitFunctor, emitFunctor);

    static QTestSignaler testSignaler;
    static QTestSignalerD testSignalerD;
    QObject* signaler = useDerived ? &testSignalerD : &testSignaler;

    // Only test signals.
    if(signaler->metaObject()->indexOfSignal(methodSignature) == -1) {
        return;
    }

    signaler->setObjectName(objectName);

    // Prepare the buffers.
    QBuffer bufferIO;
    QByteArray bufferBA;
    QByteArray bufferEX;
    bufferIO.open(QIODevice::WriteOnly);
    bufferIO.buffer().reserve(4096);
    bufferBA.reserve(4096);
    bufferEX.reserve(4096);
    bufferQD.reserve(4096);
    bufferQD.truncate(0);

    QByteArray marker("[QSignalDumper] ");
    QSignalDumper dumper;

    const auto checkFlags = [&] (bool flagDump, bool flagDumpParameters
            , bool flagMarker, bool flagTargetQDebug
            , bool flagTargetQByteArray, bool flagTargetQIODevice) {
        QCOMPARE(dumper.isEnabled (),   flagDump);
        QCOMPARE(dumper.isDisabled(), ! flagDump);
        QCOMPARE(dumper.isEnabled (QSignalDumper::Flag::Dump),   flagDump);
        QCOMPARE(dumper.isDisabled(QSignalDumper::Flag::Dump), ! flagDump);
        QCOMPARE(dumper.isEnabled (QSignalDumper::Flag::Parameters),   flagDumpParameters);
        QCOMPARE(dumper.isDisabled(QSignalDumper::Flag::Parameters), ! flagDumpParameters);
        QCOMPARE(dumper.isEnabled (QSignalDumper::Flag::TargetQDebug),   flagTargetQDebug);
        QCOMPARE(dumper.isDisabled(QSignalDumper::Flag::TargetQDebug), ! flagTargetQDebug);
        QCOMPARE(dumper.isEnabled (QSignalDumper::Flag::TargetQIODevice),   flagTargetQIODevice);
        QCOMPARE(dumper.isDisabled(QSignalDumper::Flag::TargetQIODevice), ! flagTargetQIODevice);
        QCOMPARE(dumper.isEnabled (QSignalDumper::Flag::TargetQByteArray),   flagTargetQByteArray);
        QCOMPARE(dumper.isDisabled(QSignalDumper::Flag::TargetQByteArray), ! flagTargetQByteArray);
        QCOMPARE(dumper.isEnabled (QSignalDumper::Flag::Marker),   flagMarker);
        QCOMPARE(dumper.isDisabled(QSignalDumper::Flag::Marker), ! flagMarker);
    };

    const auto checkTargets = [&] (QByteArray* bufferBAPtr, QIODevice* bufferIOPtr) {
        QCOMPARE(dumper.getTargetQByteArray(), bufferBAPtr);
        QCOMPARE(dumper.getTargetQIODevice() , bufferIOPtr);
    };

    const auto emitSignal = [&] () {
        qInstallMessageHandler(&qDebugHandler);
        emitFunctor(signaler, parameters);
        qInstallMessageHandler(nullptr);
    };

    const auto emitAndCheckBuffers = [&] (const QString& expected) {
        emitSignal();

        bufferEX.append(marker).append(expected.toLocal8Bit()).append('\n');

        QByteArrayAddressWiper::wipe(bufferIO.buffer());
        QByteArrayAddressWiper::wipe(bufferBA);
        QByteArrayAddressWiper::wipe(bufferEX);
        QByteArrayAddressWiper::wipe(bufferQD);

        QCOMPARE(bufferIO.buffer(), bufferBA);
        QCOMPARE(bufferIO.buffer(), bufferEX);
        QCOMPARE(bufferIO.buffer(), bufferQD);
    };

    // Test default flags.
    checkFlags(true, false, true, true, false, false);

    // Test default targets and marker.
    QCOMPARE(dumper.getTargetQByteArray(), nullptr);
    QCOMPARE(dumper.getTargetQIODevice() , nullptr);
    QCOMPARE(dumper.getMarker(), marker);

    // Enable all targets.
    dumper.enable(QSignalDumper::Flag::TargetQDebug);
    dumper.enable(QSignalDumper::Flag::TargetQByteArray);
    dumper.enable(QSignalDumper::Flag::TargetQIODevice);
    checkFlags(true, false, true, true, true, true);

    // Set marker.
    dumper.setMarker(marker = QByteArrayLiteral("[QSignalDumper][TEST] "));
    QCOMPARE(dumper.getMarker(), marker);

    // Set targets.
    dumper.setTargetQByteArray(&bufferBA);
    dumper.setTargetQIODevice (&bufferIO);
    checkTargets(&bufferBA, &bufferIO);

    // Connect all signals from the signaler to the dumper.
    dumper.connectSignaler(signaler);

    // Dump with parameters.
    dumper.enable(QSignalDumper::Flag::Parameters);
    checkFlags(true, true, true, true, true, true);
    emitAndCheckBuffers(expectedResultWithParameters);

    // Dump without parameters.
    dumper.disable(QSignalDumper::Flag::Parameters);
    checkFlags(true, false, true, true, true, true);
    emitAndCheckBuffers(expectedResultWithoutParameters);

    // Disable all targets.
    dumper.disable(QSignalDumper::Flag::TargetQByteArray);
    dumper.disable(QSignalDumper::Flag::TargetQIODevice);
    dumper.disable(QSignalDumper::Flag::TargetQDebug);
    checkFlags(true, false, true, false, false, false);

    // Reset targets.
    dumper.setTargetQByteArray(nullptr);
    dumper.setTargetQIODevice (nullptr);
    checkTargets(nullptr, nullptr);

    // Test with all targets disabled.
    const int size = bufferBA.size();
    emitSignal();
    // Check that buffers are unchanged.
    QCOMPARE(bufferIO.buffer().size(), size);
    QCOMPARE(bufferBA.size(), size);
    QCOMPARE(bufferEX.size(), size);
    QCOMPARE(bufferQD.size(), size);
}

void QDebugUtilsTest::testQSignalDumper_data() {
    test_data();
}
