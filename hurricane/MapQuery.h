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
#include "utils.h"

namespace takevos {
namespace hurricane {

template <class K, class V>
class MapQueryExpression;

/** A query which can be compared with a map.
 * The query can be build up using boolean operation. 
 */
template <class K, class V>
class MapQuery {
public:
    MapQuery() {
    }

    virtual ~MapQuery() {
    }

    /** Create a newly allocated clone of this object.
     * The returned pointer must be deleted by the caller.
     */
    virtual std::shared_ptr<MapQuery<K,V> > clone(void) const = 0;

    virtual std::map<K,V> map(void) const = 0;

    /** OR two querires together.
     */
    MapQueryExpression<K,V> operator|(const MapQuery<K,V> &other) const {
        return MapQueryExpression<K,V>('|', *this, other);
    }

    /** AND two querires together.
     */
    MapQueryExpression<K,V> operator&(const MapQuery<K,V> &other) const {
        return MapQueryExpression<K,V>('&', *this, other);
    }

    /** OR a query together with a std::map.
     * The std::map is interpeted as MapQueryItems together in an AND-MapQueryExpression.
     */
    MapQueryExpression<K,V> operator|(const std::map<K,V> &other) const {
        return MapQueryExpression<K,V>('|', *this, other);
    }

    /** AND a query together with a std::map.
     * The std::map is interpeted as MapQueryItems together in an AND-MapQueryExpression.
     */
    MapQueryExpression<K,V> operator&(const std::map<K,V> &other) const {
        return MapQueryExpression<K,V>('&', *this, other);
    }

    /** Compare a std::map with a query.
     * Return the number of query elements that have matched with the map.
     */
    virtual int compare(const std::map<K, V> &other) const = 0;

    /** Check if std::map matches exactly with this query.
     */
    virtual bool operator==(const std::map<K,V> &other) {
        // If the number of elements in the map is equal to the number of query elements that matched.
        return compare(other) == other.size();
    }

    /** Check if std::map matches exactly with this query.
     */
    virtual bool operator==(const MapQuery<K,V> &other) {
        std::map<K,V> tmp = other.map();

        // If the number of elements in the map is equal to the number of query elements that matched.
        return compare(tmp) == tmp.size();
    }

    /** Check if std::map matches with this query.
     */
    virtual bool operator!=(const std::map<K,V> &other) {
        return !(*this == other);
    }

    /** Check if std::map matches with this query.
     */
    virtual bool operator!=(const MapQuery<K,V> &other) {
        return !(*this == other);
    }

    /** Return a human readable string representing the query.
     * The std::to_string() function must be overriden for types K and V.
     */
    virtual std::string string() const = 0;
};


template <class K, class V>
class MapQueryItem : public MapQuery<K,V> {
    K       key;
    V       value;
    bool    value_valid;
public:
    MapQueryItem(const K &key) : key(key), value_valid(false) {
    }

    MapQueryItem(const K &key, const V &value) : key(key), value(value), value_valid(true) {
    }

    virtual std::shared_ptr<MapQuery<K,V> > clone(void) const {
        return std::make_shared<MapQueryItem<K,V> >(*this);
    }

    virtual int compare(const std::map<K, V> &other) const {
        auto x = other.find(key);

        if (x == other.end()) {
            return 0;
        } else if (value_valid) {
            return value == x->second ? 1 : 0;
        } else {
            return 1;
        }
    }

    virtual std::map<K,V> map(void) const {
        if (value_valid) {
            auto r = std::map<K,V>();
            r[key] = value;
            return r;
        } else {
            throw std::runtime_error("Can not convert a MapQueryItem without a value into a map");
        }
    }

    virtual std::string string() const
    {
        if (value_valid) {
            return std::to_string(key) + ":" + std::to_string(value);
        } else {
            return std::to_string(key) + ":*";
        }
    }
};


template <class K, class V>
class MapQueryExpression : public MapQuery<K,V> {
    char                                            operation;
    std::vector<std::shared_ptr<MapQuery<K,V> > >   items;
public:

    MapQueryExpression(int operation) : operation(operation) {
    }

    MapQueryExpression(const std::map<K,V> &m) : operation('&') {
        for (auto &x: m) {
            items.push_back(new MapQueryItem<K,V>(x->first, x->second));
        }
    }

    MapQueryExpression(int operation, const MapQuery<K,V> &lhs, const std::map<K,V> &m) : operation(operation) {
        add(lhs);
        for (auto &x: m) {
            items.push_back(new MapQueryItem<K,V>(x->first, x->second));
        }
    }

    MapQueryExpression(int operation, const MapQuery<K,V> &lhs, const MapQuery<K,V> &rhs) : operation(operation) {
        add(lhs);
        add(rhs);
    }

    virtual ~MapQueryExpression() {
    }

    virtual std::shared_ptr<MapQuery<K,V> > clone(void) const {
        auto r = std::make_shared<MapQueryExpression<K,V> >(operation);

        for (auto &x: items) {
            r->items.push_back(std::move(x->clone()));
        }

        return r;
    }

    virtual int compare(const std::map<K, V> &other) const {
        if (operation == '&') {
            int count = 0;
            int result;

            for (auto &x: items) {
                if ((result = x->compare(other)) == 0) {
                    return 0;
                }
                count+= result;
            }
            return count;

        } else if (operation == '|') {
            int count = 0;
            int result;

            for (auto &x: items) {
                result = x->compare(other);

                if (result > count) {
                    count = result;
                }
            }
            return count;

        } else {
            throw std::runtime_error("Uknown operation");
        }
    }

    virtual std::string string() const
    {
        int         i = 0;
        std::string r = "(";

        for (auto &x: items) {
            if (i) {
                r+= operation;
            }
            r+= x->string();
            i++;
        }

        return r + ")";
    }

    virtual std::map<K,V> map(void) const {
        if (operation == '&') {
            auto r = std::map<K,V>();

            for (auto &x: items) {
                auto x_map = x.map();
                r.insert(x_map.begin(), x_map.end());
            }

            return r;
        } else {
            throw std::runtime_error("Can not convert a MapQueryExpression to a map unless the operation is '&'");
        }
    }

    virtual void add(const MapQuery<K,V> &other) {
        const MapQueryExpression *other_collection = static_cast<const MapQueryExpression<K,V> *>(&other);

        if (other_collection && operation == other_collection->operation) {
            // If other is an expression with the same operation then merge the items.
            for (auto &x: other_collection->items) {
                items.push_back(std::move(x->clone()));
            }

        } else {
            items.push_back(std::move(other.clone()));
        }
    }
};

typedef MapQueryItem<std::string,std::string> DQ;

}}

namespace std {

template <class K, class V>
std::string to_string(takevos::hurricane::MapQuery<K,V> x) {
    return x.string();
}

}
#endif
