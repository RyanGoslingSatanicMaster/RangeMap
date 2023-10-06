
#ifndef SHOCK_AUDIO_MUTABLERANGENODE_H
#define SHOCK_AUDIO_MUTABLERANGENODE_H

#include "RangeNode.h"

namespace shock_audio_impl {

    enum class Color {
        RED, BLACK, DOUBLE_BLACK
    };

    template<typename KEY_TYPE, typename DATA_TYPE>
    class MutableRangeNode: public shock_audio::RangeNode<KEY_TYPE, DATA_TYPE> {
    public:

        MutableRangeNode() = delete;

        MutableRangeNode(MutableRangeNode<KEY_TYPE, DATA_TYPE> const &node)  :
                shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>(static_cast<shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>&>(node)),
                _max(node._max),
                _color(node._color),
                _left(std::move(node._left)),
                _right(std::move(node._right)),
                _min(node._min) {}

        explicit MutableRangeNode(KEY_TYPE from, KEY_TYPE to, DATA_TYPE value) :
                shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>(from, to, value),
                _max(to),
                _min(from){
            assertRange(from, to);
        }

        explicit MutableRangeNode(std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value) :
                shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>(range, value),
                _max(range.second),
                _min(range.first) {
            assertRange(range.first, range.second);
        }

        explicit MutableRangeNode(KEY_TYPE from, KEY_TYPE to, DATA_TYPE value, Color color) :
                shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>(from, to, value),
                _color(color),
                _max(to),
                _min(from){
            assertRange(from, to);
        }

        explicit MutableRangeNode(std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value, Color color)  :
                shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>(range, value),
                _color(color),
                _max(range.second),
                _min(range.first){
            assertRange(range.first, range.second);
        }

        explicit MutableRangeNode(std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value,
                                  std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> left,
                                  std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> right) :
                shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>(range, value),
                _left(std::move(left)),
                _right(std::move(right)),
                _max(range.second),
                _min(range.first){
            assertRange(range.first, range.second);
        }

        explicit MutableRangeNode(KEY_TYPE from, KEY_TYPE to, DATA_TYPE value,
                                  std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> left,
                                  std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> right)  :
                shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>(from, to, value),
                _left(std::move(left)),
                _right(std::move(right)),
                _max(to),
                _min(from){
            assertRange(from, to);
        }

        void setMax(KEY_TYPE max) {
            _max = max;
        }

        KEY_TYPE getMax() const {
            return _max;
        }

        void setMin(KEY_TYPE min) {
            _min = min;
        }

        KEY_TYPE getMin() const {
            return _min;
        }

        void setFrom(KEY_TYPE from) {
            assertRange(from, shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_to);
            shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_from = from;
        }

        void setTo(KEY_TYPE to) {
            assertRange(shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_from, to);
            shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_to = to;
        }

        void setRange(KEY_TYPE from, KEY_TYPE to) {
            assertRange(from, to);
            shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_from = from;
            shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_to = to;
        }

        void setRange(std::pair<KEY_TYPE, KEY_TYPE> range) {
            assertRange(range.first, range.second);
            shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_from = range.first;
            shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_to = range.second;
        }

        void addValue(DATA_TYPE value) {
            shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_value.push_back(value);
        }

        void setValue(std::vector<DATA_TYPE> val)  {
            shock_audio::RangeItem<KEY_TYPE, DATA_TYPE>::_value = val;
        }

        void removeValue(DATA_TYPE value) {
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

        void setColor(Color newColor) {
            _color = newColor;
        }

        Color getColor() const {
            return _color;
        }

        bool isOverlap(const shock_audio::RangeNode<KEY_TYPE, DATA_TYPE> &other) const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isOverlap(other.getFrom(), other.getTo());
        }

        shock_audio::RangeNode<KEY_TYPE, DATA_TYPE> *getLeft() const override {
            return static_cast<shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>*>(_left.get());
        }

        shock_audio::RangeNode<KEY_TYPE, DATA_TYPE> *getRight() const override {
            return static_cast<shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>*>(_right.get());
        }

        void setLeft(std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> left) {
            _left = std::move(left);
        }

        void setRight(std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> right) {
            _right = std::move(right);
        }

        MutableRangeNode<KEY_TYPE, DATA_TYPE> *getRightPtr() const {
            return _right.get();
        }

        MutableRangeNode<KEY_TYPE, DATA_TYPE> *getLeftPtr() const {
            return _left.get();
        }

        std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> getRightUniquePtr() {
            return std::move(_right);
        }

        std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> getLeftUniquePtr() {
            return std::move(_left);
        }

        static std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> generateNULLNode() {
            return std::make_unique<MutableRangeNode<KEY_TYPE, DATA_TYPE>>((KEY_TYPE) NULL,(KEY_TYPE) NULL, (DATA_TYPE) NULL, Color::DOUBLE_BLACK);
        }

