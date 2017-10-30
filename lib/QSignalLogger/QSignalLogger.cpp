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
#include "QSignalLogger.h"

QSignalLogger::QSignalLogger(QObject* parent, uint reserve)
    : QUniversalSlot(parent)
    , _log() {
    _log.reserve(reserve);
}

const QVector<QSignalLogger::Entry>& QSignalLogger::getLog() const {
    return _log;
}

void QSignalLogger::clear() {
    _log.clear();
}

QSignalLogger::Entry::Entry(QObject* signaler, const QMetaMethod& metaMethod
                            , const QVector<QVariant>& parameters)
    : _signaler(signaler)
    , _signalerObjectName(signaler->objectName())
    , _signalerAddress(signaler)
    , _metaObject(signaler->metaObject())
    , _metaMethod(metaMethod)
    , _parameters(parameters) {
}

const QPointer<QObject>& QSignalLogger::Entry::getSignaler() const {
    return _signaler;
}

const void* QSignalLogger::Entry::getSignalerAddress() const {
    return _signalerAddress;
}

const QString& QSignalLogger::Entry::getSignalerObjectName() const {
    return _signalerObjectName;
}

const QMetaObject* QSignalLogger::Entry::getSignalerMetaObject() const {
    return _metaObject;
}

const QMetaMethod& QSignalLogger::Entry::getSignalMetaMethod() const {
    return _metaMethod;
}

const QVector<QVariant>& QSignalLogger::Entry::getParameters() const {
    return _parameters;
}

QVariant QSignalLogger::Entry::getParameterByName(const QByteArray& name) const {
    return _parameters.value(getSignalMetaMethod().parameterNames().indexOf(name));
}


void QSignalLogger::universal(QObject* signaler, const QMetaMethod& signalMetaMethod
                              , const QVector<QVariant>& parameters) {
    _log.append(Entry(signaler, signalMetaMethod, parameters));
}
