//
// Created by vince on 03.08.2023.
//

#ifndef SHOCK_AUDIO_RANGEMAP_H
#define SHOCK_AUDIO_RANGEMAP_H

#include <vector>
#include "RangeNode.h"
#include "RangeItem.h"

namespace shock_audio{

    template<typename KEY_TYPE, typename DATA_TYPE>
    class RangeMap{

    public:
        virtual std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> get(KEY_TYPE key, unsigned int count = 1) const = 0;

        virtual std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*>getByOverlap(KEY_TYPE from, KEY_TYPE to, unsigned int count = 1) const = 0;

        virtual std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getByContain(KEY_TYPE from, KEY_TYPE to, unsigned int count = 1) const = 0;

        virtual std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getByContained(KEY_TYPE from, KEY_TYPE to, unsigned int count = 1) const = 0;

        virtual std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getIf(std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE>*)> predicate, unsigned int count = 1) const = 0;

        virtual const RangeItem<KEY_TYPE, DATA_TYPE>* getByRange(KEY_TYPE from, KEY_TYPE to) const = 0;

        virtual std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getByValue(DATA_TYPE val, unsigned int count = 1) const = 0;

        virtual std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getAll(KEY_TYPE key) const = 0;

        virtual std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getAllByOverlap(KEY_TYPE from, KEY_TYPE to) const = 0;

        virtual std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getAllByContain(KEY_TYPE from, KEY_TYPE to) const = 0;

        virtual std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getAllByContained(KEY_TYPE from, KEY_TYPE to) const = 0;

        virtual std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getAllIf(std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE>*)> predicate) const = 0;

        virtual std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getAllByValue(DATA_TYPE val) const = 0;

        virtual RangeNode<KEY_TYPE, DATA_TYPE>* getMaxNode() const = 0;

        virtual RangeNode<KEY_TYPE, DATA_TYPE>* getMinNode() const = 0;

        virtual int getBalanceDifference() const = 0;

        virtual int getBlackDepth() const = 0;

        virtual RangeNode<KEY_TYPE, DATA_TYPE>* getRoot() const = 0;

        virtual void printTree() const = 0;

        virtual unsigned int size() const = 0;

        virtual void traversal(std::function<void(const RangeNode<KEY_TYPE, DATA_TYPE>*)> predicate) const = 0;

        virtual ~RangeMap() = default;

    };

}

#endif //SHOCK_AUDIO_RANGEMAP_H
