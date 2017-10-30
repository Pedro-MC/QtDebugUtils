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
#ifndef QADDRESSWIPER_H
#define QADDRESSWIPER_H

#include <QString>
#include <QByteArray>

template<typename CONTAINER, typename ITEM, typename LITERAL>
class QAddressWiper {
public:
    typedef CONTAINER container_t;
    typedef ITEM item_t;
    typedef LITERAL literal_t;
    typedef int index_t;

    /**
     * @brief Returns true is the data in the given container at the given index
     *        looks like a hexadecimal memory address (e.g. 0xffffffff for 32 bit
     *        , 0xffffffffffffffff for 64 bit).
     * @param container
     * @param index
     * @return Returns true if it looks like a hexadecimal address.
     *         Returns false otherwise.
     */
    static bool isAddress(const container_t& container, index_t index) {
        if(container[index] != c0() || container[index + 1] != cx()) {
            return false;
        }
        const index_t maxIndex = index + pointer_size;
        index += 2;
        while(index < maxIndex) {
            const item_t item = container[index++];
            const bool isHexa = (item >= c0() && item <= c9())
                    || (item >= ca() && item <= cf());
            if(!isHexa) {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Wipes any memory address look-alike in the given container.
     * @param container
     * @return Returns the given container with the addresses wiped.
     */
    static container_t& wipe(container_t& container) {
        const index_t maxIndex = container.size() - pointer_size + 1;
        for(index_t index = 0 ; index < maxIndex ; ++index) {
            if(isAddress(container, index)) {
                wipeAddress(container, index);
                index += pointer_size;
            }
        }
        return container;
    }

    /**
     * @brief Makes a copy of the given container and wipes any memory address
     *        look-alike in the copy.
     * @param container
     * @return Returns a copy of the given container with the addresses wiped.
     */
    static container_t copywipe(container_t container) {
        return wipe(container);
    }

private:

    static const int pointer_size = 2 + sizeof(void*) * 2;

    constexpr static item_t cx() {
        return LITERAL('x');
    }

    constexpr static item_t c0() {
        return LITERAL('0');
    }

    constexpr static item_t c9() {
        return LITERAL('9');
    }

    constexpr static item_t ca() {
        return LITERAL('a');
    }

    constexpr static item_t cf() {
        return LITERAL('f');
    }

    /**
     * @brief Wipes the address at the given index of the given container.
     * @param container Container with the address to be wiped.
     * @param index Index where the address starts.
     * @return Returns the given container.
     */
    static container_t& wipeAddress(container_t& container, index_t index) {
        index += 2;
        const index_t maxIndex = index + sizeof(void*) * 2;
        while(index < maxIndex) {
            container[index++] = cf();
        }
        return container;
    }

};

typedef QAddressWiper<QByteArray, char, char> QByteArrayAddressWiper;
typedef QAddressWiper<QString, QChar, QLatin1Char> QStringAddressWiper;

#endif // QADDRESSWIPER_H
