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
        if (buffers.empty()) {
            return CacheBuffer<Key, Value>(totalCapacity > 0 ? totalCapacity : 1);
        }

        // If totalCapacity is 0, use sum of all buffer capacities
        if (totalCapacity == 0) {
            totalCapacity = 0;
            for (const auto& buffer : buffers) {
                totalCapacity += buffer.capacity();
            }
        }

        CacheBuffer<Key, Value> result(totalCapacity);
        std::unordered_map<Key, std::pair<Value, size_t>> merged;

        // Merge all buffers, summing frequencies for duplicate keys
        for (const auto& buffer : buffers) {
            for (const auto& node : buffer.m_data) {
                auto [it, inserted] = merged.try_emplace(
                    node.key,
                    node.value,
                    node.frequency
                );
                if (!inserted) {
                    it->second.second += node.frequency;
                }
            }
        }

        // Sort by frequency and insert into result buffer
        std::vector<std::pair<Key, std::pair<Value, size_t>>> sortedItems(
            merged.begin(), merged.end());
        
        std::sort(sortedItems.begin(), sortedItems.end(),
            [](const auto& a, const auto& b) {
                return a.second.second > b.second.second;
            });

        for (const auto& item : sortedItems) {
            if (result.size() >= totalCapacity) break;
            result.m_data.emplace_back(
                item.first, item.second.first, item.second.second);
            result.m_map[item.first] = std::prev(result.m_data.end());
        }

        return result;
    }

    void backup() {
        m_backup_data = m_data;
        m_backup_map.clear();
        
        // Rebuild the backup map with new iterators
        for (auto it = m_backup_data.begin(); it != m_backup_data.end(); ++it) {
            m_backup_map[it->key] = it;
        }
    }

    void restore() {
        if (!m_backup_data.size()) {
            throw std::runtime_error("No backup data available");
        }

        m_data = m_backup_data;
        m_map.clear();
        
        // Rebuild the current map with new iterators
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

private:
    std::size_t m_capacity;
    std::list<BufNode<Key, Value>> m_data;
    std::unordered_map<Key, typename std::list<BufNode<Key, Value>>::iterator> m_map;
    
    // Backup storage
    std::list<BufNode<Key, Value>> m_backup_data;
    std::unordered_map<Key, typename std::list<BufNode<Key, Value>>::iterator> m_backup_map;
};

#endif // __CACHE_BUFFER__
