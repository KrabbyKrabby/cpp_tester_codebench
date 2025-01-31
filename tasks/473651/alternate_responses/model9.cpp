#ifndef __TEST_CLASS__
#define __TEST_CLASS__

#include <unordered_map>
#include <list>
#include <stdexcept>
#include <cstddef>
#include <vector>
#include <algorithm>

template <typename Key, typename Value>
struct BufNode
{
    Key key;
    Value value;
    BufNode(const Key& k, const Value& v)
        : key(k), value(v) {}
};

template <typename Key, typename Value>
class FreqBuffer
{
public:
    explicit FreqBuffer(std::size_t capacity)
        : m_capacity(capacity)
    {
        if (capacity == 0) {
            throw std::invalid_argument("FreqBuffer capacity must be > 0");
        }
    }

    void put(const Key& key, const Value& value)
    {
        auto it = m_map.find(key);
        if (it != m_map.end()) {
            it->second->value = value;
            return;
        }

        m_data.emplace_front(key, value);
        // Link the key -> node in map
        m_map[key] = m_data.begin();
    }

    const Value& get(const Key& key)
    {
        auto it = m_map.find(key);
        if (it == m_map.end()) {
            throw std::runtime_error("Key not found in FreqBuffer");
        }

        return it->second->value;
    }

    std::size_t size() const
    {
        return m_map.size();
    }

    std::size_t capacity() const
    {
        return m_capacity;
    }

    void resize(std::size_t newCapacity)
    {
        if (newCapacity < 0) {
            throw std::invalid_argument("newCapacity must be >= 0");
        }

        if (newCapacity == 0) {
            m_capacity = 0;
            m_map.clear();
            m_data.clear();
            return;
        }

        if (newCapacity < size()) {
            while (size() > newCapacity) {
                pop_least_used(1);
            }
        }

        m_capacity = newCapacity;
    }

    std::vector<std::pair<Key, Value>> getTopKFrequent(std::size_t k)
    {
        std::vector<std::pair<Key, Value>> result;
        std::map<std::size_t, std::vector<std::pair<Key, Value>>> freqMap;

        for (auto& node : m_data) {
            auto freq = m_freqMap[node.key]++;
            freqMap[freq].emplace_back(node.key, node.value);
        }

        for (auto it = freqMap.rbegin(); it != freqMap.rend(); ++it) {
            for (auto& pair : it->second) {
                result.push_back(pair);
                if (result.size() == k) {
                    return result;
                }
            }
        }

        return result;
    }

    void pop_least_used(std::size_t n)
    {
        if (n > size()) {
            throw std::invalid_argument("n must be <= size()");
        }

        std::map<std::size_t, std::vector<std::pair<Key, Value>>> freqMap;

        for (auto& node : m_data) {
            auto freq = m_freqMap[node.key]++;
            freqMap[freq].emplace_back(node.key, node.value);
        }

        for (auto it = freqMap.begin(); it != freqMap.end(); ++it) {
            for (auto& pair : it->second) {
                if (n > 0) {
                    m_map.erase(pair.first);
                    m_data.remove(pair.first);
                    n--;
                } else {
                    return;
                }
            }
        }
    }

private:
    std::size_t m_capacity;

    // This list holds BufNode in no particular order (just the order inserted).
    std::list<BufNode<Key, Value>> m_data;

    // Key -> iterator in m_data
    std::unordered_map<Key, typename std::list<BufNode<Key, Value>>::iterator> m_map;

    // Key -> frequency
    std::unordered_map<Key, std::size_t> m_freqMap;
};


#endif //__TEST_CLASS__
