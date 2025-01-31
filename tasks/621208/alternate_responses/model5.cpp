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
        // OPTIONAL: std::lock_guard<std::mutex> lock(m_mutex);
        
        auto it = m_map.find(key);
        if (it == m_map.end()) {
            throw std::runtime_error("Key not found in cache");
        }

        // Check if the entry has expired
        if (isExpired(it->second->second.timestamp)) {
            m_map.erase(it);
            m_list.erase(it->second);
            throw std::runtime_error("Key has expired");
        }

        // Move to front (most recently used)
        m_list.splice(m_list.begin(), m_list, it->second);
        
        // Update timestamp if TTL is enabled
        if (m_ttl.count() > 0) {
            it->second->second.timestamp = std::chrono::steady_clock::now();
        }

        return it->second->first.second;
    }

    void put(const Key& key, const Value& value) {
        // OPTIONAL: std::lock_guard<std::mutex> lock(m_mutex);
        
        auto it = m_map.find(key);
        if (it != m_map.end()) {
            // Key exists, check if expired
            if (isExpired(it->second->second.timestamp)) {
                m_map.erase(it);
                m_list.erase(it->second);
            } else {
                // Update existing entry
                it->second->first.second = value;
                m_list.splice(m_list.begin(), m_list, it->second);
                if (m_ttl.count() > 0) {
                    it->second->second.timestamp = std::chrono::steady_clock::now();
                }
                return;
            }
        }

        // Remove LRU item if cache is full
        if (m_list.size() >= m_capacity) {
            auto lru = m_list.back();
            m_map.erase(lru.first.first);
            m_list.pop_back();
        }

        // Insert new item at front
        m_list.push_front({
            std::make_pair(key, value),
            CacheMetadata{std::chrono::steady_clock::now()}
        });
        m_map[key] = m_list.begin();
    }

    void setExpirationTime(std::chrono::milliseconds ttl) {
        // OPTIONAL: std::lock_guard<std::mutex> lock(m_mutex);
        m_ttl = ttl;
        if (ttl.count() > 0) {
            cleanupExpired();
        }
    }

    void putAll(const std::vector<std::pair<Key, Value>>& entries) {
        // OPTIONAL: std::lock_guard<std::mutex> lock(m_mutex);
        for (const auto& entry : entries) {
            put(entry.first, entry.second);
        }
    }

    std::size_t size() const {
        // OPTIONAL: std::lock_guard<std::mutex> lock(m_mutex);
        cleanupExpired();
        return m_list.size();
    }

    std::size_t capacity() const {
        return m_capacity;
    }

    std::list<Key> getUsageOrder() const {
        // OPTIONAL: std::lock_guard<std::mutex> lock(m_mutex);
        cleanupExpired();
        std::list<Key> order;
        for (const auto& pair : m_list) {
            order.push_back(pair.first.first);
        }
        return order;
    }

private:
    struct CacheMetadata {
        std::chrono::steady_clock::time_point timestamp;
    };

    bool isExpired(const std::chrono::steady_clock::time_point& timestamp) const {
        if (m_ttl.count() == 0) return false;
        auto now = std::chrono::steady_clock::now();
        return (now - timestamp) > m_ttl;
    }

    void cleanupExpired() const {
        if (m_ttl.count() == 0) return;

        auto it = m_list.begin();
        while (it != m_list.end()) {
            if (isExpired(it->second.timestamp)) {
                m_map.erase(it->first.first);
                it = m_list.erase(it);
            } else {
                ++it;
            }
        }
    }

    std::size_t m_capacity;
    std::chrono::milliseconds m_ttl;
    // mutable because cleanupExpired() modifies state in const methods
    mutable std::list<std::pair<std::pair<Key, Value>, CacheMetadata>> m_list;
    mutable std::unordered_map<Key, typename std::list<std::pair<std::pair<Key, Value>, CacheMetadata>>::iterator> m_map;
    // OPTIONAL: mutable std::mutex m_mutex;
};