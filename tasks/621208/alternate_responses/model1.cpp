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

    Value& get(const Key& key) {
        cleanExpired();
        
        auto it = m_map.find(key);
        if (it == m_map.end()) {
            throw std::runtime_error("Key not found in cache");
        }

        auto list_it = it->second;
        if (isExpired(list_it->timestamp)) {
            m_map.erase(it);
            m_list.erase(list_it);
            throw std::runtime_error("Key has expired");
        }

        // Move to front (most recently used)
        m_list.splice(m_list.begin(), m_list, list_it);
        // Update timestamp
        list_it->timestamp = std::chrono::steady_clock::now();
        return list_it->value;
    }

    void put(const Key& key, const Value& value) {
        cleanExpired();

        auto it = m_map.find(key);
        if (it != m_map.end()) {
            // Key exists, update value and move to front
            auto list_it = it->second;
            list_it->value = value;
            list_it->timestamp = std::chrono::steady_clock::now();
            m_list.splice(m_list.begin(), m_list, list_it);
            return;
        }

        // If cache is full, remove least recently used item
        if (m_list.size() >= m_capacity) {
            auto last = m_list.back();
            m_map.erase(last.key);
            m_list.pop_back();
        }

        // Insert new item at front
        m_list.push_front(CacheEntry{key, value, std::chrono::steady_clock::now()});
        m_map[key] = m_list.begin();
    }

    void setExpirationTime(std::chrono::milliseconds ttl) {
        m_ttl = ttl;
        if (ttl > std::chrono::milliseconds(0)) {
            cleanExpired();
        }
    }

    void putAll(const std::vector<std::pair<Key, Value>>& entries) {
        for (const auto& entry : entries) {
            put(entry.first, entry.second);
        }
    }

    std::size_t size() const {
        const_cast<LRUCache*>(this)->cleanExpired();
        return m_list.size();
    }

    std::size_t capacity() const {
        return m_capacity;
    }

    std::list<Key> getUsageOrder() const {
        const_cast<LRUCache*>(this)->cleanExpired();
        std::list<Key> order;
        for (const auto& entry : m_list) {
            order.push_back(entry.key);
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
        if (m_ttl <= std::chrono::milliseconds(0)) return false;
        auto now = std::chrono::steady_clock::now();
        return (now - timestamp) > m_ttl;
    }

    void cleanExpired() {
        if (m_ttl <= std::chrono::milliseconds(0)) return;

        auto it = m_list.begin();
        while (it != m_list.end()) {
            if (isExpired(it->timestamp)) {
                m_map.erase(it->key);
                it = m_list.erase(it);
            } else {
                ++it;
            }
        }
    }

    std::size_t m_capacity;
    std::chrono::milliseconds m_ttl;
    std::list<CacheEntry> m_list;
    std::unordered_map<Key, typename std::list<CacheEntry>::iterator> m_map;
};
