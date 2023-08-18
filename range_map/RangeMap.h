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
        virtual std::vector<DATA_TYPE> get(KEY_TYPE key) const = 0;

        virtual std::vector<DATA_TYPE> get(KEY_TYPE from, KEY_TYPE to) const = 0;

        virtual std::vector<DATA_TYPE> get(KEY_TYPE key, unsigned int count) const = 0;

        virtual std::vector<DATA_TYPE> get(KEY_TYPE from, KEY_TYPE to, unsigned int count) const = 0;

        virtual std::vector<DATA_TYPE> getAll(KEY_TYPE key) const = 0;

        virtual std::vector<DATA_TYPE> getAll(KEY_TYPE from, KEY_TYPE to) const = 0;

        virtual std::vector<DATA_TYPE> getBy(std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE>&)> predicate) const = 0;

        virtual std::vector<DATA_TYPE> getBy(unsigned int count, std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE>&)> predicate) const = 0;

        virtual std::vector<DATA_TYPE> getAllBy(std::function<bool(const RangeNode<KEY_TYPE, DATA_TYPE>&)> predicate) const = 0;

    };

}

#endif //SHOCK_AUDIO_RANGEMAP_H
