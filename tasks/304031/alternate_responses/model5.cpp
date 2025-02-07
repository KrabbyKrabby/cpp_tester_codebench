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
    atomic<bool> isInitialized{false};
    const string stateFile = "redis_state.bin";
    const string historyFile = "redis_history.bin";

    void saveStateToDisk() {
        ofstream state(stateFile, ios::binary);
        ofstream hist(historyFile, ios::binary);
        
        if (state.is_open()) {
            size_t size = currentState.size();
            state.write(reinterpret_cast<char*>(&size), sizeof(size));
            for (const auto& node : currentState) {
                state.write(reinterpret_cast<const char*>(&node.value), sizeof(node.value));
            }
        }
        
        if (hist.is_open()) {
            size_t stackSize = history.size();
            hist.write(reinterpret_cast<char*>(&stackSize), sizeof(stackSize));
            stack<vector<Node>> tempStack = history;
            while (!tempStack.empty()) {
                const auto& vec = tempStack.top();
                size_t vecSize = vec.size();
                hist.write(reinterpret_cast<char*>(&vecSize), sizeof(vecSize));
                for (const auto& node : vec) {
                    hist.write(reinterpret_cast<const char*>(&node.value), sizeof(node.value));
                }
                tempStack.pop();
            }
        }
    }

    void loadStateFromDisk() {
        ifstream state(stateFile, ios::binary);
        ifstream hist(historyFile, ios::binary);
        
        if (state.is_open()) {
            size_t size;
            state.read(reinterpret_cast<char*>(&size), sizeof(size));
            currentState.clear();
            for (size_t i = 0; i < size; ++i) {
                int value;
                state.read(reinterpret_cast<char*>(&value), sizeof(value));
                currentState.push_back(Node(value));
            }
        }
        
        if (hist.is_open()) {
            size_t stackSize;
            hist.read(reinterpret_cast<char*>(&stackSize), sizeof(stackSize));
            while (!history.empty()) history.pop();
            for (size_t i = 0; i < stackSize; ++i) {
                size_t vecSize;
                hist.read(reinterpret_cast<char*>(&vecSize), sizeof(vecSize));
                vector<Node> vec;
                for (size_t j = 0; j < vecSize; ++j) {
                    int value;
                    hist.read(reinterpret_cast<char*>(&value), sizeof(value));
                    vec.push_back(Node(value));
                }
                history.push(vec);
            }
        }
    }

public:
    RedisDataBase() {
        if (fs::exists(stateFile) && fs::exists(historyFile)) {
            loadStateFromDisk();
        }
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
        currentState.push_back(Node(value));
        return 0;
    }

    int pop() {
        unique_lock<shared_mutex> lock(mutex_);
        if (!currentState.empty()) {
            int value = currentState.back().value;
            currentState.pop_back();
            return 0;
        }
        return -1;
    }

    vector<int> read() {
        shared_lock<shared_mutex> lock(mutex_);
        vector<int> res;
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
        history.push(currentState);
        saveStateToDisk();
        return 0;
    }

    void clear() {
        unique_lock<shared_mutex> lock(mutex_);
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
