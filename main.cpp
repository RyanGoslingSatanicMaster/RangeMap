#include <iostream>
#include "range_map/MutableRangeMap.cpp"

template<typename DATA_TYPE>
std::ostream &operator <<(std::ostream& output, const std::vector<DATA_TYPE>& value)
{
    for (auto node: value)
    {
        output << node << " ";
    }
    return output;
}

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
 * 11). Successor -
 */
int main() {
    auto t = new shock_audio::MutableRangeMap<int, char>();
    t->put(10, 10, 'a');
    t->put(35, 35, 'a');
    t->put(11, 11, 'd');
    t->put(11, 11, 's');
    t->put(9, 9, 's');
    t->put(1, 1, 't');
    t->put(3, 3, 'n');
    t->put(6, 6, 'l');
    t->put(32, 32, 'b');
    t->put(41, 41, 's');
    t->put(22, 22, 's');
    t->put(18, 18, 'd');
    t->put(7, 7, 's');
    t->removeByRange(std::pair(7, 7));
    t->removeByRange(std::pair(1, 1));
    t->removeByRange(std::pair(41, 41));
    t->removeByRange(std::pair(18, 18));
    t->put(23, 23, 'y');
    t->removeByRange(std::pair(35, 35));
    t->put(2, 2, 'e');
    t->put(1, 1, 'w');
    t->removeByRange(std::pair(6, 6));
    t->removeByRange(std::pair(10, 10));
    t->removeByRange(std::pair(11, 11));
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
    t->printTree();
//    std::cout<<std::endl;
//    std::cout<<"MaxDepth:"<<t->getMaxDepth();
//    std::cout<<"BalanceFactor:"<<t->getBalanceDifference();
    return 0;
}
