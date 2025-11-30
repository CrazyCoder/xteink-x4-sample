#include <SPI.h>
#include <SD.h>
#include "FileSystem.h"

#define SD_SPI_CS   12
#define SPI_FQ 40000000

FileSystem::FileSystem()
    : sdReady_(false)
{
}

bool FileSystem::begin() {
    if (SD.begin(SD_SPI_CS, SPI, SPI_FQ)) {
        sdReady_ = true;
    } else {
        sdReady_ = false;
    }
    return sdReady_;
}

bool FileSystem::isReady() const {
    return sdReady_;
}

std::vector<FileInfo> FileSystem::readFolder(const char* path) {
    std::vector<FileInfo> entries;

    if (!sdReady_) {
        begin();
        if (!sdReady_) return entries; // return empty if SD init fails
    }

    File dir = SD.open(path);
    if (!dir || !dir.isDirectory()) {
        return entries;
    }

    for (File f = dir.openNextFile(); f; f = dir.openNextFile()) {
        const char* rawName = f.name();
        std::string name;

        // Strip leading path
        const char* basename = rawName;
        const char* slash = strrchr(basename, '/');
        if (slash && *(slash + 1)) {
            basename = slash + 1;
        }

        if (basename) {
            name = basename;
        } else {
            name = "";
        }

        entries.push_back({ name, f.isDirectory() });
        f.close();
    }

    dir.close();
    return entries;
}