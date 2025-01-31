#include <unordered_map>
#include <list>
#include <stdexcept>
#include <string>
#include <vector>
#include <mutex>
#include <chrono>
#include <thread>
#include <condition_variable>

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
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_list.size();
    }

    // Return the maximum capacity of the cache
    std::size_t capacity() const {
        return m_capacity;
    }

    // For debugging: returns a list of keys in most-recent to least-recent order
    // (Warning: exposes internal order - for test or debugging only)
    std::list<Key> getUsageOrder() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::list<Key> order;
        for (auto &pair : m_list) {
            order.push_back(pair.first);
        }
        return order;
    }

    // Retrieves the value associated with 'key'.
    // Throws a runtime error if key not found.
    Value& get(const Key& key) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_map.find(key);
        if (it == m_map.end()) {
            throw std::runtime_error("Key not found");
        }
        auto listIt = it->second;
        if (m_ttl != std::chrono::milliseconds::zero()) {
            auto now = std::chrono::steady_clock::now();
            if (listIt->second.expirationTime < now) {
                m_list.erase(listIt);
                m_map.erase(it);
                throw std::runtime_error("Key has expired");
            }
        }
        m_list.splice(m_list.begin(), m_list, listIt);
        return listIt->second.value;
    }

    // Store a key-value pair in the cache (at the front).
    // If the cache is full, evict the LRU entry.
    // If key already exists, move it to the front (most recent).
    void put(const Key& key, const Value& value) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_map.find(key);
        if (it != m_map.end()) {
            auto listIt = it->second;
            m_list.erase(listIt);
        } else if (m_list.size() >= m_capacity) {
            m_map.erase(m_list.back().first);
            m_list.pop_back();
        }
        auto now = std::chrono::steady_clock::now();
        auto expirationTime = m_ttl != std::chrono::milliseconds::zero() ? now + m_ttl : std::chrono::steady_clock::time_point::max();
        m_list.emplace_front(key, Entry{value, expirationTime});
        m_map[key] = m_list.begin();
    }

    // Set time-to-live for all newly inserted or updated keys
    void setExpirationTime(std::chrono::milliseconds ttl) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_ttl = ttl;
    }

    // Insert or update a batch of key-value pairs in one shot
    void putAll(const std::vector<std::pair<Key, Value>>& entries) {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (const auto& entry : entries) {
            put(entry.first, entry.second);
        }
    }

private:
    // capacity of the cache
    std::size_t m_capacity;

    // doubly-linked list of <key, value> pairs
    // most recently used at front; least recently used at back
    std::list<std::pair<Key, Entry>> m_list;

    // maps keys to the corresponding list iterator
    std::unordered_map<Key, typename std::list<std::pair<Key, Entry>>::iterator> m_map;

    // time-to-live for all newly inserted or updated keys
    std::chrono::milliseconds m_ttl = std::chrono::milliseconds::zero();

    // mutex for thread-safety
    mutable std::mutex m_mutex;

    // Entry structure to store value and expiration time
    struct Entry {
        Value value;
        std::chrono::steady_clock::time_point expirationTime;
    };
};
