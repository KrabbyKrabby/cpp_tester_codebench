#ifndef __TEST_CLASS__
#define __TEST_CLASS__

#include <unordered_map>
#include <map>
#include <list>
#include <stdexcept>
#include <cstddef>
#include <queue>
#include <algorithm> 
#include <vector>

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
        if (m_capacity <= 0) return;

        auto it = m_map.find(key);
        if (it != m_map.end()) {
            it->second->value = value;
            it->second->frequency++;
            // Move to the front for better access locality
            m_data.splice(m_data.begin(), m_data, it->second);
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

    void resize(int newCapacity)
    {
        if (newCapacity < 0) {
            throw std::runtime_error("New capacity cannot be negative");
        }
        
        while (size() > newCapacity) {
            pop_least_used(1);
        }
        m_capacity = newCapacity;
    }

   std::vector<std::pair<Key, Value>> getTopKFrequent(std::size_t k)
    {
        if (k > size()) {
            k = size();
        }

        // Custom comparator for the priority queue
        auto comp = [](const BufNode<Key, Value>& a, const BufNode<Key, Value>& b) {
            if (a.frequency == b.frequency) {
                return a.value < b.value;
            }
            return a.frequency > b.frequency;
        };

        std::vector<BufNode<Key, Value>> nodes;
        for (const auto& node : m_data) {
            nodes.push_back(node);
        }

        std::sort(nodes.begin(), nodes.end(), comp);

        // Prepare the result in the correct order
        std::vector<std::pair<Key, Value>> result;
        for (size_t i = 0; i < k; ++i) {
            result.emplace_back(nodes[i].key, nodes[i].value);
        }
        return result;
    }

    void pop_least_used(int n)
    {
        if (n <= 0) return;
        if (n > static_cast<int>(size())) {
            n = size();
        }

        std::vector<typename std::list<BufNode<Key, Value>>::iterator> to_remove;
        for (auto it = m_data.begin(); it != m_data.end(); ++it) {
            to_remove.push_back(it);
        }

        std::sort(to_remove.begin(), to_remove.end(),
            [](const auto& a, const auto& b) {
                if (a->frequency == b->frequency) {
                    return a->value > b->value;
                }
                return a->frequency > b->frequency;
            });

        for (int i = 0; i < n && !to_remove.empty(); ++i) {
            auto it = to_remove[to_remove.size() - 1];
            m_map.erase(it->key);
            m_data.erase(it);
            to_remove.pop_back();
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
