# QValueStringifier (QtDebugUtils)

**QValueStringifier** is a class used to produce human friendly string representations of values.

## API

**QValueStringifier** provides the following static method to do the stringification:

* `stringify(const QVariant& var, QString& buffer)`

The string representation of the value contained in the given var will be appended to the given buffer.

**QValueStringifier** allows thread safe operations  (disabled by default due to overhead) but must be explicitly enabled/disabled using the following static methods:

* `enableThreadSafe()` to enable thread safe operations.
* `disableThreadSafe()` to disable thread safe operations.

A new value stringifier can be defined by defining a stringifier function and instanciating a **QValueStringifier** with the stringifier function as a parameter. The macro `IMPLEMENT_VALUE_STRINGIFIER(TYPE)` can help with the boilerplate code.

Custom types need to be declared to Qt's metatype system using the macro `Q_DECLARE_METATYPE(TYPE)` before a **QValueStringifier** for the custom type can be created.

A **QValueStringifier** instance can be enable or disabled using the following methods:

* `enable()`.
* `disable()`.

## Examples

```C++
#include "QValueStringifier.h"
#include <QByteArray>

struct FooBar {
    int foo;
    char bar;
};

// Remember to declare the custom type to the Qt's metatype system.
Q_DECLARE_METATYPE(FooBar)

// Implement a stringifier function for FooBar instances.
IMPLEMENT_QOBJECT_STRINGIFIER(FooBar) {
        Q_ASSERT(var.userType() == qMetaTypeId<FooBar>());
        FooBar value = var.value<FooBar>();
        buffer.append(QLatin1Literal("{foo="));
        QValueStringifier::stringify(QVariant::fromValue<int>(value.foo), buffer);
        buffer.append(QLatin1Literal(";bar="));
        QValueStringifier::stringify(QVariant::fromValue<char>(value.bar), buffer);
        buffer.append(QLatin1Char('}'));
    }
};

int main() {
    QByteArray buffer;
    FooBar fb{9,'X'};
    QValueStringifier::stringify(QVariant::fromValue(fb), buffer);
    /* buffer == "{foo=9;bar='X'}" */
    buffer.clear();
    QValueStringifier::stringify(QVariant::fromValue(fb), buffer, /*withType*/ true);
    /* buffer == "FooBar({foo=9;bar='X'})" */
    return 0;
}
```
