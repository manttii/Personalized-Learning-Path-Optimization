#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include <vector>
#include <utility>
#include <stdexcept>
#include <unordered_map>
#include <algorithm>

/**
 * @brief Custom Binary Min-Heap Priority Queue with Index Tracking
 * 
 * Implemented as part of Data Structures in C++ (PBL Phase-I/II).
 * Supports O(log N) push and pop, O(1) top, and O(log N) decrease-key.
 */
template <typename T, typename Priority = double>
class MinHeapPriorityQueue {
public:
    struct Element {
        T value;
        Priority priority;
    };

private:
    std::vector<Element> heap_;
    std::unordered_map<T, size_t> indexMap_;

    void heapifyUp(size_t index) {
        while (index > 0) {
            size_t parent = (index - 1) / 2;
            if (heap_[index].priority < heap_[parent].priority) {
                swapElements(index, parent);
                index = parent;
            } else {
                break;
            }
        }
    }

    void heapifyDown(size_t index) {
        size_t size = heap_.size();
        while (true) {
            size_t smallest = index;
            size_t left = 2 * index + 1;
            size_t right = 2 * index + 2;

            if (left < size && heap_[left].priority < heap_[smallest].priority) {
                smallest = left;
            }
            if (right < size && heap_[right].priority < heap_[smallest].priority) {
                smallest = right;
            }

            if (smallest != index) {
                swapElements(index, smallest);
                index = smallest;
            } else {
                break;
            }
        }
    }

    void swapElements(size_t i, size_t j) {
        std::swap(heap_[i], heap_[j]);
        indexMap_[heap_[i].value] = i;
        indexMap_[heap_[j].value] = j;
    }

public:
    MinHeapPriorityQueue() = default;

    bool empty() const {
        return heap_.empty();
    }

    size_t size() const {
        return heap_.size();
    }

    bool contains(const T& value) const {
        return indexMap_.find(value) != indexMap_.end();
    }

    void push(const T& value, Priority priority) {
        if (contains(value)) {
            decreaseKey(value, priority);
            return;
        }
        size_t index = heap_.size();
        heap_.push_back({value, priority});
        indexMap_[value] = index;
        heapifyUp(index);
    }

    Element pop() {
        if (empty()) {
            throw std::runtime_error("PriorityQueue underflow: queue is empty");
        }
        Element minElement = heap_.front();
        Element lastElement = heap_.back();
        heap_.pop_back();
        indexMap_.erase(minElement.value);

        if (!heap_.empty()) {
            heap_[0] = lastElement;
            indexMap_[lastElement.value] = 0;
            heapifyDown(0);
        }
        return minElement;
    }

    Element top() const {
        if (empty()) {
            throw std::runtime_error("PriorityQueue underflow: queue is empty");
        }
        return heap_.front();
    }

    void decreaseKey(const T& value, Priority newPriority) {
        auto it = indexMap_.find(value);
        if (it == indexMap_.end()) return;

        size_t index = it->second;
        if (newPriority < heap_[index].priority) {
            heap_[index].priority = newPriority;
            heapifyUp(index);
        } else {
            heap_[index].priority = newPriority;
            heapifyDown(index);
        }
    }

    void clear() {
        heap_.clear();
        indexMap_.clear();
    }
};

#endif // PRIORITY_QUEUE_HPP
