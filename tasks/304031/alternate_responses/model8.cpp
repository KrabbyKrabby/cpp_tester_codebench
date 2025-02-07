#include <iostream>
#include <vector>
#include <stack>
#include <mutex>
#include <shared_mutex>
#include <fstream>
#include <sstream>
#include <json/json.h> // Requires jsoncpp library

using namespace std;

struct Node {
    int value;
    Node(int v) : value(v) {}
};

class RedisDataBase {
private:
    vector<Node> currentState;
    stack<vector<Node>> history;
    mutex mtx; // Mutex for exclusive access
    shared_mutex sharedMtx; // Shared mutex for shared access
    string filePath; // Path to save and load the state

    // Helper function to save the state to disk
    void saveState() {
        Json::Value jsonData;
        Json::Value currentStateArray;
        for (const auto& node : currentState) {
            currentStateArray.append(node.value);
        }
        jsonData["currentState"] = currentStateArray;

        Json::Value historyArray;
        while (!history.empty()) {
            Json::Value stateArray;
            for (const auto& node : history.top()) {
                stateArray.append(node.value);
            }
            historyArray.append(stateArray);
            history.pop();
        }
        jsonData["history"] = historyArray;

        ofstream file(filePath);
        Json::StyledWriter writer;
        file << writer.write(jsonData);
        file.close();
    }

    // Helper function to load the state from disk
    void loadState() {
        ifstream file(filePath);
        if (file.is_open()) {
            Json::Value jsonData;
            Json::Reader reader;
            reader.parse(file, jsonData);

            for (const auto& value : jsonData["currentState"]) {
                currentState.push_back(Node(value.asInt()));
            }

            for (const auto& state : jsonData["history"]) {
                vector<Node> stateVector;
                for (const auto& value : state) {
                    stateVector.push_back(Node(value.asInt()));
                }
                history.push(stateVector);
            }
        }
        file.close();
    }

public:
    RedisDataBase(string filePath) : filePath(filePath) {
        loadState();
    }

    ~RedisDataBase() {
        saveState();
    }

    int push(int value) {
        lock_guard<mutex> lock(mtx);
        currentState.push_back(Node(value));
        return 0;
    }

    int pop() {
        lock_guard<mutex> lock(mtx);
        if (!currentState.empty()) {
            currentState.pop_back();
            return 0;
        } else {
            return -1;
        }
    }

    vector<int> read() {
        shared_lock<shared_mutex> lock(sharedMtx);
        vector<int> res;
        for (const auto& node : currentState) {
            res.push_back(node.value);
        }
        return res;
    }

    int rollback() {
        lock_guard<mutex> lock(mtx);
        if (!history.empty()) {
            currentState = history.top();
            history.pop();
            return 0;
        } else {
            return -1;
        }
    }

    int commit() {
        lock_guard<mutex> lock(mtx);
        history.push(currentState);
        saveState();
        return 0;
    }

    void clear() {
        lock_guard<mutex> lock(mtx);
        currentState.clear();
        while (!history.empty()) {
            history.pop();
        }
        saveState();
    }
};
