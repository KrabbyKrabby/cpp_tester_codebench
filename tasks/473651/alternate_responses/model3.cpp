#ifndef __TEST_CLASS__
#define __TEST_CLASS__

#include <unordered_map>
#include <list>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cstddef>
#include <queue>

template <typename Key, typename Value>
struct BufNode
{
    Key key;
    Value value;
    size_t frequency;
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

        if (size() >= m_capacity) {
            pop_least_used(1);
        }

        m_data.emplace_front(key, value);
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

    void resize(std::size_t newCapacity)
    {
        if (newCapacity < 0) {
            throw std::invalid_argument("New capacity cannot be negative");
        }
        
        while (size() > newCapacity) {
            pop_least_used(1);
        }
        m_capacity = newCapacity;
    }

    std::vector<std::pair<Key, Value>> getTopKFrequent(size_t k)
    {
        if (k > size()) {
            k = size();
        }

        std::vector<typename std::list<BufNode<Key, Value>>::iterator> nodes;
        for (auto it = m_data.begin(); it != m_data.end(); ++it) {
            nodes.push_back(it);
        }

        std::sort(nodes.begin(), nodes.end(),
            [](const auto& a, const auto& b) {
                if (a->frequency != b->frequency) {
                    return a->frequency > b->frequency;
                }
                return a->value < b->value;
            });

        std::vector<std::pair<Key, Value>> result;
        for (size_t i = 0; i < k; ++i) {
            result.emplace_back(nodes[i]->key, nodes[i]->value);
        }
        return result;
    }

    void pop_least_used(size_t n)
    {
        if (n > size()) {
            n = size();
        }

        std::vector<typename std::list<BufNode<Key, Value>>::iterator> nodes;
        for (auto it = m_data.begin(); it != m_data.end(); ++it) {
            nodes.push_back(it);
        }

        std::sort(nodes.begin(), nodes.end(),
            [](const auto& a, const auto& b) {
                if (a->frequency != b->frequency) {
                    return a->frequency < b->frequency;
                }
                return a->value > b->value;
            });

        for (size_t i = 0; i < n; ++i) {
            m_map.erase(nodes[i]->key);
            m_data.erase(nodes[i]);
        }
    }

    std::size_t size() const
    {
        return m_map.size();
    }

    std::size_t capacity() const
    {
        return m_capacity;
    }

private:
    std::size_t m_capacity;
    std::list<BufNode<Key, Value>> m_data;
    std::unordered_map<Key, typename std::list<BufNode<Key, Value>>::iterator> m_map;
};

#endif //__TEST_CLASS__
