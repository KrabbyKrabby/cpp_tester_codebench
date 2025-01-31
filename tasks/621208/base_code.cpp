#pragma once

#include <unordered_map>
#include <list>
#include <stdexcept>
#include <string>

template<typename Key, typename Value>
class LRUCache {
public:
    // Constructor: capacity is the maximum number of key-value pairs stored.
    explicit LRUCache(std::size_t capacity)
        : m_capacity(capacity)
    {
        if (capacity == 0) {
            throw std::invalid_argument("LRUCache capacity must be > 0");
        }
    }

    // Return how many items are currently in the cache
    std::size_t size() const {
        // OPTIONAL: std::lock_guard<std::mutex> lock(m_mutex);
        return m_list.size();
    }

    // Return the maximum capacity of the cache
    std::size_t capacity() const {
        return m_capacity;
    }

    // For debugging: returns a list of keys in most-recent to least-recent order
    // (Warning: exposes internal order - for test or debugging only)
    std::list<Key> getUsageOrder() const {
        std::list<Key> order;
        for (auto &pair : m_list) {
            order.push_back(pair.first);
        }
        return order;
    }

private:
    // capacity of the cache
    std::size_t m_capacity;

    // doubly-linked list of <key, value> pairs
    // most recently used at front; least recently used at back
    std::list<std::pair<Key, Value>> m_list;

    // maps keys to the corresponding list iterator
    std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator> m_map;
};