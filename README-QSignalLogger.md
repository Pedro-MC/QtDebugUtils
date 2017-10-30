# QSignalLogger (QtDebugUtils)

**QSignalLogger** is a class that logs the signals it receives. **QSignalLogger** is derived from **QUniversalSlot** so it can be connected to any combination of signals, including **all** signals from **all** objects.

## API

**QSignalLogger** is derived from [**QUniversalSlot**](https://github.com/Pedro-MC/QtDebugUtils/blob/master/README-QUniversalSlot.md) and has the same API to connect/disconnect signals.

**QSignalLogger** has two available methods:

* `const QVector<QSignalLogger::Entry>& getLog()`
* `void clear()`

To get the information from a **QSignalLogger::Entry** instance the following methods are available:

* `const QPointer<QObject>& getSignaler()`
* `const void* getSignalerAddress()`
* `const QString& getSignalerObjectName()`
* `const QMetaObject* getSignalerMetaObject()`
* `const QMetaMethod& getSignalMetaMethod()`
* `const QVector<QVariant>& getParameters()`
* `QVariant getParameterByName(const QByteArray& name)`

## Examples

```C++
QSignalLogger logger;

QObject objA;
QObject objB;
QObject objC;
logger.connectSignaler(&objA);
logger.connect(&objB, SIGNAL(objectNameChanged(QString));
logger.connect(&objC, &QObject::objectNameChanged);

objA.setObjectName("Object A");
objB.setObjectName("Object B");
objC.setObjectName("Object C");

QMethodStringifier stringifier;
for(const QSignalLogger::Entry& entry : logger.getLog()) {
    const void* object = entry.getSignalerAddress();
    const QString& objectName = entry.getSignalerObjectName();
    const QMetaObject* metaObject = entry.getSignalerMetaObject();
    const QMetaMethod& metaMethod = entry.getSignalMetaMethod();
    const QVector<QVariant>& parameters = entry.getParameters();
    const QString strSignal = stringifier.stringify(object, objectName, metaObject, metaMethod, parameters);
    qDebug("SIGNAL: %s", strSignal.toLocal8Bit().data());
}
```
