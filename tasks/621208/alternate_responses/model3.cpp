#pragma once

#include <unordered_map>
#include <list>
#include <stdexcept>
#include <string>
#include <vector>
#include <chrono>
#include <optional>

template<typename Key, typename Value>
class LRUCache {
public:
    explicit LRUCache(std::size_t capacity)
        : m_capacity(capacity)
        , m_ttl(std::chrono::milliseconds(0))
    {
        if (capacity == 0) {
            throw std::invalid_argument("LRUCache capacity must be > 0");
        }
    }

    std::size_t size() const {
        // OPTIONAL: std::lock_guard<std::mutex> lock(m_mutex);
        return m_list.size();
    }

    std::size_t capacity() const {
        return m_capacity;
    }

    void setExpirationTime(std::chrono::milliseconds ttl) {
        // OPTIONAL: std::lock_guard<std::mutex> lock(m_mutex);
        m_ttl = ttl;
    }

    Value& get(const Key& key) {
        // OPTIONAL: std::lock_guard<std::mutex> lock(m_mutex);
        
        auto it = m_map.find(key);
        if (it == m_map.end()) {
            throw std::runtime_error("Key not found in cache");
        }

        auto list_it = it->second;
        if (isExpired(list_it->timestamp)) {
            m_map.erase(key);
            m_list.erase(list_it);
            throw std::runtime_error("Key has expired");
        }

        // Move to front (most recently used)
        list_it->timestamp = std::chrono::steady_clock::now();
        m_list.splice(m_list.begin(), m_list, list_it);
        return list_it->value;
    }

    void put(const Key& key, const Value& value) {
        // OPTIONAL: std::lock_guard<std::mutex> lock(m_mutex);
        
        auto now = std::chrono::steady_clock::now();
        auto it = m_map.find(key);

        if (it != m_map.end()) {
            // Key exists, update value and move to front
            auto list_it = it->second;
            list_it->value = value;
            list_it->timestamp = now;
            m_list.splice(m_list.begin(), m_list, list_it);
            return;
        }

        // Remove LRU item if cache is full
        if (m_list.size() >= m_capacity) {
            const auto& lru = m_list.back();
            m_map.erase(lru.key);
            m_list.pop_back();
        }

        // Insert new item at front
        m_list.push_front(CacheEntry{key, value, now});
        m_map[key] = m_list.begin();
    }

    void putAll(const std::vector<std::pair<Key, Value>>& entries) {
        // OPTIONAL: std::lock_guard<std::mutex> lock(m_mutex);
        
        for (const auto& entry : entries) {
            put(entry.first, entry.second);
        }
    }

    std::list<Key> getUsageOrder() const {
        // OPTIONAL: std::lock_guard<std::mutex> lock(m_mutex);
        
        std::list<Key> order;
        for (const auto& entry : m_list) {
            if (!isExpired(entry.timestamp)) {
                order.push_back(entry.key);
            }
        }
        return order;
    }

private:
    struct CacheEntry {
        Key key;
        Value value;
        std::chrono::steady_clock::time_point timestamp;
    };

    bool isExpired(const std::chrono::steady_clock::time_point& timestamp) const {
        if (m_ttl.count() == 0) return false;
        auto now = std::chrono::steady_clock::now();
        return (now - timestamp) > m_ttl;
    }

    std::size_t m_capacity;
    std::chrono::milliseconds m_ttl;
    std::list<CacheEntry> m_list;
    std::unordered_map<Key, typename std::list<CacheEntry>::iterator> m_map;
    // OPTIONAL: mutable std::mutex m_mutex;
};
