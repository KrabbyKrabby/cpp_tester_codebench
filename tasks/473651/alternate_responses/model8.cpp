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
        : key(k), value(v), frequency(0) {}
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
            m_data.clear();
            m_map.clear();
            m_capacity = 0;
            return;
        }

        while (size() > newCapacity) {
            // Find the least frequently used item
            auto leastFrequent = std::min_element(m_data.begin(), m_data.end(),
                [](const BufNode<Key, Value>& a, const BufNode<Key, Value>& b) {
                    return a.frequency < b.frequency;
                });

            // Remove the least frequently used item
            m_map.erase(leastFrequent->key);
            m_data.erase(m_data.iterator_to(*leastFrequent));
        }

        m_capacity = newCapacity;
    }

    std::vector<std::pair<Key, Value>> getTopKFrequent(std::size_t k)
    {
        std::vector<std::pair<Key, Value>> result;
        std::vector<BufNode<Key, Value>> dataCopy = std::vector<BufNode<Key, Value>>(m_data.begin(), m_data.end());

        // Sort the data in descending frequency order and ascending value order
        std::sort(dataCopy.begin(), dataCopy.end(),
            [](const BufNode<Key, Value>& a, const BufNode<Key, Value>& b) {
                if (a.frequency == b.frequency) {
                    return a.value < b.value;
                }
                return a.frequency > b.frequency;
            });

        for (std::size_t i = 0; i < k && i < dataCopy.size(); ++i) {
            result.push_back(std::make_pair(dataCopy[i].key, dataCopy[i].value));
        }

        return result;
    }

    void pop_least_used(std::size_t n)
    {
        while (n > 0 && !m_data.empty()) {
            // Find the least frequently used item
            auto leastFrequent = std::min_element(m_data.begin(), m_data.end(),
                [](const BufNode<Key, Value>& a, const BufNode<Key, Value>& b) {
                    return a.frequency < b.frequency;
                });

            // Remove the least frequently used item
            m_map.erase(leastFrequent->key);
            m_data.erase(m_data.iterator_to(*leastFrequent));
            --n;
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
