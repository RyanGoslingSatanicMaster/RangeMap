#ifndef SHOCK_AUDIO_RANGENODE_H
#define SHOCK_AUDIO_RANGENODE_H

#include <utility>
#include <vector>
#include <memory>
#include "RangeItem.h"

namespace shock_audio{

    template<typename KEY_TYPE, typename DATA_TYPE>
    class RangeNode: public RangeItem<KEY_TYPE, DATA_TYPE> {

    public:

        RangeNode() = delete;

        RangeNode(RangeNode<KEY_TYPE, DATA_TYPE> const &node);

        explicit RangeNode(KEY_TYPE from, KEY_TYPE to, DATA_TYPE value);

        explicit RangeNode(std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value);

        virtual RangeNode* getLeft() const = 0;

        virtual RangeNode* getRight() const = 0;

        virtual bool isOverlap(const RangeNode<KEY_TYPE, DATA_TYPE>& other) const = 0;

        virtual bool isContain(const RangeNode<KEY_TYPE, DATA_TYPE>& other) const = 0;

        virtual bool isEqualRange(const RangeNode<KEY_TYPE, DATA_TYPE>& other) const = 0;

        std::pair<KEY_TYPE, KEY_TYPE> getRange() const override;

        std::vector<DATA_TYPE> getValue() const override;

        KEY_TYPE getFrom() const override;

        KEY_TYPE getTo() const override;

        bool isOverlap(KEY_TYPE other) const override;

        bool isOverlap(std::pair<KEY_TYPE, KEY_TYPE> range) const override;

        bool isOverlap(KEY_TYPE from, KEY_TYPE to) const override;

        bool isOverlap(const shock_audio::RangeItem<KEY_TYPE, DATA_TYPE> &other) const override;

        bool isContain(std::pair<KEY_TYPE, KEY_TYPE> range) const override;

        bool isContain(KEY_TYPE from, KEY_TYPE to) const override;

        bool isContain(const shock_audio::RangeItem<KEY_TYPE, DATA_TYPE> &other) const override;

        bool isEqualRange(std::pair<KEY_TYPE, KEY_TYPE> range) const override;

        bool isEqualRange(KEY_TYPE from, KEY_TYPE to) const override;

        bool isEqualRange(const shock_audio::RangeItem<KEY_TYPE, DATA_TYPE> &other) const override;

        bool containValue(DATA_TYPE value) const override;

        int containCount(DATA_TYPE value) const override;

    };

}

#endif //SHOCK_AUDIO_RANGENODE_H
