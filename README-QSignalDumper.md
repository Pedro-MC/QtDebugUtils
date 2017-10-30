# QSignalDumper (QtDebugUtils)

**QSignalDumper** is a class that outputs string representations of the signals it receives. The output can be to a QIODevice, a QByteArray or QDebug. **QSignalDumper** is derived from **QUniversalSlot** so it can be connected to any combination of signals, including **all** signals from **all** objects.

## API

**QSignalDumper** is derived from [**QUniversalSlot**](https://github.com/Pedro-MC/QtDebugUtils/blob/master/README-QUniversalSlot.md) and has the same API to connect/disconnect signals.

To get the state of a **QSignalDumper** instance the following methods are available:

* `QIODevice* getTargetQIODevice()`
* `QByteArray* getTargetQByteArray()`
* `const QByteArray& getMarker()`
* `bool isEnabled(Flag flag)`
* `bool isDisabled(Flag flag)`

To set the state of a **QSignalDumper** instance the following methods are available:

* `void setTargetQIODevice(QIODevice* device)`
* `void setTargetQByteArray(QByteArray* buffer)`
* `void setMarker(const QByteArray& marker)`
* `void enable(Flag flag)`
* `void disable(Flag flag)`

The following flags can be enabled/disabled to control a **QSignalDumper** instance behaviour:

* `Flag::Dump`, flag to control the actual dump;
* `Flag::Parameters`, flag to control the dump of signal/slot parameters;
* `Flag::TargetQIODevice`, flag to control the dump to the QIODevice target;
* `Flag::TargetQByteArray`, flag to control the dump to the QByteArray target;
* `Flag::TargetQDebug`, flag to control the dump to the a QDebug target;
* `Flag::Marker`, flag to control the dump of dump marker;

## Examples

```C++
void setupSignalDumper(QIODevice* dumpDevice) {
    static QSignalDumper dumper;
    dumper.setTargetQIODevice(dumpDevice);
    dumper.setMarker(QStringLiteral("[SIGNAL]"));
    dumper.disable(Flag::TargetQDebug);
    dumper.enable(Flag::TargetQIODevice);
    dumper.enable(Flag::DumpParameters);
    dumper.enable(Flag::Marker);
}
```
