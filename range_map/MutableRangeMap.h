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

        std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> get(KEY_TYPE key, unsigned int count) const override;

        std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getBy(unsigned int count, std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE>*)> predicate) const override;

        std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getByOverlap(KEY_TYPE from, KEY_TYPE to, unsigned int count) const override;

        std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getByContain(KEY_TYPE from, KEY_TYPE to, unsigned int count) const override;

        std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getAll(KEY_TYPE key) const override;

        std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getAllByOverlap(KEY_TYPE from, KEY_TYPE to) const override;

        std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getAllByContain(KEY_TYPE from, KEY_TYPE to) const override;

        std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getAllBy(std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE>*)> predicate) const override;

        std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getTestAllByKey(KEY_TYPE key) const;

        unsigned int size() const override;

        void put(KEY_TYPE from, KEY_TYPE to, DATA_TYPE value);

        void put(std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value);

        void removeByOverlapRange(std::pair<KEY_TYPE, KEY_TYPE> range);

        void removeByContainRange(std::pair<KEY_TYPE, KEY_TYPE> range);

        void removeByRange(std::pair<KEY_TYPE, KEY_TYPE> range);

        void removeByOverlapRange(std::pair<KEY_TYPE, KEY_TYPE> range, unsigned int count);

        void removeByContainRange(std::pair<KEY_TYPE, KEY_TYPE> range, unsigned int count);

        void removeAllByOverlapRange(std::pair<KEY_TYPE, KEY_TYPE> range);

        void removeAllByContainRange(std::pair<KEY_TYPE, KEY_TYPE> range);

        void removeByKey(KEY_TYPE key);

        void removeByKey(KEY_TYPE key, unsigned int count);

        void removeAllByKey(KEY_TYPE key);

        void removeIf(std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE>&)> predicate);

        void removeAllIf(std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE>&)> predicate);

        void removeIf(unsigned int count, std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE>&)> predicate);

        void removeByValue(DATA_TYPE val);

        void removeByValue(DATA_TYPE val, unsigned int count);

        void removeAllByValue(DATA_TYPE val);

        void merge(MutableRangeMap<KEY_TYPE, DATA_TYPE> &map);

        void merge(std::unique_ptr<MutableRangeMap<KEY_TYPE, DATA_TYPE>> map);

        std::unique_ptr<MutableRangeMap<KEY_TYPE, DATA_TYPE>> split(std::pair<KEY_TYPE, KEY_TYPE> range, bool isByOverlap);

        RangeNode<KEY_TYPE, DATA_TYPE>* getMaxNode() const override;

        RangeNode<KEY_TYPE, DATA_TYPE>* getMinNode() const override;

        int getBalanceDifference() const override;

        int getBlackDepth() const override;

        RangeNode<KEY_TYPE, DATA_TYPE>* getRoot() const override;

        void printTree() const override;

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

        std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>> removeByValueRecur(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node, DATA_TYPE key);

        std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>> removeIfRecur(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node, std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE>&)> predicate);

        void getByOverlapRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node, std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> *list, std::pair<KEY_TYPE, KEY_TYPE> range, unsigned int* count) const;

        void getByContainRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node, std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> *list, std::pair<KEY_TYPE, KEY_TYPE> range, unsigned int* count) const;

        void getRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node, std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> *list, KEY_TYPE key, unsigned int* count) const;

        void getByRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node, std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> *list, std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE>*)> predicate, unsigned int* count) const;

        void getAllByOverlapRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node, std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> *list, std::pair<KEY_TYPE, KEY_TYPE> range) const;

        void getAllByContainRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node, std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> *list, std::pair<KEY_TYPE, KEY_TYPE> range) const;

        void getAllRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node, std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> *list, KEY_TYPE key) const;

        void getAllByRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node, std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> *list, std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE>*)> predicate) const;

        void getTestAllByKeyRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node, std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> *list, KEY_TYPE key, unsigned int *countComplexity) const;

        const RangeItem<KEY_TYPE, DATA_TYPE>* getByRange(KEY_TYPE from, KEY_TYPE to) const override;

        shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* getMaxNode(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) const;

        shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* getMinNode(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) const;

        bool isNullNode(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node);

        void updateMinMax(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node);

        void updateColorsBeforeDelete(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node);

        BalanceCaseInsert checkBalanceCaseAfterInsert(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node);

        BalanceCaseDelete checkBalanceCaseAfterDelete(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node);

        void printTreeRecurrent(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node, int space) const;

        int getDepth(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) const;

        int checkDifference(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node) const;

        bool isContain(std::pair<KEY_TYPE, KEY_TYPE> range, shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node) const;

        unsigned int sizeRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node) const;

    };

}

#endif //SHOCK_AUDIO_MUTABLERANGEMAP_H
