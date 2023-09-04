//
// Created by vince on 03.08.2023.
//

#ifndef SHOCK_AUDIO_RANGEMAP_H
#define SHOCK_AUDIO_RANGEMAP_H

#include <vector>

namespace shock_audio{

    template<typename KEY_TYPE, typename DATA_TYPE>
    class RangeMap{

    public:
        virtual std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> get(KEY_TYPE key, unsigned int count) const = 0;

        virtual std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*>getByOverlap(KEY_TYPE from, KEY_TYPE to, unsigned int count) const = 0;

        virtual std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getByContain(KEY_TYPE from, KEY_TYPE to, unsigned int count) const = 0;

        virtual std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getBy(unsigned int count, std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE>*)> predicate) const = 0;

        virtual const RangeItem<KEY_TYPE, DATA_TYPE>* getByRange(KEY_TYPE from, KEY_TYPE to) const = 0;

        virtual std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getAll(KEY_TYPE key) const = 0;

        virtual std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getAllByOverlap(KEY_TYPE from, KEY_TYPE to) const = 0;

        virtual std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getAllByContain(KEY_TYPE from, KEY_TYPE to) const = 0;

        virtual std::vector<const RangeItem<KEY_TYPE, DATA_TYPE>*> getAllBy(std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE>*)> predicate) const = 0;

        virtual RangeNode<KEY_TYPE, DATA_TYPE>* getMaxNode() const = 0;

        virtual RangeNode<KEY_TYPE, DATA_TYPE>* getMinNode() const = 0;

        virtual int getBalanceDifference() const = 0;

        virtual int getBlackDepth() const = 0;

        virtual RangeNode<KEY_TYPE, DATA_TYPE>* getRoot() const = 0;

        virtual void printTree() const = 0;

        virtual unsigned int size() const = 0;

    };

}

#endif //SHOCK_AUDIO_RANGEMAP_H
