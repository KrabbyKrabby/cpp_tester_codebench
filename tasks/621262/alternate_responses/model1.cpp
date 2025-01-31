#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <stdexcept>
#include <sstream>
#include <algorithm>

class FileSystem {
private:
    std::unordered_set<std::string> filePaths;

    // Helper function to check if one path is a subpath of another
    bool isSubPath(const std::string& path1, const std::string& path2) const {
        return path1.length() < path2.length() && 
               path2.substr(0, path1.length()) == path1 &&
               path2[path1.length()] == '/';
    }

    // Helper function to get directory path from a file path
    std::string getDirectoryPath(const std::string& path) const {
        size_t lastSlash = path.find_last_of('/');
        if (lastSlash == 0) return "/";
        return path.substr(0, lastSlash);
    }

public:
    std::string normalizePath(const std::string& path) {
        if (path.empty() || path[0] != '/') {
            throw std::invalid_argument("Path must start with '/'");
        }

        std::vector<std::string> components;
        std::istringstream iss(path);
        std::string component;
        
        // Split path by '/' and process each component
        size_t start = 1;
        size_t end = path.find('/', start);
        
        while (start < path.length()) {
            if (end == std::string::npos) {
                component = path.substr(start);
                start = path.length();
            } else {
                component = path.substr(start, end - start);
                start = end + 1;
                end = path.find('/', start);
            }
            
            if (component.empty() || component == ".") {
                continue;
            } else if (component == "..") {
                if (!components.empty()) {
                    components.pop_back();
                }
            } else {
                components.push_back(component);
            }
        }

        // Construct normalized path
        if (components.empty()) {
            return "/";
        }

        std::string normalized = "";
        for (const auto& comp : components) {
            normalized += "/" + comp;
        }
        return normalized;
    }

    void addFilePath(const std::string& path) {
        std::string normalizedPath = normalizePath(path);
        
        // Check for directory-file conflicts
        for (const auto& existingPath : filePaths) {
            if (isSubPath(normalizedPath, existingPath) || 
                isSubPath(existingPath, normalizedPath)) {
                throw std::invalid_argument("Path conflicts with existing file or directory");
            }
        }
        
        filePaths.insert(normalizedPath);
    }

    bool filePathExists(const std::string& path) const {
        try {
            std::string normalizedPath = const_cast<FileSystem*>(this)->normalizePath(path);
            return filePaths.find(normalizedPath) != filePaths.end();
        } catch (const std::invalid_argument&) {
            return false;
        }
    }

    std::vector<std::string> listFilesInDirectory(const std::string& directory) {
        std::string normalizedDir = normalizePath(directory);
        if (normalizedDir != "/" && normalizedDir.back() == '/') {
            normalizedDir.pop_back();
        }

        std::vector<std::string> files;
        for (const auto& path : filePaths) {
            std::string dirPath = getDirectoryPath(path);
            if (dirPath == normalizedDir) {
                files.push_back(path);
            }
        }
        
        std::sort(files.begin(), files.end());
        return files;
    }
};