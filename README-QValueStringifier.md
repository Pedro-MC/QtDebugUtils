# QValueStringifier (QtDebugUtils)

**QValueStringifier** is a class used to produce human friendly string representations of values.

## API

**QValueStringifier** provides one method to do the work:
```C++
static void QValueStringifier::stringify(const QVariant& value, QString& buffer, bool withType = false);
```

The string representation of the given value will be appended to the given buffer.

It is possible to define new value stringifiers by defining a class derived from **QValueStringifier** and creating a instance of it.

## Examples

```C++
#include "QValueStringifier.h"
#include <QByteArray>

struct FooBar {
    int foo;
    char bar;
};

// Remember to declare the type to the Qt's metatype system.
Q_DECLARE_METATYPE(FooBar)

// Create a class derived from QValueStringifier to stringify FooBar values.
class FooBarStringifier : public QValueStringifier {
public:
    FooBarStringifier()
        : QValueStringifier(static_cast<QMetaType::Type>(QVariant::fromValue(FooBar{0,'A'}).userType())) {
    }
private:
    virtual void doStringify(const QVariant& var, QString& buffer) const override {
        Q_ASSERT(var.userType() == static_cast<int>(getType()));
        FooBar value = var.value<FooBar>();
        buffer.append(QLatin1Literal("{foo="));
        QValueStringifier::stringify(QVariant::fromValue<int>(value.foo), buffer);
        buffer.append(QLatin1Literal(";bar="));
        QValueStringifier::stringify(QVariant::fromValue<char>(value.bar), buffer);
        buffer.append(QLatin1Char('}'));
    }
};

// Create an instance of the foobar stringifier.
static const FooBarStringifier fooBarStringifier;

#if 0
// The IMPLEMENT_VALUE_STRINGIFIER macro can be used to reduce the boilerplate code.
// The following code is equivalent to the FooBarStringifier class and static instance above.
IMPLEMENT_VALUE_STRINGIFIER(FooBarStringifier, FooBar{0,'A'}) {
    Q_ASSERT(var.userType() == static_cast<int>(getType()));
    FooBar value = var.value<FooBar>();
    buffer.append(QLatin1Literal("{foo="));
    QValueStringifier::stringify(QVariant::fromValue<int>(value.foo), buffer);
    buffer.append(QLatin1Literal(";bar="));
    QValueStringifier::stringify(QVariant::fromValue<char>(value.bar), buffer);
    buffer.append(QLatin1Char('}'));
}
#endif

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
