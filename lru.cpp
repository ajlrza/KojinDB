#include <string>
using namespace std;

struct Cache {
    int pos = 0;
    int next = 0;
    int prev = 0;
};

Cache LRU[128];
Cache *LRUPointer = LRU;

size_t LRUSize = 0;

void LRU_Evict() {

    if (LRUSize == 0) {
        return;
    }

    LRUPointer[LRUSize].pos = (LRUSize + 1) % sizeof(LRU) / sizeof(LRU[0]);

    LRUPointer[LRUPointer[LRUSize].pos].next = 0;
    LRUPointer[LRUPointer[LRUSize].pos].prev = 0;

    LRUSize--;

    return;
}

void LRU_Add(Cache &cache_node) {
    
    if (LRUSize == 0) {

       cache_node.pos = LRUSize;

       cache_node.next = LRUSize + 1;
       cache_node.prev = LRUSize;

       LRUPointer[LRUSize] = cache_node;
       LRUSize++;
    }

    if (LRUSize + 1 > sizeof(LRU) / sizeof(LRU[0])) {
        
        LRU_Evict();
        cache_node.pos = LRUSize;

        cache_node.next = LRUSize;
        cache_node.prev = LRUPointer[LRUSize - 1].pos;

        LRUPointer[LRUSize] = cache_node;

        LRUSize++;
    } 

    LRUSize++;
    cache_node.pos = LRUSize;

    cache_node.next = LRUPointer[LRUSize + 1].pos;
    cache_node.prev = LRUPointer[LRUSize - 1].pos;
    
    LRUPointer[LRUSize] = cache_node;
    
};
