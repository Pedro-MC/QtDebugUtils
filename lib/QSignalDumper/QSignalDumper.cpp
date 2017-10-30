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

#include "QSignalDumper.h"

#include <QIODevice>
#include <QByteArray>
#include <QDebug>

QSignalDumper::QSignalDumper(QObject* parent)
    : QUniversalSlot(parent)
    , _flags(static_cast<uint>(Flag::Dump)
             | static_cast<uint>(Flag::TargetQDebug)
             | static_cast<uint>(Flag::Marker))
    , _targetQByteArray(nullptr)
    , _targetQIODevice(nullptr)
    , _methodStringifier()
    , _buffer()
    , _marker(QByteArrayLiteral("[QSignalDumper] ")) {
    _buffer.reserve(4096);
}

QIODevice* QSignalDumper::getTargetQIODevice() const {
    return _targetQIODevice;
}

void QSignalDumper::setTargetQIODevice(QIODevice* device) {
    _targetQIODevice = device;
}

QByteArray* QSignalDumper::getTargetQByteArray() const {
    return _targetQByteArray;
}

void QSignalDumper::setTargetQByteArray(QByteArray* byteArray) {
    _targetQByteArray = byteArray;
}

const QByteArray&QSignalDumper::getMarker() const {
    return _marker;
}

void QSignalDumper::setMarker(const QByteArray& marker) {
    _marker = marker;
}

void QSignalDumper::enable(QSignalDumper::Flag flag) {
    _flags |= static_cast<uint>(flag);
}

void QSignalDumper::disable(QSignalDumper::Flag flag) {
    _flags &= ~static_cast<uint>(flag);
}

bool QSignalDumper::isEnabled(QSignalDumper::Flag flag) const {
    return (_flags & static_cast<uint>(flag)) != 0;
}

bool QSignalDumper::isDisabled(QSignalDumper::Flag flag) const {
    return (_flags & static_cast<uint>(flag)) == 0;
}

void QSignalDumper::universal(QObject* signaler
                              , const QMetaMethod& signalMetaMethod
                              , const QVector<QVariant>& parameters) {
    if(isEnabled(Flag::Dump)) {
        if(isEnabled(Flag::Marker)) {
            _buffer.append(_marker);
        }
        _buffer.append(_methodStringifier
                      .stringify(signaler
                                 , signaler->objectName()
                                 , signaler->metaObject()
                                 , signalMetaMethod
                                 , isEnabled(Flag::Parameters)
                                 ? parameters : QVector<QVariant>())
                      .toLocal8Bit());
        if(isEnabled(Flag::TargetQDebug)) {
            qDebug(_buffer.data());
        }
        _buffer.append('\n');
        if(isEnabled(Flag::TargetQIODevice) && _targetQIODevice) {
            _targetQIODevice->write(_buffer);
        }
        if(isEnabled(Flag::TargetQByteArray) && _targetQByteArray) {
            _targetQByteArray->append(_buffer);
        }
        _buffer.truncate(0);
    }
}
