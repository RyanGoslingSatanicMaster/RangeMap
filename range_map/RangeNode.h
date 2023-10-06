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

        RangeNode(RangeNode<KEY_TYPE, DATA_TYPE> const &node) :
                RangeItem<KEY_TYPE, DATA_TYPE>(node) {}

        explicit RangeNode(KEY_TYPE from, KEY_TYPE to, DATA_TYPE value)  :
                RangeItem<KEY_TYPE, DATA_TYPE>(from, to, value) {}

        explicit RangeNode(std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value)  :
                RangeItem<KEY_TYPE, DATA_TYPE>(range, value) {}

        virtual RangeNode* getLeft() const = 0;

        virtual RangeNode* getRight() const = 0;

        virtual bool isOverlap(const RangeNode<KEY_TYPE, DATA_TYPE>& other) const = 0;

        virtual bool isContain(const RangeNode<KEY_TYPE, DATA_TYPE>& other) const = 0;

        virtual bool isEqualRange(const RangeNode<KEY_TYPE, DATA_TYPE>& other) const = 0;

        virtual bool isMoreThan(const RangeNode<KEY_TYPE, DATA_TYPE>& other) const = 0;

        std::pair<KEY_TYPE, KEY_TYPE> getRange() const override {
            return RangeItem<KEY_TYPE, DATA_TYPE>::getRange();
        }

        std::vector<DATA_TYPE> getValue() const override {
            return RangeItem<KEY_TYPE, DATA_TYPE>::getValue();
        }

        KEY_TYPE getFrom() const override {
            return RangeItem<KEY_TYPE, DATA_TYPE>::getFrom();
        }

        KEY_TYPE getTo() const override {
            return RangeItem<KEY_TYPE, DATA_TYPE>::getTo();
        }

        bool isOverlap(KEY_TYPE other) const override {
            return RangeItem<KEY_TYPE, DATA_TYPE>::isOverlap(other);
        }

        bool isOverlap(std::pair<KEY_TYPE, KEY_TYPE> range) const override {
            return RangeItem<KEY_TYPE, DATA_TYPE>::isOverlap(range);
        }

        bool isOverlap(KEY_TYPE from, KEY_TYPE to) const override {
            return RangeItem<KEY_TYPE, DATA_TYPE>::isOverlap(from, to);
        }

        bool isOverlap(const shock_audio::RangeItem<KEY_TYPE, DATA_TYPE> &other) const override {
            return RangeItem<KEY_TYPE, DATA_TYPE>::isOverlap(other);
        }

        bool isContain(std::pair<KEY_TYPE, KEY_TYPE> range) const override {
            return RangeItem<KEY_TYPE, DATA_TYPE>::isContain(range);
        }

        bool isContain(KEY_TYPE from, KEY_TYPE to) const override {
            return RangeItem<KEY_TYPE, DATA_TYPE>::isContain(from, to);
        }

        bool isContain(const shock_audio::RangeItem<KEY_TYPE, DATA_TYPE> &other) const override {
            return RangeItem<KEY_TYPE, DATA_TYPE>::isContain(other);
        }

        bool isMoreThan(std::pair<KEY_TYPE, KEY_TYPE> range) const override {
            return RangeItem<KEY_TYPE, DATA_TYPE>::isMoreThan(range);
        }

        bool isMoreThan(KEY_TYPE from, KEY_TYPE to) const override {
            return RangeItem<KEY_TYPE, DATA_TYPE>::isMoreThan(from, to);
        }

        bool isMoreThan(const shock_audio::RangeItem<KEY_TYPE, DATA_TYPE> &other) const override {
            return RangeItem<KEY_TYPE, DATA_TYPE>::isMoreThan(other);
        }

        bool isContained(std::pair<KEY_TYPE, KEY_TYPE> range) const override {
            return RangeItem<KEY_TYPE, DATA_TYPE>::isContained(range);
        }

        bool isContained(KEY_TYPE from, KEY_TYPE to) const override {
            return RangeItem<KEY_TYPE, DATA_TYPE>::isContained(from, to);
        }

        bool isContained(const shock_audio::RangeItem<KEY_TYPE, DATA_TYPE> &other) const override {
            return RangeItem<KEY_TYPE, DATA_TYPE>::isContained(other);
        }

        virtual bool isContained(const shock_audio::RangeNode<KEY_TYPE, DATA_TYPE> &other) const = 0;

        bool isEqualRange(std::pair<KEY_TYPE, KEY_TYPE> range) const override {
            return RangeItem<KEY_TYPE, DATA_TYPE>::isEqualRange(range);
        }

        bool isEqualRange(KEY_TYPE from, KEY_TYPE to) const override {
            return RangeItem<KEY_TYPE, DATA_TYPE>::isEqualRange(from, to);
        }

        bool isEqualRange(const shock_audio::RangeItem<KEY_TYPE, DATA_TYPE> &other) const override {
            return RangeItem<KEY_TYPE, DATA_TYPE>::isEqualRange(other);
        }

        bool containValue(DATA_TYPE value) const override  {
            return RangeItem<KEY_TYPE, DATA_TYPE>::containValue(value);
        }

        int containCount(DATA_TYPE value) const override {
            return RangeItem<KEY_TYPE, DATA_TYPE>::containCount(value);
        }

        bool operator==(const RangeNode<KEY_TYPE, DATA_TYPE>& other) {
            return RangeItem<KEY_TYPE, DATA_TYPE>::operator==((RangeItem<KEY_TYPE, DATA_TYPE>) other) &&
                   getLeft() == other.getLeft() && getRight() == other.getRight();
        }

    };

}

#endif //SHOCK_AUDIO_RANGENODE_H
