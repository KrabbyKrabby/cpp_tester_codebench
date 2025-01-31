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
#include <optional>

template <typename Key, typename Value>
struct BufNode
{
    Key key;
    Value value;
    size_t frequency;
    BufNode(const Key& k, const Value& v, size_t f = 1)
        : key(k), value(v), frequency(f) {}
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

    static CacheBuffer<Key, Value> mergeBuffers(
        const std::vector<CacheBuffer<Key, Value>>& buffers,
        size_t totalCapacity)
    {
        // Calculate total capacity if not specified
        if (totalCapacity == 0) {
            totalCapacity = 0;
            for (const auto& buffer : buffers) {
                totalCapacity += buffer.capacity();
            }
        }

        CacheBuffer<Key, Value> mergedBuffer(totalCapacity);
        std::unordered_map<Key, size_t> frequencyMap;
        std::unordered_map<Key, Value> valueMap;

        // Combine frequencies and values
        for (const auto& buffer : buffers) {
            for (const auto& node : buffer.m_data) {
                frequencyMap[node.key] += node.frequency;
                valueMap[node.key] = node.value;
            }
        }

        // Create vector of entries for sorting
        std::vector<std::pair<Key, size_t>> entries;
        for (const auto& [key, freq] : frequencyMap) {
            entries.emplace_back(key, freq);
        }

        // Sort by frequency (highest first)
        std::sort(entries.begin(), entries.end(),
            [](const auto& a, const auto& b) {
                return a.second > b.second;
            });

        // Add entries to merged buffer, respecting capacity
        for (const auto& [key, freq] : entries) {
            if (mergedBuffer.size() >= totalCapacity) break;
            mergedBuffer.m_data.emplace_back(key, valueMap[key], freq);
            auto it = --mergedBuffer.m_data.end();
            mergedBuffer.m_map[key] = it;
        }

        return mergedBuffer;
    }

    void backup()
    {
        m_backup = std::make_optional<BackupState>();
        m_backup->data = m_data;
        m_backup->map.clear();
        
        // Rebuild the map for the backup
        for (auto it = m_backup->data.begin(); it != m_backup->data.end(); ++it) {
            m_backup->map[it->key] = it;
        }
    }

    void restore()
    {
        if (!m_backup) {
            throw std::runtime_error("No backup available to restore");
        }

        m_data = m_backup->data;
        m_map.clear();
        
        // Rebuild the current map
        for (auto it = m_data.begin(); it != m_data.end(); ++it) {
            m_map[it->key] = it;
        }
    }

    void put(const Key& key, const Value& value)
    {
        if (m_capacity <= 0) return;

        auto it = m_map.find(key);
        if (it != m_map.end()) {
            it->second->value = value;
            it->second->frequency++;
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

private:
    struct BackupState {
        std::list<BufNode<Key, Value>> data;
        std::unordered_map<Key, typename std::list<BufNode<Key, Value>>::iterator> map;
    };

    std::size_t m_capacity;
    std::list<BufNode<Key, Value>> m_data;
    std::unordered_map<Key, typename std::list<BufNode<Key, Value>>::iterator> m_map;
    std::optional<BackupState> m_backup;
};

#endif // __CACHE_BUFFER__