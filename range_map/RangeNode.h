#ifndef SHOCK_AUDIO_RANGENODE_H
#define SHOCK_AUDIO_RANGENODE_H

#include <utility>
#include <vector>
#include <memory>
#include "RangeItem.h"

namespace shock_audio{

    template<typename KEY_TYPE, typename DATA_TYPE>
    class RangeNode: public RangeItem<KEY_TYPE, DATA_TYPE> {

    public:

        virtual RangeNode* getLeft() const = 0;

        virtual RangeNode* getRight() const = 0;

        virtual bool isOverlap(const RangeNode<KEY_TYPE, DATA_TYPE>& other) const = 0;

        virtual bool isContain(const RangeNode<KEY_TYPE, DATA_TYPE>& other) const = 0;

        virtual bool isEqualRange(const RangeNode<KEY_TYPE, DATA_TYPE>& other) const = 0;

    };

}

#endif //SHOCK_AUDIO_RANGENODE_H
