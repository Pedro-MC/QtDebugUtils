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
#ifndef QSIGNALLOGGER_H
#define QSIGNALLOGGER_H

#include "QUniversalSlot.h"

#include <QPointer>

class QSignalLogger : public QUniversalSlot {
    Q_OBJECT

public:

    class Entry {

        friend class QSignalLogger;
        template <typename T> friend class QVector;

    public:

        /**
         * @brief Returns a QPointer to the signaler object.
         * @note Check the pointer for validity before using it because the
         *       signaler may be deleted.
         * @return
         */
        const QPointer<QObject>& getSignaler() const;

        /**
         * @brief Returns the memory address of the signaler object.
         * @warning Do not use this pointer to access the object because there is
         *          no way of knowing if the object is still valid.
         *          Use getSignaler() if access to the object is intended.
         * @note The returned type is intentionally void* to prevent acidental usage.
         * @return
         */
        const void* getSignalerAddress() const;

        /**
         * @brief Returns the signaler's object name at the time the signal was
         *        emitted. May be empty.
         * @return
         */
        const QString& getSignalerObjectName() const;

        /**
         * @brief Returns the signaler's meta object.
         * @return
         */
        const QMetaObject* getSignalerMetaObject() const;

        /**
         * @brief Returns the signal's meta method.
         * @return
         */
        const QMetaMethod& getSignalMetaMethod() const;

        /**
         * @brief Returns the signal's parameters.
         * @return
         * @warning Be careful of pointers in the parameters that may point to
         *     objects/memory freed or relocated after the signal was emitted.
         */
        const QVector<QVariant>& getParameters() const;

        /**
         * @brief Returns a parameter's value for the parameter with the given name.
         * @param name Parameter's name.
         * @return Returns a QVariant with the parameter's value.
         *         Returns an invalid QVariant if there is not parameter with
         *         the given name.
         * @note Parameters' names are taken from the signals' declarations.
         *       Parameters in declarations are not required to have names.
         *       For those unnamed parameters this function will not work.
         */
        QVariant getParameterByName(const QByteArray& name) const;

    private:

        /**
         * @brief Default constructor needed by QVector<Entry>.
         */
        Entry() = default;

        /**
         * @brief Constructor.
         * @param signaler Pointer to the QObject that sent the signal.
         * @param methodIndex Signal's method index.
         * @param arguments Vector with the signal's parameters, in the correct
         *                  order.
         */
        Entry(QObject* _signaler, const QMetaMethod& methodIndex
              , const QVector<QVariant>& _parameters);

        QPointer<QObject> _signaler;
        QString _signalerObjectName;
        const void* _signalerAddress;
        const QMetaObject* _metaObject;
        const QMetaMethod _metaMethod;
        QVector<QVariant> _parameters;
    };

    /**
     * @brief Constructor.
     * @param parent
     * @param reserve Preallocates memory in the log for these many entries.
     */
    explicit QSignalLogger(QObject* parent = nullptr, uint reserve = 0);

    /**
     * @brief Returns the log.
     * @return
     */
    const QVector<Entry>& getLog() const;

    /**
     * @brief Clears the log.
     */
    void clear();

private:

    /**
     * @brief This function is called for every signal connected to the logger.
     * @param signaler Pointer to the signaler object.
     * @param signalMetaMethod The signal's QMetaMethod.
     * @param parameters The signal's parameters.
     */
    virtual void universal(QObject* signaler, const QMetaMethod& signalMetaMethod
                           , const QVector<QVariant>& parameters) override;

    /**
     * @brief The vector with the log entries.
     */
    QVector<Entry> _log;

};

#endif // QSIGNALLOGGER_H
