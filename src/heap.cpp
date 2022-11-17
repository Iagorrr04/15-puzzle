#include <vector>
#include <algorithm>
#include <functional>


template<typename T>
class PriorityQueue
{
    using Comp = std::function<bool(const T&, const T&)>;

    Comp comp;
    std::vector<T> heap;
public:
    PriorityQueue(Comp comp_) : comp(comp_), heap(1)
    {
    }

    bool empty() const
    {
        return heap.size() <= 1;
    }

    void push(const T& x)
    {
        heap.push_back(x);
        fixup((int)heap.size()-1);
    }

    void emplace(const T& x)
    {
        push(x);
    }

    const T &top() const
    {
        return heap[1];
    }

    const T &pop()
    {
        const auto &x = top();
        heap[1] = heap.back();
        heap.pop_back();
        fixdown(1);

        return x;
    }

private:
    void fixup(int i)
    {
        for (; i > 1 && comp(heap[i / 2], heap[i]); i /= 2)
            std::swap(heap[i], heap[i/2]);
    }

    void fixdown(int i)
    {
        for (auto larger = i * 2u; i * 2u < heap.size(); i = larger, larger = i * 2)
        {
            if (larger < heap.size()-1 and comp(heap[larger], heap[larger + 1]))
                larger++;
            
            if (not comp(heap[i], heap[larger]))
                break;
            
            std::swap(heap[i], heap[larger]);
        }
    }
};
