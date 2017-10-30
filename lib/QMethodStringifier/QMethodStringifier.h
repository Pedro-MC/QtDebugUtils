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
#ifndef QMETHODSTRINGIFIER_H
#define QMETHODSTRINGIFIER_H

#include <QVector>
#include <QVariant>
#include <QMetaObject>
#include <QMetaMethod>

class QMethodStringifier {

public:

    /**
     * @brief Constructor
     * @param parent
     */
    QMethodStringifier();

    /**
     * @brief Returns a string representation of a method call.
     * @param object The object pointer. Must be non null.
     * @param methodIndex The index of the method. Must be a valid method index
     *                    for the object.
     * @param parameters If the given parameters are empty, no parameter data
     *                   will be included in the string.
     *                   If the given parameters are not empty, the count and
     *                   types of the parameters must be equal or greater to the
     *                   method's parameter count.
     * @return If all the parameters are valid, returns a string like
     *         "OBJECT_CLASS*(OBJECT_ADDRESS;"OBJECT_NAME")
     *             ->METHOD_NAME(PARAM_TYPE PARAM_NAME ...)"
     *         or
     *         "OBJECT_CLASS*(OBJECT_ADDRESS;"OBJECT_NAME")
     *             ->METHOD_NAME(PARAM_TYPE PARAM_NAME=PARAM_VALUE ...)",
     *         depending on the presence or the parameters or not.
     *         If the object, method or parameters are invalid, returns a empty
     *         string.
     */
    QString stringify(const QObject* object, int methodIndex
                      , const QVector<QVariant>& parameters = QVector<QVariant>());

    /**
     * @brief Returns a string representation of a method call.
     * @param object The object pointer. Must be non null.
     * @param metaMethod The method QMetaMethod object. Must be valid.
     * @param parameters If the given parameters are empty, no parameter data
     *                   will be included in the string.
     *                   If the given parameters are not empty, the count and
     *                   types of the parameters must be equal or greater to the
     *                   method's parameter count.
     * @return If all the parameters are valid, returns a string like
     *         "OBJECT_CLASS*(OBJECT_ADDRESS;"OBJECT_NAME")
     *             ->METHOD_NAME(PARAM_TYPE PARAM_NAME ...)"
     *         or
     *         "OBJECT_CLASS*(OBJECT_ADDRESS;"OBJECT_NAME")
     *             ->METHOD_NAME(PARAM_TYPE PARAM_NAME=PARAM_VALUE ...)",
     *         depending on the presence or the parameters or not.
     *         If the object, method or parameters are invalid, returns a empty
     *         string.
     */
    QString stringify(const QObject* object, const QMetaMethod& metaMethod
                      , const QVector<QVariant>& parameters = QVector<QVariant>());

    /**
     * @brief Returns a string representation of a method call.
     * @param objectAddess The object address. Can be null or no longer valid.
     * @param objectName The object name.
     * @param metaObject The meta object pointer. Must be non null.
     * @param metaMethod The meta method. Must be a valid method method.
     * @param parameters If the given parameters are empty, no parameter data
     *                   will be included in the string.
     *                   If the given parameters are not empty, the count and
     *                   types of the parameters must be equal or greater to the
     *                   method's parameter count.
     * @return If all the parameters are valid, returns a string like
     *         "OBJECT_CLASS*(OBJECT_ADDRESS;"OBJECT_NAME")
     *             ->METHOD_NAME(PARAM_TYPE PARAM_NAME ...)"
     *         or
     *         "OBJECT_CLASS*(OBJECT_ADDRESS;"OBJECT_NAME")
     *             ->METHOD_NAME(PARAM_TYPE PARAM_NAME=PARAM_VALUE ...)",
     *         depending on the presence or the parameters or not.
     *         If the object, method or parameters are invalid, returns a empty
     *         string.
     */
    QString stringify(const void* objectAddress, const QString& objectName
                      , const QMetaObject* metaObject, const QMetaMethod& metaMethod
                      , const QVector<QVariant>& parameters = QVector<QVariant>());

private:

    QString _buffer;

};

#endif // QMETHODSTRINGIFIER_H
