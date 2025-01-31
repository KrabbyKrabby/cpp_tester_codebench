#ifndef __CACHE_BUFFER__
#define __CACHE_BUFFER__

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
class CacheBuffer
{
public:
    explicit CacheBuffer(std::size_t capacity)
        : m_capacity(capacity)
    {
        if (capacity == 0) {
            throw std::invalid_argument("CacheBuffer capacity must be > 0");
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
            evictNodes(1);
        }

        m_data.emplace_front(key, value);
        m_map[key] = m_data.begin();
    }

    const Value& get(const Key& key)
    {
        auto it = m_map.find(key);
        if (it == m_map.end()) {
            throw std::runtime_error("Key not found in CacheBuffer");
        }
        it->second->frequency++;
        return it->second->value;
    }

    void evictNodes(int n)
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

    static CacheBuffer<Key, Value> mergeBuffers(const std::vector<CacheBuffer<Key, Value>>& buffers, size_t totalCapacity)
    {
        if (totalCapacity == 0) {
            totalCapacity = 0;
            for (const auto& buffer : buffers) {
                totalCapacity += buffer.capacity();
            }
        }

        CacheBuffer<Key, Value> mergedBuffer(totalCapacity);
        std::unordered_map<Key, size_t> frequencyMap;

        for (const auto& buffer : buffers) {
            for (const auto& node : buffer.m_data) {
                frequencyMap[node.key] += node.frequency;
            }
        }

        std::vector<std::pair<Key, size_t>> sortedFrequencies;
        for (const auto& pair : frequencyMap) {
            sortedFrequencies.emplace_back(pair.first, pair.second);
        }

        std::sort(sortedFrequencies.begin(), sortedFrequencies.end(),
            [](const auto& a, const auto& b) {
                return a.second > b.second;
            });

        for (const auto& pair : sortedFrequencies) {
            const Key& key = pair.first;
            size_t frequency = pair.second;

            // Find the value from the original buffers
            Value value;
            for (const auto& buffer : buffers) {
                auto it = buffer.m_map.find(key);
                if (it != buffer.m_map.end()) {
                    value = it->second->value;
                    break;
                }
            }

            mergedBuffer.put(key, value);
            // Manually set the frequency since put will set it to 1
            mergedBuffer.m_map[key]->frequency = frequency;

            if (mergedBuffer.size() >= mergedBuffer.capacity()) {
                break;
            }
        }

        return mergedBuffer;
    }

    void backup()
    {
        m_backup = *this;
    }

    void restore()
    {
        if (!m_backup.m_map.size()) {
            throw std::runtime_error("No backup data available");
        }

        *this = m_backup;
    }

private:
    std::size_t m_capacity;
    std::list<BufNode<Key, Value>> m_data;
    std::unordered_map<Key, typename std::list<BufNode<Key, Value>>::iterator> m_map;
    CacheBuffer m_backup;
};

#endif // __CACHE_BUFFER__
