
#ifndef SHOCK_AUDIO_MUTABLERANGENODE_H
#define SHOCK_AUDIO_MUTABLERANGENODE_H

#include "RangeNode.h"

namespace shock_audio {

    enum class Color{
        RED, BLACK, DOUBLE_BLACK
    };

    template<typename KEY_TYPE, typename DATA_TYPE>
    class MutableRangeNode: public RangeNode<KEY_TYPE, DATA_TYPE> {
    public:

        MutableRangeNode() = delete;

        MutableRangeNode(MutableRangeNode<KEY_TYPE, DATA_TYPE> const &node);

        explicit MutableRangeNode(KEY_TYPE from, KEY_TYPE to, DATA_TYPE value);

        explicit MutableRangeNode(std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value);

        explicit MutableRangeNode(KEY_TYPE from, KEY_TYPE to, DATA_TYPE value, Color color);

        explicit MutableRangeNode(std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value, Color color);

        explicit MutableRangeNode(std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value,
                std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> left, std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> right);

        explicit MutableRangeNode(KEY_TYPE from, KEY_TYPE to, DATA_TYPE value,
        std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> left, std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> right);

        void setMax(KEY_TYPE max);

        KEY_TYPE getMax();

        void setFrom(KEY_TYPE from);

        void setTo(KEY_TYPE to);

        void setRange(KEY_TYPE from, KEY_TYPE to);

        void setRange(std::pair<KEY_TYPE, KEY_TYPE> range);

        void addValue(DATA_TYPE value);

        void setValue(std::vector<DATA_TYPE> val);

        void removeValue(DATA_TYPE value);

        void setColor(Color newColor);

        Color getColor();

        std::pair<KEY_TYPE, KEY_TYPE> getRange() const override;

        KEY_TYPE getFrom() const override;

        KEY_TYPE getTo() const override;

        std::vector<DATA_TYPE> getValue() const override;

        RangeNode<KEY_TYPE, DATA_TYPE>* getLeft() const override;

        RangeNode<KEY_TYPE, DATA_TYPE>* getRight() const override;

        void setLeft(std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> left);

        void setRight(std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> right);

        MutableRangeNode<KEY_TYPE, DATA_TYPE>* getRightPtr();

        MutableRangeNode<KEY_TYPE, DATA_TYPE>* getLeftPtr();

        std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> getRightUniquePtr();

        std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> getLeftUniquePtr();

        static std::unique_ptr<MutableRangeNode<KEY_TYPE,DATA_TYPE>> generateNULLNode();

    private:
        KEY_TYPE _from;
        KEY_TYPE _to;
        KEY_TYPE _max;
        Color _color{Color::BLACK};
        std::vector<DATA_TYPE> _value;
        std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> _left{nullptr};
        std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> _right{nullptr};

        void assertRange(KEY_TYPE from, KEY_TYPE to);
    };

}

#endif //SHOCK_AUDIO_MUTABLERANGENODE_H