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
        std::string normalizedPath = normalizePath(path);
        if (normalizedPath.empty() || normalizedPath[0] != '/') {
            throw std::invalid_argument("File path must start with '/' and cannot be empty");
        }

        // Check if the path is a directory and if it conflicts with existing files
        if (isDirectory(normalizedPath) && hasConflictingFiles(normalizedPath)) {
            throw std::invalid_argument("File path cannot overlap a directory path");
        }

        filePaths.insert(normalizedPath);
    }

    // Checks if a file path exists
    bool filePathExists(const std::string& path) const {
        std::string normalizedPath = normalizePath(path);
        return filePaths.find(normalizedPath) != filePaths.end();
    }

    // Normalizes a file path
    std::string normalizePath(const std::string& path) const {
        std::vector<std::string> components;
        std::string component;

        for (char c : path) {
            if (c == '/') {
                if (!component.empty()) {
                    components.push_back(component);
                    component.clear();
                }
            } else {
                component += c;
            }
        }

        if (!component.empty()) {
            components.push_back(component);
        }

        std::vector<std::string> normalizedComponents;
        for (const std::string& component : components) {
            if (component == "..") {
                if (!normalizedComponents.empty()) {
                    normalizedComponents.pop_back();
                }
            } else if (component != ".") {
                normalizedComponents.push_back(component);
            }
        }

        std::string normalizedPath;
        for (const std::string& component : normalizedComponents) {
            normalizedPath += "/" + component;
        }

        if (normalizedPath.empty()) {
            normalizedPath = "/";
        }

        return normalizedPath;
    }

    // Lists all files in a directory
    std::vector<std::string> listFilesInDirectory(const std::string& directory) const {
        std::string normalizedDirectory = normalizePath(directory);
        std::vector<std::string> files;

        for (const std::string& filePath : filePaths) {
            if (isInDirectory(filePath, normalizedDirectory)) {
                files.push_back(filePath);
            }
        }

        return files;
    }

    // Checks if a file path is a directory
    bool isDirectory(const std::string& path) const {
        for (const std::string& filePath : filePaths) {
            if (isInDirectory(filePath, path)) {
                return true;
            }
        }

        return false;
    }

    // Checks if a file path is in a directory
    bool isInDirectory(const std::string& filePath, const std::string& directory) const {
        return filePath.find(directory) == 0 && filePath.find('/', directory.size()) == std::string::npos;
    }

    // Checks if a directory has conflicting files
    bool hasConflictingFiles(const std::string& directory) const {
        for (const std::string& filePath : filePaths) {
            if (filePath.find(directory) == 0 && filePath.find('/', directory.size()) != std::string::npos) {
                return true;
            }
        }

        return false;
    }
};