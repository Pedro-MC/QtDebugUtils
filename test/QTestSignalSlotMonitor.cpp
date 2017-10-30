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

#include "QTestSignalSlotMonitor.h"

QTestSignalSlotMonitor::QTestSignalSlotMonitor(QObject* parent)
    : QSignalSlotMonitor(parent) {
}

void QTestSignalSlotMonitor::signalBegin(const SignalInfo& signalInfo) {
    callRecords.push_back(CallRecord{
                              true,
                              signalInfo.getSignaler(),
                              nullptr,
                              signalInfo.getMethodIndex(),
                              -1,
                              signalInfo.getMetaMethod(),
                              QMetaMethod(),
                              signalInfo.getParametersPointers(),
                              signalInfo.getParameters(),
                          });
}

void QTestSignalSlotMonitor::signalEnd(const SignalInfo& signalInfo) {
    callRecords.push_back(CallRecord{
                              false,
                              signalInfo.getSignaler(),
                              nullptr,
                              signalInfo.getMethodIndex(),
                              -1,
                              signalInfo.getMetaMethod(),
                              QMetaMethod(),
                              signalInfo.getParametersPointers(),
                              signalInfo.getParameters(),
                          });
}

void QTestSignalSlotMonitor::slotBegin(const SignalInfo& signalInfo
                                       , const SlotInfo& slotInfo) {
    callRecords.push_back(CallRecord{
                              true,
                              signalInfo.getSignaler(),
                              slotInfo.getReceiver(),
                              signalInfo.getMethodIndex(),
                              slotInfo.getMethodIndex(),
                              signalInfo.getMetaMethod(),
                              slotInfo.getMetaMethod(),
                              signalInfo.getParametersPointers(),
                              signalInfo.getParameters(),
                          });
}

void QTestSignalSlotMonitor::slotEnd(const SignalInfo& signalInfo
                                     , const SlotInfo& slotInfo) {
    callRecords.push_back(CallRecord{
                              false,
                              signalInfo.getSignaler(),
                              slotInfo.getReceiver(),
                              signalInfo.getMethodIndex(),
                              slotInfo.getMethodIndex(),
                              signalInfo.getMetaMethod(),
                              slotInfo.getMetaMethod(),
                              signalInfo.getParametersPointers(),
                              signalInfo.getParameters(),
                          });
}

bool QTestSignalSlotMonitor::CallRecord::operator ==(const QTestSignalSlotMonitor::CallRecord& other) const {
    return signaler == other.signaler
            && receiver == other.receiver
            && signalMethodIndex == other.signalMethodIndex
            && slotMethodIndex == other.slotMethodIndex
            && signalMetaMethod == other.signalMetaMethod
            && slotMetaMethod == other.slotMetaMethod
            && parametersPointers == other.parametersPointers;
}
