#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>

template<typename Key, typename Value>
class LRUCache {
public:
    // Constructor: capacity is the maximum number of key-value pairs stored.
    explicit LRUCache(std::size_t capacity)
        : m_capacity(capacity), m_ttl(0)
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

    // Retrieves the value associated with 'key'. 
    // Throws a runtime error  if key not found.
    Value& get(const Key& key) {
        // Check if key exists
        if (m_map.find(key) == m_map.end()) {
            throw std::runtime_error("Key not found in cache");
        }

        // Check if key has expired
        if (m_ttl != 0 && m_expirationTimes[key] < std::chrono::system_clock::now()) {
            // Remove expired key
            m_list.erase(m_map[key]);
            m_map.erase(key);
            m_expirationTimes.erase(key);
            throw std::runtime_error("Key has expired");
        }

        // Move key to front of list
        auto it = m_map[key];
        m_list.splice(m_list.begin(), m_list, it);

        // Update expiration time
        if (m_ttl != 0) {
            m_expirationTimes[key] = std::chrono::system_clock::now() + m_ttl;
        }

        return m_list.front().second;
    }

    // Store a key-value pair in the cache (at the front). 
    // If the cache is full, evict the LRU entry.
    // If key already exists, move it to the front (most recent).
    void put(const Key& key, const Value& value) {
        // Check if key already exists
        if (m_map.find(key) != m_map.end()) {
            // Remove existing key
            m_list.erase(m_map[key]);
            m_map.erase(key);
            m_expirationTimes.erase(key);
        }

        // Check if cache is full
        if (m_list.size() >= m_capacity) {
            // Evict LRU entry
            m_map.erase(m_list.back().first);
            m_expirationTimes.erase(m_list.back().first);
            m_list.pop_back();
        }

        // Add new key-value pair
        m_list.push_front(std::make_pair(key, value));
        m_map[key] = m_list.begin();

        // Set expiration time
        if (m_ttl != 0) {
            m_expirationTimes[key] = std::chrono::system_clock::now() + m_ttl;
        }
    }

    // Set time-based expiration
    void setExpirationTime(std::chrono::milliseconds ttl) {
        m_ttl = ttl;
    }

    // Bulk insertion
    void putAll(const std::vector<std::pair<Key, Value>>& entries) {
        for (const auto& entry : entries) {
            put(entry.first, entry.second);
        }
    }

private:
    // capacity of the cache
    std::size_t m_capacity;

    // doubly-linked list of <key, value> pairs
    // most recently used at front; least recently used at back
    std::list<std::pair<Key, Value>> m_list;

    // maps keys to the corresponding list iterator
    std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator> m_map;

    // maps keys to expiration times
    std::unordered_map<Key, std::chrono::system_clock::time_point> m_expirationTimes;

    // time-based expiration time
    std::chrono::milliseconds m_ttl;

    // mutex for thread safety
    std::mutex m_mutex;
};