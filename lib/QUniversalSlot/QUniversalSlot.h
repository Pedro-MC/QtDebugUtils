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
#ifndef QUNIVERSALSLOT_H
#define QUNIVERSALSLOT_H

#include "QSignalSlotMonitor.h"

#include <QObject>
#include <QVector>
#include <QMetaObject>
#include <QMetaMethod>

class QUniversalSlot : public QSignalSlotMonitor {
    Q_OBJECT

public:

    /**
     * @brief Constructor.
     * @param parent
     */
    explicit QUniversalSlot(QObject* parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~QUniversalSlot();

    /**
     * @brief Connects the given signal from the given signaler object to this
     *        object's universal slot.
     * @param signaler Pointer to the signaler object.
     * @param signal Must be a pointer to a signal function.
     * @return Returns a Connection object that can be used to check if the
     *         connection was successful and to disconnect this connection.
     */
    template<typename PointerToSignalFunction>
    QMetaObject::Connection connect(const QObject* signaler
                                    , PointerToSignalFunction signal);

    /**
     * @brief Connects the given signal from the given signaler object to this
     *        object's universal slot.
     * @param signaler Pointer to the signaler object.
     * @param signal String constructed using the SIGNAL macro.
     * @return Returns a Connection object that can be used to check if the
     *         connection was successful and to disconnect this connection.
     */
    QMetaObject::Connection connect(const QObject* signaler, const char* signal);

    /**
     * @brief Connects all the signals from the given signaler object to this
     *        object's universal slot.
     * @param signaler Pointer to the signaler object.
     * @return Returns a vector of Connection objects that can be used to check
     *         if the various signals were successfuly connected and to
     *         disconnect them.
     */
    QVector<QMetaObject::Connection> connectSignaler(const QObject* signaler);

    /**
     * @brief Disconnects all the signals from the given signaler object to this
     *        object's universal slot.
     * @param signaler Pointer to the signaler object.
     * @return Returns true on success and false on failure.
     */
    bool disconnectSignaler(const QObject* signaler);

    /**
     * @brief Returns true if this object's universal slot is connected to
     *        everything (all signals from all objects) and false otherwise.
     * @details This method is just a wrapper of the QSignalSlotMonitor::isMonitorEnabled().
     * @return Returns true if connected and false otherwise.
     */
    bool isConnectedToEverything() const;

    /**
     * @brief Connects every signal from every object to this object's universal
     *        slot. Even objects created after calling this function will be
     *        connected to this object's universal slot.
     * @details This method is just a wrapper of the QSignalSlotMonitor::enableMonitor().
     * @see disconnectEverything()
     */
    void connectEverything();

    /**
     * @brief Disconnects the connected with connectEverything().
     * @details This method is just a wrapper of the QSignalSlotMonitor::disableMonitor().
     * @see connectEverything()
     */
    void disconnectEverything();

private slots:

    /**
     * @brief This slot function is handled specially and receives a pointer to
     *        all the signal's parameters instead of just the first parameter.
     * @note To see how this is accomplished see the xmoc.sh and lib.pro files.
     * @param parametersPointers Pointer to an array with the signal's
     *        parameters pointers. The first parameter pointer is at index 1.
     */
    void xslot(const void* const* parametersPointers);

private:

    /**
     * @brief This function will be called when a signal to the universal slot
     *        is received.
     *        Signals explicitly connected to other slots are handled by those
     *        slots, as usual.
     * @param signaler Pointer to the signaler object.
     * @param signalMetaMethod The signal meta method.
     * @param parameters The signal parameters.
     */
    virtual void universal(QObject* signaler
                           , const QMetaMethod& signalMetaMethod
                           , const QVector<QVariant>& parameters) = 0;

    /**
     * @brief Connect the signal with the given signalIndex from the given
     *        signaler to this slot.
     * @param signaler Pointer to the signaler object.
     * @param methodIndex Signal's method index.
     * @return Returns a Connection object that can be used to check if the
     *         connection was successful and to disconnect this connection.
     */
    QMetaObject::Connection connect(const QObject* signaler, int methodIndex);

    /**
     * @brief This function is called after a signal is emited and before it is
     *        dispatched to any potential receivers.
     * @param signalInfo Signal's information.
     */
    virtual void signalBegin(const SignalInfo& signalInfo) override;
};

#define QUNIVERSALSLOT_GUARD(TEST, RETURN, WARN) \
    while(! (TEST)) { \
    qWarning("QUniversalSlot::%s : %s", __func__, (WARN)); \
    return (RETURN); \
    }

template<typename PointerToSignalFunction>
QMetaObject::Connection QUniversalSlot::connect(const QObject* signaler
                                                , PointerToSignalFunction signal) {
    QUNIVERSALSLOT_GUARD(signaler
                         , QMetaObject::Connection()
                         , "Cannot connect to a null signaler.");
    QUNIVERSALSLOT_GUARD(signal
                         , QMetaObject::Connection()
                         , "Cannot connect to a null signal function pointer.");
    const QMetaMethod signalMetaMethod = QMetaMethod::fromSignal(signal);
    QUNIVERSALSLOT_GUARD(signalMetaMethod.methodType() == QMetaMethod::Signal
                         , QMetaObject::Connection()
                         , "Cannot connect to a non signal function.");
    return connect(signaler, signalMetaMethod.methodIndex());
}

#endif // QUNIVERSALSLOT_H
