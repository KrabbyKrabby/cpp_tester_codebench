#ifndef __TEST_CLASS__
#define __TEST_CLASS__

#include <unordered_map>
#include <list>
#include <vector>
#include <stdexcept>
#include <cstddef>
#include <algorithm>
#include <utility>

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
            m_freq[key]++;
            return;
        }

        if (size() >= m_capacity) {
            pop_least_used(1);
        }

        m_data.emplace_front(key, value);
        m_map[key] = m_data.begin();
        m_freq[key] = 1;
    }

    const Value& get(const Key& key)
    {
        auto it = m_map.find(key);
        if (it == m_map.end()) {
            throw std::runtime_error("Key not found in FreqBuffer");
        }
        m_freq[key]++;
        return it->second->value;
    }

    void resize(std::size_t newCapacity)
    {
        if (newCapacity < 0) {
            throw std::invalid_argument("New capacity cannot be negative");
        }
        
        if (newCapacity < size()) {
            std::size_t to_remove = size() - newCapacity;
            pop_least_used(to_remove);
        }
        m_capacity = newCapacity;
    }

    std::vector<std::pair<Key, Value>> getTopKFrequent(std::size_t k)
    {
        if (k > size()) {
            k = size();
        }

        std::vector<std::pair<Key, Value>> result;
        for (const auto& pair : m_map) {
            result.emplace_back(pair.first, pair.second->value);
        }

        std::sort(result.begin(), result.end(),
            [this](const auto& a, const auto& b) {
                if (m_freq[a.first] != m_freq[b.first]) {
                    return m_freq[a.first] > m_freq[b.first];
                }
                return a.second < b.second;
            });

        result.resize(k);
        return result;
    }

    void pop_least_used(std::size_t n)
    {
        if (n > size()) {
            n = size();
        }

        std::vector<std::pair<Key, std::size_t>> freq_pairs;
        for (const auto& pair : m_freq) {
            freq_pairs.emplace_back(pair.first, pair.second);
        }

        std::sort(freq_pairs.begin(), freq_pairs.end(),
            [](const auto& a, const auto& b) {
                return a.second < b.second;
            });

        for (std::size_t i = 0; i < n; ++i) {
            const Key& key = freq_pairs[i].first;
            auto map_it = m_map.find(key);
            if (map_it != m_map.end()) {
                m_data.erase(map_it->second);
                m_map.erase(map_it);
                m_freq.erase(key);
            }
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
    std::unordered_map<Key, std::size_t> m_freq;
};

#endif //__TEST_CLASS__
