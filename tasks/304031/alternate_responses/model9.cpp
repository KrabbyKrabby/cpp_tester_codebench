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
    mutable shared_mutex mtx;
    string filePath;

    // Serialize and Deserialize functions for saving/loading data
    void serialize(const vector<Node>& data, const string& filePath) {
        ofstream file(filePath, ios::binary);
        if (file.is_open()) {
            size_t size = data.size();
            file.write(reinterpret_cast<const char*>(&size), sizeof(size));
            for (const auto& node : data) {
                file.write(reinterpret_cast<const char*>(&node.value), sizeof(node.value));
            }
            file.close();
        }
    }

    void deserialize(vector<Node>& data, const string& filePath) {
        ifstream file(filePath, ios::binary);
        if (file.is_open()) {
            size_t size;
            file.read(reinterpret_cast<char*>(&size), sizeof(size));
            data.reserve(size);
            for (size_t i = 0; i < size; ++i) {
                int value;
                file.read(reinterpret_cast<char*>(&value), sizeof(value));
                data.emplace_back(value);
            }
            file.close();
        }
    }

public:
    RedisDataBase(const string& filePath) : filePath(filePath) {
        // Load saved state from disk
        deserialize(currentState, filePath + "_currentState");
        deserialize(history.top(), filePath + "_history");
    }

    ~RedisDataBase() {
        // Save state to disk
        serialize(currentState, filePath + "_currentState");
        serialize(history.top(), filePath + "_history");
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
        unique_lock<shared_mutex> lock(mtx);
        if (!history.empty()) {
            currentState = history.top();
            history.pop();
            return 0;
        } else {
            return -1;
        }
    }

    int commit() {
        unique_lock<shared_mutex> lock(mtx);
        history.push(currentState);
        // Save state to disk
        serialize(currentState, filePath + "_currentState");
        serialize(history.top(), filePath + "_history");
        return 0;
    }

    void clear() {
        unique_lock<shared_mutex> lock(mtx);
        currentState.clear();
        while (!history.empty()) {
            history.pop();
        }
        // Delete saved state from disk
        remove((filePath + "_currentState").c_str());
        remove((filePath + "_history").c_str());
    }
};
