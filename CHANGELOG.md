# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/) and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2017-10-30
### Added

* **QAddressWiper**, a class to wipe memory addresses from strings to facilitate test comparisons.
* **QValueStringifier**, a class to produce human friendly string representations of values.
* **QObjectStringifier**, a class to produce human friendly string representations of QObject derived class instances.
* **QMethodStringifier**, a class to produce human friendly string representations of method calls.
* **QSignalSlotMonitor**, a class to monitor all emited signals and slots calls.
* **QUniversalSlot**, a class that provides a slot that can be connected to any explicitly specified signals, all signals from any explicitly specified objects or **all** signals from **all** objects.
* **QSignalLogger**, a class to log the signals it receives. **QSignalLogger** is derived from **QUniversalSlot** so it has **QUniversalSlot** connect capabilities.
* **QSignalDumper**, a class to dump a string representation of the signals it receives. **QSignalDumper** is derived from **QUniversalSlot** so it has **QUniversalSlot** connect capabilities.
