#ifndef HEAP_H
#define HEAP_H

#include <QVector>
typedef QPair<int, QPair<int, int>> PPII;

class Heap
{
public:
    Heap();

    void down(int u);
    void up(int u);
    void push(PPII x);
    void pop();
    PPII top();
    int size();

private:
    QVector<PPII> h;
};

#endif // HEAP_H