        std::pair<KEY_TYPE, KEY_TYPE> getRange() const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::getRange();
        }

        std::vector<DATA_TYPE> getValue() const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::getValue();
        }

        KEY_TYPE getFrom() const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::getFrom();
        }

        KEY_TYPE getTo() const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::getTo();
        }

        bool isOverlap(KEY_TYPE other) const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isOverlap(other);
        }

        bool isOverlap(std::pair<KEY_TYPE, KEY_TYPE> range) const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isOverlap(range);
        }

        bool isOverlap(KEY_TYPE from, KEY_TYPE to) const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isOverlap(from, to);
        }

        bool isOverlap(const shock_audio::RangeItem<KEY_TYPE, DATA_TYPE> &other) const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isOverlap(other);
        }

        bool isContain(const shock_audio::RangeNode<KEY_TYPE, DATA_TYPE> &other) const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isContain(other.getFrom(), other.getTo());
        }

        bool isContain(std::pair<KEY_TYPE, KEY_TYPE> range) const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isContain(range);
        }

        bool isContain(KEY_TYPE from, KEY_TYPE to) const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isContain(from, to);
        }

        bool isContain(const shock_audio::RangeItem<KEY_TYPE, DATA_TYPE> &other) const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isContain(other);
        }

        bool isMoreThan(const shock_audio::RangeNode<KEY_TYPE, DATA_TYPE> &other) const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isMoreThan(other.getFrom(), other.getTo());
        }

        bool isMoreThan(std::pair<KEY_TYPE, KEY_TYPE> range) const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isMoreThan(range);
        }

        bool isMoreThan(KEY_TYPE from, KEY_TYPE to) const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isMoreThan(from, to);
        }

        bool isMoreThan(const shock_audio::RangeItem<KEY_TYPE, DATA_TYPE> &other) const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isMoreThan(other);
        }

        bool isContained(std::pair<KEY_TYPE, KEY_TYPE> range) const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isContained(range);
        }

        bool isContained(KEY_TYPE from, KEY_TYPE to) const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isContained(from, to);
        }

        bool isContained(const shock_audio::RangeItem<KEY_TYPE, DATA_TYPE> &other) const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isContained(other);
        }

        bool isContained(const shock_audio::RangeNode<KEY_TYPE, DATA_TYPE> &other) const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isContained(other.getFrom(), other.getTo());
        }

        bool isEqualRange(std::pair<KEY_TYPE, KEY_TYPE> range) const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isEqualRange(range);
        }

        bool isEqualRange(KEY_TYPE from, KEY_TYPE to) const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isEqualRange(from, to);
        }

        bool isEqualRange(const shock_audio::RangeItem<KEY_TYPE, DATA_TYPE> &other) const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isEqualRange(other);
        }

        bool isEqualRange(const shock_audio::RangeNode<KEY_TYPE, DATA_TYPE> &other) const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::isEqualRange(other.getFrom(), other.getTo());
        }

        bool containValue(DATA_TYPE value) const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::containValue(value);
        }

        int containCount(DATA_TYPE value) const override {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::containCount(value);
        }

        bool isOverlapMinMax(KEY_TYPE key) const {
            return _min <= key && _max >= key;
        }

        bool isOverlapMinMax(KEY_TYPE from, KEY_TYPE to) const {
            return _min <= to && _min >= from
                   || _max <= to && _max >= from
                   || _min <= from && _max >= to
                   || from <= _min && to >= _max;
        }

        bool isOverlapMinMax(std::pair<KEY_TYPE, KEY_TYPE> range) const {
            return isOverlapMinMax(range.first, range.second);
        }

        bool isContainMinMax(KEY_TYPE from, KEY_TYPE to) const {
            return _min <= from && _max >= to;
        }

        bool isContainMinMax(std::pair<KEY_TYPE, KEY_TYPE> range) const {
            return isContainMinMax(range.first, range.second);
        }

        bool isContainedMinMax(KEY_TYPE from, KEY_TYPE to) const {
            return _min >= from && _max <= to;
        }

        bool isContainedMinMax(std::pair<KEY_TYPE, KEY_TYPE> range) const {
            return isContainedMinMax(range.first, range.second);
        }

        bool operator==(const MutableRangeNode<KEY_TYPE, DATA_TYPE>& other) {
            return shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>::operator==((shock_audio::RangeNode<KEY_TYPE, DATA_TYPE>) other) &&
                   _max == other.getMax() && _color == other.getColor();
        }

    private:
        KEY_TYPE _max;
        KEY_TYPE _min;
        Color _color{Color::BLACK};
        std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> _left{nullptr};
        std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> _right{nullptr};

        void assertRange(KEY_TYPE from, KEY_TYPE to){
            if (from >= to && from != NULL && to != NULL)
                throw shock_audio::InvalidRangeException();
        }
    };

}

#endif //SHOCK_AUDIO_MUTABLERANGENODE_H
