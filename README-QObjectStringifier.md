# QObjectStringifier (QtDebugUtils)

**QObjectStringifier** is a class used to produce human friendly string representations of QObjects.

## API

**QObjectStringifier** provides the following static method to do the stringification:

* `stringify(const QObject* object, QString& buffer)`

The string representation of the given object will be appended to the given buffer.

**QObjectStringifier** allows thread safe operations but must be explicitly enabled/disabled using the following static methods:

* `enableThreadSafe()` to enable thread safe operations (disabled by default due to overhead).
* `disableThreadSafe()` to disable thread safe operations.

A new QObject stringifier can be defined by defining a stringifier function and instanciating a **QObjectStringifier** with the stringifier function as a parameter. The macro `IMPLEMENT_QOBJECT_STRINGIFIER(CLASS)` can help with the boilerplate code.

A **QObjectStringifier** instance can be enable or disabled using the following methods:

* `enable()`.
* `disable()`.

The **QObjectStringifier** also connects to the **QValueStringifier** so  that `QValueStringifier::stringify()` will also use the **QObjectStringifier** stringifiers when stringifying QObject pointers.

## Examples

```C++
#include "QObjectStringifier.h"
#include <QByteArray>

class FooBar : public QObject {
Q_OBJECT
public:
	FooBar(int foo, char bar) : QObject(), _foo(foo), _bar(bar) {}
	int getFoo() const { return _foo; }
	char getBar() const { return _bar; }

private:
    int _foo;
    char _bar;
};

// Implement a stringifier function for the FooBar class instances.
IMPLEMENT_QOBJECT_STRINGIFIER(FooBar) {
    const FooBar* const fb = qobject_cast<const FooBar*>(object);
    Q_ASSERT(fb);
    buffer.append(QLatin1Literal("FooBar{foo="));
    QValueStringifier::stringify(QVariant::fromValue<int>(fb->getFoo()), buffer);
    buffer.append(QLatin1Literal(";bar="));
    QValueStringifier::stringify(QVariant::fromValue<char>(fb->getBar()), buffer);
    buffer.append(QLatin1Char('}'));
}

int main() {
    QByteArray buffer;
    FooBar fb(9,'X');
    QObjectStringifier::stringify(&fb, buffer);
    // Since QObjectStringifier is regitered with QValueStringifier the following line is equivalent.
    // QValueStringifier::stringify(QVariant::fromValue<QObject*>(&fb), buffer);
    /* buffer == "FooBar{foo=9;bar='X'}" */
    buffer.clear();
    QValueStringifier::stringify(QVariant::fromValue<QObject*>(&fb), buffer, /*withType*/ true);
    /* buffer == "QObject*(FooBar{foo=9;bar='X'})" */
    return 0;
}
```
