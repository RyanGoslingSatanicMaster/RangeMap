#ifndef SHOCK_AUDIO_MUTABLERANGEMAP_H
#define SHOCK_AUDIO_MUTABLERANGEMAP_H

#include <utility>
#include <vector>
#include <functional>
#include <iostream>
#include "MutableRangeNode.h"
#include "RangeNode.h"
#include "RangeMap.h"
#include "RangeItem.h"

namespace shock_audio {

    template<typename DATA_TYPE>
    std::ostream &operator<<(std::ostream &output, const std::vector<DATA_TYPE> &value) {
        for (auto node: value) {
            output << node << " ";
        }
        return output;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    class MutableRangeMap: public RangeMap<KEY_TYPE, DATA_TYPE>{

        enum BalanceCaseInsert{
            LL, LR, RL, RR, SC, NOTHING
        };

        enum BalanceCaseDelete{
            BB, LRED, RRED, BRLL, BRLR, BRRL, BRRR, NO
        };

    public:

        MutableRangeMap() = default;;

        explicit MutableRangeMap(KEY_TYPE from, KEY_TYPE to, DATA_TYPE value) :
                _root(std::make_unique<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>(from, to, value,
                                                                                                shock_audio_impl::Color::BLACK)) {}

        explicit MutableRangeMap(std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value) :
                _root(std::make_unique<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>(range, value,
                                                                                                shock_audio_impl::Color::BLACK)) {}

        /** GET Functions*/

        std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> get(KEY_TYPE key, unsigned int count = 1) const override {
            std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> list;
            getRecur(_root.get(), &list, key, &count);
            return list;
        }

        std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getIf(std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE>*)> predicate, unsigned int count = 1) const override {
            std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> list;
            auto p = &list;
            getByRecur(_root.get(), p, predicate, &count);
            return list;
        }

        std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getByOverlap(KEY_TYPE from, KEY_TYPE to, unsigned int count = 1) const override {
            std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> list;
            auto p = &list;
            getByOverlapRecur(_root.get(), p, std::pair(from, to), &count);
            return list;
        }

        std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getByContain(KEY_TYPE from, KEY_TYPE to, unsigned int count = 1) const override {
            std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> list;
            auto p = &list;
            getByContainRecur(_root.get(), p, std::pair(from, to), &count);
            return list;
        }

        std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getByContained(KEY_TYPE from, KEY_TYPE to, unsigned int count = 1) const override {
            std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> list;
            auto p = &list;
            getByContainedRecur(_root.get(), p, std::pair(from, to), &count);
            return list;
        }

        const RangeItem<KEY_TYPE, DATA_TYPE>* getByRange(KEY_TYPE from, KEY_TYPE to) const override  {
            return getByRangeRecur(_root.get(), std::pair(from, to));
        }

        std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getAll(KEY_TYPE key) const override {
            std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> list;
            getAllRecur(_root.get(), &list, key);
            return list;
        }

        std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getAllByOverlap(KEY_TYPE from, KEY_TYPE to) const override {
            std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> list;
            getAllByOverlapRecur(_root.get(), &list, std::pair(from, to));
            return list;
        }

        std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getAllByContain(KEY_TYPE from, KEY_TYPE to) const override {
            std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> list;
            getAllByContainRecur(_root.get(), &list, std::pair(from, to));
            return list;
        }

        std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getAllByContained(KEY_TYPE from, KEY_TYPE to) const override {
            std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> list;
            getAllByContainedRecur(_root.get(), &list, std::pair(from, to));
            return list;
        }

        std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getAllIf(std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE>*)> predicate) const override {
            std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> list;
            getAllIfRecur(_root.get(), &list, predicate);
            return list;
        }

        std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getTestAllByKey(KEY_TYPE key) const {
            std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> list;
            unsigned int countComplexity = 0;
            getTestAllByKeyRecur(_root.get(), &list, key, &countComplexity);
            std::cout<<"CountComplexity: "<< countComplexity<<std::endl;
            std::cout<<"ResultCount: "<<list.size()<<std::endl<<std::endl;
//        std::cout<<"SizeOfMap: "<< size() <<std::endl;
            return list;
        }

        /**END GET Functions*/

        /** INSERT Functions*/

        void put(KEY_TYPE from, KEY_TYPE to, DATA_TYPE value) {
            _root = std::move(
                    checkIfRootDoubleBlackOrRed(insert(std::move(_root), std::pair<KEY_TYPE, KEY_TYPE>(from, to), value)));
        }

        void put(std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value)  {
            _root = std::move(checkIfRootDoubleBlackOrRed(insert(std::move(_root), range, value)));
        }

        /**END INSERT Functions*/

        /** DELETE Functions*/

        void removeByOverlapRange(std::pair<KEY_TYPE, KEY_TYPE> range, unsigned int count = 1) {
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> new_root = std::move(_root);
            auto resFlag = true;
            while (resFlag && count > 0) {
                auto result = removeByOverlapRangeRecur(std::move(new_root), range);
                new_root = std::move(checkIfRootDoubleBlackOrRed(std::move(result.second)));
                resFlag = result.first;
                count--;
            }
            _root = std::move(new_root);
        }

        void removeByContainRange(std::pair<KEY_TYPE, KEY_TYPE> range, unsigned int count = 1) {
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> new_root = std::move(_root);
            auto resFlag = true;
            while (resFlag && count > 0) {
                auto result = removeByContainRangeRecur(std::move(new_root), range);
                new_root = std::move(checkIfRootDoubleBlackOrRed(std::move(result.second)));
                resFlag = result.first;
                count--;
            }
            _root = std::move(new_root);
        }

        void removeByRange(std::pair<KEY_TYPE, KEY_TYPE> range) {
            auto result = removeByRangeRecur(std::move(_root), range);
            _root = std::move(checkIfRootDoubleBlackOrRed(std::move(result.second)));
        }

        void removeByContainedRange(std::pair<KEY_TYPE, KEY_TYPE> range, unsigned int count = 1) {
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> new_root = std::move(_root);
            auto resFlag = true;
            while (resFlag && count > 0) {
                auto result = removeByContainedRangeRecur(std::move(new_root), range);
                new_root = std::move(checkIfRootDoubleBlackOrRed(std::move(result.second)));
                resFlag = result.first;
                count--;
            }
            _root = std::move(new_root);
        }

