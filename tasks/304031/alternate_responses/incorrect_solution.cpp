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
    shared_mutex mtx; // Shared mutex for concurrent read access
    mutex mtxHistory; // Mutex for exclusive access to history
    string filename = "redis_database.dat"; // File to store the database state

    // Serialize the database state to a string
    string serialize() {
        stringstream ss;
        ss << currentState.size() << "\n";
        for (const auto& node : currentState) {
            ss << node.value << "\n";
        }
        ss << history.size() << "\n";
        for (const auto& state : history) {
            ss << state.size() << "\n";
            for (const auto& node : state) {
                ss << node.value << "\n";
            }
        }
        return ss.str();
    }

    // Deserialize the database state from a string
    void deserialize(const string& data) {
        stringstream ss(data);
        int size;
        ss >> size;
        currentState.resize(size);
        for (auto& node : currentState) {
            ss >> node.value;
        }
        ss >> size;
        history = stack<vector<Node>>();
        for (int i = 0; i < size; ++i) {
            int stateSize;
            ss >> stateSize;
            vector<Node> state(stateSize);
            for (auto& node : state) {
                ss >> node.value;
            }
            history.push(state);
        }
    }

public:
    RedisDataBase() {
        // Load the saved state from disk
        ifstream file(filename, ios::binary);
        if (file.is_open()) {
            stringstream buffer;
            buffer << file.rdbuf();
            file.close();
            deserialize(buffer.str());
        }
    }

    ~RedisDataBase() {
        // Save the state to disk
        ofstream file(filename, ios::binary);
        if (file.is_open()) {
            file << serialize();
            file.close();
        }
    }

    int push(int value) {
        unique_lock<shared_mutex> lock(mtx);
        currentState.push_back(Node(value));
        return 0;
    }

    int pop() {
        unique_lock<shared_mutex> lock(mtx);
        if (!currentState.empty()) {
            currentState.pop_back();
            return 0;
        } else {
            return -1;
        }
    }

    vector<int> read() {
        shared_lock<shared_mutex> lock(mtx);
        vector<int> res;
        for (const auto& node : currentState) {
            res.push_back(node.value);
        }
        return res;
    }

    int rollback() {
        lock_guard<mutex> lock(mtxHistory);
        if (!history.empty()) {
            currentState = history.top();
            history.pop();
            return 0;
        } else {
            return -1;
        }
    }

    int commit() {
        lock_guard<mutex> lock(mtxHistory);
        history.push(currentState);
        // Save the state to disk
        ofstream file(filename, ios::binary);
        if (file.is_open()) {
            file << serialize();
            file.close();
        }
        return 0;
    }

    void clear() {
        // Delete the saved state from disk
        remove(filename.c_str());
        currentState.clear();
        history = stack<vector<Node>>();
    }
};
