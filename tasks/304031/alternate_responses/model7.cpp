#include <iostream>
#include <vector>
#include <stack>
#include <mutex>
#include <shared_mutex>
#include <fstream>
#include <sstream>

using namespace std;

struct Node {
    int value;
    Node(int v) : value(v) {}
};

class RedisDataBase {
private:
    vector<Node> currentState;
    stack<vector<Node>> history;
    std::shared_mutex mtx; // Shared mutex for efficient read-write access
    std::string filePath;   // File path for persistence

    // Load saved state from disk
    void loadState() {
        ifstream file(filePath);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                int value;
                while (iss >> value) {
                    currentState.emplace_back(value);
                }
            }
            file.close();
        }
    }

    // Save state to disk
    void saveState() const {
        ofstream file(filePath);
        if (file.is_open()) {
            for (const auto& node : currentState) {
                file << node.value << " ";
            }
            file << std::endl;
            file.close();
        }
    }

public:
    RedisDataBase(const std::string& filePath) : filePath(filePath) {
        loadState();
    }

    ~RedisDataBase() {
        saveState();
    }

    // Clear saved state from disk
    void clear() {
        std::unique_lock<std::shared_mutex> lock(mtx);
        currentState.clear();
        history = stack<vector<Node>>();
        saveState();
    }

    int push(int value) {
        std::unique_lock<std::shared_mutex> lock(mtx);
        currentState.push_back(Node(value));
        return 0;
    }

    int pop() {
        std::unique_lock<std::shared_mutex> lock(mtx);
        if (!currentState.empty()) {
            currentState.pop_back();
            return 0;
        } else {
            return -1;
        }
    }

    vector<int> read() {
        std::shared_lock<std::shared_mutex> lock(mtx);
        vector<int> res;
        for (const auto& node : currentState) {
            res.push_back(node.value);
        }
        return res;
    }

    int rollback() {
        std::unique_lock<std::shared_mutex> lock(mtx);
        if (!history.empty()) {
            currentState = history.top();
            history.pop();
            return 0;
        } else {
            return -1;
        }
    }

    int commit() {
        std::unique_lock<std::shared_mutex> lock(mtx);
        history.push(currentState);
        saveState();
        return 0;
    }
};
