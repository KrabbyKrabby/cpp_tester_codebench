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
    mutable std::shared_mutex mtx; // Shared mutex for read-write access
    std::atomic<bool> isCommitting{false}; // Atomic flag for commit operation

    // Serialize and deserialize helper functions
    void serialize(const vector<Node>& state, ostream& os) {
        for (const auto& node : state) {
            os << node.value << ' ';
        }
    }

    void deserialize(istream& is, vector<Node>& state) {
        int value;
        while (is >> value) {
            state.emplace_back(value);
        }
    }

public:
    RedisDataBase() {
        // Load saved state from disk
        ifstream file("redis_state.txt");
        if (file.is_open()) {
            deserialize(file, currentState);
            file.close();
        }
    }

    ~RedisDataBase() {
        // Save state to disk on destruction
        ofstream file("redis_state.txt");
        if (file.is_open()) {
            serialize(currentState, file);
            file.close();
        }
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
        if (isCommitting.exchange(true)) {
            // Another thread is committing, return error
            return -1;
        }
        history.push(currentState);
        // Save state to disk
        ofstream file("redis_state.txt");
        if (file.is_open()) {
            serialize(currentState, file);
            file.close();
        }
        isCommitting = false;
        return 0;
    }

    int clear() {
        std::unique_lock<std::shared_mutex> lock(mtx);
        currentState.clear();
        history = stack<vector<Node>>();
        // Delete saved state from disk
        remove("redis_state.txt");
        return 0;
    }
};
