#ifndef SHOCK_AUDIO_RANGENODE_H
#define SHOCK_AUDIO_RANGENODE_H

#include <utility>
#include <vector>
#include <memory>

namespace shock_audio{

    template<typename KEY_TYPE, typename DATA_TYPE>
    class RangeNode {

    public:
        virtual std::pair<KEY_TYPE, KEY_TYPE> getRange() const = 0;

        virtual std::vector<DATA_TYPE> getValue() const = 0;

        virtual KEY_TYPE getFrom() const = 0;

        virtual KEY_TYPE getTo() const = 0;

        virtual RangeNode* getLeft() const = 0;

        virtual RangeNode* getRight() const = 0;
    };

}

#endif //SHOCK_AUDIO_RANGENODE_H
