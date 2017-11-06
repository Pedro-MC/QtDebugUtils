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
#include "QSignalSlotMonitor.h"

/* Taken from qobject_p.h */
/* BEGIN */
struct QSignalSpyCallbackSet
{
    typedef void (*BeginCallback)(QObject *caller, int signal_or_method_index, void **argv);
    typedef void (*EndCallback)(QObject *caller, int signal_or_method_index);
    BeginCallback signal_begin_callback, slot_begin_callback;
    EndCallback signal_end_callback, slot_end_callback;
};

extern void qt_register_signal_spy_callbacks(const QSignalSpyCallbackSet &callback_set);
/* END */

QVector<QSignalSlotMonitor*> QSignalSlotMonitor::_monitors;
QVector<QSignalSlotMonitor::SignalInfo> QSignalSlotMonitor::_signalInfos;

QSignalSlotMonitor::QSignalSlotMonitor(QObject* parent)
    : QObject(parent) {
    _signalInfos.reserve(32);
}

QSignalSlotMonitor::~QSignalSlotMonitor() {
    disableMonitor();
}

bool QSignalSlotMonitor::isMonitorEnabled() const {
    return _monitors.contains(const_cast<QSignalSlotMonitor*>(this));
}

void QSignalSlotMonitor::enableMonitor() {
    if(isMonitorEnabled()) {
        return;
    }
    if(_monitors.isEmpty()) {
        static const QSignalSpyCallbackSet callbackSet = {
            &signalBeginCallback, &slotBeginCallback,
            &signalEndCallback, &slotEndCallback
        };
        qt_register_signal_spy_callbacks(callbackSet);
    }
    _monitors.append(this);
}

void QSignalSlotMonitor::disableMonitor() {
    if(_monitors.removeOne(this) && _monitors.isEmpty()) {
        static const QSignalSpyCallbackSet nullCallbackSet
                = {nullptr, nullptr, nullptr, nullptr};
        qt_register_signal_spy_callbacks(nullCallbackSet);
    }
}

void QSignalSlotMonitor::signalBegin(const SignalInfo& signalInfo) {
    Q_UNUSED(signalInfo);
}

void QSignalSlotMonitor::signalEnd(const SignalInfo& signalInfo) {
    Q_UNUSED(signalInfo);
}

void QSignalSlotMonitor::slotBegin(const SignalInfo& signalInfo
                                   , const SlotInfo& slotInfo) {
    Q_UNUSED(signalInfo);
    Q_UNUSED(slotInfo);
}

void QSignalSlotMonitor::slotEnd(const SignalInfo& signalInfo
                                 , const SlotInfo& slotInfo) {
    Q_UNUSED(signalInfo);
    Q_UNUSED(slotInfo);
}

void QSignalSlotMonitor::signalBeginCallback(QObject* signaler, int signalIndex
                                             , void** signalParametersPointers) {
    SignalInfo signalInfo(signaler, signalIndex, -1, signalParametersPointers);
    _signalInfos.push_back(signalInfo);
    for(QSignalSlotMonitor* callback : _monitors) {
        callback->signalBegin(signalInfo);
    }
}

void QSignalSlotMonitor::signalEndCallback(QObject* signaler, int signalIndex) {
    Q_ASSERT(_signalInfos.size() >= 1);
    const SignalInfo& signalInfo = _signalInfos.constLast();
    Q_ASSERT(signalInfo.getSignaler() == signaler);
    Q_ASSERT(signalInfo.getSignalIndex() == signalIndex);
    for(QSignalSlotMonitor* callback: _monitors) {
        callback->signalEnd(signalInfo);
    }
    _signalInfos.pop_back();
}

void QSignalSlotMonitor::slotBeginCallback(QObject* receiver, int methodIndex
                                           , void** signalParametersPointers) {
    Q_ASSERT(_signalInfos.size() >= 1);
    const SignalInfo& signalInfo = _signalInfos.constLast();
    Q_ASSERT(signalInfo.getParametersPointers() == signalParametersPointers);
    const SlotInfo slotInfo(receiver, methodIndex);
    for(QSignalSlotMonitor* callback : _monitors) {
        callback->slotBegin(signalInfo, slotInfo);
    }
}

void QSignalSlotMonitor::slotEndCallback(QObject* receiver, int methodIndex) {
    Q_ASSERT(_signalInfos.size() >= 1);
    const SignalInfo& signalInfo = _signalInfos.constLast();
    const SlotInfo slotInfo(receiver, methodIndex);
    for(QSignalSlotMonitor* callback : _monitors) {
        callback->slotEnd(signalInfo, slotInfo);
    }
}

