#include <set>
#include <random>
#include "DeleteTests.h"
#include "../range_map/MutableRangeMap.h"

namespace test{

    void testTest(std::vector<std::pair<int, int>> *listToAdd, std::vector<shock_audio::RangeItem<int, int>>* listToRemove){
        shock_audio::MutableRangeMap<int, int> t;
        for (int i = 0; i < listToAdd->size(); ++i) {
            t.put((*listToAdd)[i].first, (*listToAdd)[i].second, (*listToAdd)[i].first);
        }
        auto counter = 0;
        t.traversal([&counter] (const shock_audio::RangeNode<int, int> * node) {
            if (node->getFrom() == 1192 && node->getTo() == 1217)
                counter++;
        });
        for (int i = 0; i < listToRemove->size(); ++i) {
            t.removeByRange((*listToRemove)[i].getRange());
        }
    }

    void testCaseRemoveByRange(unsigned int countItem, unsigned int removeCount){
        shock_audio::MutableRangeMap<int, int> t;
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(1, 10000);
        std::vector<std::pair<int, int>> ranges;
        for (int i = 0; i < countItem; ++i) {
            int random = dist(mt);
            int randomTo = random + dist(mt) % 100 + 2;
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
            auto element = t.getByRange(listToRemove[i].getFrom(), listToRemove[i].getTo());
            if (element != nullptr) {
//                testTest(&ranges, &listToRemove);
                std::cout<<"Failed on "<<element->getFrom()<<" "<<element->getTo()<<std::endl;
                throw "testCaseRemoveByRange Failed";
            }
        }
        if (t.getBalanceDifference() > 1) {
            testTest(&ranges, &listToRemove);
            std::cout<<"testCaseRemoveByRange Failed on BalanceFactor"<<std::endl;
            throw "testCaseRemoveByRange Failed";
        }
        std::cout<<std::endl<<"testCaseRemoveByRange Success for "<<countItem<<" elements and "<<i + 1<<" elements to DELETE"<<std::endl;
    }

    void testCaseRemoveByOverlap(unsigned int countItem, unsigned int removeCount){
        shock_audio::MutableRangeMap<int, int> t;
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(0, 10000);
        std::vector<std::pair<int, int>> ranges;
        for (int i = 0; i < countItem; ++i) {
            int random = dist(mt) % 10000;
            int randomTo = random + dist(mt) % 100 + 2;
            t.put(random, randomTo, random);
            ranges.push_back(std::pair(random, randomTo));

        }
        auto listToRemove = shock_audio::MutableRangeMap<int, int>::copyFromPtrs(t.getIf([&dist, &mt] (const shock_audio::RangeNode<int, int> * node) {
            return dist(mt) % 200 > 50;
        }, removeCount));
        auto i = 0;
        for (; i < listToRemove.size() && i < removeCount; ++i) {
            t.removeAllByOverlapRange(listToRemove[i]->getRange());
            auto element = t.getAllByOverlap(listToRemove[i]->getFrom(), listToRemove[i]->getTo());
            if (element.size() > 0) {
//                testTest(&ranges, &listToRemove);
                std::cout<<"Failed on "<<element[0]->getFrom()<<" "<<element[0]->getTo()<<std::endl;
                throw "testCaseRemoveByOverlap Failed";
            }
        }
        if (t.getBalanceDifference() > 1) {
            //testTest(&ranges, &listToRemove);
            std::cout<<"testCaseRemoveByOverlap Failed on BalanceFactor"<<std::endl;
            throw "testCaseRemoveByOverlap Failed";
        }
        std::cout<<std::endl<<"testCaseRemoveByOverlap Success for "<<countItem<<" elements and "<<i + 1<<" elements to DELETE"<<std::endl;
    }

