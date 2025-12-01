#pragma once
#include <vector>
#include <string>
#include <FS.h>
#include <SD.h>

struct FileInfo {
    std::string name;
    bool isDirectory;
};

class FileSystem {
public:
    // Singleton access
    static FileSystem& getInstance() {
        static FileSystem instance;
        return instance;
    }

    // Initialize the SD card, returns true if success
    bool begin();

    // Read all entries in the given folder
    std::vector<FileInfo> readFolder(const char* path = "/");

    // Check if SD is ready
    bool isReady() const;

private:
    FileSystem() {};
    uint8_t csPin_;
    bool sdReady_;
};