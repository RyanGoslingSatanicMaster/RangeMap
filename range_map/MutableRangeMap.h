#ifndef SHOCK_AUDIO_MUTABLERANGEMAP_H
#define SHOCK_AUDIO_MUTABLERANGEMAP_H

#include <utility>
#include <vector>
#include <functional>
#include "MutableRangeNode.h"
#include "RangeNode.h"
#include "RangeMap.h"

namespace shock_audio {

    template<typename KEY_TYPE, typename DATA_TYPE>
    class MutableRangeMap: public RangeMap<KEY_TYPE, DATA_TYPE>{

        enum BalanceCaseInsert{
            LL, LR, RL, RR, SC, NOTHING
        };

        enum BalanceCaseDelete{
            BB, LRED, RRED, BRLL, BRLR, BRRL, BRRR, NO
        };

    public:

        MutableRangeMap();

        explicit MutableRangeMap(KEY_TYPE from, KEY_TYPE to, DATA_TYPE value);

        explicit MutableRangeMap(std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value);

        std::vector<DATA_TYPE> get(KEY_TYPE key) const override;

        std::vector<DATA_TYPE> get(KEY_TYPE from, KEY_TYPE to) const override;

        std::vector<DATA_TYPE> getBy(std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE>&)> predicate) const override;

        void put(KEY_TYPE from, KEY_TYPE to, DATA_TYPE value);

        void put(std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value);

        void removeFirstByRange(std::pair<KEY_TYPE, KEY_TYPE> range);

        void removeByRange(std::pair<KEY_TYPE, KEY_TYPE> range, unsigned int count);

        void removeAllInRange(std::pair<KEY_TYPE, KEY_TYPE> range);

        void removeFirstByKey(KEY_TYPE key);

        void removeByKey(KEY_TYPE key, unsigned int count);

        void removeAllByKey(KEY_TYPE key);

        void removeIf(std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE>&)> predicate);

        void removeFirstByValue(DATA_TYPE val);

        void removeByValue(DATA_TYPE val, unsigned int count);

        void removaAllByValue(DATA_TYPE val);

        void merge(MutableRangeMap<KEY_TYPE, DATA_TYPE> &map);

        void merge(std::unique_ptr<MutableRangeMap<KEY_TYPE, DATA_TYPE>> map);

        std::unique_ptr<MutableRangeMap<KEY_TYPE, DATA_TYPE>> split(std::pair<KEY_TYPE, KEY_TYPE> range, bool isByOverlap);

        MutableRangeNode<KEY_TYPE, DATA_TYPE>* getMaxNode();

        MutableRangeNode<KEY_TYPE, DATA_TYPE>* getMinNode();

        int getBalanceDifference();

        int getMaxDepth();

        RangeNode<KEY_TYPE, DATA_TYPE>* getRoot();

        void printTree();

    private:
        std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> _root{nullptr};

        std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> insert(std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> node, std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value);

        std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> rebalanceTreeAfterInsert(std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> node);

        std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> rebalanceTreeAfterDelete(std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> node);

        std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> successorDelete(MutableRangeNode<KEY_TYPE, DATA_TYPE> *root, std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> node);

        std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> leftLeftRotate(std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> node);

        std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> leftRightRotate(std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> node);

        std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> rightLeftRotate(std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> node);

        std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> rightRightRotate(std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> node);

        std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> switchColors(std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> node);

        std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> checkIfRootDoubleBlackOrRed(std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> node);

        std::pair<bool, std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>>> removeByRangeRecur(std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> node, std::pair<KEY_TYPE, KEY_TYPE> range);

        bool isNullNode(MutableRangeNode<KEY_TYPE, DATA_TYPE> *node);



        void updateMax(MutableRangeNode<KEY_TYPE, DATA_TYPE> *node);

        void updateColorsBeforeDelete(MutableRangeNode<KEY_TYPE, DATA_TYPE> *node);

        BalanceCaseInsert checkBalanceCaseAfterInsert(MutableRangeNode<KEY_TYPE, DATA_TYPE> *node);

        BalanceCaseDelete checkBalanceCaseAfterDelete(MutableRangeNode<KEY_TYPE, DATA_TYPE> *node);

        void printTreeRecurrent(MutableRangeNode<KEY_TYPE, DATA_TYPE> *node, int space);

        int getDepth(MutableRangeNode<KEY_TYPE, DATA_TYPE> *node);

        int checkDifference(MutableRangeNode<KEY_TYPE, DATA_TYPE>* node);

    };

}

#endif //SHOCK_AUDIO_MUTABLERANGEMAP_H
