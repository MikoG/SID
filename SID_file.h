/* 
 * File:   file.h
 * Author: geth2k
 *
 * Created on 05 May 2013, 16:58
 */

#ifndef FILE_H
#define	FILE_H

#include <fstream>
#include <dirent.h>
#include <vector>
#include <stdexcept>
#include "SID_globals.h"

namespace sid {
namespace file {
    
    std::string get_dir_from_filepath(std::string const& strFilePath);
    
    std::string get_file_from_filepath(std::string const& strFilePath);
    
    bool file_exists(std::string const& strFilePath);
    
    bool dirs_exist(std::vector<std::string> const& paths);
    
    bool dir_exists(std::string const& path);
    
    int directory_contents(std::string const& rootDir,
            std::vector<std::string>& retArray,
            bool excludeBack = true,
            uchar dataType = DT_UNKNOWN);
    
    void file_to_memory(std::string const& strFilePath,
            std::string& contents);
    
} // namespace file
} // namespace sid
#endif	/* FILE_H */

