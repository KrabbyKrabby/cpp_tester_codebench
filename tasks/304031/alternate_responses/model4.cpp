#include <iostream>
#include <vector>
#include <stack>
#include <shared_mutex>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <atomic>

namespace fs = std::filesystem;

class RedisDataBase {
private:
    std::vector<Node> currentState;
    std::stack<std::vector<Node>> history;
    mutable std::shared_mutex stateMutex;
    std::mutex commitMutex;
    const std::string stateFile = "redis_state.bin";
    const std::string historyFile = "redis_history.bin";
    std::atomic<bool> isInitialized{false};

    void saveStateToDisk() {
        std::ofstream state(stateFile, std::ios::binary);
        std::ofstream hist(historyFile, std::ios::binary);
        
        if (!state || !hist) {
            throw std::runtime_error("Failed to open files for writing");
        }

        // Save current state
        size_t stateSize = currentState.size();
        state.write(reinterpret_cast<char*>(&stateSize), sizeof(size_t));
        for (const auto& node : currentState) {
            state.write(reinterpret_cast<const char*>(&node.value), sizeof(int));
        }

        // Save history
        size_t historySize = history.size();
        hist.write(reinterpret_cast<char*>(&historySize), sizeof(size_t));
        
        std::stack<std::vector<Node>> tempHistory = history;
        while (!tempHistory.empty()) {
            const auto& state = tempHistory.top();
            size_t vectorSize = state.size();
            hist.write(reinterpret_cast<char*>(&vectorSize), sizeof(size_t));
            
            for (const auto& node : state) {
                hist.write(reinterpret_cast<const char*>(&node.value), sizeof(int));
            }
            tempHistory.pop();
        }
    }

    void loadStateFromDisk() {
        if (!fs::exists(stateFile) || !fs::exists(historyFile)) {
            return;
        }

        std::ifstream state(stateFile, std::ios::binary);
        std::ifstream hist(historyFile, std::ios::binary);

        if (!state || !hist) {
            throw std::runtime_error("Failed to open files for reading");
        }

        // Load current state
        size_t stateSize;
        state.read(reinterpret_cast<char*>(&stateSize), sizeof(size_t));
        currentState.clear();
        
        for (size_t i = 0; i < stateSize; ++i) {
            int value;
            state.read(reinterpret_cast<char*>(&value), sizeof(int));
            currentState.emplace_back(value);
        }

        // Load history
        size_t historySize;
        hist.read(reinterpret_cast<char*>(&historySize), sizeof(size_t));
        
        while (!history.empty()) history.pop();
        
        for (size_t i = 0; i < historySize; ++i) {
            size_t vectorSize;
            hist.read(reinterpret_cast<char*>(&vectorSize), sizeof(size_t));
            
            std::vector<Node> stateVector;
            for (size_t j = 0; j < vectorSize; ++j) {
                int value;
                hist.read(reinterpret_cast<char*>(&value), sizeof(int));
                stateVector.emplace_back(value);
            }
            history.push(stateVector);
        }
    }

public:
    RedisDataBase() {
        loadStateFromDisk();
        isInitialized.store(true);
    }

    ~RedisDataBase() {
        if (isInitialized.load()) {
            std::unique_lock<std::shared_mutex> lock(stateMutex);
            saveStateToDisk();
        }
    }

    int push(int value) {
        std::unique_lock<std::shared_mutex> lock(stateMutex);
        currentState.push_back(Node(value));
        return 0;
    }

    int pop() {
        std::unique_lock<std::shared_mutex> lock(stateMutex);
        if (!currentState.empty()) {
            int value = currentState.back().value;
            currentState.pop_back();
            return 0;
        }
        return -1;
    }

    std::vector<int> read() const {
        std::shared_lock<std::shared_mutex> lock(stateMutex);
        std::vector<int> res;
        if (currentState.empty()) {
            return res;
        }
        res.reserve(currentState.size());
        for (const auto& node : currentState) {
            res.push_back(node.value);
        }
        return res;
    }

    int rollback() {
        std::unique_lock<std::shared_mutex> lock(stateMutex);
        std::lock_guard<std::mutex> commitLock(commitMutex);
        
        if (!history.empty()) {
            currentState = history.top();
            history.pop();
            saveStateToDisk();
            return 0;
        }
        return -1;
    }

    int commit() {
        std::unique_lock<std::shared_mutex> lock(stateMutex);
        std::lock_guard<std::mutex> commitLock(commitMutex);
        
        history.push(currentState);
        saveStateToDisk();
        return 0;
    }

    void clear() {
        std::unique_lock<std::shared_mutex> lock(stateMutex);
        std::lock_guard<std::mutex> commitLock(commitMutex);
        
        if (fs::exists(stateFile)) {
            fs::remove(stateFile);
        }
        if (fs::exists(historyFile)) {
            fs::remove(historyFile);
        }
        
        currentState.clear();
        while (!history.empty()) {
            history.pop();
        }
    }
};
