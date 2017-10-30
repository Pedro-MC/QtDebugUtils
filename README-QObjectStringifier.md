# QObjectStringifier (QtDebugUtils)

**QObjectStringifier** is a class used to produce human friendly string representations of QObjects.

## API

**QObjectStringifier** provides one method to do the work:
```C++
static void QObjectStringifier::stringify(const QObject* object, QString& buffer);
```

The string representation of the given object will be appended to the given buffer.

New QObject stringifiers can be defined by defining classes derived from **QObjectStringifier** and creating an instance of those classes.

The **QObjectStringifier** also connects to the **QValueStringifier** so  that `QValueStringifier::stringify()` will also use the instanciated **QObjectStringifier**s.

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

// Create a class derived from QObjectStringifier to stringify FooBar objects.
class FooBarStringifier : public QObjectStringifier {
public:
    FooBarStringifier() : QObjectStringifier(&FooBar::staticMetaObject) {}

private:
    virtual void doStringify(const QObject* object, QString& buffer) const override {
        const FooBar* const fb = qobject_cast<const FooBar*>(object);
	Q_ASSERT(fb);
        buffer.append(QLatin1Literal("FooBar{foo="));
        QValueStringifier::stringify(QVariant::fromValue<int>(fb->getFoo()), buffer);
        buffer.append(QLatin1Literal(";bar="));
        QValueStringifier::stringify(QVariant::fromValue<char>(fb->getBar()), buffer);
        buffer.append(QLatin1Char('}'));
    }
};

// Create an instance of the FooBar stringifier.
static const FooBarStringifier _FooBarStringifier;

#if 0
// The IMPLEMENT_QOBJECT_STRINGIFIER macro can be used to reduce the boilerplate code.
// The following code is produces code equivalent to the FooBarStringifier class and static instance above.
IMPLEMENT_QOBJECT_STRINGIFIER(FooBar) {
        const FooBar* const fb = qobject_cast<const FooBar*>(object);
	Q_ASSERT(fb);
        buffer.append(QLatin1Literal("FooBar{foo="));
        QValueStringifier::stringify(QVariant::fromValue<int>(fb->getFoo()), buffer);
        buffer.append(QLatin1Literal(";bar="));
        QValueStringifier::stringify(QVariant::fromValue<char>(fb->getBar()), buffer);
        buffer.append(QLatin1Char('}'));
}
#endif

int main() {
    QByteArray buffer;
    FooBar fb(9,'X');
    // Usualy,
    QValueStringifier::stringify(QVariant::fromValue<QObject*>(&fb), buffer);
    /* buffer == "FooBar{foo=9;bar='X'}" */
    buffer.clear();
    QValueStringifier::stringify(QVariant::fromValue<QObject*>(&fb), buffer, /*withType*/ true);
    /* buffer == "QObject*(FooBar{foo=9;bar='X'})" */
    return 0;
}
```
