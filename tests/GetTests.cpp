#include <iostream>
#include "../range_map/MutableRangeMap.h"

void fillMap(shock_audio::MutableRangeMap<int, char> *t){
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
    std::cout<<"START MAP -------------------"<<std::endl;
}

void testCaseGetFirstByKeyRoot(shock_audio::MutableRangeMap<int, char> *t){
    auto el = t->get(44, 1);
    if (!el[0]->isEqualRange(3, 44)){
        throw "testCaseGetFirstByKeyRoot Failed";
    } else
        std::cout<<std::endl<<"testCaseGetFirstByKeyRoot Success"<<std::endl;
}

void testCaseGetFirstByKeyMiddle(shock_audio::MutableRangeMap<int, char> *t){
    auto el = t->get(61, 1);
    if (!el[0]->isEqualRange(31, 75)){
        throw "testCaseGetFirstByKeyMiddle Failed";
    } else
        std::cout<<std::endl<<"testCaseGetFirstByKeyMiddle Success"<<std::endl;
}

void testCaseGetFirstByKeyLeaf(shock_audio::MutableRangeMap<int, char> *t){
    auto el = t->get(123, 1);
    if (!el[0]->isEqualRange(23, 123)){
        throw "testCaseGetFirstByKeyLeaf Failed";
    } else
        std::cout<<std::endl<<"testCaseGetFirstByKeyLeaf Success"<<std::endl;
}

void testCaseGetCountByKey(shock_audio::MutableRangeMap<int, char> *t){
    auto el = t->get(11, 7);
    auto flag = true;
    std::vector<std::pair<int, int>> m{std::pair(3, 44), std::pair(3, 34), std::pair(1, 11), std::pair(11, 54)};
    for (int i = 0; i < el.size(); ++i) {
        flag = el[i]->isEqualRange(m[i]) && flag;
        if(!flag)
            break;
    }
    if (!flag){
        throw "testCaseGetCountByKey Failed";
    } else
        std::cout<<std::endl<<"testCaseGetCountByKey Success"<<std::endl;
}

void testCaseGetByOverlap(shock_audio::MutableRangeMap<int, char> *t){
    auto el = t->getByOverlap(63, 100, 4);
    auto flag = true;
    std::vector<std::pair<int, int>> m{std::pair(31, 75), std::pair(23, 123), std::pair(62, 65)};
    for (int i = 0; i < el.size(); ++i) {
        flag = el[i]->isEqualRange(m[i]) && flag;
    }
    if (!flag){
        throw "testCaseGetByOverlap Failed";
    } else
        std::cout<<std::endl<<"testCaseGetByOverlap Success"<<std::endl;
}

void testCaseGetByContain(shock_audio::MutableRangeMap<int, char> *t){
    auto el = t->getByContain(2, 65, 3);
    auto flag = true;
    std::vector<std::pair<int, int>> m{std::pair(3, 44), std::pair(3, 34), std::pair(11, 54)};
    for (int i = 0; i < el.size(); ++i) {
        flag = el[i]->isEqualRange(m[i]) && flag;
    }
    if (!flag){
        throw "testCaseGetByContain Failed";
    } else
        std::cout<<std::endl<<"testCaseGetByContain Success"<<std::endl;
}

void testCaseGetBy(shock_audio::MutableRangeMap<int, char> *t){
    auto el = t->getByContain(2, 65, 3);
    auto flag = true;
    std::vector<std::pair<int, int>> m{std::pair(3, 44), std::pair(3, 34), std::pair(11, 54)};
    for (int i = 0; i < el.size(); ++i) {
        flag = el[i]->isEqualRange(m[i]) && flag;
    }
    if (!flag){
        throw "testCaseGetByContain Failed";
    } else
        std::cout<<std::endl<<"testCaseGetByContain Success"<<std::endl;
}

void TestCaseGet(){
    shock_audio::MutableRangeMap<int, char> t;
    fillMap(&t);
    testCaseGetFirstByKeyRoot(&t);
    testCaseGetFirstByKeyMiddle(&t);
    testCaseGetFirstByKeyLeaf(&t);
    testCaseGetCountByKey(&t);
    testCaseGetByOverlap(&t);
    testCaseGetByContain(&t);
}
