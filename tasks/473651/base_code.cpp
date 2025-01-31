#ifndef __TEST_CLASS__
#define __TEST_CLASS__

#include <unordered_map>
#include <list>
#include <stdexcept>
#include <cstddef>

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

private:
    std::size_t m_capacity;

    // This list holds BufNode in no particular order (just the order inserted).
    std::list<BufNode<Key, Value>> m_data;

    // Key -> iterator in m_data
    std::unordered_map<Key, typename std::list<BufNode<Key, Value>>::iterator> m_map;
};


#endif //__TEST_CLASS__