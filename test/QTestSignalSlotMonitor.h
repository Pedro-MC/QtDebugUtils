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
#ifndef QTESTSIGNALSLOTMONITOR_H
#define QTESTSIGNALSLOTMONITOR_H

#include "QSignalSlotMonitor.h"

#include <QVector>
#include <QVariant>

class QTestSignalSlotMonitor : public QSignalSlotMonitor {
    Q_OBJECT

public:

    QTestSignalSlotMonitor(QObject* parent = nullptr);

    void enableMonitor() { QSignalSlotMonitor::enableMonitor(); }
    void disableMonitor() { QSignalSlotMonitor::disableMonitor(); }

    struct CallRecord {
        bool begin;
        QObject* signaler;
        QObject* receiver;
        int signalMethodIndex;
        int slotMethodIndex;
        QMetaMethod signalMetaMethod;
        QMetaMethod slotMetaMethod;
        const void* const* parametersPointers;
        QVector<QVariant> parameters;

        bool operator ==(const CallRecord& other) const;
    };

    QVector<CallRecord> callRecords;

private:

    virtual void signalBegin(const SignalInfo& signalInfo) override;
    virtual void signalEnd(const SignalInfo& signalInfo) override;
    virtual void slotBegin(const SignalInfo& signalInfo
                           , const SlotInfo& slotInfo) override;
    virtual void slotEnd(const SignalInfo& signalInfo
                         , const SlotInfo& slotInfo) override;

};

#endif // QTESTSIGNALSLOTMONITOR_H
