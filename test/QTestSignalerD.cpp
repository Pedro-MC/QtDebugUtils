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

#include "QTestSignalerD.h"

QTestSignalerD::QTestSignalerD(QObject* parent)
    : QTestSignaler(parent) {
}

QTestSignalerD::QTestSignalerD(const QTestSignalerD& other)
    : QTestSignalerD(other.parent()) {
}

void QTestSignalerD::method_0B() {
}

void QTestSignalerD::slot_0B() {
}

void QTestSignalerD::emit_signal_0B() {
    emit signal_0B();
}
