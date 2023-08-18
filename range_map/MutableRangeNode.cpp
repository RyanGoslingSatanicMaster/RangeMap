#include "MutableRangeNode.h"
#include "exceptions/InvalidRangeException.h"
#include "RangeNode.cpp"

namespace shock_audio_impl {

    template<typename KEY_TYPE, typename DATA_TYPE>
    MutableRangeNode<KEY_TYPE, DATA_TYPE>::MutableRangeNode(std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value,
                                                            Color color) :
            shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>(range, value),
            _color(color),
            _max(range.second) {
        assertRange(range.first, range.second);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    MutableRangeNode<KEY_TYPE, DATA_TYPE>::MutableRangeNode(KEY_TYPE from, KEY_TYPE to, DATA_TYPE value, Color color) :
            shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>(from, to, value),
            _color(color),
            _max(to){
        assertRange(from, to);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    MutableRangeNode<KEY_TYPE, DATA_TYPE>::MutableRangeNode(const MutableRangeNode<KEY_TYPE, DATA_TYPE> &node) :
    shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>(static_cast<shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>&>(node)),
    _max(node._max),
    _color(node._color),
    _left(std::move(node._left)),
    _right(std::move(node._right)) {}

    template<typename KEY_TYPE, typename DATA_TYPE>
    MutableRangeNode<KEY_TYPE, DATA_TYPE>::MutableRangeNode(KEY_TYPE from, KEY_TYPE to, DATA_TYPE value) :
            shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>(from, to, value),
            _max(to){
        assertRange(from, to);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    MutableRangeNode<KEY_TYPE, DATA_TYPE>::MutableRangeNode(std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value) :
            shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>(range, value),
            _max(range.second) {
        assertRange(range.first, range.second);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    MutableRangeNode<KEY_TYPE, DATA_TYPE>::MutableRangeNode(KEY_TYPE from, KEY_TYPE to, DATA_TYPE value,
                                                            std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> left,
                                                            std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> right) :
            shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>(from, to, value),
            _left(std::move(left)),
            _right(std::move(right)),
            _max(to){
        assertRange(from, to);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    MutableRangeNode<KEY_TYPE, DATA_TYPE>::MutableRangeNode(std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value,
                                                            std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> left,
                                                            std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> right) :
            shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>(range, value),
            _left(std::move(left)),
            _right(std::move(right)),
            _max(range.second) {
        assertRange(range.first, range.second);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeNode<KEY_TYPE, DATA_TYPE>::setValue(std::vector<DATA_TYPE> val) {
        shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_value = val;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    MutableRangeNode<KEY_TYPE, DATA_TYPE> *MutableRangeNode<KEY_TYPE, DATA_TYPE>::getRightPtr() {
        return _right.get();
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    MutableRangeNode<KEY_TYPE, DATA_TYPE> *MutableRangeNode<KEY_TYPE, DATA_TYPE>::getLeftPtr() {
        return _left.get();
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    KEY_TYPE MutableRangeNode<KEY_TYPE, DATA_TYPE>::getMax() {
        return _max;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    shock_audio::RangeNode<KEY_TYPE, DATA_TYPE> *MutableRangeNode<KEY_TYPE, DATA_TYPE>::getLeft() const {
        return static_cast<shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>*>(_left.get());
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    shock_audio::RangeNode<KEY_TYPE, DATA_TYPE> *MutableRangeNode<KEY_TYPE, DATA_TYPE>::getRight() const {
        return static_cast<shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>*>(_right.get());
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeNode<KEY_TYPE, DATA_TYPE>::setMax(KEY_TYPE max) {
        _max = max;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeNode<KEY_TYPE, DATA_TYPE>::setFrom(KEY_TYPE from) {
        assertRange(from, shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_to);
        shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_from = from;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeNode<KEY_TYPE, DATA_TYPE>::setTo(KEY_TYPE to) {
        assertRange(shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_from, to);
        shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_to = to;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeNode<KEY_TYPE, DATA_TYPE>::setRange(KEY_TYPE from, KEY_TYPE to) {
        assertRange(from, to);
        shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_from = from;
        shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_to = to;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeNode<KEY_TYPE, DATA_TYPE>::setRange(std::pair<KEY_TYPE, KEY_TYPE> range) {
        assertRange(range.first, range.second);
        shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_from = range.first;
        shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_to = range.second;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool MutableRangeNode<KEY_TYPE, DATA_TYPE>::isOverlap(const shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>& other) const {
        auto otherFrom = other.getFrom();
        auto otherTo = other.getTo();
        return shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_from <= otherTo && shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_from >= otherFrom
        || shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_to <= otherTo && shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_to >= otherFrom
        || shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_from <= otherFrom && shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_to >= otherTo
        || otherFrom <= shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_from && otherTo >= shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_to;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool
    MutableRangeNode<KEY_TYPE, DATA_TYPE>::isContain(const shock_audio::RangeNode<KEY_TYPE, DATA_TYPE> &other) const {
        return shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_from <= other.getFrom() && shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_to >= other.getTo();
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeNode<KEY_TYPE, DATA_TYPE>::addValue(DATA_TYPE value) {
        shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_value.push_back(value);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool MutableRangeNode<KEY_TYPE, DATA_TYPE>::isEqualRange(
            const shock_audio::RangeNode<KEY_TYPE, DATA_TYPE> &other) const {
        return shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_from == other.getFrom() && shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_to == other.getTo();
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    Color MutableRangeNode<KEY_TYPE, DATA_TYPE>::getColor() {
        return _color;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeNode<KEY_TYPE, DATA_TYPE>::setColor(Color newColor) {
        _color = newColor;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeNode<KEY_TYPE, DATA_TYPE>::setRight(std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> right) {
        _right = std::move(right);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeNode<KEY_TYPE, DATA_TYPE>::setLeft(std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> left) {
        _left = std::move(left);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> MutableRangeNode<KEY_TYPE, DATA_TYPE>::getLeftUniquePtr(){
        return std::move(_left);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> MutableRangeNode<KEY_TYPE, DATA_TYPE>::getRightUniquePtr(){
        return std::move(_right);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeNode<KEY_TYPE, DATA_TYPE>::removeValue(DATA_TYPE value) {
        shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_value.erase(
                std::remove_if(
                        shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_value.begin(),
                        shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_value.end(),
                        [&value] (const DATA_TYPE element) {
                            return element == value;
                        }
                        ),
                shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_value.end()
                );
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool MutableRangeNode<KEY_TYPE, DATA_TYPE>::containValue(DATA_TYPE value) const {
        return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::containValue(value);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool MutableRangeNode<KEY_TYPE, DATA_TYPE>::isEqualRange(
            const shock_audio::RangeItem<KEY_TYPE, DATA_TYPE> &other) const {
        return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isEqualRange(other);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool MutableRangeNode<KEY_TYPE, DATA_TYPE>::isEqualRange(KEY_TYPE from, KEY_TYPE to) const {
        return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isEqualRange(from, to);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool MutableRangeNode<KEY_TYPE, DATA_TYPE>::isEqualRange(std::pair<KEY_TYPE, KEY_TYPE> range) const {
        return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isEqualRange(range);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool
    MutableRangeNode<KEY_TYPE, DATA_TYPE>::isContain(const shock_audio::RangeItem<KEY_TYPE, DATA_TYPE> &other) const {
        return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isContain(other);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool MutableRangeNode<KEY_TYPE, DATA_TYPE>::isContain(KEY_TYPE from, KEY_TYPE to) const {
        return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isContain(from, to);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool MutableRangeNode<KEY_TYPE, DATA_TYPE>::isContain(std::pair<KEY_TYPE, KEY_TYPE> range) const {
        return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isContain(range);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool
    MutableRangeNode<KEY_TYPE, DATA_TYPE>::isOverlap(const shock_audio::RangeItem<KEY_TYPE, DATA_TYPE> &other) const {
        return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isOverlap(other);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool MutableRangeNode<KEY_TYPE, DATA_TYPE>::isOverlap(KEY_TYPE from, KEY_TYPE to) const {
        return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isOverlap(from, to);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool MutableRangeNode<KEY_TYPE, DATA_TYPE>::isOverlap(std::pair<KEY_TYPE, KEY_TYPE> range) const {
        return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isOverlap(range);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool MutableRangeNode<KEY_TYPE, DATA_TYPE>::isOverlap(KEY_TYPE other) const {
        return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isOverlap(other);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    KEY_TYPE MutableRangeNode<KEY_TYPE, DATA_TYPE>::getTo() const {
        return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::getTo();
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    KEY_TYPE MutableRangeNode<KEY_TYPE, DATA_TYPE>::getFrom() const {
        return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::getFrom();
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::vector<DATA_TYPE> MutableRangeNode<KEY_TYPE, DATA_TYPE>::getValue() const {
        return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::getValue();
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::pair<KEY_TYPE, KEY_TYPE> MutableRangeNode<KEY_TYPE, DATA_TYPE>::getRange() const {
        return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::getRange();
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    int MutableRangeNode<KEY_TYPE, DATA_TYPE>::containCount(DATA_TYPE value) const {
        return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::containCount(value);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> MutableRangeNode<KEY_TYPE, DATA_TYPE>::generateNULLNode() {
        return std::make_unique<MutableRangeNode<KEY_TYPE, DATA_TYPE>>((KEY_TYPE) NULL,(KEY_TYPE) NULL, (DATA_TYPE) NULL, Color::DOUBLE_BLACK);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeNode<KEY_TYPE, DATA_TYPE>::assertRange(KEY_TYPE from, KEY_TYPE to) {
        if (from >= to && from != NULL && to != NULL)
            throw shock_audio::InvalidRangeException();
    }
}