        void removeIf(std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE>*)> predicate, unsigned int count = 1) {
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> new_root = std::move(_root);
            auto resFlag = true;
            while (resFlag && count > 0) {
                auto result = removeIfRecur(std::move(new_root), predicate);
                new_root = std::move(checkIfRootDoubleBlackOrRed(std::move(result.second)));
                resFlag = result.first;
                count--;
            }
            _root = std::move(new_root);
        }

        void removeByValue(DATA_TYPE val, unsigned int count = 1) {
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> new_root = std::move(_root);
            auto resFlag = true;
            while (resFlag && count > 0) {
                auto result = removeByValueRecur(std::move(new_root), val);
                new_root = std::move(checkIfRootDoubleBlackOrRed(std::move(result.second)));
                resFlag = result.first;
                count--;
            }
            _root = std::move(new_root);
        }

        void removeByKey(KEY_TYPE key, unsigned int count = 1) {
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> new_root = std::move(_root);
            auto resFlag = true;
            while (resFlag && count > 0) {
                auto result = removeByKeyRecur(std::move(new_root), key);
                new_root = std::move(checkIfRootDoubleBlackOrRed(std::move(result.second)));
                resFlag = result.first;
                count--;
            }
            _root = std::move(new_root);
        }

        void removeAllByOverlapRange(std::pair<KEY_TYPE, KEY_TYPE> range) {
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> new_root = std::move(_root);
            auto resFlag = true;
            while (resFlag) {
                auto result = removeByOverlapRangeRecur(std::move(new_root), range);
                resFlag = result.first;
                new_root = std::move(checkIfRootDoubleBlackOrRed(std::move(result.second)));
            }
            _root = std::move(new_root);
        }

        void removeAllByContainRange(std::pair<KEY_TYPE, KEY_TYPE> range) {
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> new_root = std::move(_root);
            auto resFlag = true;
            while (resFlag) {
                auto result = removeByContainRangeRecur(std::move(new_root), range);
                resFlag = result.first;
                new_root = std::move(checkIfRootDoubleBlackOrRed(std::move(result.second)));
            }
            _root = std::move(new_root);
        }

        void removeAllByContainedRange(std::pair<KEY_TYPE, KEY_TYPE> range) {
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> new_root = std::move(_root);
            auto resFlag = true;
            while (resFlag) {
                auto result = removeByContainedRangeRecur(std::move(new_root), range);
                new_root = std::move(checkIfRootDoubleBlackOrRed(std::move(result.second)));
                resFlag = result.first;
            }
            _root = std::move(new_root);
        }

        void removeAllByKey(KEY_TYPE key) {
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> new_root = std::move(_root);
            auto resFlag = true;
            while (resFlag) {
                auto result = removeByKeyRecur(std::move(new_root), key);
                new_root = std::move(checkIfRootDoubleBlackOrRed(std::move(result.second)));
                resFlag = result.first;
            }
            _root = std::move(new_root);
        }

        void removeAllIf(std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE>*)> predicate) {
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> new_root = std::move(_root);
            auto resFlag = true;
            while (resFlag) {
                auto result = removeIfRecur(std::move(new_root), predicate);
                new_root = std::move(checkIfRootDoubleBlackOrRed(std::move(result.second)));
                resFlag = result.first;
            }
            _root = std::move(new_root);
        }

        void removeAllByValue(DATA_TYPE val) {
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> new_root = std::move(_root);
            auto resFlag = true;
            while (resFlag) {
                auto result = removeByValueRecur(std::move(new_root), val);
                new_root = std::move(checkIfRootDoubleBlackOrRed(std::move(result.second)));
                resFlag = result.first;
            }
            _root = std::move(new_root);
        }

        /**END DELETE Functions*/

        /** MERGE Functions*/

        void merge(MutableRangeMap<KEY_TYPE, DATA_TYPE> &map) {
            auto currentBlackHeight = getBlackDepth();
            auto mapBlackHeight = map.getBlackDepth();
            if (currentBlackHeight == mapBlackHeight){

            } else if (currentBlackHeight > mapBlackHeight) {

            } else {

            }

        }

        void merge(std::unique_ptr<MutableRangeMap<KEY_TYPE, DATA_TYPE>> map);

        /** END MERGE Functions*/

        /** SPLIT Functions*/

        std::unique_ptr<MutableRangeMap<KEY_TYPE, DATA_TYPE>> splitByOverlap(std::pair<KEY_TYPE, KEY_TYPE> range);

        /** END SPLIT Functions*/

        /** UTILS Functions*/

        RangeNode<KEY_TYPE, DATA_TYPE>* getMaxNode() const override {
            return (RangeNode<KEY_TYPE, DATA_TYPE>*) getMaxNode(_root.get());
        }

        RangeNode<KEY_TYPE, DATA_TYPE>* getMinNode() const override {
            return (RangeNode<KEY_TYPE, DATA_TYPE>*) getMinNode(_root.get());
        }

        void traversal(std::function<void(const RangeNode<KEY_TYPE, DATA_TYPE>*)> predicate) const override {
            traversalRecur(_root.get(), predicate);
        }

        int getBalanceDifference() const override {
            return checkDifference(_root.get());
        }

        int getBlackDepth() const override {
            return getDepth(_root.get());
        }

        unsigned int size() const override {
            return sizeRecur(_root.get());
        }

        RangeNode<KEY_TYPE, DATA_TYPE>* getRoot() const override{
            return _root.get();
        }

        void printTree() const override {
            printTreeRecurrent(_root.get(), 0);
        }

        /** END UTILS Functions*/

        ~MutableRangeMap() = default;

    private:
        std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> _root{nullptr};

        /** REBALANCE Functions*/

        std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> successorDelete(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *root, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
            if (node->getLeftPtr()->getLeftPtr() != nullptr) {
                node->setLeft(successorDelete(root, node->getLeftUniquePtr()));
            } else {
                root->setRange(node->getLeftPtr()->getRange());
                root->setValue(node->getLeftPtr()->getValue());
                updateColorsBeforeDelete(node->getLeftPtr());
                node->setLeft(node->getLeftPtr()->getRightUniquePtr());
                updateMinMax(node->getLeftPtr());
                updateMinMax(root);
                return rebalanceTreeAfterDelete(std::move(node));
            }
            updateMinMax(node->getLeftPtr());
            return rebalanceTreeAfterDelete(std::move(node));
        }

        void updateMinMax(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) {
            if (node == nullptr)
                return;
            auto nodeMax = node->getRange().second;
            auto nodeMin = node->getRange().first;
            if (node->getLeftPtr() == nullptr || isNullNode(node->getLeftPtr())) {
                if (node->getRightPtr() == nullptr || isNullNode(node->getRightPtr()))
                    node->setMax(nodeMax);
                else {
                    auto rightMax = node->getRightPtr()->getMax();
                    if (rightMax > nodeMax)
                        node->setMax(rightMax);
                    else
                        node->setMax(nodeMax);
                }
                node->setMin(nodeMin);
            }else if (node->getRightPtr() == nullptr || isNullNode(node->getRightPtr())) {
                auto leftMax = node->getLeftPtr()->getMax();
                auto leftMin = node->getLeftPtr()->getMin();
                if (leftMax > nodeMax)
                    node->setMax(leftMax);
                else
                    node->setMax(nodeMax);
                node->setMin(leftMin);
            } else {
                auto leftMin = node->getLeftPtr()->getMin();
                auto max = std::max(node->getLeftPtr()->getMax(), node->getRightPtr()->getMax());
                if (max > nodeMax)
                    node->setMax(max);
                else
                    node->setMax(nodeMax);
                node->setMin(leftMin);
            }
        }

        void updateColorsBeforeDelete(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) {
            if (node->getRightPtr() == nullptr && node->getLeftPtr() == nullptr) {
                if (node->getColor() == shock_audio_impl::Color::BLACK) {
                    node->setLeft(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>::generateNULLNode());
                    node->setRight(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>::generateNULLNode());
                }
                return;
            }
            if (node->getRightPtr() == nullptr) {
                if (node->getColor() == shock_audio_impl::Color::RED ||
                    node->getLeftPtr()->getColor() == shock_audio_impl::Color::RED) {
                    node->getLeftPtr()->setColor(shock_audio_impl::Color::BLACK);
                } else {
                    node->getLeftPtr()->setColor(shock_audio_impl::Color::DOUBLE_BLACK);
                }
            } else {
                if (node->getColor() == shock_audio_impl::Color::RED ||
                    node->getRightPtr()->getColor() == shock_audio_impl::Color::RED) {
                    node->getRightPtr()->setColor(shock_audio_impl::Color::BLACK);
                } else {
                    node->getRightPtr()->setColor(shock_audio_impl::Color::DOUBLE_BLACK);
                }
            }
        }

        BalanceCaseInsert checkBalanceCaseAfterInsert(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) {
            if (node == nullptr || node->getColor() == shock_audio_impl::Color::RED)
                return NOTHING;
            else {
                auto leftChild = node->getLeftPtr();
                auto rightChild = node->getRightPtr();
                auto flagSwitchColor = false;
                if (leftChild != nullptr && leftChild->getColor() == shock_audio_impl::Color::RED &&
                    rightChild != nullptr && rightChild->getColor() == shock_audio_impl::Color::RED
                        )
                    flagSwitchColor = true;
                if (rightChild == nullptr || rightChild->getColor() == shock_audio_impl::Color::BLACK) {
                    if (leftChild == nullptr || leftChild->getColor() == shock_audio_impl::Color::BLACK)
                        return NOTHING;
                    else {
                        auto leftGrandChild = leftChild->getLeftPtr();
                        auto rightGrandChild = leftChild->getRightPtr();
                        if (leftGrandChild == nullptr || leftGrandChild->getColor() == shock_audio_impl::Color::BLACK)
                            if (rightGrandChild == nullptr || rightGrandChild->getColor() == shock_audio_impl::Color::BLACK)
                                return NOTHING;
                            else {
                                if (flagSwitchColor)
                                    return SC;
                                else
                                    return LR;
                            }
                        else {
                            if (flagSwitchColor)
                                return SC;
                            else
                                return LL;
                        }
                    }
                } else {
                    if (leftChild == nullptr || leftChild->getColor() == shock_audio_impl::Color::BLACK) {
                        auto leftGrandChild = rightChild->getLeftPtr();
                        auto rightGrandChild = rightChild->getRightPtr();
                        if (leftGrandChild == nullptr || leftGrandChild->getColor() == shock_audio_impl::Color::BLACK)
                            if (rightGrandChild == nullptr || rightGrandChild->getColor() == shock_audio_impl::Color::BLACK)
                                return NOTHING;
                            else {
                                if (flagSwitchColor)
                                    return SC;
                                else
                                    return RR;
                            }
                        else {
                            if (flagSwitchColor)
                                return SC;
                            else
                                return RL;
                        }
                    }
                }
                if (flagSwitchColor)
                    return SC;
            }
        }

        BalanceCaseDelete checkBalanceCaseAfterDelete(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) {
            if (node->getLeftPtr() == nullptr || node->getRightPtr() == nullptr ||
                node->getLeftPtr()->getColor() != shock_audio_impl::Color::DOUBLE_BLACK &&
                node->getRightPtr()->getColor() != shock_audio_impl::Color::DOUBLE_BLACK)
                return NO;
            if (node->getLeftPtr()->getColor() == shock_audio_impl::Color::DOUBLE_BLACK) {
                if (node->getRightPtr()->getColor() == shock_audio_impl::Color::RED)
                    return RRED;
                else if (node->getRightPtr()->getRightPtr() != nullptr &&
                         node->getRightPtr()->getRightPtr()->getColor() == shock_audio_impl::Color::RED)
                    return BRRR;
                else if (node->getRightPtr()->getLeftPtr() == nullptr ||
                         node->getRightPtr()->getLeftPtr()->getColor() == shock_audio_impl::Color::BLACK)
                    return BB;
                else
                    return BRRL;

            } else {
                if (node->getLeftPtr()->getColor() == shock_audio_impl::Color::RED)
                    return LRED;
                else if (node->getLeftPtr()->getLeftPtr() != nullptr &&
                         node->getLeftPtr()->getLeftPtr()->getColor() == shock_audio_impl::Color::RED)
                    return BRLL;
                else if (node->getLeftPtr()->getRightPtr() == nullptr ||
                         node->getLeftPtr()->getRightPtr()->getColor() == shock_audio_impl::Color::BLACK)
                    return BB;
                else
                    return BRLR;
            }
        }

        std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> leftLeftRotate(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
            auto temp_subtree = std::move(node->getLeftPtr()->getRightUniquePtr());
            auto new_root = std::move(node->getLeftUniquePtr());
            auto temp_color = new_root->getColor();
            new_root->setMax(node->getMax());
            new_root->setColor(node->getColor());
            new_root->setRight(std::move(node));
            new_root->getRightPtr()->setLeft(std::move(temp_subtree));
            new_root->getRightPtr()->setColor(temp_color);
            updateMinMax(new_root->getRightPtr());
            updateMinMax(new_root->getLeftPtr());
            if (new_root->getLeftPtr() != nullptr)
                new_root->setMin(new_root->getLeftPtr()->getMin());
            else
                new_root->setMin(new_root->getFrom());
            return new_root;
        }

        std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> leftRightRotate(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
            auto temp_subtree = std::move(node->getLeftPtr()->getRightPtr()->getLeftUniquePtr());
            auto new_root = std::move(node->getLeftPtr()->getRightUniquePtr());
            auto temp_color = new_root->getColor();
            new_root->setMax(node->getMax());
            new_root->setColor(node->getLeftPtr()->getColor());
            new_root->setLeft(std::move(node->getLeftUniquePtr()));
            new_root->getLeftPtr()->setColor(temp_color);
            node->setLeft(std::move(new_root));
            node->getLeftPtr()->getLeftPtr()->setRight(std::move(temp_subtree));
            updateMinMax(node->getLeftPtr()->getLeftPtr());
            updateMinMax(node->getLeftPtr()->getRightPtr());
            if (node->getLeftPtr()->getLeftPtr() != nullptr)
                node->getLeftPtr()->setMin(node->getLeftPtr()->getLeftPtr()->getMin());
            else
                node->getLeftPtr()->setMin(node->getLeftPtr()->getFrom());
            return leftLeftRotate(std::move(node));
        }

        std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> rightLeftRotate(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
            auto temp_subtree = std::move(node->getRightPtr()->getLeftPtr()->getRightUniquePtr());
            auto new_root = std::move(node->getRightPtr()->getLeftUniquePtr());
            auto temp_color = new_root->getColor();
            new_root->setMax(node->getMax());
            new_root->setColor(node->getRightPtr()->getColor());
            new_root->setRight(std::move(node->getRightUniquePtr()));
            new_root->getRightPtr()->setColor(temp_color);
            node->setRight(std::move(new_root));
            node->getRightPtr()->getRightPtr()->setLeft(std::move(temp_subtree));
            updateMinMax(node->getRightPtr()->getRightPtr());
            updateMinMax(node->getRightPtr()->getLeftPtr());
            if (node->getRightPtr()->getLeftPtr() != nullptr)
                node->getRightPtr()->setMin(node->getRightPtr()->getLeftPtr()->getMin());
            else
                node->getRightPtr()->setMin(node->getRightPtr()->getFrom());
            return rightRightRotate(std::move(node));
        }

        std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> rightRightRotate(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
            auto temp_subtree = std::move(node->getRightPtr()->getLeftUniquePtr());
            auto new_root = std::move(node->getRightUniquePtr());
            auto temp_color = new_root->getColor();
            new_root->setMax(node->getMax());
            new_root->setColor(node->getColor());
            new_root->setLeft(std::move(node));
            new_root->getLeftPtr()->setRight(std::move(temp_subtree));
            new_root->getLeftPtr()->setColor(temp_color);
            updateMinMax(new_root->getLeftPtr());
            updateMinMax(new_root->getRightPtr());
            if (new_root->getLeftPtr() != nullptr)
                new_root->setMin(new_root->getLeftPtr()->getMin());
            else
                new_root->setMin(new_root->getFrom());
            return new_root;
        }

        std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> switchColors(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
            if (node->getLeftPtr()->getColor() == shock_audio_impl::Color::RED &&
                node->getRightPtr()->getColor() == shock_audio_impl::Color::RED) {
                node->setColor(shock_audio_impl::Color::RED);
                node->getRightPtr()->setColor(shock_audio_impl::Color::BLACK);
                node->getLeftPtr()->setColor(shock_audio_impl::Color::BLACK);
            } else if (node->getLeftPtr()->getColor() == shock_audio_impl::Color::DOUBLE_BLACK
                       && node->getRightPtr()->getColor() == shock_audio_impl::Color::BLACK) {
                if (node->getColor() == shock_audio_impl::Color::BLACK)
                    node->setColor(shock_audio_impl::Color::DOUBLE_BLACK);
                else
                    node->setColor(shock_audio_impl::Color::BLACK);
                node->getLeftPtr()->setColor(shock_audio_impl::Color::BLACK);
                node->getRightPtr()->setColor(shock_audio_impl::Color::RED);
                if (isNullNode(node->getLeftPtr()))
                    node->setLeft(nullptr);
            } else if (node->getRightPtr()->getColor() == shock_audio_impl::Color::DOUBLE_BLACK
                       && node->getLeftPtr()->getColor() == shock_audio_impl::Color::BLACK) {
                if (node->getColor() == shock_audio_impl::Color::BLACK)
                    node->setColor(shock_audio_impl::Color::DOUBLE_BLACK);
                else
                    node->setColor(shock_audio_impl::Color::BLACK);
                node->getLeftPtr()->setColor(shock_audio_impl::Color::RED);
                node->getRightPtr()->setColor(shock_audio_impl::Color::BLACK);
                if (isNullNode(node->getRightPtr()))
                    node->setRight(nullptr);
            }
            return node;
        }

        std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> checkIfRootDoubleBlackOrRed(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
            if (node->getColor() == shock_audio_impl::Color::RED) {
                node->setColor(shock_audio_impl::Color::BLACK);
                return rebalanceTreeAfterInsert(std::move(node));
            } else {
                if (node->getColor() == shock_audio_impl::Color::DOUBLE_BLACK)
                    node->setColor(shock_audio_impl::Color::BLACK);
                return node;
            }
        }

        std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> rebalanceTreeAfterInsert(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
            switch (checkBalanceCaseAfterInsert(node.get())) {
                case LL:
                    return leftLeftRotate(std::move(node));
                case LR:
                    return leftRightRotate(std::move(node));
                case RL:
                    return rightLeftRotate(std::move(node));
                case RR:
                    return rightRightRotate(std::move(node));
                case SC:
                    return switchColors(std::move(node));
                case NOTHING:
                    return node;
            }
        }

        std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> rebalanceTreeAfterDelete(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> temp_node;
            switch (checkBalanceCaseAfterDelete(node.get())) {
                case BB:
                    temp_node = switchColors(std::move(node));
                    break;
                case LRED:
                    temp_node = leftLeftRotate(std::move(node));
                    temp_node->setRight(rebalanceTreeAfterDelete(temp_node->getRightUniquePtr()));
                    break;
                case RRED:
                    temp_node = rightRightRotate(std::move(node));
                    temp_node->setLeft(rebalanceTreeAfterDelete(temp_node->getLeftUniquePtr()));
                    break;
                case BRLL:
                    temp_node = leftLeftRotate(std::move(node));
                    if (isNullNode(temp_node->getRightPtr()->getRightPtr()))
                        temp_node->getRightPtr()->setRight(nullptr);
                    else
                        temp_node->getRightPtr()->getRightPtr()->setColor(shock_audio_impl::Color::BLACK);
                    temp_node->getLeftPtr()->setColor(shock_audio_impl::Color::BLACK);
                    break;
                case BRLR:
                    temp_node = leftRightRotate(std::move(node));
                    if (isNullNode(temp_node->getRightPtr()->getRightPtr()))
                        temp_node->getRightPtr()->setRight(nullptr);
                    else
                        temp_node->getRightPtr()->getRightPtr()->setColor(shock_audio_impl::Color::BLACK);
                    temp_node->getLeftPtr()->setColor(shock_audio_impl::Color::BLACK);
                    break;
                case BRRL:
                    temp_node = rightLeftRotate(std::move(node));
                    if (isNullNode(temp_node->getLeftPtr()->getLeftPtr()))
                        temp_node->getLeftPtr()->setLeft(nullptr);
                    else
                        temp_node->getLeftPtr()->getLeftPtr()->setColor(shock_audio_impl::Color::BLACK);
                    temp_node->getRightPtr()->setColor(shock_audio_impl::Color::BLACK);
                    break;
                case BRRR:
                    temp_node = rightRightRotate(std::move(node));
                    if (isNullNode(temp_node->getLeftPtr()->getLeftPtr()))
                        temp_node->getLeftPtr()->setLeft(nullptr);
                    else
                        temp_node->getLeftPtr()->getLeftPtr()->setColor(shock_audio_impl::Color::BLACK);
                    temp_node->getRightPtr()->setColor(shock_audio_impl::Color::BLACK);
                    break;
                case NO:
                    temp_node = std::move(node);
                    break;
            }
            return temp_node;
        }

        /** END REBALANCE Functions*/

        /** REMOVE INTERNAL Functions*/

        std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>> removeByRangeRecur(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node, std::pair<KEY_TYPE, KEY_TYPE> range)  {
            if (node == nullptr)
                return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(false,
                                                                                                                 std::move(
                                                                                                                         node));
            if (node->isEqualRange(range)) {
                if (node->getLeftPtr() == nullptr) {
                    updateColorsBeforeDelete(node.get());
                    return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(true,
                                                                                                                     std::move(
                                                                                                                             node->getRightUniquePtr()));
                } else if (node->getRightPtr() == nullptr) {
                    updateColorsBeforeDelete(node.get());
                    return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(true,
                                                                                                                     std::move(
                                                                                                                             node->getLeftUniquePtr()));
                } else {
                    if (node->getRightPtr()->getLeftPtr() == nullptr) {
                        node->setRange(node->getRightPtr()->getRange());
                        node->setValue(node->getRightPtr()->getValue());
                        updateColorsBeforeDelete(node->getRightPtr());
                        node->setRight(node->getRightPtr()->getRightUniquePtr());
                        updateMinMax(node.get());
                    } else {
                        node->setRight(successorDelete(node.get(), node->getRightUniquePtr()));
                    }
                    return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(true,
                                                                                                                     rebalanceTreeAfterDelete(
                                                                                                                             std::move(
                                                                                                                                     node)));
                }
            } else {
                std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>> result;
                if (range.first <= node->getFrom()) {
                    result = removeByRangeRecur(node->getLeftUniquePtr(), range);
                    node->setLeft(std::move(result.second));
                    updateMinMax(node.get());
                } else {
                    result = removeByRangeRecur(node->getRightUniquePtr(), range);
                    node->setRight(std::move(result.second));
                    updateMinMax(node.get());
                }
                return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(
                        result.first, rebalanceTreeAfterDelete(std::move(node)));
            }
        }

        std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>> removeByOverlapRangeRecur(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node, std::pair<KEY_TYPE, KEY_TYPE> range) {
            if (node == nullptr)
                return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(false,
                                                                                                                 std::move(
                                                                                                                         node));
            if (node->isOverlap(range)) {
                if (node->getLeftPtr() == nullptr) {
                    updateColorsBeforeDelete(node.get());
                    return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(true,
                                                                                                                     std::move(
                                                                                                                             node->getRightUniquePtr()));
                } else if (node->getRightPtr() == nullptr) {
                    updateColorsBeforeDelete(node.get());
                    return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(true,
                                                                                                                     std::move(
                                                                                                                             node->getLeftUniquePtr()));
                } else {
                    if (node->getRightPtr()->getLeftPtr() == nullptr) {
                        node->setRange(node->getRightPtr()->getRange());
                        node->setValue(node->getRightPtr()->getValue());
                        updateColorsBeforeDelete(node->getRightPtr());
                        node->setRight(node->getRightPtr()->getRightUniquePtr());
                        updateMinMax(node.get());
                    } else {
                        node->setRight(successorDelete(node.get(), node->getRightUniquePtr()));
                    }
                    return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(true,
                                                                                                                     rebalanceTreeAfterDelete(
                                                                                                                             std::move(
                                                                                                                                     node)));
                }
            } else {
                std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>> result;
                if (node->getLeftPtr() != nullptr && node->getLeftPtr()->isOverlapMinMax(range)) {
                    result = removeByOverlapRangeRecur(node->getLeftUniquePtr(), range);
                    node->setLeft(std::move(result.second));
                    updateMinMax(node.get());
                } else if (node->getRightPtr() != nullptr && node->getRightPtr()->isOverlapMinMax(range)) {
                    result = removeByOverlapRangeRecur(node->getRightUniquePtr(), range);
                    node->setRight(std::move(result.second));
                    updateMinMax(node.get());
                } else
                    return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(false, std::move(node));
                return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(
                        result.first, rebalanceTreeAfterDelete(std::move(node)));
            }
        }

        std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>> removeByContainRangeRecur(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node, std::pair<KEY_TYPE, KEY_TYPE> range) {
            if (node == nullptr)
                return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(false,
                                                                                                                 std::move(
                                                                                                                         node));
            if (node->isContain(range)) {
                if (node->getLeftPtr() == nullptr) {
                    updateColorsBeforeDelete(node.get());
                    return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(true,
                                                                                                                     std::move(
                                                                                                                             node->getRightUniquePtr()));
                } else if (node->getRightPtr() == nullptr) {
                    updateColorsBeforeDelete(node.get());
                    return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(true,
                                                                                                                     std::move(
                                                                                                                             node->getLeftUniquePtr()));
                } else {
                    if (node->getRightPtr()->getLeftPtr() == nullptr) {
                        node->setRange(node->getRightPtr()->getRange());
                        node->setValue(node->getRightPtr()->getValue());
                        updateColorsBeforeDelete(node->getRightPtr());
                        node->setRight(node->getRightPtr()->getRightUniquePtr());
                        updateMinMax(node.get());
                    } else {
                        node->setRight(successorDelete(node.get(), node->getRightUniquePtr()));
                    }
                    return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(true,
                                                                                                                     rebalanceTreeAfterDelete(
                                                                                                                             std::move(
                                                                                                                                     node)));
                }
            } else {
                std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>> result;
                if (node->getLeftPtr() != nullptr && node->getLeftPtr()->isContainMinMax(range)) {
                    result = removeByContainRangeRecur(node->getLeftUniquePtr(), range);
                    node->setLeft(std::move(result.second));
                    updateMinMax(node.get());
                } else if (node->getRightPtr() != nullptr && node->getRightPtr()->isContainMinMax(range)) {
                    result = removeByContainRangeRecur(node->getRightUniquePtr(), range);
                    node->setRight(std::move(result.second));
                    updateMinMax(node.get());
                } else
                    return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(false, std::move(node));
                return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(
                        result.first, rebalanceTreeAfterDelete(std::move(node)));
            }
        }

        std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>> removeByContainedRangeRecur(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node, std::pair<KEY_TYPE, KEY_TYPE> range) {
            if (node == nullptr)
                return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(false,
                                                                                                                 std::move(
                                                                                                                         node));
            if (node->isOverlap(range)) {
                if (node->getLeftPtr() == nullptr) {
                    updateColorsBeforeDelete(node.get());
                    return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(true,
                                                                                                                     std::move(
                                                                                                                             node->getRightUniquePtr()));
                } else if (node->getRightPtr() == nullptr) {
                    updateColorsBeforeDelete(node.get());
                    return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(true,
                                                                                                                     std::move(
                                                                                                                             node->getLeftUniquePtr()));
                } else {
                    if (node->getRightPtr()->getLeftPtr() == nullptr) {
                        node->setRange(node->getRightPtr()->getRange());
                        node->setValue(node->getRightPtr()->getValue());
                        updateColorsBeforeDelete(node->getRightPtr());
                        node->setRight(node->getRightPtr()->getRightUniquePtr());
                        updateMinMax(node.get());
                    } else {
                        node->setRight(successorDelete(node.get(), node->getRightUniquePtr()));
                    }
                    return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(true,
                                                                                                                     rebalanceTreeAfterDelete(
                                                                                                                             std::move(
                                                                                                                                     node)));
                }
            } else {
                std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>> result;
                if (node->getLeftPtr() != nullptr && node->getLeftPtr()->isContainedMinMax(range)) {
                    result = removeByOverlapRangeRecur(node->getLeftUniquePtr(), range);
                    node->setLeft(std::move(result.second));
                    updateMinMax(node.get());
                } else if (node->getRightPtr() != nullptr && node->getRightPtr()->isContainedMinMax(range)) {
                    result = removeByOverlapRangeRecur(node->getRightUniquePtr(), range);
                    node->setRight(std::move(result.second));
                    updateMinMax(node.get());
                } else
                    return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(false, std::move(node));
                return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(
                        result.first, rebalanceTreeAfterDelete(std::move(node)));
            }
        }

        std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>> removeByKeyRecur(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node, KEY_TYPE key) {
            if (node == nullptr)
                return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(false,
                                                                                                                 std::move(
                                                                                                                         node));
            if (node->isOverlap(key)) {
                if (node->getLeftPtr() == nullptr) {
                    updateColorsBeforeDelete(node.get());
                    return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(true,
                                                                                                                     std::move(
                                                                                                                             node->getRightUniquePtr()));
                } else if (node->getRightPtr() == nullptr) {
                    updateColorsBeforeDelete(node.get());
                    return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(true,
                                                                                                                     std::move(
                                                                                                                             node->getLeftUniquePtr()));
                } else {
                    if (node->getRightPtr()->getLeftPtr() == nullptr) {
                        node->setRange(node->getRightPtr()->getRange());
                        node->setValue(node->getRightPtr()->getValue());
                        updateColorsBeforeDelete(node->getRightPtr());
                        node->setRight(node->getRightPtr()->getRightUniquePtr());
                        updateMinMax(node.get());
                    } else {
                        node->setRight(successorDelete(node.get(), node->getRightUniquePtr()));
                    }
                    return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(true,
                                                                                                                     rebalanceTreeAfterDelete(
                                                                                                                             std::move(
                                                                                                                                     node)));
                }
            } else {
                std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>> result;
                if (node->getLeftPtr() != nullptr && node->getLeftPtr()->isOverlapMinMax(key)) {
                    result = removeByKeyRecur(node->getLeftUniquePtr(), key);
                    node->setLeft(std::move(result.second));
                    updateMinMax(node.get());
                } else if (node->getRightPtr() != nullptr && node->getRightPtr()->isOverlapMinMax(key)) {
                    result = removeByKeyRecur(node->getRightUniquePtr(), key);
                    node->setRight(std::move(result.second));
                    updateMinMax(node.get());
                } else
                    return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(false, std::move(node));
                return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(
                        result.first, rebalanceTreeAfterDelete(std::move(node)));
            }
        }

        std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>> removeByValueRecur(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node, DATA_TYPE key) {
            if (node == nullptr)
                return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(false,
                                                                                                                 std::move(
                                                                                                                         node));
            if (node->containValue(key)) {
                if (node->getValue().size() - node->containCount(key) > 1) {
                    node->removeValue(key);
                    return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(true,
                                                                                                                     std::move(
                                                                                                                             node));
                } else if (node->getLeftPtr() == nullptr) {
                    updateColorsBeforeDelete(node.get());
                    return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(true,
                                                                                                                     std::move(
                                                                                                                             node->getRightUniquePtr()));
                } else if (node->getRightPtr() == nullptr) {
                    updateColorsBeforeDelete(node.get());
                    return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(true,
                                                                                                                     std::move(
                                                                                                                             node->getLeftUniquePtr()));
                } else {
                    if (node->getRightPtr()->getLeftPtr() == nullptr) {
                        node->setRange(node->getRightPtr()->getRange());
                        node->setValue(node->getRightPtr()->getValue());
                        updateColorsBeforeDelete(node->getRightPtr());
                        node->setRight(node->getRightPtr()->getRightUniquePtr());
                        updateMinMax(node.get());
                    } else {
                        node->setRight(successorDelete(node.get(), node->getRightUniquePtr()));
                    }
                    return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(true,
                                                                                                                     rebalanceTreeAfterDelete(
                                                                                                                             std::move(
                                                                                                                                     node)));
                }
            } else {
                std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>> result;
                result = removeByValueRecur(node->getLeftUniquePtr(), key);
                node->setLeft(std::move(result.second));
                updateMinMax(node.get());
                if (!result.first) {
                    result = removeByValueRecur(node->getRightUniquePtr(), key);
                    node->setRight(std::move(result.second));
                    updateMinMax(node.get());
                }
                return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(
                        result.first, rebalanceTreeAfterDelete(std::move(node)));
            }
        }

        std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>> removeIfRecur(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node, std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE>*)> predicate) {
            if (node == nullptr)
                return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(false,
                                                                                                                 std::move(
                                                                                                                         node));
            if (predicate(static_cast<const RangeNode<KEY_TYPE, DATA_TYPE> *>(node))) {
                if (node->getLeftPtr() == nullptr) {
                    updateColorsBeforeDelete(node.get());
                    return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(true,
                                                                                                                     std::move(
                                                                                                                             node->getRightUniquePtr()));
                } else if (node->getRightPtr() == nullptr) {
                    updateColorsBeforeDelete(node.get());
                    return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(true,
                                                                                                                     std::move(
                                                                                                                             node->getLeftUniquePtr()));
                } else {
                    if (node->getRightPtr()->getLeftPtr() == nullptr) {
                        node->setRange(node->getRightPtr()->getRange());
                        node->setValue(node->getRightPtr()->getValue());
                        updateColorsBeforeDelete(node->getRightPtr());
                        node->setRight(node->getRightPtr()->getRightUniquePtr());
                        updateMinMax(node.get());
                    } else {
                        node->setRight(successorDelete(node.get(), node->getRightUniquePtr()));
                    }
                    return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(true,
                                                                                                                     rebalanceTreeAfterDelete(
                                                                                                                             std::move(
                                                                                                                                     node)));
                }
            } else {
                std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>> result;
                result = removeIfRecur(node->getLeftUniquePtr(), predicate);
                node->setLeft(std::move(result.second));
                updateMinMax(node.get());
                if (!result.first) {
                    result = removeIfRecur(node->getRightUniquePtr(), predicate);
                    node->setRight(std::move(result.second));
                    updateMinMax(node.get());
                }
                return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(
                        result.first, rebalanceTreeAfterDelete(std::move(node)));
            }
        }

        /** END REMOVE INTERNAL Functions*/

        /** GET INTERNAL Functions*/

        void getByOverlapRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node, std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> *list, std::pair<KEY_TYPE, KEY_TYPE> range, unsigned int* count) const {
            if (*count == 0)
                return;
            if (node->isOverlap(range)){
                list->push_back(static_cast<RangeItem<KEY_TYPE, DATA_TYPE> *>(node));
                (*count)--;
            }
            if (node->getLeftPtr() != nullptr && node->getLeftPtr()->isOverlapMinMax(range))
                getByOverlapRecur(node->getLeftPtr(), list, range, count);

            if (node->getRightPtr()!= nullptr && node->getRightPtr()->isOverlapMinMax(range))
                getByOverlapRecur(node->getRightPtr(), list, range, count);
        }

        void getByContainRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node, std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> *list, std::pair<KEY_TYPE, KEY_TYPE> range, unsigned int* count) const {
            if (*count == 0)
                return;
            if (node->isContain(range)){
                list->push_back(static_cast<RangeItem<KEY_TYPE, DATA_TYPE> *>(node));
                (*count)--;
            }
            if (node->getLeftPtr() != nullptr && node->getLeftPtr()->isContainMinMax(range))
                getByContainRecur(node->getLeftPtr(), list, range, count);

            if (node->getRightPtr()!= nullptr && node->getRightPtr()->isContainMinMax(range))
                getByContainRecur(node->getRightPtr(), list, range, count);
        }

        void getByContainedRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node, std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> *list, std::pair<KEY_TYPE, KEY_TYPE> range, unsigned int* count) const {
            if (*count == 0)
                return;
            if (node->isContained(range)){
                list->push_back(static_cast<RangeItem<KEY_TYPE, DATA_TYPE> *>(node));
                (*count)--;
            }
            if (node->getLeftPtr() != nullptr && node->getLeftPtr()->isContainedMinMax(range))
                getByContainedRecur(node->getLeftPtr(), list, range, count);

            if (node->getRightPtr()!= nullptr && node->getRightPtr()->isContainedMinMax(range))
                getByContainedRecur(node->getRightPtr(), list, range, count);
        }

        void getRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node, std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> *list, KEY_TYPE key, unsigned int* count) const {
            if (*count == 0)
                return;
            if (node->isOverlap(key)){
                list->push_back(static_cast<RangeItem<KEY_TYPE, DATA_TYPE> *>(node));
                (*count)--;
            }

            if (node->getLeftPtr() != nullptr && node->getLeftPtr()->isOverlapMinMax(key))
                getRecur(node->getLeftPtr(), list, key, count);

            if (node->getRightPtr()!= nullptr && node->getRightPtr()->isOverlapMinMax(key))
                getRecur(node->getRightPtr(), list, key, count);
        }

        void getByRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node, std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> *list, std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE>*)> predicate, unsigned int* count) const {
            if (*count == 0)
                return;
            if (predicate(static_cast<const RangeNode<KEY_TYPE, DATA_TYPE>*>(node))){
                list->push_back(static_cast<const RangeItem<KEY_TYPE, DATA_TYPE>*>(node));
                (*count)--;
            } else {
                if (node->getLeftPtr() != nullptr)
                    getByRecur(node->getLeftPtr(), list, predicate, count);
                if (node->getRightPtr() != nullptr)
                    getByRecur(node->getRightPtr(), list, predicate, count);
            }
        }

        const RangeItem<KEY_TYPE, DATA_TYPE>* getByRangeRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node, std::pair<KEY_TYPE, KEY_TYPE> range) const {
            if (node == nullptr)
                return nullptr;
            if (node->isEqualRange(range))
                return static_cast<const RangeItem<KEY_TYPE, DATA_TYPE> *>(node);
            if (node->getFrom() >= range.first)
                return getByRangeRecur(node->getLeftPtr(), range);
            else
                return getByRangeRecur(node->getRightPtr(), range);
        }

        void getAllByOverlapRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node, std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> *list, std::pair<KEY_TYPE, KEY_TYPE> range) const {
            if (node->isOverlap(range)){
                list->push_back(static_cast<RangeItem<KEY_TYPE, DATA_TYPE> *>(node));
            }
            if (node->getLeftPtr() != nullptr && node->getLeftPtr()->isOverlapMinMax(range))
                getAllByOverlapRecur(node->getLeftPtr(), list, range);

            if (node->getRightPtr()!= nullptr && node->getRightPtr()->isOverlapMinMax(range))
                getAllByOverlapRecur(node->getRightPtr(), list, range);
        }

        void getAllByContainRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node, std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> *list, std::pair<KEY_TYPE, KEY_TYPE> range) const {
            if (node->isContain(range)){
                list->push_back(static_cast<RangeItem<KEY_TYPE, DATA_TYPE> *>(node));
            }

            if (node->getLeftPtr() != nullptr && node->getLeftPtr()->isContainMinMax(range))
                getAllByContainRecur(node->getLeftPtr(), list, range);

            if (node->getRightPtr()!= nullptr && node->getRightPtr()->isContainMinMax(range))
                getAllByContainRecur(node->getRightPtr(), list, range);
        }

        void getAllByContainedRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node, std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> *list, std::pair<KEY_TYPE, KEY_TYPE> range) const {
            if (node->isContained(range)){
                list->push_back(static_cast<RangeItem<KEY_TYPE, DATA_TYPE> *>(node));
            }

            if (node->getLeftPtr() != nullptr && node->getLeftPtr()->isContainedMinMax(range))
                getAllByContainedRecur(node->getLeftPtr(), list, range);

            if (node->getRightPtr()!= nullptr && node->getRightPtr()->isContainedMinMax(range))
                getAllByContainedRecur(node->getRightPtr(), list, range);
        }

        void getAllRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node, std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> *list, KEY_TYPE key) const {
            if (node->isOverlap(key)){
                list->push_back(static_cast<RangeItem<KEY_TYPE, DATA_TYPE> *>(node));
            }

            if (node->getLeftPtr() != nullptr && node->getLeftPtr()->isOverlapMinMax(key))
                getAllRecur(node->getLeftPtr(), list, key);

            if (node->getRightPtr()!= nullptr && node->getRightPtr()->isOverlapMinMax(key))
                getAllRecur(node->getRightPtr(), list, key);
        }

        void getAllIfRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node, std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> *list, std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE>*)> predicate) const {
            if (predicate(static_cast<const RangeNode<KEY_TYPE, DATA_TYPE>*>(node))){
                list->push_back(static_cast<const RangeItem<KEY_TYPE, DATA_TYPE>*>(node));
            } else {
                if (node->getLeftPtr() != nullptr)
                    getAllIfRecur(node->getLeftPtr(), list, predicate);
                if (node->getRightPtr() != nullptr)
                    getAllIfRecur(node->getRightPtr(), list, predicate);
            }
        }

        void getTestAllByKeyRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node, std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> *list, KEY_TYPE key, unsigned int *countComplexity) const {
            (*countComplexity)++;
            if (node->isOverlap(key)){
                list->push_back(static_cast<RangeItem<KEY_TYPE, DATA_TYPE> *>(node));
            }
            if (node->getLeftPtr() != nullptr && node->getLeftPtr()->isOverlapMinMax(key))
                getTestAllByKeyRecur(node->getLeftPtr(), list, key, countComplexity);

            if (node->getRightPtr()!= nullptr && node->getRightPtr()->isOverlapMinMax(key))
                getTestAllByKeyRecur(node->getRightPtr(), list, key, countComplexity);
        }

        /** END GET INTERNAL Functions*/

        /** INSERT INTERNAL Functions*/

        std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> insert(std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node, std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value) {
            if (node == nullptr)
                return std::make_unique<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>(range, value,
                                                                                                 shock_audio_impl::Color::RED);

            if (node->isEqualRange(range)) {
                node->addValue(value);
                return node;
            }
            if (range.first <= node->getFrom()) {
                if (node->getLeftPtr() != nullptr)
                    node->setLeft(insert(node->getLeftUniquePtr(), range, value));
                else
                    node->setLeft(std::make_unique<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>(range, value,
                                                                                                            shock_audio_impl::Color::RED));
                if (range.first < node->getMin())
                    node->setMin(range.first);
            } else {
                if (node->getRightPtr() != nullptr)
                    node->setRight(insert(node->getRightUniquePtr(), range, value));
                else
                    node->setRight(std::make_unique<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>(range, value,
                                                                                                             shock_audio_impl::Color::RED));
            }

            if (node->getMax() < range.second)
                node->setMax(range.second);

            return rebalanceTreeAfterInsert(std::move(node));
        }

        /** END INSERT INTERNAL Functions*/

        /** UTILS INTERNAL Functions*/

        shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* getMaxNode(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) const {
            if (node->getRightPtr() == nullptr)
                return node;
            else
                return getMaxNode(node->getRightPtr());
        }

        shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* getMinNode(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) const {
            if (node->getLeftPtr() == nullptr)
                return node;
            else
                return getMinNode(node->getLeftPtr());
        }

        bool isNullNode(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) {
            return node->getFrom() == NULL || node->getTo() == NULL;
        }

        void printTreeRecurrent(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node, int space) const {
            if (node != nullptr) {
                auto newSpace = space + 10;
                printTreeRecurrent(node->getLeftPtr(), newSpace);
                std::cout << std::endl;
                for (auto i = 10; i < newSpace; i++) {
                    std::cout << " ";
                }
                auto val = node->getValue();
                char m;
                switch (node->getColor()) {
                    case shock_audio_impl::Color::BLACK:
                        m = 'B';
                        break;
                    case shock_audio_impl::Color::DOUBLE_BLACK:
                        m = 'D';
                        break;
                    case shock_audio_impl::Color::RED:
                        m = 'R';
                        break;
                }
                std::cout << val << " " << m << " From: " << node->getRange().first << " To: " << node->getRange().second
                          << " Max: " << node->getMax() << " Min: "<< node->getMin();
                std::cout << std::endl;
                printTreeRecurrent(node->getRightPtr(), newSpace);
            }
        }

        int getDepth(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) const {
            if (node == nullptr)
                return 0;
            if (node->getColor() == shock_audio_impl::Color::RED)
                return std::max(getDepth(node->getLeftPtr()), getDepth(node->getRightPtr()));
            else
                return std::max(getDepth(node->getLeftPtr()), getDepth(node->getRightPtr())) + 1;
        }

        int checkDifference(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node) const {
            if (node == nullptr)
                return 0;

            auto maxWithLeft = std::max(abs(getDepth(node->getLeftPtr()) - getDepth(node->getRightPtr())),
                                        checkDifference(node->getLeftPtr()));

            return std::max(maxWithLeft, checkDifference(node->getRightPtr()));
        }

        unsigned int sizeRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node) const {
            if (node == nullptr)
                return 1;
            else
                return sizeRecur(node->getLeftPtr()) + sizeRecur(node->getRightPtr());
        }

        void traversalRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node, std::function<void(const RangeNode<KEY_TYPE, DATA_TYPE>*)> predicate) const{
            if (node == nullptr)
                return;
            predicate(node);
            traversalRecur(node->getLeftPtr(), predicate);
            traversalRecur(node->getRightPtr(), predicate);
        }

        /** END UTILS INTERNAL Functions*/

    };

}

#endif //SHOCK_AUDIO_MUTABLERANGEMAP_H
