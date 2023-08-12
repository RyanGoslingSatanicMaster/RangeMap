#ifndef SHOCK_AUDIO_MUTABLERANGEMAP_H
#define SHOCK_AUDIO_MUTABLERANGEMAP_H

#include <utility>
#include <vector>
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

        void put(KEY_TYPE from, KEY_TYPE to, DATA_TYPE value);

        void put(MutableRangeNode<KEY_TYPE, DATA_TYPE> node);

        void put(std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value);

        void removeByRange(std::pair<KEY_TYPE, KEY_TYPE> range);

        void removeAllInRange(std::pair<KEY_TYPE, KEY_TYPE> range);

        void removeByKey(KEY_TYPE key);

        void removeInValue(DATA_TYPE val);

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

        std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> removeAllInRangeRecurr(std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> node, std::pair<KEY_TYPE, KEY_TYPE> range);

        std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> removeByRangeRecurr(std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> node, std::pair<KEY_TYPE, KEY_TYPE> range);

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
