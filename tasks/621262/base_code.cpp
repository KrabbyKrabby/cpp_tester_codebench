#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <stdexcept>

class FileSystem {
private:
    std::unordered_set<std::string> filePaths;

public:
    // Adds a file path to the system filePaths
    void addFilePath(const std::string& path) {
        if (path.empty() || path[0] != '/') {
            throw std::invalid_argument("File path must start with '/' and cannot be empty");
        }
        filePaths.insert(path);
    }

    // Checks if a file path exists
    bool filePathExists(const std::string& path) const {
        return filePaths.find(path) != filePaths.end();
    }
};