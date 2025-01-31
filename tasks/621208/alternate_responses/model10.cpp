#include <unordered_map>
#include <list>
#include <stdexcept>
#include <string>
#include <vector>
#include <chrono>
#include <mutex>
#include <shared_mutex>

template<typename Key, typename Value>
class LRUCache {
public:
    // Constructor: capacity is the maximum number of key-value pairs stored.
    explicit LRUCache(std::size_t capacity)
        : m_capacity(capacity), m_ttl(0ms)
    {
        if (capacity == 0) {
            throw std::invalid_argument("LRUCache capacity must be > 0");
        }
    }

    // Return how many items are currently in the cache
    std::size_t size() const {
        std::shared_lock<std::shared_mutex> lock(m_mutex);
        return m_list.size();
    }

    // Return the maximum capacity of the cache
    std::size_t capacity() const {
        return m_capacity;
    }

    // For debugging: returns a list of keys in most-recent to least-recent order
    // (Warning: exposes internal order - for test or debugging only)
    std::list<Key> getUsageOrder() const {
        std::shared_lock<std::shared_mutex> lock(m_mutex);
        std::list<Key> order;
        for (auto &pair : m_list) {
            order.push_back(pair.first);
        }
        return order;
    }

    // Retrieves the value associated with 'key'. 
    // Throws a runtime error  if key not found.
    Value& get(const Key& key) {
        std::unique_lock<std::shared_mutex> lock(m_mutex);
        auto it = m_map.find(key);
        if (it == m_map.end()) {
            throw std::runtime_error("Key not found in cache");
        }

        auto& entry = *it->second;
        if (m_ttl.count() > 0 && entry.second + m_ttl < std::chrono::system_clock::now()) {
            // Key has expired, remove it
            m_list.erase(it->second);
            m_map.erase(it);
            throw std::runtime_error("Key has expired");
        }

        // Move the accessed key to the front of the list
        m_list.splice(m_list.begin(), m_list, it->second);
        return m_list.front().second;
    }

    // Store a key-value pair in the cache (at the front). 
    // If the cache is full, evict the LRU entry.
    // If key already exists, move it to the front (most recent).
    void put(const Key& key, const Value& value) {
        std::unique_lock<std::shared_mutex> lock(m_mutex);
        auto it = m_map.find(key);
        if (it != m_map.end()) {
            // Key already exists, update its value and move it to the front
            it->second->second = value;
            m_list.splice(m_list.begin(), m_list, it->second);
        } else {
            // Add new key-value pair to the front of the list
            m_list.emplace_front(key, value);
            m_map[key] = m_list.begin();

            // Check for capacity-based eviction
            if (m_list.size() > m_capacity) {
                // Evict the LRU entry
                m_map.erase(m_list.back().first);
                m_list.pop_back();
            }
        }

        // Update expiration time if ttl is set
        if (m_ttl.count() > 0) {
            m_list.front().second = std::chrono::system_clock::now() + m_ttl;
        }
    }

    // Set expiration time for all keys in the cache
    void setExpirationTime(std::chrono::milliseconds ttl) {
        std::unique_lock<std::shared_mutex> lock(m_mutex);
        m_ttl = ttl;
        if (m_ttl.count() > 0) {
            // Update expiration time for all existing keys
            for (auto& entry : m_list) {
                entry.second = std::chrono::system_clock::now() + m_ttl;
            }
        }
    }

    // Inserts or updates a batch of key-value pairs in one shot
    void putAll(const std::vector<std::pair<Key, Value>>& entries) {
        std::unique_lock<std::shared_mutex> lock(m_mutex);
        for (const auto& entry : entries) {
            put(entry.first, entry.second);
        }
    }

private:
    // capacity of the cache
    std::size_t m_capacity;

    // doubly-linked list of <key, value> pairs
    // most recently used at front; least recently used at back
    std::list<std::pair<Key, std::pair<Value, std::chrono::system_clock::time_point>>> m_list;

    // maps keys to the corresponding list iterator
    std::unordered_map<Key, typename std::list<std::pair<Key, std::pair<Value, std::chrono::system_clock::time_point>>>::iterator> m_map;

    // time to live for each key
    std::chrono::milliseconds m_ttl;

    // mutex for thread safety
    mutable std::shared_mutex m_mutex;
};
