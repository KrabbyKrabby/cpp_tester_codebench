#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <stdexcept>
#include <sstream>

class FileSystem {
private:
    std::unordered_set<std::string> filePaths;

    // Normalizes a file path
    std::string normalizePath(const std::string& path) const {
        std::istringstream iss(path);
        std::string component;
        std::vector<std::string> components;

        // Split the path into components
        while (std::getline(iss, component, '/')) {
            if (component == "..") {
                if (!components.empty()) {
                    components.pop_back();
                }
            } else if (component != "" && component != ".") {
                components.push_back(component);
            }
        }

        // Reconstruct the normalized path
        std::string normalizedPath = "/";
        for (const auto& c : components) {
            normalizedPath += c + "/";
        }

        // Remove the trailing slash if not root
        if (normalizedPath.length() > 1) {
            normalizedPath.pop_back();
        }

        return normalizedPath;
    }

public:
    // Adds a file path to the system filePaths
    void addFilePath(const std::string& path) {
        std::string normalizedPath = normalizePath(path);

        // Check if the normalized path is valid
        if (normalizedPath.empty() || normalizedPath[0] != '/') {
            throw std::invalid_argument("File path must start with '/' and cannot be empty");
        }

        // Check if the normalized path conflicts with existing ones
        for (const auto& existingPath : filePaths) {
            if (existingPath.find(normalizedPath + "/") == 0) {
                throw std::invalid_argument("File path cannot overlap a directory path");
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
            if (path.find(normalizedDirectory + "/") == 0) {
                files.push_back(path);
            }
        }

        return files;
    }
};