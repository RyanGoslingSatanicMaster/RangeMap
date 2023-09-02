
#include <iostream>
#include "MutableRangeMap.h"
#include "MutableRangeNode.cpp"

namespace shock_audio {

    template<typename KEY_TYPE, typename DATA_TYPE>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::MutableRangeMap() = default;

    template<typename KEY_TYPE, typename DATA_TYPE>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::MutableRangeMap(std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value) :
            _root(std::make_unique<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>(range, value,
                                                                                            shock_audio_impl::Color::BLACK)) {}

    template<typename KEY_TYPE, typename DATA_TYPE>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::MutableRangeMap(KEY_TYPE from, KEY_TYPE to, DATA_TYPE value) :
            _root(std::make_unique<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>(from, to, value,
                                                                                            shock_audio_impl::Color::BLACK)) {}

    template<typename KEY_TYPE, typename DATA_TYPE>
    RangeNode<KEY_TYPE, DATA_TYPE> *MutableRangeMap<KEY_TYPE, DATA_TYPE>::getRoot() const{
        return _root.get();
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::checkIfRootDoubleBlackOrRed(
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
        if (node->getColor() == shock_audio_impl::Color::RED) {
            node->setColor(shock_audio_impl::Color::BLACK);
            return rebalanceTreeAfterInsert(std::move(node));
        } else {
            if (node->getColor() == shock_audio_impl::Color::DOUBLE_BLACK)
                node->setColor(shock_audio_impl::Color::BLACK);
            return node;
        }
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::rebalanceTreeAfterDelete(
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
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

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::updateColorsBeforeDelete(
            shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) {
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

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::successorDelete(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *root,
                                                          std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
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
    std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeByContainRangeRecur(
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node,
            std::pair<KEY_TYPE, KEY_TYPE> range) {
        if (node == nullptr)
            return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(false,
                                                                                                             std::move(
                                                                                                                     node));
        if (isContain(range, node.get())) {
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
                    updateMax(node.get());
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
                result = removeByContainRangeRecur(node->getLeftUniquePtr(), range);
                node->setLeft(std::move(result.second));
                updateMax(node.get());
            } else {
                result = removeByContainRangeRecur(node->getRightUniquePtr(), range);
                node->setRight(std::move(result.second));
                updateMax(node.get());
            }
            return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(
                    result.first, rebalanceTreeAfterDelete(std::move(node)));
        }
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeByOverlapRangeRecur(
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node,
            std::pair<KEY_TYPE, KEY_TYPE> range) {
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
                    updateMax(node.get());
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
                result = removeByOverlapRangeRecur(node->getLeftUniquePtr(), range);
                node->setLeft(std::move(result.second));
                updateMax(node.get());
            } else {
                result = removeByOverlapRangeRecur(node->getRightUniquePtr(), range);
                node->setRight(std::move(result.second));
                updateMax(node.get());
            }
            return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(
                    result.first, rebalanceTreeAfterDelete(std::move(node)));
        }
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeByKeyRecur(
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node, KEY_TYPE key) {
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
                    updateMax(node.get());
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
            if (key <= node->getFrom()) {
                result = removeByKeyRecur(node->getLeftUniquePtr(), key);
                node->setLeft(std::move(result.second));
                updateMax(node.get());
            } else {
                result = removeByKeyRecur(node->getRightUniquePtr(), key);
                node->setRight(std::move(result.second));
                updateMax(node.get());
            }
            return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(
                    result.first, rebalanceTreeAfterDelete(std::move(node)));
        }
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeByValueRecur(
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node, DATA_TYPE key) {
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
                    updateMax(node.get());
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
            updateMax(node.get());
            if (!result.first) {
                result = removeByValueRecur(node->getRightUniquePtr(), key);
                node->setRight(std::move(result.second));
                updateMax(node.get());
            }
            return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(
                    result.first, rebalanceTreeAfterDelete(std::move(node)));
        }
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeIfRecur(
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node,
            std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE> &)> predicate) {
        return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>();
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeByRangeRecur(
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node,
            std::pair<KEY_TYPE, KEY_TYPE> range) {
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
                    updateMax(node.get());
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
                updateMax(node.get());
            } else {
                result = removeByRangeRecur(node->getRightUniquePtr(), range);
                node->setRight(std::move(result.second));
                updateMax(node.get());
            }
            return std::pair<bool, std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>>(
                    result.first, rebalanceTreeAfterDelete(std::move(node)));
        }
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeByContainRange(std::pair<KEY_TYPE, KEY_TYPE> range) {
        auto result = removeByContainRangeRecur(std::move(_root), range);
        _root = std::move(checkIfRootDoubleBlackOrRed(std::move(result.second)));
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeAllByContainRange(std::pair<KEY_TYPE, KEY_TYPE> range) {
        std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> new_root = std::move(_root);
        auto resFlag = true;
        while (resFlag) {
            auto result = removeByContainRangeRecur(std::move(new_root), range);
            resFlag = result.first;
            new_root = std::move(checkIfRootDoubleBlackOrRed(std::move(result.second)));
        }
        _root = std::move(new_root);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeByContainRange(std::pair<KEY_TYPE, KEY_TYPE> range,
                                                                    unsigned int count) {
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

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeByRange(std::pair<KEY_TYPE, KEY_TYPE> range) {
        auto result = removeByRangeRecur(std::move(_root), range);
        _root = std::move(checkIfRootDoubleBlackOrRed(std::move(result.second)));
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeByOverlapRange(std::pair<KEY_TYPE, KEY_TYPE> range) {
        auto result = removeByOverlapRangeRecur(std::move(_root), range);
        _root = std::move(checkIfRootDoubleBlackOrRed(std::move(result.second)));
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeAllByOverlapRange(std::pair<KEY_TYPE, KEY_TYPE> range) {
        std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> new_root = std::move(_root);
        auto resFlag = true;
        while (resFlag) {
            auto result = removeByOverlapRangeRecur(std::move(new_root), range);
            resFlag = result.first;
            new_root = std::move(checkIfRootDoubleBlackOrRed(std::move(result.second)));
        }
        _root = std::move(new_root);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeByOverlapRange(std::pair<KEY_TYPE, KEY_TYPE> range,
                                                                    unsigned int count) {
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

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeByKey(KEY_TYPE key) {
        auto result = removeByKeyRecur(std::move(_root), key);
        _root = std::move(checkIfRootDoubleBlackOrRed(std::move(result.second)));
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeAllByKey(KEY_TYPE key) {
        std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> new_root = std::move(_root);
        auto resFlag = true;
        while (resFlag) {
            auto result = removeByKeyRecur(std::move(new_root), key);
            new_root = std::move(checkIfRootDoubleBlackOrRed(std::move(result.second)));
            resFlag = result.first;
        }
        _root = std::move(new_root);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeByKey(KEY_TYPE key, unsigned int count) {
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

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeByValue(DATA_TYPE val) {
        auto result = removeByValueRecur(std::move(_root), val);
        _root = std::move(checkIfRootDoubleBlackOrRed(std::move(result.second)));
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeByValue(DATA_TYPE val, unsigned int count) {
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

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeAllByValue(DATA_TYPE val) {
        std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> new_root = std::move(_root);
        auto resFlag = true;
        while (resFlag) {
            auto result = removeByValueRecur(std::move(new_root), val);
            new_root = std::move(checkIfRootDoubleBlackOrRed(std::move(result.second)));
            resFlag = result.first;
        }
        _root = std::move(new_root);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeIf(unsigned int count, std::function<bool(
            const RangeNode<KEY_TYPE, DATA_TYPE> &)> predicate) {

    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeAllIf(
            std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE> &)> predicate) {

    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::removeIf(
            std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE> &)> predicate) {

    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::getByRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node, std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> *list, std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE>*)> predicate, unsigned int* count) const{
        if (*count == 0)
            return;
        if (predicate(static_cast<const RangeNode<KEY_TYPE, DATA_TYPE>*>(node))){
            list->push_back(static_cast<const RangeItem<KEY_TYPE, DATA_TYPE>*>(node));
            count--;
        } else {
            if (node->getLeftPtr() != nullptr)
                getByRecur(node->getLeftPtr(), list, predicate, count);
            if (node->getRightPtr() != nullptr)
                getByRecur(node->getRightPtr(), list, predicate, count);
        }
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::getByContainRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node, std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> *list, std::pair<KEY_TYPE, KEY_TYPE> range, unsigned int* count) const{
        if (*count == 0)
            return;
        if (node->isContain(range)){
            list->push_back(static_cast<RangeItem<KEY_TYPE, DATA_TYPE> *>(node));
            (*count)--;
        }
        if (node->getLeftPtr() != nullptr && node->getLeftPtr()->getMax() > range.first){
            getByContainRecur(node->getLeftPtr(), list, range, count);
            if (node->getRightPtr() != nullptr)
                getByContainRecur(node->getRightPtr(), list, range, count);
        } else {
            if (node->getRightPtr() != nullptr)
                getByContainRecur(node->getRightPtr(), list, range, count);
            if (node->getLeftPtr() != nullptr)
                getByContainRecur(node->getLeftPtr(), list, range, count);
        }
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::getByOverlapRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node, std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> *list, std::pair<KEY_TYPE, KEY_TYPE> range, unsigned int* count) const{
        if (*count == 0)
            return;
        if (node->isOverlap(range)){
            list->push_back(static_cast<RangeItem<KEY_TYPE, DATA_TYPE> *>(node));
            (*count)--;
        }
        if (node->getLeftPtr() != nullptr && node->getLeftPtr()->getMax() > range.first){
            getByOverlapRecur(node->getLeftPtr(), list, range, count);
            if (node->getRightPtr() != nullptr)
                getByOverlapRecur(node->getRightPtr(), list, range, count);
        } else {
            if (node->getRightPtr() != nullptr)
                getByOverlapRecur(node->getRightPtr(), list, range, count);
            if (node->getLeftPtr() != nullptr)
                getByOverlapRecur(node->getLeftPtr(), list, range, count);
        }
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::getRecur(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>* node, std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> *list, KEY_TYPE key, unsigned int* count) const{
        if (*count == 0)
            return;
        if (node->isOverlap(key)){
            list->push_back(static_cast<RangeItem<KEY_TYPE, DATA_TYPE> *>(node));
            (*count)--;
        }
        if (node->getLeftPtr() != nullptr && node->getLeftPtr()->getMax() >= key){
            getRecur(node->getLeftPtr(), list, key, count);
            if (node->getRightPtr() != nullptr)
                getRecur(node->getRightPtr(), list, key, count);
        } else {
            if (node->getRightPtr() != nullptr)
                getRecur(node->getRightPtr(), list, key, count);
            if (node->getLeftPtr() != nullptr)
                getRecur(node->getLeftPtr(), list, key, count);
        }
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::vector<const RangeItem<KEY_TYPE, DATA_TYPE> *>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::getAllByContain(KEY_TYPE from, KEY_TYPE to) const {
        return std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*>();
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> MutableRangeMap<KEY_TYPE, DATA_TYPE>::getAllBy(
            std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE> *)> predicate) const {
        return std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*>();
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> MutableRangeMap<KEY_TYPE, DATA_TYPE>::getAllByOverlap(KEY_TYPE from, KEY_TYPE to) const {
        return std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*>();
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> MutableRangeMap<KEY_TYPE, DATA_TYPE>::getAll(KEY_TYPE key) const {
        return std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*>();
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::getByOverlap(KEY_TYPE from, KEY_TYPE to, unsigned int count) const {
        std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> list;
        auto p = &list;
        getByOverlapRecur(_root.get(), p, std::pair(from, to), &count);
        return list;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> MutableRangeMap<KEY_TYPE, DATA_TYPE>::getBy(unsigned int count, std::function<bool(
            const RangeNode<KEY_TYPE, DATA_TYPE>*)> predicate) const {
        std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> list;
        auto p = &list;
        getByRecur(_root.get(), p, predicate, &count);
        return list;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::vector<const RangeItem<KEY_TYPE, DATA_TYPE> *>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::getByContain(KEY_TYPE from, KEY_TYPE to, unsigned int count) const {
        std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> list;
        auto p = &list;
        getByContainRecur(_root.get(), p, std::pair(from, to), &count);
        return list;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> MutableRangeMap<KEY_TYPE, DATA_TYPE>::get(KEY_TYPE key, unsigned int count) const {
        std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> list;
        getRecur(_root.get(), &list, key, &count);
        return list;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::updateMax(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) {
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
    bool
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::isNullNode(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) {
        return node->getFrom() == NULL || node->getTo() == NULL;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::BalanceCaseDelete
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::checkBalanceCaseAfterDelete(
            shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) {
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

    template<typename KEY_TYPE, typename DATA_TYPE>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::BalanceCaseInsert
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::checkBalanceCaseAfterInsert(
            shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) {
        if (node == nullptr || node->getColor() == shock_audio_impl::Color::RED)
            return NOTHING;
        else {
            auto leftChild = node->getLeftPtr();
            auto rightChild = node->getRightPtr();
            if (rightChild == nullptr || rightChild->getColor() == shock_audio_impl::Color::BLACK) {
                if (leftChild == nullptr || leftChild->getColor() == shock_audio_impl::Color::BLACK)
                    return NOTHING;
                else {
                    auto leftGrandChild = leftChild->getLeftPtr();
                    auto rightGrandChild = leftChild->getRightPtr();
                    if (leftGrandChild == nullptr || leftGrandChild->getColor() == shock_audio_impl::Color::BLACK)
                        if (rightGrandChild == nullptr || rightGrandChild->getColor() == shock_audio_impl::Color::BLACK)
                            return NOTHING;
                        else
                            return LR;
                    else
                        return LL;
                }
            } else {
                if (leftChild == nullptr || leftChild->getColor() == shock_audio_impl::Color::BLACK) {
                    auto leftGrandChild = rightChild->getLeftPtr();
                    auto rightGrandChild = rightChild->getRightPtr();
                    if (leftGrandChild == nullptr || leftGrandChild->getColor() == shock_audio_impl::Color::BLACK)
                        if (rightGrandChild == nullptr || rightGrandChild->getColor() == shock_audio_impl::Color::BLACK)
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
    std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::leftLeftRotate(
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
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
    std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::rightRightRotate(
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
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
    std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::leftRightRotate(
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
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
    std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::rightLeftRotate(
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
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
    std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::switchColors(
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
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

    template<typename KEY_TYPE, typename DATA_TYPE>
    std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::rebalanceTreeAfterInsert(
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node) {
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
    std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>>
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::insert(
            std::unique_ptr<shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE>> node,
            std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value) {
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

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::put(std::pair<KEY_TYPE, KEY_TYPE> range, DATA_TYPE value) {
        _root = std::move(checkIfRootDoubleBlackOrRed(insert(std::move(_root), range, value)));
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::put(KEY_TYPE from, KEY_TYPE to, DATA_TYPE value) {
        _root = std::move(
                checkIfRootDoubleBlackOrRed(insert(std::move(_root), std::pair<KEY_TYPE, KEY_TYPE>(from, to), value)));
    }

    template<typename DATA_TYPE>
    std::ostream &operator<<(std::ostream &output, const std::vector<DATA_TYPE> &value) {
        for (auto node: value) {
            output << node << " ";
        }
        return output;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    int MutableRangeMap<KEY_TYPE, DATA_TYPE>::getBalanceDifference() const {
        return checkDifference(_root.get());
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    int MutableRangeMap<KEY_TYPE, DATA_TYPE>::getBlackDepth() const {
        return getDepth(_root.get());
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::getMinNode(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) const{
        if (node->getLeftPtr() == nullptr)
            return node;
        else
            return getMinNode(node->getLeftPtr());
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    RangeNode<KEY_TYPE, DATA_TYPE> *MutableRangeMap<KEY_TYPE, DATA_TYPE>::getMinNode() const {
        return (RangeNode<KEY_TYPE, DATA_TYPE>*) getMinNode(_root.get());
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::getMaxNode(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) const{
        if (node->getRightPtr() == nullptr)
            return node;
        else
            return getMaxNode(node->getRightPtr());
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    RangeNode<KEY_TYPE, DATA_TYPE> *MutableRangeMap<KEY_TYPE, DATA_TYPE>::getMaxNode() const {
        return (RangeNode<KEY_TYPE, DATA_TYPE>*) getMaxNode(_root.get());
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::merge(MutableRangeMap<KEY_TYPE, DATA_TYPE> &map) {
        auto currentBlackHeight = getBlackDepth();
        auto mapBlackHeight = map.getBlackDepth();
        if (currentBlackHeight == mapBlackHeight){
            
        } else if (currentBlackHeight > mapBlackHeight) {

        } else {

        }

    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void
    MutableRangeMap<KEY_TYPE, DATA_TYPE>::printTreeRecurrent(
            shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node, int spac) const{
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
                      << " Max: " << node->getMax();
            std::cout << std::endl;
            printTreeRecurrent(node->getRightPtr(), space);
        }
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    void MutableRangeMap<KEY_TYPE, DATA_TYPE>::printTree() const{
        printTreeRecurrent(_root.get(), 0);
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    int MutableRangeMap<KEY_TYPE, DATA_TYPE>::getDepth(shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) const{
        if (node == nullptr)
            return 0;
        if (node->getColor() == shock_audio_impl::Color::RED)
            return std::max(getDepth(node->getLeftPtr()), getDepth(node->getRightPtr()));
        else
            return std::max(getDepth(node->getLeftPtr()), getDepth(node->getRightPtr())) + 1;
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    int MutableRangeMap<KEY_TYPE, DATA_TYPE>::checkDifference(
            shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) const{
        if (node == nullptr)
            return 0;

        auto maxWithLeft = std::max(abs(getDepth(node->getLeftPtr()) - getDepth(node->getRightPtr())),
                                    checkDifference(node->getLeftPtr()));

        return std::max(maxWithLeft, checkDifference(node->getRightPtr()));
    }

    template<typename KEY_TYPE, typename DATA_TYPE>
    bool MutableRangeMap<KEY_TYPE, DATA_TYPE>::isContain(std::pair<KEY_TYPE, KEY_TYPE> range,
                                                         shock_audio_impl::MutableRangeNode<KEY_TYPE, DATA_TYPE> *node) const{
        return range.first <= node->getFrom() && range.second >= node->getTo();
    }
}