    void testCaseRemoveByContain(unsigned int countItem, unsigned int removeCount){
        shock_audio::MutableRangeMap<int, int> t;
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(1, 10000);
        std::vector<std::pair<int, int>> ranges;
        for (int i = 0; i < countItem; ++i) {
            int random = dist(mt) % 10000;
            int randomTo = random + dist(mt) % 100 + 2;
            t.put(random, randomTo, random);
            ranges.push_back(std::pair(random, randomTo));

        }
        auto listToRemove = shock_audio::MutableRangeMap<int, int>::copyFromPtrs(t.getIf([&dist, &mt] (const shock_audio::RangeNode<int, int> * node) {
            return dist(mt) > 50;
        }, removeCount));
        auto i = 0;
        for (; i < listToRemove.size() && i < removeCount; ++i) {
            t.removeAllByContainRange(listToRemove[i]->getRange());
            auto element = t.getAllByContain(listToRemove[i]->getFrom(), listToRemove[i]->getTo());
            if (element.size() > 0) {
//                testTest(&ranges, &listToRemove);
                std::cout<<"Failed on "<<element[0]->getFrom()<<" "<<element[0]->getTo()<<std::endl;
                throw "testCaseRemoveByContain Failed";
            }
        }
        auto balanceFactor = t.getBalanceDifference();
        if (balanceFactor > 1) {
            //testTest(&ranges, &listToRemove);
            std::cout<<"testCaseRemoveByContain Failed on BalanceFactor Difference: "<< balanceFactor << std::endl;
            throw "testCaseRemoveByContain Failed";
        }
        std::cout<<std::endl<<"testCaseRemoveByContain Success for "<<countItem<<" elements and "<<i + 1<<" elements to DELETE"<<std::endl;
    }

    void testCaseRemoveByContained(unsigned int countItem, unsigned int removeCount){
        shock_audio::MutableRangeMap<int, int> t;
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(1, 10000);
        std::vector<std::pair<int, int>> ranges;
        for (int i = 0; i < countItem; ++i) {
            int random = dist(mt) % 10000;
            int randomTo = random + dist(mt) % 100 + 2;
            t.put(random, randomTo, random);
            ranges.push_back(std::pair(random, randomTo));

        }
        auto listToRemove = shock_audio::MutableRangeMap<int, int>::copyFromPtrs(t.getIf([&dist, &mt] (const shock_audio::RangeNode<int, int> * node) {
            return dist(mt) > 50;
        }, removeCount));
        auto i = 0;
        for (; i < listToRemove.size() && i < removeCount; ++i) {
            t.removeAllByContainedRange(listToRemove[i]->getRange());
            auto element = t.getAllByContained(listToRemove[i]->getFrom(), listToRemove[i]->getTo());
            if (element.size() > 0) {
//                testTest(&ranges, &listToRemove);
                std::cout<<"Failed on "<<element[0]->getFrom()<<" "<<element[0]->getTo()<<std::endl;
                throw "testCaseRemoveByContained Failed";
            }
        }
        if (t.getBalanceDifference() > 1) {
            //testTest(&ranges, &listToRemove);
            std::cout<<"testCaseRemoveByContained Failed on BalanceFactor"<<std::endl;
            throw "testCaseRemoveByContained Failed";
        }
        std::cout<<std::endl<<"testCaseRemoveByContained Success for "<<countItem<<" elements and "<<i + 1<<" elements to DELETE"<<std::endl;
    }

    void testCaseRemoveByValue(unsigned int countItem, unsigned int removeCount){
        shock_audio::MutableRangeMap<int, int> t;
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(1, 10000);
        std::vector<std::pair<int, int>> ranges;
        for (int i = 0; i < countItem; ++i) {
            int random = dist(mt);
            int randomTo = random + dist(mt) % 100 + 2;
            int randomValue = dist(mt);
            t.put(random, randomTo, randomValue);
            ranges.push_back(std::pair(random, randomTo));

        }
        auto i = 0;
        while(i < removeCount){
            auto randVal = dist(mt);
            if (t.getAllByValue(randVal).size() > 0){
                t.removeAllByValue(randVal);
                auto elements = t.getAllByValue(randVal);
                if (elements.size() > 0){
                    std::cout<<"Failed on "<<elements[0]->getFrom()<<" "<<elements[0]->getTo()<<std::endl;
                    throw "testCaseRemoveByValue Failed";
                }
                i++;
            }
        }
//        if (t.getBalanceDifference() > 1) {
//            t.printTree();
//            std::cout<<"testCaseRemoveIf Failed on BalanceFactor"<<std::endl;
//            throw "testCaseRemoveIf Failed";
//        }
        std::cout<<std::endl<<"testCaseRemoveIf Success for "<<countItem<<" elements and "<<i<<" elements to DELETE"<<std::endl;
    }

