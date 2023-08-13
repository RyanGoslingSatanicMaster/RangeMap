# RangeMap
Range map implementation combine several data structures: 
1. Red-Black Tree(for self balancing).
2. Interval Tree(for fast interaction with ranges).

Data structure was developed according to immutable principles. Tree allow to pass equal ranges, therefor values of nodes represented as a vector.
## Structure:
### RangeNode
Immutable interface for data reading.
Methods:
getRange(): std::pair - range getter.
getFrom(): KEY_TYPE - left border getter.
getTo(): KEY_TYPE - right border getter.
getLeft(): RangeNode* - left child getter.
getRight(): RangeNode* - right child getter.
getValue(): std::vector<DATA_TYPE> - value getter.
isOverlap(...): bool - method, for check overlapping with other elements/ranges.

### MutableRangeNode : RangeNode
Mutable class for data reading and writing.
Methods:
setRange(...) - range setter;
setColor(Color) - color setter;
getColor(): Color - color getter;
setLeft(unique_ptr<MutableRangeNode>) - left child setter;
setRight(unique_ptr<MutableRangeNode>) - right child setter;
getLeftPtr(): MutableRangeNode* - left child pointer getter;
getRightPtr(): MutableRangeNode* - left child pointer getter;
getLeftUniquePtr(): unique_ptr<MutableRangeNode* > - left child unique pointer getter(used for ownership transition);
getRightUniquePtr(): unique_ptr<MutableRangeNode* > -right child unique pointer getter(used for ownership transition);
addValue(DATA_TYPE) - add some value to current vector in this node;
removeValue(DATA_TYPE) - remove some value from current vector in this node;
setValue(std::vector<DATA_TYPE>) - set new vector.
setMax(KEY_TYPE) - max setter;
getMax(KEY_TYPE) - max getter;
static generateNULLNode() - static function for generation double black nil leafs(used for tree balancing).

Exceptions:
InvalidRangeException - error, that may occur when incorrect range was passed.


