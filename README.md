# QtDebugUtils

**QtDebugUtils** is a set of utility classes useful for debugging, unit testing and other non-evil uses.

**QtDebugUtils** includes:

* [**QAddressWiper**](https://github.com/Pedro-MC/QtDebugUtils/blob/master/README-QAddressWiper.md), a class to wipe memory addresses from strings to facilitate unit test comparisons.
* [**QValueStringifier**](https://github.com/Pedro-MC/QtDebugUtils/blob/master/README-QValueStringifier.md), a class to produce human friendly string representations of values.
* [**QObjectStringifier**](https://github.com/Pedro-MC/QtDebugUtils/blob/master/README-QObjectStringifier.md), a class to produce human friendly string representations of QObject derived class instances.
* [**QMethodStringifier**](https://github.com/Pedro-MC/QtDebugUtils/blob/master/README-QMethodStringifier.md), a class to produce human friendly string representations of method calls.
* [**QSignalSlotMonitor**](https://github.com/Pedro-MC/QtDebugUtils/blob/master/README-QSignalSlotMonitor.md), a class to monitor all emited signals and slots calls, using Qt internal API `qt_register_signal_spy_callbacks()`.
* [**QUniversalSlot**](https://github.com/Pedro-MC/QtDebugUtils/blob/master/README-QUniversalSlot.md), a class that provides a slot that can be connected to any signals, all signals from any objects or **all** signals from **all** objects.
* [**QSignalLogger**](https://github.com/Pedro-MC/QtDebugUtils/blob/master/README-QSignalLogger.md), a class that logs the signals it receives. **QSignalLogger** is derived from **QUniversalSlot** so it can log any combination of emited signals, including **all** signals from **all** objects.
* [**QSignalDumper**](https://github.com/Pedro-MC/QtDebugUtils/blob/master/README-QSignalDumper.md), a class that outputs string representations of the signals it receives. The output can be to a QIODevice, a QByteArray or QDebug. **QSignalDumper** is derived from **QUniversalSlot** so it can dump any combination of emited signals, including **all** signals from **all** objects.

## Dependencies

**QtDebugUtils** depends only on [Qt 5](https://www.qt.io/) and uses qmake for building. **QtDebugUtils** was developed and tested with Qt 5.9 but should work with older Qt 5 versions. Note that since **QSignalSlotMonitor** uses a internal Qt API, it is not protected by  Qt's public API stability guarantees.

## Unit Tests

All classes have tests that cover almost 100% of the code and pass all tests.

## Examples

Each class has a corresponding .md file that has some basic documentation and examples.
The classs' header files have the API documented.
The unit tests can also server as (somewhat convuluted) examples on how to use the classes.

## Building

To build the library and test units:
```shell
git clone https://github.com/Pedro-MC/QtDebugUtils.git
cd QtDebugUtils
qmake QtDebugUtils.pro CONFIG+=debug
make
```
Replace `CONFIG+=debug`with `CONFIG+=release` to build a release version (optimized binaries without debug data).

To install the library:
```shell
make install
```

To run the unit tests:
```shell
cd test
LD_LIBRARY_PATH=../lib ./test
```

If only some part is needed, instead of linking with the library, it may be more practical to just copy the needed files or code to another project, but first consider the copyright implications.

## Bugs and feature requests

If you find any bugs or have any feature requests please open an issue at [https://github.com/Pedro-MC/QtDebugUtils/issues]().

## Contributions

Contributions are welcomed.

See [AUTHORS](https://github.com/Pedro-MC/QtDebugUtils/blob/master/AUTHORS) for a list of contributors.

## License

**QtDebugUtils** is licensed under the [GNU Lesser General Public License v3](https://github.com/Pedro-MC/QtDebugUtils/blob/master/LICENSE).

Please contact the copyrights holder(s) if you are interested in a commercial license.
