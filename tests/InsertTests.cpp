#include <iostream>
#include "../range_map/MutableRangeMap.h"

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

    void miniTest(){
        shock_audio::MutableRangeMap<int, int> t;
        t.put(8703, 12515, 8703);
        t.put(1322, 1656, 1322);
        t.put(5141, 12853, 5141);
        t.printTree();
        t.put(7673, 12338, 7673);
        std::cout<<"____________"<<std::endl;
        t.printTree();
    }

    void TestCaseInsert() {
//        miniTest();
        balanceFactorTest(100);
        balanceFactorTest(1000);
        balanceFactorTest(10000);
        balanceFactorTest(100000);
    }
}