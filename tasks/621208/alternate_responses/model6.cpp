#pragma once

#include <unordered_map>
#include <list>
#include <stdexcept>
#include <string>
#include <vector>
#include <chrono>
#include <mutex>

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

    // Retrieves the value associated with 'key'. Throws a runtime error if key not found.
    Value& get(const Key& key) {
        auto it = m_map.find(key);
        if (it == m_map.end()) {
            throw std::runtime_error("Key not found in cache");
        }

        auto listIt = it->second;
        if (m_ttl.count() > 0 && listIt->second.expiration < std::chrono::steady_clock::now()) {
            // Key has expired, remove it
            m_list.erase(listIt);
            m_map.erase(it);
            throw std::runtime_error("Key has expired");
        }

        // Move accessed key to the front of the list
        m_list.splice(m_list.begin(), m_list, listIt);
        return m_list.front().second.value;
    }

    // Store a key-value pair in the cache (at the front). If the cache is full, evict the LRU entry.
    // If key already exists, move it to the front (most recent).
    void put(const Key& key, const Value& value) {
        auto it = m_map.find(key);
        if (it != m_map.end()) {
            // Key already exists, remove it
            m_list.erase(it->second);
            m_map.erase(it);
        } else if (m_list.size() >= m_capacity) {
            // Cache is full, evict the LRU entry
            m_map.erase(m_list.back().first);
            m_list.pop_back();
        }

        // Add the new key-value pair to the front of the list
        m_list.emplace_front(key, CacheEntry{value, m_ttl.count() > 0 ? std::chrono::steady_clock::now() + m_ttl : std::chrono::steady_clock::time_point::max()});
        m_map[key] = m_list.begin();
    }

    // Set the expiration time for newly inserted or updated keys
    void setExpirationTime(std::chrono::milliseconds ttl) {
        m_ttl = ttl;
    }

    // Inserts or updates a batch of key-value pairs in one shot
    void putAll(const std::vector<std::pair<Key, Value>>& entries) {
        for (const auto& entry : entries) {
            put(entry.first, entry.second);
        }
    }

private:
    struct CacheEntry {
        Value value;
        std::chrono::steady_clock::time_point expiration;
    };

    // capacity of the cache
    std::size_t m_capacity;

    // doubly-linked list of <key, value> pairs
    // most recently used at front; least recently used at back
    std::list<std::pair<Key, CacheEntry>> m_list;

    // maps keys to the corresponding list iterator
    std::unordered_map<Key, typename std::list<std::pair<Key, CacheEntry>>::iterator> m_map;

    // time to live for cache entries
    std::chrono::milliseconds m_ttl{0};
};
