#include <set>
#include "DeleteTests.h"
#include "../range_map/MutableRangeMap.h"

namespace test{

    void testCaseRemoveByRange(unsigned int countItem, unsigned int removeCount){
        shock_audio::MutableRangeMap<int, char> t;
        for (int i = 0; i < countItem; ++i) {
            int random = rand() % 10000;
            t.put(random, random + rand() % 40 + 1, random);
        }
        std::set<std::pair<int, int>> listToRemove;
        while (listToRemove.size() < removeCount){
            int random = rand() % 10000;
            auto overlapedEl = t.getByOverlap(random, random + (rand() % 10000) + 1);
            if (overlapedEl.size() > 0 && overlapedEl[0] != nullptr)
                listToRemove.insert(overlapedEl[0]->getRange());
        }
        for (auto el : listToRemove) {
            t.removeByRange(el);
            if (t.getByRange(el.first, el.second) != nullptr)
                throw "testCaseRemoveByRange Failed";
        }
        if (t.getBalanceDifference() > 1)
            throw "testCaseRemoveByRange Failed";
        std::cout<<std::endl<<"testCaseRemoveByRange Success for "<<countItem<<" elements and "<<removeCount<<" elements to DELETE"<<std::endl;
    }

    void TestCaseDelete(){
        testCaseRemoveByRange(100, 10);
        testCaseRemoveByRange(500, 50);
        testCaseRemoveByRange(1000, 500);
        testCaseRemoveByRange(5000, 1000);
        testCaseRemoveByRange(30000, 50);
    }
}
