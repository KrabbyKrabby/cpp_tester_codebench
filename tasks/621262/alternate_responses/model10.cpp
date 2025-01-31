#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <stdexcept>
#include <sstream>

class FileSystem {
private:
    std::unordered_set<std::string> filePaths;

    // Normalize a file path
    std::string normalizePath(const std::string& path) const {
        std::istringstream iss(path);
        std::vector<std::string> components;
        std::string component;

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
        for (const auto& component : components) {
            normalizedPath += component + "/";
        }

        // Remove trailing slash if not root
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

        // Check if the path conflicts with existing ones
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

    // List all files in a directory
    std::vector<std::string> listFilesInDirectory(const std::string& directory) const {
        std::vector<std::string> files;
        std::string normalizedDirectory = normalizePath(directory);

        // Ensure the directory path ends with a slash
        if (normalizedDirectory.back() != '/') {
            normalizedDirectory += '/';
        }

        for (const auto& filePath : filePaths) {
            if (filePath.find(normalizedDirectory) == 0 && filePath.find('/', normalizedDirectory.length()) == std::string::npos) {
                files.push_back(filePath);
            }
        }

        return files;
    }
};