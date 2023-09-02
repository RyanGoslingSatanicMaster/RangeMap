#include "RangeNode.h"
#include "RangeItem.cpp"

namespace shock_audio {
    template<typename KEY_TYPE, typename DATA_TYPE>
    RangeNode<KEY_TYPE, DATA_TYPE>::RangeNode(std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value) :
            RangeItem<KEY_TYPE, DATA_TYPE>(range, value) {}

    template<typename KEY_TYPE, typename DATA_TYPE>
    RangeNode<KEY_TYPE, DATA_TYPE>::RangeNode(KEY_TYPE from, KEY_TYPE to, DATA_TYPE value) :
            RangeItem<KEY_TYPE, DATA_TYPE>(from, to, value) {}

    template<typename KEY_TYPE, typename DATA_TYPE>
    RangeNode<KEY_TYPE, DATA_TYPE>::RangeNode(const RangeNode<KEY_TYPE, DATA_TYPE> &node):
            RangeItem<KEY_TYPE, DATA_TYPE>(node) {}

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool RangeNode<KEY_TYPE, DATA_TYPE>::containValue(DATA_TYPE value) const {
        return RangeItem<KEY_TYPE, DATA_TYPE>::containValue(value);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool RangeNode<KEY_TYPE, DATA_TYPE>::isEqualRange(const RangeItem<KEY_TYPE, DATA_TYPE> &other) const {
        return RangeItem<KEY_TYPE, DATA_TYPE>::isEqualRange(other);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool RangeNode<KEY_TYPE, DATA_TYPE>::isEqualRange(KEY_TYPE from, KEY_TYPE to) const {
        return RangeItem<KEY_TYPE, DATA_TYPE>::isEqualRange(from, to);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool RangeNode<KEY_TYPE, DATA_TYPE>::isEqualRange(std::pair<KEY_TYPE, KEY_TYPE> range) const {
        return RangeItem<KEY_TYPE, DATA_TYPE>::isEqualRange(range);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool RangeNode<KEY_TYPE, DATA_TYPE>::isContain(const RangeItem<KEY_TYPE, DATA_TYPE> &other) const {
        return RangeItem<KEY_TYPE, DATA_TYPE>::isContain(other);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool RangeNode<KEY_TYPE, DATA_TYPE>::isContain(KEY_TYPE from, KEY_TYPE to) const {
        return RangeItem<KEY_TYPE, DATA_TYPE>::isContain(from, to);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool RangeNode<KEY_TYPE, DATA_TYPE>::isContain(std::pair<KEY_TYPE, KEY_TYPE> range) const {
        return RangeItem<KEY_TYPE, DATA_TYPE>::isContain(range);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool RangeNode<KEY_TYPE, DATA_TYPE>::isOverlap(const RangeItem<KEY_TYPE, DATA_TYPE> &other) const {
        return RangeItem<KEY_TYPE, DATA_TYPE>::isOverlap(other);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool RangeNode<KEY_TYPE, DATA_TYPE>::isOverlap(KEY_TYPE from, KEY_TYPE to) const {
        return RangeItem<KEY_TYPE, DATA_TYPE>::isOverlap(from, to);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool RangeNode<KEY_TYPE, DATA_TYPE>::isOverlap(std::pair<KEY_TYPE, KEY_TYPE> range) const {
        return RangeItem<KEY_TYPE, DATA_TYPE>::isOverlap(range);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool RangeNode<KEY_TYPE, DATA_TYPE>::isOverlap(KEY_TYPE other) const {
        return RangeItem<KEY_TYPE, DATA_TYPE>::isOverlap(other);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    KEY_TYPE RangeNode<KEY_TYPE, DATA_TYPE>::getTo() const {
        return RangeItem<KEY_TYPE, DATA_TYPE>::getTo();
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    KEY_TYPE RangeNode<KEY_TYPE, DATA_TYPE>::getFrom() const {
        return RangeItem<KEY_TYPE, DATA_TYPE>::getFrom();
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::vector<DATA_TYPE> RangeNode<KEY_TYPE, DATA_TYPE>::getValue() const {
        return RangeItem<KEY_TYPE, DATA_TYPE>::getValue();
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::pair<KEY_TYPE, KEY_TYPE> RangeNode<KEY_TYPE, DATA_TYPE>::getRange() const {
        return RangeItem<KEY_TYPE, DATA_TYPE>::getRange();
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    int RangeNode<KEY_TYPE, DATA_TYPE>::containCount(DATA_TYPE value) const {
        return RangeItem<KEY_TYPE, DATA_TYPE>::containCount(value);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool RangeNode<KEY_TYPE, DATA_TYPE>::operator==(const RangeNode<KEY_TYPE, DATA_TYPE> &other) {
        return RangeItem<KEY_TYPE, DATA_TYPE>::operator==((RangeItem<KEY_TYPE, DATA_TYPE>) other) &&
               getLeft() == other.getLeft() && getRight() == other.getRight();
    }
}