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
#ifndef QTESTSIGNALER_H
#define QTESTSIGNALER_H

#include <QObject>
#include <QVariant>

class QTestSignaler : public QObject {
    Q_OBJECT

public:

    QTestSignaler(QObject* parent = nullptr);
    QTestSignaler(const QTestSignaler& other);

    Q_INVOKABLE void method_0A();

public slots:

    void slot_0A();
    void slotEmit_0A();

signals:

    void signal_0A();
    void signal_1A(int p1);
    void signal_1B(QString p1);
    void signal_1C(QObject* p1);
    void signal_1D(QStringList p1);
    void signal_1E(const QVariant& p1);
    void signal_2A(uint p1, QString p2);
    void signal_2B(const QVariant& p1, qulonglong p2);
    void signal_3A(uint p1, QString p2, QObject* p3);
    void signal_4A(const QSize& p1, const QSizeF& p2
                   , const QPoint& p3, const QPointF& p4);
    void signal_9A(int p1, int p2, int p3, int p4, int p5
                   , int p6, int p7, int p8, int p9);

};

#define SIG_METHOD_0A "method_0A()"
#define SIG_SLOT_0A "slot_0A()"
#define SIG_SLOT_EMIT_0A "slotEmit_0A()"
#define SIG_SIGNAL_0A "signal_0A()"
#define SIG_SIGNAL_1A "signal_1A(int)"
#define SIG_SIGNAL_1B "signal_1B(QString)"
#define SIG_SIGNAL_1C "signal_1C(QObject*)"
#define SIG_SIGNAL_1D "signal_1D(QStringList)"
#define SIG_SIGNAL_1E "signal_1E(QVariant)"
#define SIG_SIGNAL_2A "signal_2A(uint,QString)"
#define SIG_SIGNAL_2B "signal_2B(QVariant,qulonglong)"
#define SIG_SIGNAL_3A "signal_3A(uint,QString,QObject*)"
#define SIG_SIGNAL_4A "signal_4A(QSize,QSizeF,QPoint,QPointF)"
#define SIG_SIGNAL_9A "signal_9A(int,int,int,int,int,int,int,int,int)"

#define DECLARE_TEST_VALUES \
    int         const intVal       = -123; \
    uint        const uintVal      = 1234; \
    qulonglong  const ulongVal     = 12345; \
    QString     const strVal       = QStringLiteral("abc"); \
    QObject*    const objPtr       = this; \
    QSize       const sizeVal      = QSize(1, 2); \
    QSizeF      const sizefVal     = QSizeF(1.1, 2.2); \
    QPoint      const pointVal     = QPoint(3, 4); \
    QPointF     const pointfVal    = QPointF(3.3, 4.4); \
    QStringList const strListVal   = QStringList() << strVal << strVal << strVal; \
    QVariant    const varVal       = strListVal

#define EMIT_SIGNAL_0A(SIGNALER) emit (SIGNALER).signal_0A()
#define EMIT_SIGNAL_1A(SIGNALER) emit (SIGNALER).signal_1A(intVal)
#define EMIT_SIGNAL_1B(SIGNALER) emit (SIGNALER).signal_1B(strVal)
#define EMIT_SIGNAL_1C(SIGNALER) emit (SIGNALER).signal_1C(objPtr)
#define EMIT_SIGNAL_1D(SIGNALER) emit (SIGNALER).signal_1D(strListVal)
#define EMIT_SIGNAL_1E(SIGNALER) emit (SIGNALER).signal_1E(varVal)
#define EMIT_SIGNAL_2A(SIGNALER) emit (SIGNALER).signal_2A(uintVal, strVal)
#define EMIT_SIGNAL_2B(SIGNALER) emit (SIGNALER).signal_2B(varVal, ulongVal)
#define EMIT_SIGNAL_3A(SIGNALER) emit (SIGNALER).signal_3A(uintVal, strVal, objPtr)
#define EMIT_SIGNAL_4A(SIGNALER) emit (SIGNALER).signal_4A(sizeVal, sizefVal \
                                                          , pointVal, pointfVal)
#define EMIT_SIGNAL_9A(SIGNALER) emit (SIGNALER).signal_9A(1,2,3,4,5,6,7,8,9)

#define EMIT_ALL_SIGNALS(SIGNALER) \
    EMIT_SIGNAL_0A(SIGNALER); \
    EMIT_SIGNAL_1A(SIGNALER); \
    EMIT_SIGNAL_1B(SIGNALER); \
    EMIT_SIGNAL_1C(SIGNALER); \
    EMIT_SIGNAL_1D(SIGNALER); \
    EMIT_SIGNAL_1E(SIGNALER); \
    EMIT_SIGNAL_2A(SIGNALER); \
    EMIT_SIGNAL_2B(SIGNALER); \
    EMIT_SIGNAL_3A(SIGNALER); \
    EMIT_SIGNAL_4A(SIGNALER); \
    EMIT_SIGNAL_9A(SIGNALER)

#endif // QTESTSIGNALER_H
