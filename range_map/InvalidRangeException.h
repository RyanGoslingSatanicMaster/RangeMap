//
// Created by vince on 13.07.2023.
//

#ifndef SHOCK_AUDIO_INVALIDRANGEEXCEPTION_H
#define SHOCK_AUDIO_INVALIDRANGEEXCEPTION_H

#include <exception>

namespace shock_audio {

    class InvalidRangeException: public std::exception{
        [[nodiscard]] const char * what () const noexcept override {
            return "Invalid range was passed";
        }
    };

}

#endif //SHOCK_AUDIO_INVALIDRANGEEXCEPTION_H
