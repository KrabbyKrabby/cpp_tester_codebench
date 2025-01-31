#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <stdexcept>

class FileSystem {
private:
    std::unordered_set<std::string> filePaths;

public:
    // Normalize a file path
    std::string normalizePath(const std::string& path) {
        std::vector<std::string> components;
        std::string normalizedPath = path;

        // Remove redundant slashes
        while (normalizedPath.find("//") != std::string::npos) {
            normalizedPath.replace(normalizedPath.find("//"), 2, "/");
        }

        // Split the path into components
        size_t start = 0;
        while (start < normalizedPath.size()) {
            size_t end = normalizedPath.find('/', start + 1);
            if (end == std::string::npos) {
                end = normalizedPath.size();
            }
            std::string component = normalizedPath.substr(start + 1, end - start - 1);
            if (component == "..") {
                if (!components.empty()) {
                    components.pop_back();
                }
            } else if (component != ".") {
                components.push_back(component);
            }
            start = end;
        }

        // Reconstruct the normalized path
        normalizedPath.clear();
        for (const auto& component : components) {
            normalizedPath += "/" + component;
        }

        return normalizedPath;
    }

    // Adds a file path to the system filePaths
    void addFilePath(const std::string& path) {
        std::string normalizedPath = normalizePath(path);
        if (normalizedPath.empty() || normalizedPath[0] != '/') {
            throw std::invalid_argument("File path must start with '/' and cannot be empty");
        }
        for (auto& existingPath : filePaths) {
            if (existingPath.find(normalizedPath + '/') == 0) {
                throw std::invalid_argument("File path cannot overlap with an existing directory path");
            }
            if (normalizedPath.find(existingPath + '/') == 0) {
                throw std::invalid_argument("Directory path cannot overlap with an existing file path");
            }
        }
        filePaths.insert(normalizedPath);
    }

    // Checks if a file path exists
    bool filePathExists(const std::string& path) const {
        std::string normalizedPath = normalizePath(path);
        return filePaths.find(normalizedPath) != filePaths.end();
    }

    // List all files in a directory
    std::vector<std::string> listFilesInDirectory(const std::string& directory) {
        std::string normalizedDirectory = normalizePath(directory);
        if (normalizedDirectory.back() != '/') {
            normalizedDirectory += '/';
        }
        std::vector<std::string> files;
        for (const auto& filePath : filePaths) {
            if (filePath.find(normalizedDirectory) == 0 && filePath.find('/', normalizedDirectory.size()) == std::string::npos) {
                files.push_back(filePath);
            }
        }
        return files;
    }
};