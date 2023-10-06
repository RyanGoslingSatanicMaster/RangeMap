#include <vector>
#include <random>
#include "../range_map/MutableRangeMap.h"
#include <chrono>

namespace test{

    void timeCompareTestInsert(unsigned int elementCount){
        shock_audio::MutableRangeMap<int, int> rangeMap;
        std::unordered_map<int, int> hashMap;
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(1, 10000);
        std::vector<std::pair<int, int>> ranges;
        for (int i = 0; i < elementCount; ++i) {
            int random = dist(mt);
            int randomTo = random + dist(mt) % 100 + 2;
            auto start = std::chrono::high_resolution_clock::now();
            rangeMap.put(random, randomTo, random);
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = duration_cast<std::chrono::microseconds>(stop - start);
            std::cout<<"RangeMap duration:"<<duration.count()<<std::endl;
            auto difference = duration.count();
            start = std::chrono::high_resolution_clock::now();
            rangeMap.put(random, randomTo, random);
            stop = std::chrono::high_resolution_clock::now();
            duration = duration_cast<std::chrono::microseconds>(stop - start);
            std::cout<<"HashMap duration:"<<duration.count()<<std::endl;
            std::cout<<"Difference:"<<difference - duration.count() <<std::endl;
        }
    }

    void TestCaseComparison(){
        timeCompareTestInsert(100);
    }

}
