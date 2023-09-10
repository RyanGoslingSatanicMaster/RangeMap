#include <iostream>
#include <chrono>
#include "../range_map/MutableRangeMap.h"
#include "GetTests.h"

namespace test {

    void fillMap(shock_audio::MutableRangeMap<int, char> *t) {
        t->put(3, 44, 'n');
        t->put(3, 34, 'd');
        t->put(31, 75, 'm');
        t->put(62, 65, 'k');
        t->put(1, 11, 'd');
        t->put(11, 54, 's');
        t->put(3, 44, 'n');
        t->put(3, 34, 'd');
        t->put(31, 75, 'm');
        t->put(62, 65, 'k');
        t->put(23, 123, 'l');
        t->put(3, 44, 'd');
        t->printTree();
        std::cout << "START MAP -------------------" << std::endl;
    }

    void testCaseGetFirstByKeyRoot(shock_audio::MutableRangeMap<int, char> *t) {
        auto el = t->get(44, 1);
        if (!el[0]->isEqualRange(3, 44)) {
            throw "testCaseGetFirstByKeyRoot Failed";
        } else
            std::cout << std::endl << "testCaseGetFirstByKeyRoot Success" << std::endl;
    }

    void testCaseGetFirstByKeyMiddle(shock_audio::MutableRangeMap<int, char> *t) {
        auto el = t->get(61, 1);
        if (!el[0]->isEqualRange(31, 75)) {
            throw "testCaseGetFirstByKeyMiddle Failed";
        } else
            std::cout << std::endl << "testCaseGetFirstByKeyMiddle Success" << std::endl;
    }

    void testCaseGetFirstByKeyLeaf(shock_audio::MutableRangeMap<int, char> *t) {
        auto el = t->get(123, 1);
        if (!el[0]->isEqualRange(23, 123)) {
            throw "testCaseGetFirstByKeyLeaf Failed";
        } else
            std::cout << std::endl << "testCaseGetFirstByKeyLeaf Success" << std::endl;
    }

    void testCaseGetCountByKey(shock_audio::MutableRangeMap<int, char> *t) {
        auto el = t->get(11, 7);
        auto flag = true;
        std::vector<std::pair<int, int>> m{std::pair(3, 44), std::pair(3, 34), std::pair(1, 11), std::pair(11, 54)};
        for (int i = 0; i < el.size(); ++i) {
            flag = el[i]->isEqualRange(m[i]) && flag;
            if (!flag)
                break;
        }
        if (!flag) {
            throw "testCaseGetCountByKey Failed";
        } else
            std::cout << std::endl << "testCaseGetCountByKey Success" << std::endl;
    }

    void testCaseGetByOverlap(shock_audio::MutableRangeMap<int, char> *t) {
        auto el = t->getByOverlap(63, 100, 4);
        auto flag = true;
        std::vector<std::pair<int, int>> m{std::pair(31, 75), std::pair(23, 123), std::pair(62, 65)};
        for (int i = 0; i < el.size(); ++i) {
            flag = el[i]->isEqualRange(m[i]) && flag;
        }
        if (!flag) {
            throw "testCaseGetByOverlap Failed";
        } else
            std::cout << std::endl << "testCaseGetByOverlap Success" << std::endl;
    }

    void testCaseGetByContain(shock_audio::MutableRangeMap<int, char> *t) {
        auto el = t->getByContain(2, 65, 3);
        auto flag = true;
        std::vector<std::pair<int, int>> m{std::pair(3, 44), std::pair(3, 34), std::pair(11, 54)};
        for (int i = 0; i < el.size(); ++i) {
            flag = el[i]->isEqualRange(m[i]) && flag;
        }
        if (!flag) {
            throw "testCaseGetByContain Failed";
        } else
            std::cout << std::endl << "testCaseGetByContain Success" << std::endl;
    }

    void stressTestGet(unsigned int count, unsigned int checkCount){
        std::cout<<" Test count: "<< count<<std::endl << std::endl;
        shock_audio::MutableRangeMap<int, int> t;
        for (int i = 0; i < count; ++i) {
            int random = rand() % 10000;
            t.put(random, random + (rand() % 10000) + 1, random);
        }
        for (int i = 0; i < checkCount; ++i) {
//            auto start = std::chrono::system_clock::now();
            t.getTestAllByKey(rand() % 10000);
//            auto end = std::chrono::system_clock::now();
//            std::chrono::duration<double> elapsed_seconds = end-start;
//            std::time_t end_time = std::chrono::system_clock::to_time_t(end);

        }

    }

void testCaseGetIf(shock_audio::MutableRangeMap<int, char> *t){
    auto el = t->getIf([] (const shock_audio::RangeNode<int, char> * node) {
        return node->getLeft() == nullptr && node->getRight() == nullptr;
    }, 1);
    auto flag = true;
    std::vector<std::pair<int, int>> m{std::pair(1, 11)};
    for (int i = 0; i < el.size(); ++i) {
        flag = el[i]->isEqualRange(m[i]) && flag;
    }
    if (!flag){
        throw "testCaseGetIf Failed";
    } else
        std::cout<<std::endl<<"testCaseIf Success"<<std::endl;
}

    void TestCaseGet() {
        shock_audio::MutableRangeMap<int, char> t;
        fillMap(&t);
        testCaseGetFirstByKeyRoot(&t);
        testCaseGetFirstByKeyMiddle(&t);
        testCaseGetFirstByKeyLeaf(&t);
        testCaseGetCountByKey(&t);
        testCaseGetByOverlap(&t);
        testCaseGetByContain(&t);
        testCaseGetIf(&t);
        stressTestGet(100, 3);
        stressTestGet(1000, 5);
        stressTestGet(10000, 7);
        stressTestGet(100000, 9);
        stressTestGet(1000000, 11);
    }
}
