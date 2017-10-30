# QMethodStringifier (QtDebugUtils)

**QMethodStringifier** is a class used to produce human friendly string representations of method calls using Qt's QMetaObject data.

## API

**QMethodStringifier** provides three methods to do the work:
```C++
QString QMethodStringifier::stringify(const QObject* object, int methodIndex,
                                      const QVector<QVariant>& parameters = QVector<QVariant>());

QString stringify(const QObject* object, const QMetaMethod& metaMethod
                  , const QVector<QVariant>& parameters = QVector<QVariant>());

QString QMethodStringifier::stringify(const QObject* object, const QMetaObject* metaObject,
                                      const QString& objectName, int methodIndex,
                                      const QVector<QVariant>& parameters = QVector<QVariant>());
```

The first and second methods must get a pointer to a still valid object while the third can receive a pointer to a non longer valid object.

## Examples

```C++
const QObject* object = new QObject;
const QString objectName = object->objectName();
const QMetaObject* metaObject = object->metaObject();
const int methodIndex = metaObject->indexOfMethod("objectNameChanged(QString)");
const QMetaMethod metaMethod = metaObject->method(methodIndex);
const QVector<QVariant> parameters = QVector<QVariant>() << QStringLiteral("Test Object");

QMethodStringifier stringifier;
QString stringified;

stringified = stringify(object, methodIndex);
/* stringified == "QObject*(0xf0342350)->objectNameChanged(QString objectName)" */

stringified = stringify(object, methodIndex, parameters);
/* stringified == "QObject*(0xf0342350)->objectNameChanged(QString objectName=\"Test Object\")" */

stringified = stringify(object, metaMethod);
/* stringified == "QObject*(0xf0342350)->objectNameChanged(QString objectName)" */

stringified = stringify(object, metaMethod, parameters);
/* stringified == "QObject*(0xf0342350)->objectNameChanged(QString objectName=\"Test Object\")" */

// The object can be deleted and the second QMethodStringifier::stringify() method still works.
delete object;

stringified = stringify(object, metaObject, objectName, methodIndex);
/* stringified == "QObject*(0xf0342350)->objectNameChanged(QString objectName)" */

stringified = stringify(object, metaObject, objectName, methodIndex, parameters);
/* stringified == "QObject*(0xf0342350)->objectNameChanged(QString objectName=\"Test Object\")" */

```
