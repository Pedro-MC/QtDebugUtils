# QUniversalSlot (QtDebugUtils)

**QUniversalSlot** is a class that provides a slot that can be connected to any signals, *all* signals from any set of objects and even **all** signals from **all** objects.

**QUniversalSlot**'s functionality is achieved by two approaches:

* Use of **QSignalSlotMonitor** to receive **all** signals from **all** objects;
* Use of a moc wrapper, xmoc, that wraps Qt's moc and modifies the xslot() call to pass it a pointer to all the signal's parameters instead of just the first parameter.

## API
To use **QUniversalSlot**, first create a class derived from **QUniversalSlot** and implement the virtual method:

```C++
virtual void universal(QObject* signaler, const QMetaMethod& signalMetaMethod, const QVector<QVariant>& parameters);
```
This method will be called each time a signal connected to the universal slot is emited.


Then use one or more of the connect methods to connect signals to the a universal slot instance.

```C++
template<typename PointerToSignalFunction>
QMetaObject::Connection QUniversalSlot::connect(const QObject* sender, PointerToSignalFunction signal);

QMetaObject::Connection QUniversalSlot::connect(const QObject* sender, const char* signal);

QVector<QMetaObject::Connection> QUniversalSlot::connectSignaler(const QObject* sender);

bool QUniversalSlot::connectEverything();
```

## Examples

```C++
#include "QSignalSlotMonitor.h"
#include "QMethodStringifier.h"

class QFooSlot : public  QUniversalSlot {
	Q_OBJECT

private:
	virtual void universal(QObject* signaler, const QMetaMethod& signalMetaMethod, const QVector<QVariant>& parameters) override {
		QMethodStringifier stringifier;
		QString strSignal = stringifier.stringify(signaler, signalMetaMethod, signalInfo.getParameters());
		qDebug("SIGNAL: %s", strSignal.toLocal8Bit().data());
	}
};

void foobar() {
	QObject objA; 
	QObject objB;
	QObject objC;
	QFooSlot slot;
	slot.connectSignaler(&objA);
	slot.connect(&objB, SIGNAL(objectNameChanged(QString));
	slot.connect(&objC, &QObject::objectNameChanged);
	
	objA.setObjectName("Object A");
	objB.setObjectName("Object B");
	objC.setObjectName("Object C");
}
```
