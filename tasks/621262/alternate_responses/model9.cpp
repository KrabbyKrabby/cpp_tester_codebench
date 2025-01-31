#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <stdexcept>

class FileSystem {
private:
    std::unordered_set<std::string> filePaths;

public:
    // Normalizes a file path
    std::string normalizePath(const std::string& path) const {
        if (path.empty() || path[0] != '/') {
            throw std::invalid_argument("File path must start with '/' and cannot be empty");
        }

        std::vector<std::string> components;
        size_t start = 1; // Skip the leading '/'
        while (start < path.size()) {
            size_t end = path.find('/', start);
            if (end == std::string::npos) {
                end = path.size();
            }
            std::string component = path.substr(start, end - start);
            if (component == "..") {
                if (!components.empty()) {
                    components.pop_back();
                }
            } else if (component != "." && !component.empty()) {
                components.push_back(component);
            }
            start = end + 1;
        }

        std::string normalizedPath = "/";
        for (const auto& component : components) {
            normalizedPath += component + "/";
        }
        if (normalizedPath.size() > 1) {
            normalizedPath.pop_back(); // Remove trailing '/'
        }
        return normalizedPath;
    }

    // Adds a file path to the system filePaths
    void addFilePath(const std::string& path) {
        std::string normalizedPath = normalizePath(path);
        if (filePathExists(normalizedPath)) {
            throw std::invalid_argument("File path already exists");
        }
        // Check for conflicts with existing directory paths
        size_t lastSlash = normalizedPath.find_last_of('/');
        if (lastSlash != std::string::npos) {
            std::string directory = normalizedPath.substr(0, lastSlash);
            for (const auto& existingPath : filePaths) {
                if (existingPath.find(directory + '/') == 0) {
                    throw std::invalid_argument("File path conflicts with an existing directory path");
                }
            }
        }
        filePaths.insert(normalizedPath);
    }

    // Checks if a file path exists
    bool filePathExists(const std::string& path) const {
        return filePaths.find(normalizePath(path)) != filePaths.end();
    }

    // Lists all files in a directory
    std::vector<std::string> listFilesInDirectory(const std::string& directory) const {
        std::string normalizedDirectory = normalizePath(directory);
        std::vector<std::string> files;
        for (const auto& path : filePaths) {
            size_t lastSlash = path.find_last_of('/');
            if (lastSlash != std::string::npos) {
                std::string fileDirectory = path.substr(0, lastSlash);
                if (fileDirectory == normalizedDirectory) {
                    files.push_back(path);
                }
            }
        }
        return files;
    }
};