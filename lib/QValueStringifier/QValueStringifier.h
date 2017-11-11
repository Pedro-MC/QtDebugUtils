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
#ifndef QVALUESTRINGIFIER_H
#define QVALUESTRINGIFIER_H

#include <QString>
#include <QVariant>
#include <QMetaType>

class QValueStringifier {

public:

    /**
     * @brief Stringifier functions must have this signature.
     */
    typedef void (*StringifierFunc)(const QVariant& var, QString& buffer);

    /**
     * @brief Constructor.
     * @param typeId Id of the type to be stringified.
     * @param func Pointer to the function that actually stringifies the value.
     * @param enableAtConstruction If true, the stringifier will be enabled at
     *                             construction.
     *                             If false, the enable() method needs to be
     *                             called explicitly to enable the stringifier.
     * @note Custom types must be declared with Q_DECLARE_METATYPE(TYPE).
     * @note The type id can be determine by using the function qMetaTypeId<TYPE>().
     * @note Multiple stringifiers for the same type can be constructed but
     *       only the most recently enabled stringifier will be used.
     */
    QValueStringifier(QMetaType::Type typeId, StringifierFunc stringifierFunc
                      , bool enableAtConstruction = true);

    /**
      * @brief Destructor.
      */
    ~QValueStringifier();

    /**
     * @brief Returns the type id.
     * @return
     */
    QMetaType::Type getTypeId() const;

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
     * @brief Enable thread safe operations.
     * @note Thread safe operations are disabled by default, to avoid overhead.
     * @note This function should be called before calling QValueStringifier(),
     *       enable() or disable() with multiple threads running.
     *       If all stringifiers are constructed and enabled/disabled before
     *       any threads are used then there is no need to call this function.
     */
    static void enableThreadSafe();

    /**
     * @brief Disable thread safe operations.
     * @note Thread safe operations are disabled by default, to avoid overhead.
     */
    static void disableThreadSafe();

    /**
     * @brief Stringify the given object and append it to the given buffer.
     * @param var QVariant containing the value to be stringified.
     * @param buffer Buffer where the stringified value will be appended, with
     *               the data already in the buffer being left unchanged.
     * @param withType If true, the stringified value will be prefixed by the
     *                 type and wrapped in parenthesis, like TYPE(STRVALUE).
     *                 If false, only the stringified value will be appended to
     *                 the given buffer.
     */
    static void stringify(const QVariant& var, QString& buffer
                          , bool withType = false);

private:

    QMetaType::Type _typeId;
    StringifierFunc _stringifierFunc;

};

#define DECLARE_VALUE_STRINGIFIER_FUNC(TYPE) \
    static void TYPE##StringifierFunc(const QVariant& var, QString& buffer)

#define REGISTER_VALUE_STRINGIFIER_FUNC(TYPE) \
    static QValueStringifier TYPE##StringifierFuncRegister \
    (static_cast<QMetaType::Type>(qMetaTypeId<TYPE>()), &TYPE##StringifierFunc)

#define IMPLEMENT_VALUE_STRINGIFIER(TYPE) \
    DECLARE_VALUE_STRINGIFIER_FUNC(TYPE); \
    REGISTER_VALUE_STRINGIFIER_FUNC(TYPE); \
    DECLARE_VALUE_STRINGIFIER_FUNC(TYPE)

#endif // QVALUESTRINGIFIER_H
