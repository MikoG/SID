/* 
 * File:   file.cpp
 * Author: geth2k
 * 
 * Created on 05 May 2013, 16:58
 */

#include "SID_file.h"

std::string sid::file::get_dir_from_filepath(std::string const& strFilePath) {
    int iFilePathLength = strFilePath.length();
    int iFileNamePos;
    for (int i = iFilePathLength; i >= 0; i--) {
        if (strFilePath[i] == '/') {
            iFileNamePos = i + 1;
            break;
        }
    }
    std::string strDirPath = strFilePath.substr(0, iFileNamePos);

    return strDirPath;
}

std::string sid::file::get_file_from_filepath(const std::string& strFilePath) {
    int iFilePathLength = strFilePath.length();
    int iFileNamePos = strFilePath.find_last_of("/") + 1;
    
    int iLastCharPos = iFilePathLength - 1;
    std::string strDirPath = strFilePath.substr(iFileNamePos, iLastCharPos);

    return strDirPath;    
}

bool sid::file::file_exists(std::string const& strFilePath) {
    int iFilePathLength = strFilePath.length();
    int iFileNamePos = 0;
    for (int i = iFilePathLength; i >= 0; i--) {
        if (strFilePath[i] == '/') {
            iFileNamePos = i + 1;
            break;
        }
    }
    int iFileNameLength = iFilePathLength - iFileNamePos;
    std::string strFileName = strFilePath.substr(
            iFileNamePos, iFileNameLength);

    std::string strDirPath = strFilePath.substr(0, iFileNamePos);
    std::vector<std::string> dirFiles;
    int iDirCount = directory_contents(strDirPath, dirFiles, true, DT_REG);
    int i = 0;
    while (i < iDirCount) {
        if (dirFiles[i] == strFileName) {
            return true;
        }
        i++;
    }
    return false;
}

bool sid::file::dirs_exist(std::vector<std::string> const& paths) {
    for (std::vector<std::string>::const_iterator itStrPath = paths.begin();
            itStrPath != paths.end();
            itStrPath++) {
        
        if (!dir_exists(*itStrPath)) {
            return false;
        }
    }
    return true;
}

bool sid::file::dir_exists(std::string const& path) {
    DIR *dp;

    dp = opendir(path.c_str());

    if (dp == nullptr) {
        return false;
    } else {
        return true;
    }
}

int sid::file::directory_contents(std::string const& rootDir,
            std::vector<std::string>& retArray,
            bool excludeBack,
            uchar dataType) {

    DIR* dp;
    dirent* ep;
    std::string strDirName;

    dp = opendir(rootDir.c_str());
    int i = 0;
    if (dp != nullptr) {
        while (ep = readdir(dp)) {
            strDirName = ep->d_name;
            if (dataType == DT_UNKNOWN && (!excludeBack ||
                    (strDirName != ".." && strDirName != "."))) {

                retArray.push_back(strDirName);
                i++;
            } else if (ep->d_type == dataType &&
                    (!excludeBack ||
                    (strDirName != ".." && strDirName != "."))) {

                retArray.push_back(strDirName);
                i++;
            }
        }
        closedir(dp);
    } else {
        i = -1;
    }
    return i;
}

void sid::file::file_to_memory(
        std::string const& filePath,
        std::string& contents) {

    std::ifstream File(filePath.c_str(),
            std::ios::in | std::ios::binary | std::ios::ate);

    if (File.is_open()) {
        contents.resize(File.tellg());
        File.seekg(0, std::ios::beg);
        File.read(&contents[0], contents.size());
        File.close();
    } else {
        throw std::runtime_error("Unable to open file \"" + 
        filePath + "\".");
    }
}