    void testCaseRemoveIf(unsigned int countItem){
        shock_audio::MutableRangeMap<int, int> t;
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(1, 10000);
        std::vector<std::pair<int, int>> ranges;
        for (int i = 0; i < countItem; ++i) {
            int random = dist(mt);
            int randomTo = random + dist(mt) % 100 + 2;
            int randomValue = dist(mt);
            t.put(random, randomTo, randomValue);
            ranges.push_back(std::pair(random, randomTo));

        }
        auto i = 0;
        std::function<bool(const shock_audio::RangeNode<int, int>*)> predicate = [&i] (const shock_audio::RangeNode<int, int>* node) {
            auto flag = node->getLeft() != nullptr && node->getValue() > node->getLeft()->getValue();
            if (flag)
                i++;
            return flag;
        };
        t.removeAllIf(predicate);
        auto elements = t.getAllIf(predicate);
        if (elements.size() > 0) {
            std::cout<<"Failed on "<<elements[0]->getFrom()<<" "<<elements[0]->getTo()<<std::endl;
            throw "testCaseRemoveIf Failed";
        }
        if (t.getBalanceDifference() > 1) {
            t.printTree();
            std::cout<<"testCaseRemoveIf Failed on BalanceFactor"<<std::endl;
            throw "testCaseRemoveIf Failed";
        }
        std::cout<<std::endl<<"testCaseRemoveIf Success for "<<countItem<<" elements and "<<i + 1<<" elements to DELETE"<<std::endl;
    }

    void TestCaseDelete(){
//        testCaseRemoveByRange(70, 30);
//        testCaseRemoveByRange(100, 30);
//        testCaseRemoveByRange(1000, 500);
//        testCaseRemoveByRange(5000, 1000);
//        testCaseRemoveByRange(30000, 50);
//        testCaseRemoveByRange(700000, 300000);
//        testCaseRemoveByOverlap(70, 30);
//        testCaseRemoveByOverlap(100, 30);
//        testCaseRemoveByOverlap(1000, 500);
//        testCaseRemoveByOverlap(5000, 1000);
//        testCaseRemoveByOverlap(30000, 50);
//        testCaseRemoveByOverlap(700000, 300000);
//        testCaseRemoveByContain(70, 30);
//        testCaseRemoveByContain(100, 30);
//        testCaseRemoveByContain(1000, 500);
//        testCaseRemoveByContain(5000, 1000);
//        testCaseRemoveByContain(30000, 50);
//        testCaseRemoveByContain(700000, 300000);
//        testCaseRemoveByContained(70, 30);
//        testCaseRemoveByContained(100, 30);
//        testCaseRemoveByContained(1000, 500);
//        testCaseRemoveByContained(5000, 1000);
//        testCaseRemoveByContained(30000, 50);
//        testCaseRemoveByContained(700000, 300000);
//        testCaseRemoveIf(70);
//        testCaseRemoveIf(100);
//        testCaseRemoveIf(1000);
//        testCaseRemoveIf(5000);
//        testCaseRemoveIf(30000);
//        testCaseRemoveIf(700000);
        testCaseRemoveByValue(70, 30);
        testCaseRemoveByValue(100, 30);
        testCaseRemoveByValue(1000, 500);
        testCaseRemoveByValue(5000, 1000);
        testCaseRemoveByValue(30000, 50);
        testCaseRemoveByValue(700000, 300000);
    }
}
