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
#ifndef QSIGNALSLOTMONITOR_H
#define QSIGNALSLOTMONITOR_H

#include <QObject>
#include <QVector>
#include <QVariant>
#include <QMetaMethod>

class QSignalSlotMonitor : public QObject {
    Q_OBJECT

public:

    /**
     * @brief Constructor.
     * @param parent
     */
    explicit QSignalSlotMonitor(QObject* parent = nullptr);

    /**
      * @brief Destructor.
      */
    ~QSignalSlotMonitor();

    /**
     * @brief Returns true if the monitor is enabled and false otherwise.
     * @return
     */
    bool isMonitorEnabled() const;

    /**
     * @brief Enables the monitor.
     */
    void enableMonitor();

    /**
     * @brief Disables the monitor.
     */
    void disableMonitor();

protected:

    class SignalInfo {
        template<typename T> friend class QVector;

    public:

        /**
         * @brief Constructor.
         * @param signaler Pointer to the signal's signaler/emiter.
         * @param signalIndex The signal's index. May be -1 if methodIndex is valid.
         * @param methodIndex The method's index. May be -1 if signalIndex is valid.
         * @param parametersPointers Pointers to the signal's parameters.
         */
        SignalInfo(QObject* signaler, int signalIndex, int methodIndex
                   , const void* const* parametersPointers);

        /**
         * @brief Returns a pointer to the signaler.
         * @return
         */
        QObject* getSignaler() const;

        /**
         * @brief Returns the signal's index.
         * @return
         */
        int getSignalIndex() const;

        /**
         * @brief Returns the signal's method index.
         * @return
         */
        int getMethodIndex() const;

        /**
         * @brief Returns the signal's QMetaMethod  object.
         * @return
         */
        QMetaMethod getMetaMethod() const;

        /**
         * @brief Returns the pointer to signal's parameters.
         * @note Use getParameters() if you want to acces the parameters values.
         * @return
         */
        const void* const* getParametersPointers() const;

        /**
         * @brief Returns the signal's parameters.
         * @return
         */
        const QVector<QVariant>& getParameters() const;

    private:

        /**
          * @brief Needed by QVector<SignalInfo>.
          */
        SignalInfo() = default;

        /**
         * @brief Creates a QVector<QVariant> from the parameters pointers.
         */
        void processParameters();

        QObject* _signaler;
        int _signalIndex;
        int _methodIndex;
        const void* const* _parametersPointers;
        QVector<QVariant> _parameters;
    };

    class SlotInfo {

    public:

        /** Constructor.
         * @brief SlotInfo
         * @param receiver Pointer to the signal's receiver.
         * @param methodIndex The receiving slot's method index.
         */
        SlotInfo(QObject* receiver, int methodIndex);

        /**
         * @brief Returns a pointer to the signal's receiver.
         * @return
         */
        QObject* getReceiver() const;

        /**
         * @brief Returns the receiving slot's method index.
         * @return
         */
        int getMethodIndex() const;

        /**
         * @brief Returns the receiving slot's QMetaMethod object.
         * @return
         */
        QMetaMethod getMetaMethod() const;

    private:

        QObject* _receiver;
        int _methodIndex;
    };

private:

    /**
     * @brief This function is called after a signal is emited and before it is
     *        dispatched to any potential receivers.
     * @param signalInfo Signal's information.
     */
    virtual void signalBegin(const SignalInfo& signalInfo);

    /**
     * @brief This function is called after a signal is emited and after it is
     *        dispatched to all potential receivers.
     * @param signalInfo Signal's information.
     */
    virtual void signalEnd(const SignalInfo& signalInfo);

    /**
     * @brief This function is called immediately before a slot receives a signal.
     * @param signalInfo The emited signal's information.
     * @param slotInfo The receiving slot's information.
     */
    virtual void slotBegin(const SignalInfo& signalInfo, const SlotInfo& slotInfo);

    /**
     * @brief This function is called immediately after a slot receives a signal.
     * @param signalInfo The emited signal's information.
     * @param slotInfo The receiving slot's information.
     */
    virtual void slotEnd(const SignalInfo& signalInfo, const SlotInfo& slotInfo);

    /**
     * @brief This functions is called by Qt's internal signal dispatcher at the
     *        beginning of the signal handling.
     * @param signaler The pointer to the signal's signaler/emiter.
     * @param signalIndex The signal's index.
     * @param signalParametersPointers The pointers to the signal's parameters.
     * @note The signal index is different from the method index.
     *       The signal index only accounts for the signal methods while the
     *       method index also accounts for slots and invocable methods.
     */
    static void signalBeginCallback(QObject* signaler, int signalIndex
                                    , void** signalParametersPointers);

    /**
     * @brief This functions is called by Qt's internal signal dispatcher at the
     *        beginning of the signal handling before it is dispatched to any
     *        receivers.
     * @param signaler The pointer to the signal's signaler/emiter.
     * @param signalIndex The signal's index.
     * @note The signal index is different from the method index.
     *       The signal index only accounts for the signal methods while the
     *       method index also accounts for slots and invocable methods.
     */
    static void signalEndCallback(QObject* signaler, int signalIndex);

    /**
     * @brief This functions is called by Qt's internal signal dispatcher before
     *        it calls each of the receiving slots.
     * @param signaler The pointer to the signal's signaler/emiter.
     * @param signalIndex The slot's method index.
     */
    static void slotBeginCallback(QObject* signaler, int slotMethodIndex
                                  , void** signalParametersPointers);

    /**
     * @brief This functions is called by Qt's internal signal dispatcher after
     *        it calls each of the receiving slots.
     * @param signaler The pointer to the signal's signaler/emiter.
     * @param signalIndex The slot's method index.
     */
    static void slotEndCallback(QObject* signaler, int slotMethodIndex);

    static QVector<QSignalSlotMonitor*> _monitors;
    static QVector<SignalInfo> _signalInfos;

};

#endif // QSIGNALSLOTMONITOR_H
