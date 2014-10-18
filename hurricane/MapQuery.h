/* Copyright (c) 2014-2014, Take Vos
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice, 
 *   this list of conditions and the following disclaimer in the documentation 
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef TAKEVOS_HURRICANE_MAPQUERY_H
#define TAKEVOS_HURRICANE_MAPQUERY_H

#include <string>
#include <vector>
#include <typeinfo>
#include <map>
#include <memory>
#include "strings.h"
#include "utils.h"

namespace takevos {
namespace hurricane {


/** A query which can be compared with a map.
 * The query can be build up using boolean operation in C++.
 *
 * Polymorphy requires pointers, which requires clone() functions to be
 * called, and values to be dereferenced. This is ugly when writing queries
 * in C++ code, therefor the MapQuery class is not implemented using inherintence.
 */
template <class K, class V>
class MapQuery {
    char                            operation;
    std::vector<MapQuery<K,V> >     items;
    K                               key;
    V                               value;
public:
    /** NULL Constructor.
     * This constructor is used when creating an uninitialized query.
     */
    MapQuery() : operation('N') {
    }

    /** A wildcard constructor.
     * This constructor is used when creating a wildcard query.
     *
     * @param key   A key to query.
     */
    MapQuery(const K &key) : operation('k'), key(key) {
    }

    /** An item constructor.
     * This constructor is used when creating a key/value pair query.
     *
     * @param key   A key to query.
     * @param value A key to query.
     */
    MapQuery(const K &key, const V &value) : operation('i'), key(key), value(value) {
    }

    /** An boolean constructor.
     * This constructor is used when creating an 'or' or 'and' boolean operation.
     *
     * @param operation     An boolean operation, either '|' or '&'.
     * @param lhs           The left hand side of the boolean operation.
     * @param rhs           The right hand side of the boolean operation.
     */
    MapQuery(int operation, const MapQuery<K,V> &lhs, const MapQuery<K,V> &rhs) : operation(operation) {
        if (lhs.operation != 'N') {
            // The left most NULL will be ignore because it may be an uninitialized query.
            add(lhs);
        }
        add(rhs);
    }

    /** An std::map conversion constructor.
     * This constructor is used when creating an ''and' boolean operation
     * from a map with key/value pairs.
     *
     * @param m     A map of key/value pairs.
     */
    MapQuery(const std::map<K,V> &m) : operation('&') {
        for (auto &x: m) {
            items.push_back(MapQuery<K,V>(x.first, x.second));
        }
    }

    /** An boolean constructor, with a map
     * This constructor is used when creating an 'or' or 'and' boolean operation.
     * The map on the right hand side is first converted to an 'and' boolean operation
     * from its key/value pairs.
     *
     * @param operation     An boolean operation, either '|' or '&'.
     * @param lhs           The left hand side of the boolean operation.
     * @param m             The right hand side of the boolean operation, being a map.
     */
    MapQuery(int operation, const MapQuery<K,V> &lhs, const std::map<K,V> &m) : operation(operation) {
        MapQuery(lhs, MapQuery(m));
    }

    ~MapQuery() {
    }

    /** OR two queries together.
     */
    MapQuery<K,V> operator|(const MapQuery<K,V> &other) const {
        return MapQuery<K,V>('|', *this, other);
    }

    /** AND two queries together.
     */
    MapQuery<K,V> operator&(const MapQuery<K,V> &other) const {
        return MapQuery<K,V>('&', *this, other);
    }

    /** OR two queries together.
     */
    MapQuery<K,V> &operator|=(const MapQuery<K,V> &other) {
        if (operation == '|') {
            // Append when or is done on an or operation.
            add(other);
        } else if (operation == 'N') {
            // Replace a null value.
            *this = other;
        } else {
            // Do it the slow way.
            *this = *this | other;
        }
        return *this;
    }

    /** AND two queries together.
     */
    MapQuery<K,V> &operator&=(const MapQuery<K,V> &other) {
        if (operation == '&') {
            // Append when or is done on an or operation.
            add(other);
        } else if (operation == 'N') {
            // Replace a null value.
            *this = other;
        } else {
            // Do it the slow way.
            *this = *this & other;
        }
        return *this;
    }

    /** OR a query together with a std::map.
     * The std::map is interpeted as MapQueryItems together in an AND-MapQueryExpression.
     */
    MapQuery<K,V> operator|(const std::map<K,V> &other) const {
        return MapQuery<K,V>('|', *this, other);
    }

