#ifndef SHOCK_AUDIO_RANGEITEM_H
#define SHOCK_AUDIO_RANGEITEM_H

#include <utility>
#include <vector>
#include "InvalidRangeException.h"

namespace shock_audio{

    template<typename KEY_TYPE, typename DATA_TYPE>
    class RangeItem {

    public:

        RangeItem() = delete;

        explicit RangeItem(RangeItem<KEY_TYPE, DATA_TYPE> const &item) :
                _value(item._value),
                _from(item._from),
                _to(item._to) {}

        explicit RangeItem(KEY_TYPE from, KEY_TYPE to, DATA_TYPE value)  :
                _from(from),
                _to(to),
                _value(std::vector<DATA_TYPE>{value}){
            assertRange(from, to);
        }

        explicit RangeItem(std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value)  :
                _from(range.first),
                _to(range.second),
                _value(std::vector<DATA_TYPE>{value}) {
            assertRange(range.first, range.second);
        }

        explicit RangeItem(std::pair<KEY_TYPE, KEY_TYPE> range, std::vector<DATA_TYPE> value)  :
                _from(range.first),
                _to(range.second),
                _value(value) {
            assertRange(range.first, range.second);
        }

        virtual std::pair<KEY_TYPE, KEY_TYPE> getRange() const {
            return std::pair<KEY_TYPE, KEY_TYPE>(_from, _to);
        }

        virtual std::vector<DATA_TYPE> getValue() const {
            return _value;
        }

        virtual KEY_TYPE getFrom() const {
            return _from;
        }

        virtual KEY_TYPE getTo() const {
            return _to;
        }

        virtual bool isOverlap(KEY_TYPE other) const {
            return other >= _from && other <= _to;
        }

        virtual bool isOverlap(std::pair<KEY_TYPE, KEY_TYPE> range) const {
            return isOverlap(range.first, range.second);
        }

        virtual bool isOverlap(KEY_TYPE from, KEY_TYPE to) const {
            return _from <= to && _from >= from
                   || _to <= to && _to >= from
                   || _from <= from && _to >= to
                   || from <= _from && to >= _to;
        }

        virtual bool isOverlap(const RangeItem<KEY_TYPE, DATA_TYPE>& other) const {
            return isOverlap(other.getFrom(), other.getTo());
        }

        virtual bool isContain(std::pair<KEY_TYPE, KEY_TYPE> range) const {
            return isContain(range.first, range.second);
        }

        virtual bool isContain(KEY_TYPE from, KEY_TYPE to) const {
            return _from <= from && _to >= to;
        }

        virtual bool isContain(const RangeItem<KEY_TYPE, DATA_TYPE>& other) const {
            return isContain(other.getFrom(), other.getTo());
        }

        virtual bool isContained(std::pair<KEY_TYPE, KEY_TYPE> range) const {
            return isContained(range.first, range.second);
        }

        virtual bool isContained(KEY_TYPE from, KEY_TYPE to) const {
            return _from >= from && _to <= to;
        }

        virtual bool isContained(const RangeItem<KEY_TYPE, DATA_TYPE>& other) const {
            return isContained(other.getFrom(), other.getTo());
        }

        virtual bool isMoreThan(std::pair<KEY_TYPE, KEY_TYPE> range) const {
            return isMoreThan(range.first, range.second);
        }

        virtual bool isMoreThan(KEY_TYPE from, KEY_TYPE to) const {
            return _from > from || _from == from && _to > to;
        }

        virtual bool isMoreThan(const RangeItem<KEY_TYPE, DATA_TYPE>& other) const {
            return isMoreThan(other.getFrom(), other.getTo());
        }

        virtual bool isEqualRange(std::pair<KEY_TYPE, KEY_TYPE> range) const {
            return isEqualRange(range.first, range.second);
        }

        virtual bool isEqualRange(KEY_TYPE from, KEY_TYPE to) const {
            return _from == from && _to == to;
        }

        virtual bool isEqualRange(const RangeItem<KEY_TYPE, DATA_TYPE>& other) const {
            return isEqualRange(other.getFrom(), other.getTo());
        }

        virtual bool containValue(DATA_TYPE value) const {
            return std::count(_value.begin(), _value.end(), value) > 0;
        }

        virtual int containCount(DATA_TYPE value) const {
            return std::count(_value.begin(), _value.end(), value);
        }

        bool operator==(const RangeItem<KEY_TYPE, DATA_TYPE>& other);

    protected:
        KEY_TYPE _from;
        KEY_TYPE _to;
        std::vector<DATA_TYPE> _value;

    private:
        void assertRange(KEY_TYPE from, KEY_TYPE to) {
            if (from >= to && from != NULL && to != NULL)
                throw shock_audio::InvalidRangeException();
        }

    };

}
#endif //SHOCK_AUDIO_RANGEITEM_H
