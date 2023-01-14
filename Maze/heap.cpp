#include "heap.h"

Heap::Heap() {
    h.push_back({0, {0, 0}});
}

void Heap::down(int u) {
    int t = u;
    if(u * 2 < h.size() && h[u * 2] < h[t]) t = u * 2;
    if(u * 2 + 1 < h.size() && h[u * 2 + 1] < h[t]) t = u * 2 + 1;
    if(u != t) {
        std::swap(h[u], h[t]);
        down(t);
    }
}

void Heap::up(int u) {
    while(u / 2 && h[u / 2] > h[u]) {
        std::swap(h[u / 2], h[u]);
        u /= 2;
    }
}

void Heap::push(PPII x) {
    h.push_back(x);
    up(h.size() - 1);
}

void Heap::pop() {
    h[1] = h.back();
    h.pop_back();
    down(1);
}

PPII Heap::top() {
    return h[1];
}

int Heap::size() {
    return h.size() - 1;
}
