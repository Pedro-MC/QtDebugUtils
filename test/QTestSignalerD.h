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
#ifndef QTESTSIGNALERD_H
#define QTESTSIGNALERD_H

#include "QTestSignaler.h"

class QTestSignalerD : public QTestSignaler {
    Q_OBJECT

public:

    QTestSignalerD(QObject* parent = nullptr);
    QTestSignalerD(const QTestSignalerD& other);

    Q_INVOKABLE void method_0B();

public slots:

    void slot_0B();
    void emit_signal_0B();

signals:

    void signal_0B();
    void signal_1F(uint p1);
    void signal_1G(bool p1);
    void signal_1H(const class QUrl& p1);
    void signal_1I(QObject* p1);
    void signal_2C(QString p1, QString p2);
    void signal_2D(QVariant p1, QVariant p2);
    void signal_2E(double p1, float p2);
    void signal_3B(QString p1, int p2, QVariant p3);
    void signal_3C(const QDate& p1, const QTime& p2, const QDateTime& p3);
    void signal_3D(const QUuid& p1, const QByteArray& p2, void* p3);
    void signal_9B(uint p1, uint p2, uint p3, uint p4, uint p5
                   , uint p6, uint p7, uint p8, uint p9);
};

#define SIG_METHOD_0B "method_0B()"
#define SIG_SLOT_0B "slot_0B()"
#define SIG_SIGNAL_0B "signal_0B()"
#define SIG_SIGNAL_1F "signal_1F(uint)"
#define SIG_SIGNAL_1G "signal_1G(bool)"
#define SIG_SIGNAL_1H "signal_1H(QUrl)"
#define SIG_SIGNAL_1I "signal_1I(QObject*)"
#define SIG_SIGNAL_2C "signal_2C(QString,QString)"
#define SIG_SIGNAL_2D "signal_2D(QVariant,QVariant)"
#define SIG_SIGNAL_2E "signal_2E(double,float)"
#define SIG_SIGNAL_3B "signal_3B(QString,int,QVariant)"
#define SIG_SIGNAL_3C "signal_3C(QDate,QTime,QDateTime)"
#define SIG_SIGNAL_3D "signal_3D(QUuid,QByteArray,void*)"
#define SIG_SIGNAL_9B "signal_9B(uint,uint,uint,uint,uint,uint,uint,uint,uint)"

#define DECLARE_TEST_VALUES_D \
    DECLARE_TEST_VALUES; \
    bool        const boolVal      = true; \
    float       const floatVal     = 1.23; \
    double      const doubleVal    = 1.2345; \
    QUrl        const urlVal       = QUrl(QStringLiteral("https://example.com/")); \
    QByteArray  const byteArrayVal = "xyz"; \
    QUuid       const uuidVal      = QUuid("{bc4f0637-4f1d-46c6-aef9-997e7464004b}"); \
    QDate       const dateVal      = QDate(1999, 12, 31); \
    QTime       const timeVal      = QTime(23, 59, 59); \
    QDateTime   const dateTimeVal  = QDateTime(dateVal, timeVal); \
    QObject*    const nullObjPtr   = nullptr; \
    void*       const voidPtr      = this

#define EMIT_SIGNAL_0B(SIGNALER) emit (SIGNALER).signal_0B()
#define EMIT_SIGNAL_1F(SIGNALER) emit (SIGNALER).signal_1F(uintVal)
#define EMIT_SIGNAL_1G(SIGNALER) emit (SIGNALER).signal_1G(boolVal)
#define EMIT_SIGNAL_1H(SIGNALER) emit (SIGNALER).signal_1H(urlVal)
#define EMIT_SIGNAL_1I(SIGNALER) emit (SIGNALER).signal_1I(nullObjPtr)
#define EMIT_SIGNAL_2C(SIGNALER) emit (SIGNALER).signal_2C(strVal, strVal)
#define EMIT_SIGNAL_2D(SIGNALER) emit (SIGNALER).signal_2D(varVal, varVal)
#define EMIT_SIGNAL_2E(SIGNALER) emit (SIGNALER).signal_2E(doubleVal, floatVal)
#define EMIT_SIGNAL_3B(SIGNALER) emit (SIGNALER).signal_3B(strVal, intVal, varVal)
#define EMIT_SIGNAL_3C(SIGNALER) emit (SIGNALER).signal_3C(dateVal, timeVal, dateTimeVal)
#define EMIT_SIGNAL_3D(SIGNALER) emit (SIGNALER).signal_3D(uuidVal, byteArrayVal, voidPtr)
#define EMIT_SIGNAL_9B(SIGNALER) emit (SIGNALER).signal_9B(1,2,3,4,5,6,7,8,9)

#define EMIT_ALL_SIGNALS_D(SIGNALER) \
    EMIT_ALL_SIGNALS(SIGNALER); \
    EMIT_SIGNAL_0B(SIGNALER); \
    EMIT_SIGNAL_1F(SIGNALER); \
    EMIT_SIGNAL_1G(SIGNALER); \
    EMIT_SIGNAL_1H(SIGNALER); \
    EMIT_SIGNAL_1I(SIGNALER); \
    EMIT_SIGNAL_2C(SIGNALER); \
    EMIT_SIGNAL_2D(SIGNALER); \
    EMIT_SIGNAL_2E(SIGNALER); \
    EMIT_SIGNAL_3B(SIGNALER); \
    EMIT_SIGNAL_3C(SIGNALER); \
    EMIT_SIGNAL_3D(SIGNALER); \
    EMIT_SIGNAL_9B(SIGNALER)

#endif // QTESTSIGNALERD_H
