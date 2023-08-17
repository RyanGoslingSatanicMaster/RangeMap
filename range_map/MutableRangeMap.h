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

        void removeFirstByOverlapRange(std::pair<KEY_TYPE, KEY_TYPE> range);

        void removeFirstByContainRange(std::pair<KEY_TYPE, KEY_TYPE> range);

        void removeByOverlapRange(std::pair<KEY_TYPE, KEY_TYPE> range, unsigned int count);

        void removeByContainRange(std::pair<KEY_TYPE, KEY_TYPE> range, unsigned int count);

        void removeAllByOverlapRange(std::pair<KEY_TYPE, KEY_TYPE> range);

        void removeAllByContainRange(std::pair<KEY_TYPE, KEY_TYPE> range);

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

        shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* getMaxNode();

        shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* getMinNode();

        int getBalanceDifference();

        int getMaxDepth();

        RangeNode<KEY_TYPE, DATA_TYPE>* getRoot();

        void printTree();

    private:
        std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> _root{nullptr};

        std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> insert(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node, std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value);

        std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> rebalanceTreeAfterInsert(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node);

        std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> rebalanceTreeAfterDelete(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node);

        std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> successorDelete(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *root, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node);

        std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> leftLeftRotate(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node);

        std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> leftRightRotate(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node);

        std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> rightLeftRotate(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node);

        std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> rightRightRotate(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node);

        std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> switchColors(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node);

        std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> checkIfRootDoubleBlackOrRed(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node);

        std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>> removeByRangeRecur(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node, std::pair<KEY_TYPE, KEY_TYPE> range);

        std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>> removeByOverlapRangeRecur(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node, std::pair<KEY_TYPE, KEY_TYPE> range);

        std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>> removeByContainRangeRecur(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node, std::pair<KEY_TYPE, KEY_TYPE> range);

        std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>> removeByKeyRecur(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node, KEY_TYPE key);

        bool isNullNode(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node);

        void updateMax(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node);

        void updateColorsBeforeDelete(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node);

        BalanceCaseInsert checkBalanceCaseAfterInsert(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node);

        BalanceCaseDelete checkBalanceCaseAfterDelete(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node);

        void printTreeRecurrent(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node, int space);

        int getDepth(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node);

        int checkDifference(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node);

        bool isContain(std::pair<KEY_TYPE, KEY_TYPE> range, shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node);

    };

}

#endif //SHOCK_AUDIO_MUTABLERANGEMAP_H
