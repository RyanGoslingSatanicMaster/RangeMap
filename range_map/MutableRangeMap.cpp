
#include <iostream>
#include "MutableRangeMap.h"
#include "MutableRangeNode.cpp"

namespace shock_audio {

    template<typename KEY_TYPE, typename DATA_TYPE>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::MutableRangeMap() = default;

    template<typename KEY_TYPE, typename DATA_TYPE>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::MutableRangeMap(std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value) :
            _root(std::make_unique<MutableRangeNode<KEY_TYPE, DATA_TYPE>>(range, value, Color::BLACK)) {}

    template<typename KEY_TYPE, typename DATA_TYPE>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::MutableRangeMap(KEY_TYPE from, KEY_TYPE to, DATA_TYPE value) :
            _root(std::make_unique<MutableRangeNode<KEY_TYPE, DATA_TYPE>>(from, to, value, Color::BLACK)) {}

    template<typename KEY_TYPE, typename DATA_TYPE>
    RangeNode<KEY_TYPE, DATA_TYPE> *MutableRangeMap<KEY_TYPE, DATA_TYPE>::getRoot() {
        return _root.get();
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::checkIfRootDoubleBlackOrRed(std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
        if (node->getColor() == Color::RED) {
            node->setColor(Color::BLACK);
            return rebalanceTreeAfterInsert(std::move(node));
        } else {
            if (node->getColor() == Color::DOUBLE_BLACK)
                node->setColor(Color::BLACK);
            return node;
        }
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::rebalanceTreeAfterDelete(
            std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
        std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> temp_node;
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
                    temp_node->getRightPtr()->getRightPtr()->setColor(Color::BLACK);
                temp_node->getLeftPtr()->setColor(Color::BLACK);
                break;
            case BRLR:
                temp_node = leftRightRotate(std::move(node));
                if (isNullNode(temp_node->getRightPtr()->getRightPtr()))
                    temp_node->getRightPtr()->setRight(nullptr);
                else
                    temp_node->getRightPtr()->getRightPtr()->setColor(Color::BLACK);
                temp_node->getLeftPtr()->setColor(Color::BLACK);
                break;
            case BRRL:
                temp_node = rightLeftRotate(std::move(node));
                if (isNullNode(temp_node->getLeftPtr()->getLeftPtr()))
                    temp_node->getLeftPtr()->setLeft(nullptr);
                else
                    temp_node->getLeftPtr()->getLeftPtr()->setColor(Color::BLACK);
                temp_node->getRightPtr()->setColor(Color::BLACK);
                break;
            case BRRR:
                temp_node = rightRightRotate(std::move(node));
                if (isNullNode(temp_node->getLeftPtr()->getLeftPtr()))
                    temp_node->getLeftPtr()->setLeft(nullptr);
                else
                    temp_node->getLeftPtr()->getLeftPtr()->setColor(Color::BLACK);
                temp_node->getRightPtr()->setColor(Color::BLACK);
                break;
            case NO:
                temp_node = std::move(node);
                break;
        }
        return temp_node;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::updateColorsBeforeDelete(MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) {
        if (node->getRightPtr() == nullptr && node->getLeftPtr() == nullptr) {
            if (node->getColor() == Color::BLACK) {
                node->setLeft(MutableRangeNode<KEY_TYPE, DATA_TYPE>::generateNULLNode());
                node->setRight(MutableRangeNode<KEY_TYPE, DATA_TYPE>::generateNULLNode());
            }
            return;
        }
        if (node->getRightPtr() == nullptr) {
            if (node->getColor() == Color::RED || node->getLeftPtr()->getColor() == Color::RED) {
                node->getLeftPtr()->setColor(Color::BLACK);
            } else {
                node->getLeftPtr()->setColor(Color::DOUBLE_BLACK);
            }
        } else {
            if (node->getColor() == Color::RED || node->getRightPtr()->getColor() == Color::RED) {
                node->getRightPtr()->setColor(Color::BLACK);
            } else {
                node->getRightPtr()->setColor(Color::DOUBLE_BLACK);
            }
        }
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::successorDelete(MutableRangeNode<KEY_TYPE, DATA_TYPE> *root,
                                                          std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
        if (node->getLeftPtr()->getLeftPtr() != nullptr) {
            node->setLeft(successorDelete(root, node->getLeftUniquePtr()));
        } else {
            root->setRange(node->getLeftPtr()->getRange());
            root->setValue(node->getLeftPtr()->getValue());
            updateColorsBeforeDelete(node->getLeftPtr());
            node->setLeft(node->getLeftUniquePtr()->getRightUniquePtr());
            updateMax(node.get());
            return rebalanceTreeAfterDelete(std::move(node));
        }
        updateMax(node.get());
        return rebalanceTreeAfterDelete(std::move(node));
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::pair<bool, std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>>> MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeByRangeRecur(
            std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> node, std::pair<KEY_TYPE, KEY_TYPE> range) {
        if (node == nullptr)
            return std::pair<bool, std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(false, std::move(node));
        if (range.first == node->getFrom() && range.second == node->getTo()) {
            if (node->getLeftPtr() == nullptr) {
                updateColorsBeforeDelete(node.get());
                return std::pair<bool, std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(true, std::move(node->getRightUniquePtr()));
            } else if (node->getRightPtr() == nullptr) {
                updateColorsBeforeDelete(node.get());
                return std::pair<bool, std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(true, std::move(node->getLeftUniquePtr()));
            } else {
                if (node->getRightPtr()->getLeftPtr() == nullptr) {
                    node->setRange(node->getRightPtr()->getRange());
                    node->setValue(node->getRightPtr()->getValue());
                    updateColorsBeforeDelete(node->getRightPtr());
                    node->setRight(node->getRightPtr()->getRightUniquePtr());
                    updateMax(node.get());
                } else {
                    node->setRight(successorDelete(node.get(), node->getRightUniquePtr()));
                }
                return std::pair<bool, std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(true, rebalanceTreeAfterDelete(std::move(node)));
            }
        } else {
            std::pair<bool, std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>>> result;
            if (range.first <= node->getTo()) {
                result = removeByRangeRecur(node->getLeftUniquePtr(), range);
                node->setLeft(std::move(result.second));
                updateMax(node.get());
            } else {
                result = removeByRangeRecur(node->getRightUniquePtr(), range);
                node->setRight(std::move(result.second));
                updateMax(node.get());
            }
            return std::pair<bool, std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(result.second, rebalanceTreeAfterDelete(std::move(node)));
        }
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeFirstByRange(std::pair<KEY_TYPE, KEY_TYPE> range) {
        // TODO insert assert to range
        auto result = removeByRangeRecur(std::move(_root), range);
        _root = std::move(checkIfRootDoubleBlackOrRed(std::move(result.second)));
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeAllInRange(std::pair<KEY_TYPE, KEY_TYPE> range) {
        _root = std::move(checkIfRootDoubleBlackOrRed(removeAllInRangeRecurr(std::move(_root), range)));
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeFirstByKey(KEY_TYPE key) {

    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeFirstByValue(DATA_TYPE val) {

    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::vector<DATA_TYPE> MutableRangeMap<KEY_TYPE, DATA_TYPE>::get(KEY_TYPE from, KEY_TYPE to) const {
        return std::vector<DATA_TYPE>();
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::vector<DATA_TYPE> MutableRangeMap<KEY_TYPE, DATA_TYPE>::get(KEY_TYPE key) const {
        return std::vector<DATA_TYPE>();
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::vector<DATA_TYPE> MutableRangeMap<KEY_TYPE, DATA_TYPE>::getBy(
            std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE> &)> predicate) const {
        return std::vector<DATA_TYPE>();
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::updateMax(MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) {
        if (node == nullptr)
            return;
        auto nodeMax = node->getRange().second;
        if (node->getLeftPtr() == nullptr)
            if (node->getRightPtr() == nullptr)
                node->setMax(nodeMax);
            else {
                auto rightMax = node->getRightPtr()->getMax();
                if (rightMax > nodeMax)
                    node->setMax(rightMax);
                else
                    node->setMax(nodeMax);
            }
        else if (node->getRightPtr() == nullptr) {
            auto leftMax = node->getLeftPtr()->getMax();
            if (leftMax > nodeMax)
                node->setMax(leftMax);
            else
                node->setMax(nodeMax);
        } else {
            auto max = std::max(node->getLeftPtr()->getMax(), node->getRightPtr()->getMax());
            if (max > nodeMax)
                node->setMax(max);
            else
                node->setMax(nodeMax);
        }
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool MutableRangeMap<KEY_TYPE, DATA_TYPE>::isNullNode(MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) {
        return node->getFrom() == NULL || node->getTo() == NULL;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::BalanceCaseDelete
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::checkBalanceCaseAfterDelete(MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) {
        if (node->getLeftPtr() == nullptr || node->getRightPtr() == nullptr ||
            node->getLeftPtr()->getColor() != Color::DOUBLE_BLACK &&
            node->getRightPtr()->getColor() != Color::DOUBLE_BLACK)
            return NO;
        if (node->getLeftPtr()->getColor() == Color::DOUBLE_BLACK) {
            if (node->getRightPtr()->getColor() == Color::RED)
                return RRED;
            else if (node->getRightPtr()->getRightPtr() != nullptr &&
                     node->getRightPtr()->getRightPtr()->getColor() == Color::RED)
                return BRRR;
            else if (node->getRightPtr()->getLeftPtr() == nullptr ||
                     node->getRightPtr()->getLeftPtr()->getColor() == Color::BLACK)
                return BB;
            else
                return BRRL;

        } else {
            if (node->getLeftPtr()->getColor() == Color::RED)
                return LRED;
            else if (node->getLeftPtr()->getLeftPtr() != nullptr &&
                     node->getLeftPtr()->getLeftPtr()->getColor() == Color::RED)
                return BRLL;
            else if (node->getLeftPtr()->getRightPtr() == nullptr ||
                     node->getLeftPtr()->getRightPtr()->getColor() == Color::BLACK)
                return BB;
            else
                return BRLR;
        }
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::BalanceCaseInsert
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::checkBalanceCaseAfterInsert(MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) {
        if (node == nullptr || node->getColor() == Color::RED)
            return NOTHING;
        else {
            auto leftChild = node->getLeftPtr();
            auto rightChild = node->getRightPtr();
            if (rightChild == nullptr || rightChild->getColor() == Color::BLACK) {
                if (leftChild == nullptr || leftChild->getColor() == Color::BLACK)
                    return NOTHING;
                else {
                    auto leftGrandChild = leftChild->getLeftPtr();
                    auto rightGrandChild = leftChild->getRightPtr();
                    if (leftGrandChild == nullptr || leftGrandChild->getColor() == Color::BLACK)
                        if (rightGrandChild == nullptr || rightGrandChild->getColor() == Color::BLACK)
                            return NOTHING;
                        else
                            return LR;
                    else
                        return LL;
                }
            } else {
                if (leftChild == nullptr || leftChild->getColor() == Color::BLACK) {
                    auto leftGrandChild = rightChild->getLeftPtr();
                    auto rightGrandChild = rightChild->getRightPtr();
                    if (leftGrandChild == nullptr || leftGrandChild->getColor() == Color::BLACK)
                        if (rightGrandChild == nullptr || rightGrandChild->getColor() == Color::BLACK)
                            return NOTHING;
                        else
                            return RR;
                    else
                        return RL;
                } else {
                    return SC;
                }
            }
        }
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::leftLeftRotate(std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
        auto temp_subtree = std::move(node->getLeftPtr()->getRightUniquePtr());
        auto new_root = std::move(node->getLeftUniquePtr());
        auto temp_color = new_root->getColor();
        new_root->setMax(node->getMax());
        new_root->setColor(node->getColor());
        new_root->setRight(std::move(node));
        new_root->getRightPtr()->setLeft(std::move(temp_subtree));
        new_root->getRightPtr()->setColor(temp_color);
        updateMax(new_root->getRightPtr());
        updateMax(new_root->getLeftPtr());
        return new_root;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> MutableRangeMap<KEY_TYPE, DATA_TYPE>::rightRightRotate(
            std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
        auto temp_subtree = std::move(node->getRightPtr()->getLeftUniquePtr());
        auto new_root = std::move(node->getRightUniquePtr());
        auto temp_color = new_root->getColor();
        new_root->setMax(node->getMax());
        new_root->setColor(node->getColor());
        new_root->setLeft(std::move(node));
        new_root->getLeftPtr()->setRight(std::move(temp_subtree));
        new_root->getLeftPtr()->setColor(temp_color);
        updateMax(new_root->getLeftPtr());
        updateMax(new_root->getRightPtr());
        return new_root;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::leftRightRotate(std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
        auto temp_subtree = std::move(node->getLeftPtr()->getRightPtr()->getLeftUniquePtr());
        auto new_root = std::move(node->getLeftPtr()->getRightUniquePtr());
        auto temp_color = new_root->getColor();
        new_root->setMax(node->getMax());
        new_root->setColor(node->getColor());
        new_root->setLeft(std::move(node->getLeftUniquePtr()));
        new_root->getLeftPtr()->setColor(temp_color);
        node->setLeft(std::move(new_root));
        node->getLeftPtr()->getLeftPtr()->setRight(std::move(temp_subtree));
        updateMax(node->getLeftPtr()->getLeftPtr());
        updateMax(node->getLeftPtr()->getRightPtr());
        return leftLeftRotate(std::move(node));
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::rightLeftRotate(std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
        auto temp_subtree = std::move(node->getRightPtr()->getLeftPtr()->getRightUniquePtr());
        auto new_root = std::move(node->getRightPtr()->getLeftUniquePtr());
        auto temp_color = new_root->getColor();
        new_root->setMax(node->getMax());
        new_root->setColor(node->getRightPtr()->getColor());
        new_root->setRight(std::move(node->getRightUniquePtr()));
        new_root->getRightPtr()->setColor(temp_color);
        node->setRight(std::move(new_root));
        node->getRightPtr()->getRightPtr()->setLeft(std::move(temp_subtree));
        updateMax(node->getRightPtr()->getRightPtr());
        updateMax(node->getRightPtr()->getLeftPtr());
        return rightRightRotate(std::move(node));
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::switchColors(std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
        if (node->getLeftPtr()->getColor() == Color::RED && node->getRightPtr()->getColor() == Color::RED) {
            node->setColor(Color::RED);
            node->getRightPtr()->setColor(Color::BLACK);
            node->getLeftPtr()->setColor(Color::BLACK);
        } else if (node->getLeftPtr()->getColor() == Color::DOUBLE_BLACK
                   && node->getRightPtr()->getColor() == Color::BLACK) {
            if (node->getColor() == Color::BLACK)
                node->setColor(Color::DOUBLE_BLACK);
            else
                node->setColor(Color::BLACK);
            node->getLeftPtr()->setColor(Color::BLACK);
            node->getRightPtr()->setColor(Color::RED);
            if (isNullNode(node->getLeftPtr()))
                node->setLeft(nullptr);
        } else if (node->getRightPtr()->getColor() == Color::DOUBLE_BLACK
                   && node->getLeftPtr()->getColor() == Color::BLACK) {
            if (node->getColor() == Color::BLACK)
                node->setColor(Color::DOUBLE_BLACK);
            else
                node->setColor(Color::BLACK);
            node->getLeftPtr()->setColor(Color::RED);
            node->getRightPtr()->setColor(Color::BLACK);
            if (isNullNode(node->getRightPtr()))
                node->setRight(nullptr);
        }
        return node;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::rebalanceTreeAfterInsert(
            std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
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

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::insert(std::unique_ptr<MutableRangeNode<KEY_TYPE, DATA_TYPE>> node,
                                                 std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value) {
        if (node == nullptr)
            return std::make_unique<MutableRangeNode<KEY_TYPE, DATA_TYPE>>(range, value, Color::RED);

        if (node->getFrom() == range.first && node->getTo() == range.second) {
            node->addValue(value);
            return node;
        }

        if (range.first <= node->getTo()) {
            if (node->getLeftPtr() != nullptr)
                node->setLeft(insert(node->getLeftUniquePtr(), range, value));
            else
                node->setLeft(std::make_unique<MutableRangeNode<KEY_TYPE, DATA_TYPE>>(range, value, Color::RED));
        } else {
            if (node->getRightPtr() != nullptr)
                node->setRight(insert(node->getRightUniquePtr(), range, value));
            else
                node->setRight(std::make_unique<MutableRangeNode<KEY_TYPE, DATA_TYPE>>(range, value, Color::RED));
        }

        if (node->getMax() < range.second)
            node->setMax(range.second);

        return rebalanceTreeAfterInsert(std::move(node));
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::put(std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value) {
        _root = std::move(checkIfRootDoubleBlackOrRed(insert(std::move(_root), range, value)));
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::put(KEY_TYPE from, KEY_TYPE to, DATA_TYPE value) {
        _root = std::move(checkIfRootDoubleBlackOrRed(insert(std::move(_root), std::pair<KEY_TYPE, KEY_TYPE>(from, to), value)));
    }

    template<typename DATA_TYPE>
    std::ostream &operator<<(std::ostream &output, const std::vector<DATA_TYPE> &value) {
        for (auto node: value) {
            output << node << " ";
        }
        return output;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    int MutableRangeMap<KEY_TYPE, DATA_TYPE>::getBalanceDifference() {
        return checkDifference(_root.get());
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    int MutableRangeMap<KEY_TYPE, DATA_TYPE>::getMaxDepth() {
        return getDepth(_root.get());
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::printTreeRecurrent(MutableRangeNode<KEY_TYPE, DATA_TYPE> *node, int spac) {
        if (node != nullptr) {
            auto space = spac + 10;
            printTreeRecurrent(node->getLeftPtr(), space);
            std::cout << std::endl;
            for (auto i = 10; i < space; i++) {
                std::cout << " ";
            }
            auto val = node->getValue();
            char m;
            switch (node->getColor()) {
                case Color::BLACK:
                    m = 'B';
                    break;
                case Color::DOUBLE_BLACK:
                    m = 'D';
                    break;
                case Color::RED:
                    m = 'R';
                    break;
            }
            std::cout << val << " " << m << " From: " << node->getRange().first << " To: " << node->getRange().second
                      << " Max: " << node->getMax();
            std::cout << std::endl;
            printTreeRecurrent(node->getRightPtr(), space);
        }
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::printTree() {
        printTreeRecurrent(_root.get(), 0);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    int MutableRangeMap<KEY_TYPE, DATA_TYPE>::getDepth(MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) {
        if (node == nullptr)
            return 0;
        if (node->getColor() == Color::RED)
            return std::max(getDepth(node->getLeftPtr()), getDepth(node->getRightPtr()));
        else
            return std::max(getDepth(node->getLeftPtr()), getDepth(node->getRightPtr())) + 1;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    int MutableRangeMap<KEY_TYPE, DATA_TYPE>::checkDifference(MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) {
        if (node == nullptr)
            return 0;

        auto maxWithLeft = std::max(abs(getDepth(node->getLeftPtr()) - getDepth(node->getRightPtr())),
                                    checkDifference(node->getLeftPtr()));

        return std::max(maxWithLeft, checkDifference(node->getRightPtr()));
    }
}
