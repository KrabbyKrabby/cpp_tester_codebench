#ifndef __TEST_CLASS__
#define __TEST_CLASS__

#include <unordered_map>
#include <list>
#include <stdexcept>
#include <cstddef>
#include <vector>
#include <queue>

template <typename Key, typename Value>
struct BufNode
{
    Key key;
    Value value;
    std::size_t freq;
    BufNode(const Key& k, const Value& v)
        : key(k), value(v), freq(0) {}
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
            it->second->freq++;
            m_freq_map[it->second->freq].push_back(it->second);
            return;
        }

        if (m_map.size() == m_capacity) {
            auto least_freq = m_freq_map.begin()->first;
            auto least_freq_node = m_freq_map.begin()->second.front();
            m_data.erase(m_map[least_freq_node->key]);
            m_map.erase(least_freq_node->key);
            m_freq_map.begin()->second.pop_front();
            if (m_freq_map.begin()->second.empty()) {
                m_freq_map.erase(least_freq);
            }
        }

        m_data.emplace_front(key, value);
        m_data.front().freq++;
        m_map[key] = m_data.begin();
        m_freq_map[m_data.front().freq].push_back(m_data.begin());
    }

    const Value& get(const Key& key)
    {
        auto it = m_map.find(key);
        if (it == m_map.end()) {
            throw std::runtime_error("Key not found in FreqBuffer");
        }

        it->second->freq++;
        auto freq = it->second->freq;
        m_freq_map[freq].push_back(it->second);
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
        if (newCapacity == 0) {
            m_capacity = 0;
            m_map.clear();
            m_data.clear();
            m_freq_map.clear();
            return;
        }

        if (newCapacity < 0) {
            throw std::invalid_argument("New capacity must be >= 0");
        }

        while (m_map.size() > newCapacity) {
            auto least_freq = m_freq_map.begin()->first;
            auto least_freq_node = m_freq_map.begin()->second.front();
            m_data.erase(m_map[least_freq_node->key]);
            m_map.erase(least_freq_node->key);
            m_freq_map.begin()->second.pop_front();
            if (m_freq_map.begin()->second.empty()) {
                m_freq_map.erase(least_freq);
            }
        }

        m_capacity = newCapacity;
    }

    std::vector<std::pair<Key, Value>> getTopKFrequent(std::size_t k)
    {
        std::priority_queue<std::pair<std::size_t, std::pair<Key, Value>>> pq;
        for (auto& node : m_data) {
            pq.push({node.freq, {node.key, node.value}});
        }

        std::vector<std::pair<Key, Value>> result;
        while (k-- > 0 && !pq.empty()) {
            result.push_back(pq.top().second);
            pq.pop();
        }

        return result;
    }

    void pop_least_used(std::size_t n)
    {
        while (n-- > 0 && !m_map.empty()) {
            auto least_freq = m_freq_map.begin()->first;
            auto least_freq_node = m_freq_map.begin()->second.front();
            m_data.erase(m_map[least_freq_node->key]);
            m_map.erase(least_freq_node->key);
            m_freq_map.begin()->second.pop_front();
            if (m_freq_map.begin()->second.empty()) {
                m_freq_map.erase(least_freq);
            }
        }
    }

private:
    std::size_t m_capacity;

    // This list holds BufNode in no particular order (just the order inserted).
    std::list<BufNode<Key, Value>> m_data;

    // Key -> iterator in m_data
    std::unordered_map<Key, typename std::list<BufNode<Key, Value>>::iterator> m_map;

    // Frequency -> list of nodes
    std::map<std::size_t, std::list<typename std::list<BufNode<Key, Value>>::iterator>> m_freq_map;
};

#endif //__TEST_CLASS__
