/*******************************************************************************
** Copyright © 2017 Pedro Miguel Carvalho <PedroMC@pmc.com.pt>
**
** This file is part of QtDebugUtils.
**
** QtDebugUtils is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** any later version.
**
** QtDebugUtils is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with QtDebugUtils.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/
#ifndef QOBJECTSTRINGIFIER_H
#define QOBJECTSTRINGIFIER_H

#include <QObject>
#include <QMetaObject>

class QObjectStringifier {

public:

    /**
     * @brief Stringifier functions must have this signature.
     */
    typedef void (*StringifierFunc)(const QObject* object, QString& buffer);

    /**
     * @brief Constructor.
     * @param metaObject Pointer to the QMetaObject instance that corresponds
     *                   to the target class whose instances are to be
     *                   stringified by the stringifier being constructed.
     * @param stringifierFunc Pointer to the function that actually stringifies
     *                        instances of the target class.
     * @note Multiple stringifiers for the same class can be constructed but
     *       only the most recently enabled stringifier will be used.
     */
    QObjectStringifier(const QMetaObject* metaObject
                       , StringifierFunc stringifierFunc);

    /** Destructor. */
    ~QObjectStringifier();

    /**
     * @brief Returns the metaObject pointer.
     * @return
     */
    const QMetaObject* getMetaObject() const;

    /**
     * @brief Returns the stringifier function pointer.
     * @return
     */
    StringifierFunc getStringifierFunc() const;

    /**
     * @brief Enables the stringifier.
     */
    void enable();

    /**
     * @brief Disables the stringifier.
     */
    void disable();

    /**
     * @brief Stringify the given object and append it to the given buffer.
     * @param object Pointer to the object to be stringified. Can be an object
     *               from a class derived from QObject.
     * @param buffer Buffer where the string will be appended with any data
     *               already in the buffer being left unchanged.
     * @note When stringifying a instance of a particular class, if that
     *       particular class has a stringifier, then that stringifier is used.
     *       If that particular class does NOT have a stringifier, then the
     *       inheritance chain is followed, in order, until a super class with a
     *       stringifier is found. If no super class has a defined stringifier,
     *       then the default stringifier for the class QObject will be used.
     *       As an example, consider the following classes.
     *          class A : QObject {}
     *          class B : A {} // B has stringifier
     *          class C : B {} // C has stringifier
     *          class D : C {}
     *       Classes B and C have defined and enabled stringifiers.
     *       Instances of class D will be stringified by class C's stringifier.
     *       Instances of class C will be stringified by class C's stringifier.
     *       Instances of class B will be stringified by class B's stringifier.
     *       Instances of class A will be stringified by the default stringifier.
     */
    static void stringify(const QObject* object, QString& buffer);

private:

    const QMetaObject* _metaObject;
    StringifierFunc _stringifierFunc;

};

#define DECLARE_QOBJECT_STRINGIFIER_FUNC(CLASS) \
    static void CLASS##StringifierFunc(const QObject* object, QString& buffer)

#define REGISTER_OBJECT_STRINGIFIER_FUNC(CLASS) \
    static QObjectStringifier CLASS##StringifierFuncRegister \
    (&CLASS::staticMetaObject, &CLASS##StringifierFunc)

#define IMPLEMENT_QOBJECT_STRINGIFIER(CLASS) \
    DECLARE_QOBJECT_STRINGIFIER_FUNC(CLASS); \
    REGISTER_OBJECT_STRINGIFIER_FUNC(CLASS); \
    DECLARE_QOBJECT_STRINGIFIER_FUNC(CLASS)

#endif // QOBJECTSTRINGIFIER_H
