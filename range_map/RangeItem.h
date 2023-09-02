#ifndef SHOCK_AUDIO_RANGEITEM_H
#define SHOCK_AUDIO_RANGEITEM_H

#include <utility>
#include <vector>

namespace shock_audio{

    template<typename KEY_TYPE, typename DATA_TYPE>
    class RangeItem {

    public:

        RangeItem() = delete;

        RangeItem(RangeItem<KEY_TYPE, DATA_TYPE> const &item);

        explicit RangeItem(KEY_TYPE from, KEY_TYPE to, DATA_TYPE value);

        explicit RangeItem(std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value);

        virtual std::pair<KEY_TYPE, KEY_TYPE> getRange() const;

        virtual std::vector<DATA_TYPE> getValue() const;

        virtual KEY_TYPE getFrom() const;

        virtual KEY_TYPE getTo() const;

        virtual bool isOverlap(KEY_TYPE other) const;

        virtual bool isOverlap(std::pair<KEY_TYPE, KEY_TYPE> range) const;

        virtual bool isOverlap(KEY_TYPE from, KEY_TYPE to) const;

        virtual bool isOverlap(const RangeItem<KEY_TYPE, DATA_TYPE>& other) const;

        virtual bool isContain(std::pair<KEY_TYPE, KEY_TYPE> range) const;

        virtual bool isContain(KEY_TYPE from, KEY_TYPE to) const;

        virtual bool isContain(const RangeItem<KEY_TYPE, DATA_TYPE>& other) const;

        virtual bool isEqualRange(std::pair<KEY_TYPE, KEY_TYPE> range) const;

        virtual bool isEqualRange(KEY_TYPE from, KEY_TYPE to) const;

        virtual bool isEqualRange(const RangeItem<KEY_TYPE, DATA_TYPE>& other) const;

        virtual bool containValue(DATA_TYPE value) const;

        virtual int containCount(DATA_TYPE value) const;

        bool operator==(const RangeItem<KEY_TYPE, DATA_TYPE>& other);

    protected:
        KEY_TYPE _from;
        KEY_TYPE _to;
        std::vector<DATA_TYPE> _value;

    private:
        void assertRange(KEY_TYPE from, KEY_TYPE to);

    };

}
#endif //SHOCK_AUDIO_RANGEITEM_H
