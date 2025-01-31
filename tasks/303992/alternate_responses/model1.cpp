#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <functional>
#include <memory>
#include <limits>

class ThreadPool {
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;

public:
    explicit ThreadPool(size_t threads) : stop(false) {
        for(size_t i = 0; i < threads; ++i) {
            workers.emplace_back([this] {
                while(true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex);
                        condition.wait(lock, [this] { 
                            return stop || !tasks.empty(); 
                        });
                        if(stop && tasks.empty()) return;
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    template<class F>
    void enqueue(F&& f) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.emplace(std::forward<F>(f));
        }
        condition.notify_one();
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for(std::thread &worker: workers) {
            worker.join();
        }
    }
};

class GaussianElimination {
private:
    static constexpr double EPSILON = 1e-10;
    std::unique_ptr<ThreadPool> pool;
    
    void recursiveEliminate(std::vector<std::vector<double>>& matrix, int pivot_row, 
                           int start_row, int end_row, int n) {
        if (end_row - start_row <= 16) {
            for (int i = start_row; i < end_row; ++i) {
                if (std::abs(matrix[pivot_row][pivot_row]) < EPSILON) return;
                double factor = matrix[i][pivot_row] / matrix[pivot_row][pivot_row];
                for (int j = pivot_row; j <= n; ++j) {
                    matrix[i][j] -= factor * matrix[pivot_row][j];
                }
            }
            return;
        }

        int mid = start_row + (end_row - start_row) / 2;
        pool->enqueue([&, pivot_row, start_row, mid, n] {
            recursiveEliminate(matrix, pivot_row, start_row, mid, n);
        });
        recursiveEliminate(matrix, pivot_row, mid, end_row, n);
    }

    bool isZeroRow(const std::vector<double>& row, int n) {
        for (int i = 0; i < n; ++i) {
            if (std::abs(row[i]) > EPSILON) return false;
        }
        return std::abs(row[n]) <= EPSILON;
    }

public:
    GaussianElimination() : pool(std::make_unique<ThreadPool>(std::thread::hardware_concurrency())) {}

    std::vector<double> solve(const std::vector<std::vector<double>>& matrix, 
                            const std::vector<double>& constants) {
        if (matrix.empty() || constants.empty() || 
            matrix.size() != constants.size() || 
            matrix[0].size() != matrix.size()) {
            return std::vector<double>();
        }

        int n = matrix.size();
        std::vector<std::vector<double>> augmented(n, std::vector<double>(n + 1));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                augmented[i][j] = matrix[i][j];
            }
            augmented[i][n] = constants[i];
        }

        // Forward elimination with partial pivoting
        for (int i = 0; i < n; ++i) {
            int max_row = i;
            double max_val = std::abs(augmented[i][i]);
            
            for (int k = i + 1; k < n; ++k) {
                if (std::abs(augmented[k][i]) > max_val) {
                    max_val = std::abs(augmented[k][i]);
                    max_row = k;
                }
            }

            if (max_val < EPSILON) {
                // Check for inconsistent or dependent system
                for (int k = i; k < n; ++k) {
                    if (!isZeroRow(augmented[k], n)) {
                        return std::vector<double>(); // Inconsistent system
                    }
                }
                return std::vector<double>(); // Dependent system
            }

            if (max_row != i) {
                std::swap(augmented[i], augmented[max_row]);
            }

            recursiveEliminate(augmented, i, i + 1, n, n);
        }

        // Back substitution
        std::vector<double> solution(n);
        for (int i = n - 1; i >= 0; --i) {
            solution[i] = augmented[i][n];
            for (int j = i + 1; j < n; ++j) {
                solution[i] -= augmented[i][j] * solution[j];
            }
            if (std::abs(augmented[i][i]) < EPSILON) {
                return std::vector<double>(); // Singular matrix
            }
            solution[i] /= augmented[i][i];
        }

        return solution;
    }
};

std::vector<double> gaussianEliminationSolver(const std::vector<std::vector<double>>& matrix, 
                                            const std::vector<double>& constants) {
    GaussianElimination solver;
    return solver.solve(matrix, constants);
}
