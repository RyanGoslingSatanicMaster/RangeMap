#include "tests/GetTests.h"
#include "tests/InsertTests.h"
#include "tests/DeleteTests.h"
#include "tests/ComparisionTest.h"

/** Test Cases
 * Deletion:
 * 1). Red Leaf +
 * 2). Black Leaf +
 * 3). BB +
 * 4). BRLL +
 * 5). BRRL +
 * 6). BRLR +
 * 7). BRLL +
 * 8). LRED +
 * 9). RRED +
 * 10). Root +
 * 11). Successor +
 * 12). removeAllByOverlapRange +
 * 13). removeAllByContainRange +
 * 14). removeByContainRange +
 * 14). removeAllByKey +
 * 15). removeByKey +
 * 16). removeByValue +
 * 17). removeAllByValue +
 */
int main() {
//    t->put(10, 10, 'a');
//    t->put(35, 35, 'a');
//    t->put(11, 11, 'd');
//    t->put(11, 11, 's');
//    t->put(9, 9, 's');
//    t->put(1, 1, 't');
//    t->put(3, 3, 'n');
//    t->put(6, 6, 'l');
//    t->put(32, 32, 'b');
//    t->put(41, 41, 's');
//    t->put(22, 22, 's');
//    t->put(18, 18, 'd');
//    t->put(7, 7, 's');
//    t->removeFirstByRange(std::pair(7, 7));
//    t->removeFirstByRange(std::pair(1, 1));
//    t->removeFirstByRange(std::pair(41, 41));
//    t->removeFirstByRange(std::pair(18, 18));
//    t->put(23, 23, 'y');
//    t->removeFirstByRange(std::pair(35, 35));
//    t->put(2, 2, 'e');
//    t->put(1, 1, 'w');
//    t->removeFirstByRange(std::pair(10, 10));
//    t->removeFirstByRange(std::pair(2, 2));
//    t->removeFirstByRange(std::pair(11, 11));
//    t->put(3, 44, 'n');
//    t->put(3, 34, 'd');
//    t->put(31, 75, 'm');
//    t->put(62, 65, 'k');
//    t->put(1, 11, 'd');
//    t->put(11, 54, 's');
//    t->put(3, 44, 'n');
//    t->put(3, 34, 'd');
//    t->put(31, 75, 'm');
//    t->put(62, 65, 'k');
//    t->put(23, 123, 'l');
//    t->put(3, 44, 'd');
//    t->printTree();
//    std::cout<<std::endl<<"________"<<std::endl;
//    t->removeByValue('k', 2);
//    t->printTree();
//    std::cout<<std::endl;
//    std::cout<<"MaxDepth:"<<t->getMaxDepth();
//    std::cout<<"BalanceFactor:"<<t->getBalanceDifference();
//    test::TestCaseGet();
//    test::TestCaseInsert();
//    test::TestCaseDelete();
    test::TestCaseComparison();
    return 0;
}
