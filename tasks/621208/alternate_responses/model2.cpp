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
private:
    struct CacheEntry {
        Key key;
        Value value;
        std::optional<std::chrono::steady_clock::time_point> expirationTime;
        
        CacheEntry(const Key& k, const Value& v) 
            : key(k), value(v), expirationTime(std::nullopt) {}
    };

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

        if (isExpired(it->second)) {
            m_map.erase(it);
            m_list.erase(it->second);
            throw std::runtime_error("Key has expired");
        }

        // Move to front (most recently used)
        m_list.splice(m_list.begin(), m_list, it->second);
        
        // Reset expiration if TTL is enabled
        if (m_ttl.count() > 0) {
            it->second->expirationTime = std::chrono::steady_clock::now() + m_ttl;
        }

        return it->second->value;
    }

    void put(const Key& key, const Value& value) {
        // OPTIONAL: std::lock_guard<std::mutex> lock(m_mutex);

        auto it = m_map.find(key);
        if (it != m_map.end()) {
            // Key exists, check if expired
            if (isExpired(it->second)) {
                m_map.erase(it);
                m_list.erase(it->second);
            } else {
                // Update existing entry
                it->second->value = value;
                m_list.splice(m_list.begin(), m_list, it->second);
                if (m_ttl.count() > 0) {
                    it->second->expirationTime = std::chrono::steady_clock::now() + m_ttl;
                }
                return;
            }
        }

        // Add new entry
        if (m_list.size() >= m_capacity) {
            // Remove least recently used
            const Key& lruKey = m_list.back().key;
            m_map.erase(lruKey);
            m_list.pop_back();
        }

        m_list.emplace_front(CacheEntry(key, value));
        if (m_ttl.count() > 0) {
            m_list.front().expirationTime = std::chrono::steady_clock::now() + m_ttl;
        }
        m_map[key] = m_list.begin();
    }

    void setExpirationTime(std::chrono::milliseconds ttl) {
        // OPTIONAL: std::lock_guard<std::mutex> lock(m_mutex);
        m_ttl = ttl;
        if (ttl.count() > 0) {
            cleanExpired();
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
        cleanExpired();
        return m_list.size();
    }

    std::size_t capacity() const {
        return m_capacity;
    }

    std::list<Key> getUsageOrder() const {
        // OPTIONAL: std::lock_guard<std::mutex> lock(m_mutex);
        cleanExpired();
        std::list<Key> order;
        for (const auto& entry : m_list) {
            order.push_back(entry.key);
        }
        return order;
    }

private:
    bool isExpired(typename std::list<CacheEntry>::iterator it) const {
        if (m_ttl.count() == 0 || !it->expirationTime) {
            return false;
        }
        return std::chrono::steady_clock::now() > *it->expirationTime;
    }

    void cleanExpired() const {
        if (m_ttl.count() == 0) {
            return;
        }

        auto it = m_list.begin();
        while (it != m_list.end()) {
            if (isExpired(it)) {
                m_map.erase(it->key);
                it = m_list.erase(it);
            } else {
                ++it;
            }
        }
    }

    std::size_t m_capacity;
    std::chrono::milliseconds m_ttl;
    mutable std::list<CacheEntry> m_list;
    mutable std::unordered_map<Key, typename std::list<CacheEntry>::iterator> m_map;
    // OPTIONAL: mutable std::mutex m_mutex;
};
