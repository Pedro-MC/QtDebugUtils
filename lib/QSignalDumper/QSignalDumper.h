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
#ifndef QSIGNALDUMPER_H
#define QSIGNALDUMPER_H

#include "QUniversalSlot.h"
#include "QMethodStringifier.h"

#include <QIODevice>

class QSignalDumper : public QUniversalSlot {
    Q_OBJECT

public:

    /**
     * @brief Constructor.
     * @note The flags Dump, TargetQDebug and Marker are enabled by default.
     * @param parent
     */
    explicit QSignalDumper(QObject* parent = nullptr);

    /**
     * @brief Returns a pointer to the current QIODevice target.
     *        Default is nullptr.
     * @return Returns a QIODevice pointer or nullptr if none was previously set.
     */
    QIODevice* getTargetQIODevice() const;

    /**
     * @brief Set the QIODevice target to the given device pointer.
     * @param device Pointer to a QIODevice or nullptr to reset the target.
     */
    void setTargetQIODevice(QIODevice* device);

    /**
     * @brief Returns a pointer to the current QByteArray target.
     *        Default is nullptr.
     * @return Returns a QByteArray pointer or nullptr if none was previously set.
     */
    QByteArray* getTargetQByteArray() const;

    /**
     * @brief Set the QByteArray target to the given device pointer.
     * @param buffer Pointer to a QByteArray or nullptr to reset the target.
     */
    void setTargetQByteArray(QByteArray* byteArray);

    /**
     * @brief Returns the current marker. If the Flag::Marker is enabled, a marker
     *        will be prefixed to each output line.
     * @return Returns a QByteArray with the current marker.
     */
    const QByteArray& getMarker() const;

    /**
     * @brief Set the marker to prefix to each output line.
     * @param marker
     */
    void setMarker(const QByteArray& marker);

    /**
     * @brief Flags to control QSignalDumper's behaviour.
     */
    enum class Flag {
        Dump = 1,
        Parameters = 2,
        TargetQIODevice = 4,
        TargetQByteArray = 8,
        TargetQDebug = 16,
        Marker = 32,
    };

    /**
     * @brief Returns true if the feature corresponding to the given flag is enabled.
     *        Returns false otherwise.
     * @param flag A flag for the feature to check.
     * @return Returns true if enabled and false otherwise.
     */
    bool isEnabled(Flag flag = Flag::Dump) const;

    /**
     * @brief Returns true if the feature corresponding to the given flag is disabled.
     *        Returns false otherwise.
     * @param flag A flag for the feature to check.
     * @return Returns true if disabled and false otherwise.
     */
    bool isDisabled(Flag flag = Flag::Dump) const;

public slots:

    /**
     * @brief Enables the feature corresponding to the given flag.
     * @param flag A flag for the feature to be enabled.
     */
    void enable(Flag flag = Flag::Dump);

    /**
     * @brief Disables the feature corresponding to the given flag.
     * @param flag A flag for the feature to be disabled.
     */
    void disable(Flag flag = Flag::Dump);

private:

    /**
     * @brief This function is called for every signal connected to the dumper.
     * @param signaler Pointer to the signaler object.
     * @param signalMetaMethod The signal's QMetaMethod.
     * @param parameters The signal's parameters.
     */
    virtual void universal(QObject* signaler, const QMetaMethod& signalMetaMethod
                           , const QVector<QVariant>& parameters) override;

    uint _flags;
    QByteArray* _targetQByteArray;
    QIODevice* _targetQIODevice;
    QMethodStringifier _methodStringifier;
    QByteArray _buffer;
    QByteArray _marker;

};

#endif // QSIGNALDUMPER_H
