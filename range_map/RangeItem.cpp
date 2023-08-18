#include "MutableRangeNode.h"
#include "exceptions/InvalidRangeException.h"

namespace shock_audio {

    template<typename KEY_TYPE, typename DATA_TYPE>
    RangeItem<KEY_TYPE, DATA_TYPE>::RangeItem(const RangeItem<KEY_TYPE, DATA_TYPE> &item):
            _value(item._value),
            _from(item._from),
            _to(item._to) {}

    template<typename KEY_TYPE, typename DATA_TYPE>
    KEY_TYPE RangeItem<KEY_TYPE, DATA_TYPE>::getTo() const {
        return _to;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    KEY_TYPE RangeItem<KEY_TYPE, DATA_TYPE>::getFrom() const {
        return _from;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void RangeItem<KEY_TYPE, DATA_TYPE>::assertRange(KEY_TYPE from, KEY_TYPE to) {
        if (from >= to && from != NULL && to != NULL)
            throw shock_audio::InvalidRangeException();
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    RangeItem<KEY_TYPE, DATA_TYPE>::RangeItem(KEY_TYPE from, KEY_TYPE to, DATA_TYPE value) :
            _from(from),
            _to(to),
            _value({value}){
        assertRange(from, to);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    RangeItem<KEY_TYPE, DATA_TYPE>::RangeItem(std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value) :
            _from(range.first),
            _to(range.second),
            _value({value}) {
        assertRange(range.first, range.second);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::pair<KEY_TYPE, KEY_TYPE> RangeItem<KEY_TYPE, DATA_TYPE>::getRange() const{
        return std::pair<KEY_TYPE, KEY_TYPE>(_from, _to);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::vector<DATA_TYPE> RangeItem<KEY_TYPE, DATA_TYPE>::getValue() const{
        return _value;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool RangeItem<KEY_TYPE, DATA_TYPE>::isOverlap(KEY_TYPE other) const {
        return other >= _from && other <= _to;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool RangeItem<KEY_TYPE, DATA_TYPE>::isOverlap(std::pair<KEY_TYPE, KEY_TYPE> range) const {
        return _from <= range.second && _from >= range.first
               || _to <= range.second && _to >= range.first
               || _from <= range.first && _to >= range.second
               || range.first <= _from && range.second >= _to;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool RangeItem<KEY_TYPE, DATA_TYPE>::isOverlap(KEY_TYPE from, KEY_TYPE to) const {
        return _from <= to && _from >= from
               || _to <= to && _to >= from
               || _from <= from && _to >= to
               || from <= _from && to >= _to;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool RangeItem<KEY_TYPE, DATA_TYPE>::isOverlap(const shock_audio::RangeItem<KEY_TYPE, DATA_TYPE> &other) const {
        auto otherFrom = other.getFrom();
        auto otherTo = other.getTo();
        return _from <= otherTo && _from >= otherFrom
               || _to <= otherTo && _to >= otherFrom
               || _from <= otherFrom && _to >= otherTo
               || otherFrom <= _from && otherTo >= _to;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool RangeItem<KEY_TYPE, DATA_TYPE>::isContain(const shock_audio::RangeItem<KEY_TYPE, DATA_TYPE> &other) const {
        return _from <= other.getFrom() && _to >= other.getTo();
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool RangeItem<KEY_TYPE, DATA_TYPE>::isContain(KEY_TYPE from, KEY_TYPE to) const {
        return _from <= from && _to >= to;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool RangeItem<KEY_TYPE, DATA_TYPE>::isContain(std::pair<KEY_TYPE, KEY_TYPE> range) const {
        return _from <= range.first && _to >= range.second;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool RangeItem<KEY_TYPE, DATA_TYPE>::isEqualRange(
            const shock_audio::RangeItem<KEY_TYPE, DATA_TYPE> &other) const {
        return _from == other.getFrom() && _to == other.getTo();
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool RangeItem<KEY_TYPE, DATA_TYPE>::isEqualRange(KEY_TYPE from, KEY_TYPE to) const {
        return _from == from && _to == to;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool RangeItem<KEY_TYPE, DATA_TYPE>::isEqualRange(std::pair<KEY_TYPE, KEY_TYPE> range) const {
        return _from == range.first && _to == range.second;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool RangeItem<KEY_TYPE, DATA_TYPE>::containValue(DATA_TYPE value) const {
        return std::count(_value.begin(), _value.end(), value) > 0;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    int RangeItem<KEY_TYPE, DATA_TYPE>::containCount(DATA_TYPE value) const {
        return std::count(_value.begin(), _value.end(), value);
    }

}