    /** AND a query together with a std::map.
     * The std::map is interpeted as MapQueryItems together in an AND-MapQueryExpression.
     */
    MapQuery<K,V> operator&(const std::map<K,V> &other) const {
        return MapQuery<K,V>('&', *this, other);
    }

    /* Add an query to this boolean expression.
     * If the query itself is off the same type of boolean expression then we merge the query with this one.
     */
    void add(const MapQuery<K,V> &other) {
        switch (operation) {
        case '&':
        case '|':
            if (operation == other.operation) {
                // If other is an expression with the same operation then merge the items.
                for (auto &x: other.items) {
                    if (x.operation == 'N') {
                        throw std::runtime_error("Can not add a NULL query to a boolean operation.");
                    }
                    items.push_back(x);
                }

            } else {
                // Push the query itself.
                if (other.operation == 'N') {
                    throw std::runtime_error("Can not add a NULL query to a boolean operation.");
                }
                items.push_back(other);
            }
            break;
        default:
            throw std::runtime_error("Invalid operation during add().");
        }
    }

    /** Convert the query into a map.
     * Only items and and-boolean-expressions are supported.
     */
    std::map<K,V> map(void) const {
        std::map<K,V>   r;

        switch (operation) {
        case 'i':
            r[key] = value;
            return r;
        case '&':
            for (auto &x: items) {
                auto x_map = x.map();
                r.insert(x_map.begin(), x_map.end());
            }
            return r;
        default:
            throw std::runtime_error("Can not convert a MapQuery unless it is an item or and operation.");
        }
    }

    /** Convert the query into a string.
     */
    std::string string() const
    {
        std::string     r = "(";

        switch (operation) {
        case 'i':
            return std::to_string(key) + ":" + std::to_string(value);
        case 'k':
            return std::to_string(key) + ":*";
        case '&':
        case '|':
            for (auto &x: items) {
                if (r.length() > 1) {
                    r+= operation;
                }
                r+= x.string();
            }
            return r + ")";
        case 'N':
            return std::string("()");
        default:
            throw std::runtime_error("Unknown operation during string().");
        }
    }

    /** Compare a query to a map.
     * @param   m   A map of items to compare to the query.
     * @return      The number of items in the map that match the query.
     */
    int compare(const std::map<K,V> &other) const {
        int count = 0;
        int result;

        switch (operation) {
        case 'k':
            // The query is a wildcard; only check if the key exists in the map.
            {
                auto it = other.find(key);
                return (it != other.end()) ? 1 : 0;
            }

        case 'i':
            // The query is an item; check if both the key and value exists in the map.
            {
                auto it = other.find(key);
                return (it != other.end() && value == it->second) ? 1 : 0;
            }

        case '&':
            // The query is a and-boolean-expression; check if all items exist in the map.
            // Add all the counts together from each subquery.
            for (auto &x: items) {
                if ((result = x.compare(other)) == 0) {
                    return 0;
                }
                count+= result;
            }
            return count;

        case '|':
            // The query is a or-boolean-expression; check if any items exist in the map.
            // Take the maximum count from each subquery.
            for (auto &x: items) {
                result = x.compare(other);

                if (result > count) {
                    count = result;
                }
            }
            return count;

        default:
            throw std::runtime_error("Unknown operation during compare().");
        }
    }

    /** Check if std::map matches exactly with this query.
     */
    bool operator==(const std::map<K,V> &other) {
        // If the number of elements in the map is equal to the number of query elements that matched.
        return compare(other) == other.size();
    }

    /** Check if std::map matches exactly with this query.
     */
    bool operator==(const MapQuery<K,V> &other) {
        std::map<K,V> tmp = other.map();

        // If the number of elements in the map is equal to the number of query elements that matched.
        return compare(tmp) == tmp.size();
    }

    /** Check if std::map matches with this query.
     */
    bool operator!=(const std::map<K,V> &other) {
        return !(*this == other);
    }

    /** Check if std::map matches with this query.
     */
    bool operator!=(const MapQuery<K,V> &other) {
        return !(*this == other);
    }

};

}}

namespace std {

template <class K, class V>
static inline string to_string(const takevos::hurricane::MapQuery<K,V> &x) {
    return x.string();
}

template <class K, class V>
static inline ostream &operator<<(ostream &os, const takevos::hurricane::MapQuery<K,V> &x) {
    os << std::to_string(x);
    return os;
}

template <class K, class V>
static inline std::ostream &operator<<(std::ostream &os, const std::map<K,V> &m) {
    os << "{";
    for (auto &x: m) {
        os << std::to_string(x.first) << ": " << std::to_string(x.second) << ", ";
    }
    os << "}";
    return os;
}


}
#endif
