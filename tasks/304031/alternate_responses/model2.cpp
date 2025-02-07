#include <iostream>
#include <vector>
#include <stack>
#include <shared_mutex>
#include <fstream>
#include <filesystem>
#include <atomic>
#include <string>

using namespace std;
namespace fs = std::filesystem;

struct Node {
    int value;
    Node(int v) : value(v) {}
};

class RedisDataBase {
private:
    vector<Node> currentState;
    stack<vector<Node>> history;
    mutable shared_mutex mutex_;
    const string stateFile = "redis_state.bin";
    const string historyFile = "redis_history.bin";
    atomic<bool> isInitialized{false};

    void saveStateToDisk() {
        ofstream state_ofs(stateFile, ios::binary);
        ofstream history_ofs(historyFile, ios::binary);
        
        if (!state_ofs || !history_ofs) {
            throw runtime_error("Failed to open files for writing");
        }

        // Save current state
        size_t stateSize = currentState.size();
        state_ofs.write(reinterpret_cast<char*>(&stateSize), sizeof(stateSize));
        for (const auto& node : currentState) {
            state_ofs.write(reinterpret_cast<const char*>(&node.value), sizeof(node.value));
        }

        // Save history
        size_t historySize = history.size();
        history_ofs.write(reinterpret_cast<char*>(&historySize), sizeof(historySize));
        
        stack<vector<Node>> tempHistory = history;
        while (!tempHistory.empty()) {
            const auto& state = tempHistory.top();
            size_t size = state.size();
            history_ofs.write(reinterpret_cast<char*>(&size), sizeof(size));
            for (const auto& node : state) {
                history_ofs.write(reinterpret_cast<const char*>(&node.value), sizeof(node.value));
            }
            tempHistory.pop();
        }
    }

    void loadStateFromDisk() {
        if (!fs::exists(stateFile) || !fs::exists(historyFile)) {
            return;
        }

        ifstream state_ifs(stateFile, ios::binary);
        ifstream history_ifs(historyFile, ios::binary);

        if (!state_ifs || !history_ifs) {
            throw runtime_error("Failed to open files for reading");
        }

        // Load current state
        size_t stateSize;
        state_ifs.read(reinterpret_cast<char*>(&stateSize), sizeof(stateSize));
        currentState.clear();
        for (size_t i = 0; i < stateSize; ++i) {
            int value;
            state_ifs.read(reinterpret_cast<char*>(&value), sizeof(value));
            currentState.push_back(Node(value));
        }

        // Load history
        size_t historySize;
        history_ifs.read(reinterpret_cast<char*>(&historySize), sizeof(historySize));
        
        while (!history.empty()) history.pop();
        
        for (size_t i = 0; i < historySize; ++i) {
            size_t size;
            history_ifs.read(reinterpret_cast<char*>(&size), sizeof(size));
            vector<Node> state;
            for (size_t j = 0; j < size; ++j) {
                int value;
                history_ifs.read(reinterpret_cast<char*>(&value), sizeof(value));
                state.push_back(Node(value));
            }
            history.push(state);
        }
    }

public:
    RedisDataBase() {
        loadStateFromDisk();
        isInitialized.store(true);
    }

    ~RedisDataBase() {
        if (isInitialized.load()) {
            unique_lock<shared_mutex> lock(mutex_);
            saveStateToDisk();
        }
    }

    int push(int value) {
        unique_lock<shared_mutex> lock(mutex_);
        try {
            currentState.push_back(Node(value));
            return 0;
        } catch (...) {
            return -1;
        }
    }

    int pop() {
        unique_lock<shared_mutex> lock(mutex_);
        if (!currentState.empty()) {
            currentState.pop_back();
            return 0;
        }
        return -1;
    }

    vector<int> read() {
        shared_lock<shared_mutex> lock(mutex_);
        vector<int> res;
        res.reserve(currentState.size());
        for (const auto& node : currentState) {
            res.push_back(node.value);
        }
        return res;
    }

    int rollback() {
        unique_lock<shared_mutex> lock(mutex_);
        if (!history.empty()) {
            currentState = history.top();
            history.pop();
            saveStateToDisk();
            return 0;
        }
        return -1;
    }

    int commit() {
        unique_lock<shared_mutex> lock(mutex_);
        try {
            history.push(currentState);
            saveStateToDisk();
            return 0;
        } catch (...) {
            return -1;
        }
    }

    void clear() {
        unique_lock<shared_mutex> lock(mutex_);
        currentState.clear();
        while (!history.empty()) history.pop();
        
        if (fs::exists(stateFile)) {
            fs::remove(stateFile);
        }
        if (fs::exists(historyFile)) {
            fs::remove(historyFile);
        }
    }
};
