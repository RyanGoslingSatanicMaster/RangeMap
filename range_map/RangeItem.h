#ifndef SHOCK_AUDIO_RANGEITEM_H
#define SHOCK_AUDIO_RANGEITEM_H

#include <utility>
#include <vector>

namespace shock_audio{

    template<typename KEY_TYPE, typename DATA_TYPE>
    class RangeItem {

    public:
        virtual std::pair<KEY_TYPE, KEY_TYPE> getRange() const = 0;

        virtual std::vector<DATA_TYPE> getValue() const = 0;

        virtual KEY_TYPE getFrom() const = 0;

        virtual KEY_TYPE getTo() const = 0;

        virtual bool isOverlap(KEY_TYPE other) const = 0;

        virtual bool isOverlap(std::pair<KEY_TYPE, KEY_TYPE> range) const = 0;

        virtual bool isOverlap(KEY_TYPE from, KEY_TYPE to) const = 0;

        virtual bool isOverlap(const RangeItem<KEY_TYPE, DATA_TYPE>& other) const = 0;

        virtual bool isContain(std::pair<KEY_TYPE, KEY_TYPE> range) const = 0;

        virtual bool isContain(KEY_TYPE from, KEY_TYPE to) const = 0;

        virtual bool isContain(const RangeItem<KEY_TYPE, DATA_TYPE>& other) const = 0;

        virtual bool isEqualRange(std::pair<KEY_TYPE, KEY_TYPE> range) const = 0;

        virtual bool isEqualRange(KEY_TYPE from, KEY_TYPE to) const = 0;

        virtual bool isEqualRange(const RangeItem<KEY_TYPE, DATA_TYPE>& other) const = 0;

    };

}
#endif //SHOCK_AUDIO_RANGEITEM_H