QObject* QSignalSlotMonitor::SignalInfo::getSignaler() const {
    return _signaler;
}

int QSignalSlotMonitor::SignalInfo::getSignalIndex() const {
    if(_signalIndex == INDEX_UNKNOWN) {
        const QMetaObject* const metaObject = _signaler->metaObject();
        const int methodCount = metaObject->methodCount();
        for(int methodIndex = 0, signalIndex = -1; methodIndex < methodCount ; ++methodIndex) {
            if(metaObject->method(methodIndex).methodType() == QMetaMethod::Signal) {
                ++signalIndex;
                if(methodIndex == _methodIndex) {
                    const_cast<int&>(_signalIndex) = signalIndex;
                    break;
                }
            }
        }
    }
    return _signalIndex;
}

int QSignalSlotMonitor::SignalInfo::getMethodIndex() const {
    if(_methodIndex == INDEX_UNKNOWN) {
        const QMetaObject* const metaObject = _signaler->metaObject();
        const int methodCount = metaObject->methodCount();
        for(int methodIndex = 0, signalIndex = -1; methodIndex < methodCount; ++methodIndex) {
            if(metaObject->method(methodIndex).methodType() == QMetaMethod::Signal
               && (++signalIndex) == _signalIndex) {
                const_cast<int&>(_methodIndex) = methodIndex;
                break;
            }
        }
    }
    return _methodIndex;
}

QMetaMethod QSignalSlotMonitor::SignalInfo::getMetaMethod() const {
    return _signaler->metaObject()->method(getMethodIndex());
}

const void* const* QSignalSlotMonitor::SignalInfo::getParametersPointers() const {
    return _parametersPointers;
}

const QVector<QVariant>& QSignalSlotMonitor::SignalInfo::getParameters() const {
    if(getMetaMethod().parameterCount() != _parameters.count()) {
        const_cast<SignalInfo*>(this)->processParameters();
    }
    return _parameters;
}

void QSignalSlotMonitor::SignalInfo::processParameters() {
    Q_ASSERT(_signaler);
    const QMetaMethod metaMethod = getMetaMethod();
    Q_ASSERT(metaMethod.isValid());
    Q_ASSERT(metaMethod.methodType() == QMetaMethod::Signal);
    const int parameterCount = metaMethod.parameterCount();
    Q_ASSERT(_parametersPointers || parameterCount == 0);
    _parameters.reserve(parameterCount);
    for(int I = 0 ; I < parameterCount; ++I) {
        const QMetaType::Type type
                = static_cast<QMetaType::Type>(metaMethod.parameterType(I));
        // I + 1 because the parameter pointers start at index 1.
        const void* const parameterPointer = _parametersPointers[I + 1];
        if(type == QMetaType::QVariant) {
            _parameters.append(*reinterpret_cast<const QVariant*>(parameterPointer));
        } else {
            _parameters.append(QVariant(type, parameterPointer));
        }
    }
}

QSignalSlotMonitor::SignalInfo::SignalInfo(QObject* signaler
                                           , int signalIndex, int methodIndex
                                           , const void* const* parametersPointers)
    : _signaler(signaler)
    , _signalIndex(signalIndex)
    , _methodIndex(methodIndex)
    , _parametersPointers(parametersPointers)
    , _parameters() {
    Q_ASSERT(signaler);
    Q_ASSERT(signalIndex >= 0 || methodIndex >= 0);
    Q_ASSERT(parametersPointers);
    Q_ASSERT(signalIndex >= INDEX_UNKNOWN);
    Q_ASSERT(methodIndex >= INDEX_UNKNOWN);
    Q_ASSERT(signalIndex == INDEX_UNKNOWN || methodIndex == INDEX_UNKNOWN ||
             // set method index to unknown to force a recalculation from signal index.
             ((_methodIndex = INDEX_UNKNOWN) && methodIndex == getMethodIndex()));
    Q_ASSERT(getMetaMethod().methodType() == QMetaMethod::Signal);
}

QObject* QSignalSlotMonitor::SlotInfo::getReceiver() const {
    return _receiver;
}

int QSignalSlotMonitor::SlotInfo::getMethodIndex() const {
    return _methodIndex;
}

QMetaMethod QSignalSlotMonitor::SlotInfo::getMetaMethod() const {
    return _receiver->metaObject()->method(getMethodIndex());
}

QSignalSlotMonitor::SlotInfo::SlotInfo(QObject* receiver, int methodIndex)
    : _receiver(receiver)
    , _methodIndex(methodIndex) {
    Q_ASSERT(receiver);
    Q_ASSERT(methodIndex >= 0);
    Q_ASSERT(getMetaMethod().methodType() == QMetaMethod::Slot);
}
