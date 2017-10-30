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
#include "QUniversalSlot.h"

QUniversalSlot::QUniversalSlot(QObject* parent)
    : QSignalSlotMonitor(parent) {
}

QUniversalSlot::~QUniversalSlot() {
    if(isConnectedToEverything()) {
        disconnectEverything();
    }
}

QMetaObject::Connection QUniversalSlot::connect(const QObject* signaler
                                                , const char* signal) {
    QUNIVERSALSLOT_GUARD(signaler, QMetaObject::Connection()
                         , "Cannot connect to a null signaler.");
    QUNIVERSALSLOT_GUARD(signal, QMetaObject::Connection()
                         , "Cannot connect to a null signaler.");
    // +1 because SIGNAL() prepends a "2".
    const QByteArray sig = QMetaObject::normalizedSignature(signal + 1);
    const int methodIndex = signaler->metaObject()->indexOfSignal(sig.constData());
    return connect(signaler, methodIndex);
}

QVector<QMetaObject::Connection> QUniversalSlot::connectSignaler(const QObject* signaler) {
    QUNIVERSALSLOT_GUARD(signaler, QVector<QMetaObject::Connection>()
                         , "Cannot connect to a null signaler.");
    const QMetaObject* metaObject = signaler->metaObject();
    const int methodCount = metaObject->methodCount();
    QVector<QMetaObject::Connection> connections;
    connections.reserve(methodCount);
    for(int methodIndex = 0; methodIndex < methodCount; ++methodIndex) {
        if(metaObject->method(methodIndex).methodType() == QMetaMethod::Signal) {
            connections.append(connect(signaler, methodIndex));
        }
    }
    return connections;
}

bool QUniversalSlot::disconnectSignaler(const QObject* signaler) {
    QUNIVERSALSLOT_GUARD(signaler, false
                         , "Cannot disconnect from a null signaler.");
    return signaler->disconnect(this);
}

bool QUniversalSlot::isConnectedToEverything() const {
    return isMonitorEnabled();
}

void QUniversalSlot::connectEverything() {
    enableMonitor();
}

void QUniversalSlot::disconnectEverything() {
    disableMonitor();
}

QMetaObject::Connection QUniversalSlot::connect(const QObject* signaler
                                                , int methodIndex) {
    Q_ASSERT(signaler);
    QUNIVERSALSLOT_GUARD(methodIndex >= 0, QMetaObject::Connection()
                         , "Cannot connect to a invalid signal.");
    static const int universalIndex = QUniversalSlot::staticMetaObject
            .indexOfMethod("xslot(const void*const*)");
    return QMetaObject::connect(signaler, methodIndex, this, universalIndex);
}

void QUniversalSlot::xslot(const void* const* parametersPointers) {
    const SignalInfo signalInfo(sender(), -1, senderSignalIndex(), parametersPointers);
    universal(signalInfo.getSignaler(), signalInfo.getMetaMethod()
              , signalInfo.getParameters());
}

void QUniversalSlot::signalBegin(const SignalInfo& signalInfo) {
    universal(signalInfo.getSignaler(), signalInfo.getMetaMethod()
              , signalInfo.getParameters());
}
