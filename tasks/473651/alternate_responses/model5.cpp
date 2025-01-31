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
    size_t frequency;
    BufNode(const Key& k, const Value& v)
        : key(k), value(v), frequency(1) {}
};

template <typename Key, typename Value>
struct NodeInfo
{
    typename std::list<BufNode<Key, Value>>::iterator iter;
    size_t& frequency;
    
    NodeInfo(typename std::list<BufNode<Key, Value>>::iterator it)
        : iter(it), frequency(it->frequency) {}
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
            it->second.iter->value = value;
            it->second.frequency++;
            return;
        }

        if (size() >= m_capacity) {
            pop_least_used(1);
        }

        m_data.emplace_front(key, value);
        m_map[key] = NodeInfo(m_data.begin());
    }

    const Value& get(const Key& key)
    {
        auto it = m_map.find(key);
        if (it == m_map.end()) {
            throw std::runtime_error("Key not found in FreqBuffer");
        }
        it->second.frequency++;
        return it->second.iter->value;
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

        std::vector<std::pair<Key, Value>> result;
        std::vector<BufNode<Key, Value>> temp;
        
        for (const auto& node : m_data) {
            temp.push_back(node);
        }

        std::sort(temp.begin(), temp.end(), 
            [](const BufNode<Key, Value>& a, const BufNode<Key, Value>& b) {
                if (a.frequency != b.frequency) {
                    return a.frequency > b.frequency;
                }
                return a.value < b.value;
            });

        for (size_t i = 0; i < k && i < temp.size(); ++i) {
            result.emplace_back(temp[i].key, temp[i].value);
        }

        return result;
    }

    void pop_least_used(size_t n)
    {
        if (n > size()) {
            n = size();
        }

        std::vector<std::pair<size_t, typename std::list<BufNode<Key, Value>>::iterator>> freq_items;
        for (auto it = m_data.begin(); it != m_data.end(); ++it) {
            freq_items.emplace_back(it->frequency, it);
        }

        std::sort(freq_items.begin(), freq_items.end());

        for (size_t i = 0; i < n; ++i) {
            auto it = freq_items[i].second;
            m_map.erase(it->key);
            m_data.erase(it);
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
    std::unordered_map<Key, NodeInfo<Key, Value>> m_map;
};

#endif //__TEST_CLASS__