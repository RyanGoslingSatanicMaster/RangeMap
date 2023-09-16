#include <set>
#include "DeleteTests.h"
#include "../range_map/MutableRangeMap.h"

namespace test{

    void testTest(std::vector<std::pair<int, int>> *listToAdd, std::vector<shock_audio::RangeItem<int, int>>* listToRemove){
        shock_audio::MutableRangeMap<int, int> t;
        for (int i = 0; i < listToAdd->size(); ++i) {
            t.put((*listToAdd)[i].first, (*listToAdd)[i].second, (*listToAdd)[i].first);
        }
        for (int i = 0; i < listToRemove->size(); ++i) {
            t.removeByRange((*listToRemove)[i].getRange());
        }
    }

    void testCaseRemoveByRange(unsigned int countItem, unsigned int removeCount){
        shock_audio::MutableRangeMap<int, int> t;
        std::vector<std::pair<int, int>> ranges;
        for (int i = 0; i < countItem; ++i) {
            int random = rand() % 10000;
            int randomTo = random + rand() % 40 + 2;
            t.put(random, randomTo, random);
            ranges.push_back(std::pair(random, randomTo));

        }
        std::vector<shock_audio::RangeItem<int, int>> listToRemove;
        t.traversal([&removeCount, &listToRemove] (const shock_audio::RangeNode<int, int> * node) {
            if (listToRemove.size() < removeCount) {
                listToRemove.push_back(shock_audio::RangeItem<int, int>(node->getRange(), node->getValue()));
            }
        });
        auto i = 0;
        for (; i < listToRemove.size() && i < removeCount; ++i) {
            t.removeByRange(listToRemove[i].getRange());
        }
        if (t.getBalanceDifference() > 1) {
            testTest(&ranges, &listToRemove);
            throw "testCaseRemoveByRange Failed";
        }
        std::cout<<std::endl<<"testCaseRemoveByRange Success for "<<countItem<<" elements and "<<i + 1<<" elements to DELETE"<<std::endl;
    }

    void TestCaseDelete(){
        testCaseRemoveByRange(100, 30);
        testCaseRemoveByRange(1000, 500);
        testCaseRemoveByRange(5000, 1000);
        testCaseRemoveByRange(30000, 50);
        testCaseRemoveByRange(700000, 300000);
    }
}
