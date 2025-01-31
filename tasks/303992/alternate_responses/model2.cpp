#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <queue>
#include <limits>

using namespace std;

class GaussianSolver {
private:
    static constexpr double EPSILON = 1e-10;
    mutex mtx;
    condition_variable cv;
    queue<pair<int, int>> workQueue;
    bool finished = false;
    
    struct ThreadPool {
        vector<thread> workers;
        mutex queue_mutex;
        condition_variable cv;
        bool stop = false;
        queue<function<void()>> tasks;
        
        ThreadPool(size_t threads) {
            for(size_t i = 0; i < threads; ++i) {
                workers.emplace_back([this] {
                    while(true) {
                        function<void()> task;
                        {
                            unique_lock<mutex> lock(queue_mutex);
                            cv.wait(lock, [this] { 
                                return stop || !tasks.empty(); 
                            });
                            if(stop && tasks.empty()) return;
                            task = move(tasks.front());
                            tasks.pop();
                        }
                        task();
                    }
                });
            }
        }
        
        ~ThreadPool() {
            {
                unique_lock<mutex> lock(queue_mutex);
                stop = true;
            }
            cv.notify_all();
            for(auto& worker : workers) {
                worker.join();
            }
        }
        
        template<class F>
        void enqueue(F&& f) {
            {
                unique_lock<mutex> lock(queue_mutex);
                tasks.emplace(forward<F>(f));
            }
            cv.notify_one();
        }
    };

    void recursiveElimination(vector<vector<double>>& matrix, int pivot, int n, ThreadPool& pool) {
        if(pivot >= n - 1) return;

        auto maxElement = pivot;
        for(int i = pivot + 1; i < n; i++) {
            if(abs(matrix[i][pivot]) > abs(matrix[maxElement][pivot])) {
                maxElement = i;
            }
        }

        if(abs(matrix[maxElement][pivot]) < EPSILON) {
            throw runtime_error("Matrix is singular");
        }

        if(maxElement != pivot) {
            swap(matrix[pivot], matrix[maxElement]);
        }

        vector<future<void>> futures;
        int chunk = max(1, (n - pivot - 1) / 4);
        
        for(int start = pivot + 1; start < n; start += chunk) {
            int end = min(start + chunk, n);
            pool.enqueue([&matrix, pivot, start, end, n]() {
                for(int i = start; i < end; i++) {
                    double factor = matrix[i][pivot] / matrix[pivot][pivot];
                    for(int j = pivot; j <= n; j++) {
                        matrix[i][j] -= factor * matrix[pivot][j];
                    }
                }
            });
        }

        recursiveElimination(matrix, pivot + 1, n, pool);
    }

    vector<double> backSubstitution(vector<vector<double>>& matrix, int n) {
        vector<double> solution(n);
        for(int i = n - 1; i >= 0; i--) {
            double sum = 0.0;
            for(int j = i + 1; j < n; j++) {
                sum += matrix[i][j] * solution[j];
            }
            if(abs(matrix[i][i]) < EPSILON) {
                throw runtime_error("No unique solution exists");
            }
            solution[i] = (matrix[i][n] - sum) / matrix[i][i];
        }
        return solution;
    }

public:
    vector<double> solve(const vector<vector<double>>& matrix, const vector<double>& constants) {
        if(matrix.empty() || constants.empty() || matrix.size() != constants.size()) {
            throw invalid_argument("Invalid input dimensions");
        }

        int n = matrix.size();
        for(const auto& row : matrix) {
            if(row.size() != n) {
                throw invalid_argument("Matrix must be square");
            }
        }

        vector<vector<double>> augmentedMatrix(n, vector<double>(n + 1));
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                augmentedMatrix[i][j] = matrix[i][j];
            }
            augmentedMatrix[i][n] = constants[i];
        }

        ThreadPool pool(thread::hardware_concurrency());
        
        try {
            recursiveElimination(augmentedMatrix, 0, n, pool);
            return backSubstitution(augmentedMatrix, n);
        } catch(const exception& e) {
            return vector<double>();
        }
    }
};

vector<double> gaussianEliminationSolver(const vector<vector<double>>& matrix, const vector<double>& constants) {
    GaussianSolver solver;
    return solver.solve(matrix, constants);
}