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
    std::size_t frequency;
    BufNode(const Key& k, const Value& v)
        : key(k), value(v), frequency(1) {}
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
            it->second->frequency++;
            return;
        }

        if (m_map.size() >= m_capacity) {
            // Remove the least frequently used item
            auto leastFrequent = std::min_element(m_data.begin(), m_data.end(),
                [](const BufNode<Key, Value>& a, const BufNode<Key, Value>& b) {
                    return a.frequency < b.frequency;
                });
            m_map.erase(leastFrequent->key);
            m_data.erase(m_data.iterator_to(*leastFrequent));
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

        it->second->frequency++;
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
            throw std::invalid_argument("New capacity must be >= 0");
        }

        if (newCapacity == 0) {
            m_capacity = 0;
            m_data.clear();
            m_map.clear();
            return;
        }

        if (newCapacity < m_capacity) {
            while (m_map.size() > newCapacity) {
                // Remove the least frequently used item
                auto leastFrequent = std::min_element(m_data.begin(), m_data.end(),
                    [](const BufNode<Key, Value>& a, const BufNode<Key, Value>& b) {
                        return a.frequency < b.frequency;
                    });
                m_map.erase(leastFrequent->key);
                m_data.erase(m_data.iterator_to(*leastFrequent));
            }
        }

        m_capacity = newCapacity;
    }

    std::vector<std::pair<Key, Value>> getTopKFrequent(std::size_t k)
    {
        if (k > m_map.size()) {
            throw std::invalid_argument("k must be <= size()");
        }

        std::vector<std::pair<Key, Value>> topKFrequent;
        std::vector<BufNode<Key, Value>> nodes(m_data.begin(), m_data.end());
        std::sort(nodes.begin(), nodes.end(),
            [](const BufNode<Key, Value>& a, const BufNode<Key, Value>& b) {
                if (a.frequency == b.frequency) {
                    return a.value < b.value;
                }
                return a.frequency > b.frequency;
            });

        for (std::size_t i = 0; i < k; ++i) {
            topKFrequent.emplace_back(nodes[i].key, nodes[i].value);
        }

        return topKFrequent;
    }

    void pop_least_used(std::size_t n)
    {
        if (n > m_map.size()) {
            throw std::invalid_argument("n must be <= size()");
        }

        for (std::size_t i = 0; i < n; ++i) {
            // Remove the least frequently used item
            auto leastFrequent = std::min_element(m_data.begin(), m_data.end(),
                [](const BufNode<Key, Value>& a, const BufNode<Key, Value>& b) {
                    return a.frequency < b.frequency;
                });
            m_map.erase(leastFrequent->key);
            m_data.erase(m_data.iterator_to(*leastFrequent));
        }
    }

private:
    std::size_t m_capacity;

    // This list holds BufNode in no particular order (just the order inserted).
    std::list<BufNode<Key, Value>> m_data;

    // Key -> iterator in m_data
    std::unordered_map<Key, typename std::list<BufNode<Key, Value>>::iterator> m_map;
};

#endif //__TEST_CLASS__
