add_library(RangeMapLib SHARED
        MutableRangeMap.h
        RangeNode.h
        MutableRangeNode.h
        InvalidRangeException.h
        RangeMap.h
        RangeItem.h
        )

set_target_properties(RangeMapLib PROPERTIES LINKER_LANGUAGE CXX)