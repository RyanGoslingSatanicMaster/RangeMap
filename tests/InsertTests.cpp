#include <iostream>
#include "../range_map/MutableRangeMap.h"
#include "InsertTests.h"

namespace test {

    void balanceFactorTest(unsigned int count){
        std::cout<<" Test count: "<< count<<std::endl;
        shock_audio::MutableRangeMap<int, int> t;
        int from, to;
        for (int i = 0; i < count; ++i) {
            int random = rand() % 10000;
            auto randomTo = random + (rand() % 10000) + 1;
            t.put(random, randomTo, random);
            if (t.getBalanceDifference() > 1) {
                std::cout<<"BalanceFactorTest Failed at "<<from<<" "<<to<<std::endl;
                throw "BalanceFactorTest Failed";
            }
            from = random;
            to = randomTo;
        }
        std::cout<<"BalanceFactorTest Success for "<<count<<" items"<<std::endl<<std::endl;
    }

    void duplicateTest(unsigned int count){
        std::cout<<" Test count: "<< count<<std::endl;
        shock_audio::MutableRangeMap<int, int> t;
        shock_audio::MutableRangeMap<int, int> copyT;
        for (int i = 0; i < count; ++i) {
            int random = rand() % 10000;
            auto randomTo = random + (rand() % 10000) + 1;
            auto elementCount = 0;
            t.put(random, randomTo, random);
            t.traversal([&random, &randomTo, &elementCount] (const shock_audio::RangeNode<int, int>* node) {
                if (node->getFrom() == random && node->getTo() == randomTo)
                    elementCount++;
            });
            if (elementCount > 1) {
                copyT.printTree();
                std::cout<<"DuplicateTest Failed at "<<random<<" "<<randomTo<<std::endl;
                throw "DuplicateTest Failed";
            }
            copyT.put(random, randomTo, random);
        }
        std::cout<<"DuplicateTest Success for "<<count<<" items"<<std::endl<<std::endl;
    }

    void doubleRedTest(unsigned int count){
        std::cout<<" Test count: "<< count<<std::endl;
        shock_audio::MutableRangeMap<int, int> t;
        for (int i = 0; i < count; ++i) {
            int random = rand() % 10000;
            auto randomTo = random + (rand() % 10000) + 1;
            t.put(random, randomTo, random);
        }
        t.traversal([] (const shock_audio::RangeNode<int, int>* node) {
            auto mutableNode = static_cast<const shock_audio_impl::MutableRangeNode<int, int>*>(node);
            if (mutableNode->getColor() == shock_audio_impl::Color::BLACK){
                if (mutableNode->getLeftPtr() != nullptr && mutableNode->getLeftPtr()->getColor() == shock_audio_impl::Color::RED){
                    if (mutableNode->getLeftPtr()->getLeftPtr() != nullptr && mutableNode->getLeftPtr()->getLeftPtr()->getColor() == shock_audio_impl::Color::RED ||
                            mutableNode->getLeftPtr()->getRightPtr() != nullptr && mutableNode->getLeftPtr()->getRightPtr()->getColor() == shock_audio_impl::Color::RED)
                        throw "DoubleRedTest Failed";
                }
                if (mutableNode->getRightPtr() != nullptr && mutableNode->getRightPtr()->getColor() == shock_audio_impl::Color::RED){
                    if (mutableNode->getRightPtr()->getLeftPtr() != nullptr && mutableNode->getRightPtr()->getLeftPtr()->getColor() == shock_audio_impl::Color::RED ||
                        mutableNode->getRightPtr()->getRightPtr() != nullptr && mutableNode->getRightPtr()->getRightPtr()->getColor() == shock_audio_impl::Color::RED)
                        throw "DoubleRedTest Failed";
                }
            }
        });
        std::cout<<"DoubleRedTest Success for "<<count<<" items"<<std::endl<<std::endl;
    }

    void TestCaseInsert() {
        duplicateTest(100);
        duplicateTest(1000);
        duplicateTest(10000);
//        duplicateTest(100000);
        doubleRedTest(100);
        doubleRedTest(1000);
        doubleRedTest(10000);
        doubleRedTest(100000);
        balanceFactorTest(100);
        balanceFactorTest(1000);
        balanceFactorTest(10000);
    }
}