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

    };

}

#endif //SHOCK_AUDIO_RANGEMAP_H
