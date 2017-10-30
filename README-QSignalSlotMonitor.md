# QSignalSlotMonitor (QtDebugUtils)

**QSignalSlotMonitor** is a class to monitor **all** emited signals and slots calls, using Qt's internal API `qt_register_signal_spy_callbacks()`.

Because **QSignalSlotMonitor** uses a internal API, it does not enjoys the same stability guarantees as provided by Qt's public APIs. Still, in practice, it should be stable enough for practical use, at least until the next major version.

## API

To use **QSignalSlotMonitor**, first create a class derived from **QSignalSlotMonitor** and override one or more of the following virtual functions:

* `virtual void signalBegin(const SignalInfo& signalInfo);`
* `virtual void signalEnd(const SignalInfo& signalInfo);`
* `virtual void slotBegin(const SignalInfo& signalInfo, const SlotInfo& slotInfo);`
* `virtual void slotEnd(const SignalInfo& signalInfo, const SlotInfo& slotInfo);`

To get the signal information from **QSignalSlotMonitor::SignalInfo** use the following methods:

* `getSignaler()` to get the signaler object.
* `getSignaIndex()` to get the signal's index.
* `getMethodIndex()` to get the signal's  method index.
* `getMetaMethod()` to get the signal's  QMetaMethod object.
* `getParametersPointers()` to get the pointer to signal's parameters.
* `getParameters()` to get the signal's parameters.

To get the slot information from **QSignalSlotMonitor::SlotInfo** use the following methods:

* `getReceiver()` to get the receiver object.
* `getMethodIndex()` to get the slot's  method index.
* `getMetaMethod()` to get the slot's  QMetaMethod object.

Then just instanciate a object of the derived class and use the methods:

* `isMonitorEnabled()` to check if the monitor is enabled;
* `enableMonitor()` to enable the monitor;
* `disableMonitor()` to disable the monitor.

## Examples

```C++
#include "QSignalSlotMonitor.h"
#include "QMethodStringifier.h"

class QSlotMonitor : public  QSignalSlotMonitor {
	Q_OBJECT
	
public:

	QSlotMonitor(QObject* parent)
		: QSignalSlotMonitor(parent) {
	}

private:

	virtual void slotBegin(const SignalInfo& signalInfo, const SlotInfo& slotInfo) override {
		QMethodStringifier stringifier;
		QString strSignal = stringifier.stringify(signalInfo.getSignaler()
						, signalInfo.getMethodIndex(), signalInfo.getParameters());
		QString strSlot = stringifier.stringify(slotInfo.getReceiver()
						, slotInfo.getMethodIndex(), signalInfo.getParameters());
		qDebug("SIGNAL: %s", strSignal.toLocal8Bit().data());
		qDebug("SLOT  : %s", strSlot.toLocal8Bit().data());
	}
};
```
