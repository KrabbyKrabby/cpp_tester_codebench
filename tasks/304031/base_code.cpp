#include <iostream>
#include <vector>
#include <stack>

using namespace std;

struct Node {
    int value;
    Node(int v) : value(v) {}
};

class RedisDataBase {
private:
    vector<Node> currentState;
    stack<vector<Node>> history;

public:
    int push(int value) {
        currentState.push_back(Node(value));
        return 0;
    }

    int pop() {
        if (!currentState.empty()) {
            int value = currentState.back().value;
            currentState.pop_back();
            return 0;
        } else {
            return -1;
        }
    }

    vector<int> read() {
        vector<int> res;
        if (currentState.empty()) {
            return res;
        }
        for (const auto& node : currentState) {
            res.push_back(node.value);
        }
        return res;
    }

    int rollback() {
        if (!history.empty()) {
            currentState = history.top();
            history.pop();
            return 0;
        } else {
            return -1;
        }
    }

    int commit() {
        history.push(currentState);
        return 0;
    }
};