/*
* Copyright 2024 Joel Winarske
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "AppStreamParser.h"
#include <spdlog/spdlog.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * @brief Retrieves the current memory usage of the process.
 *
 * This function reads the memory usage statistics from the `/proc/self/stat` file,
 * which contains various information about the current process. It extracts the
 * virtual memory size (vm_usage) and the resident set size (resident_set) from the file.
 *
 * @param[out] vm_usage The virtual memory usage in kilobytes.
 * @param[out] resident_set The resident set size in kilobytes.
 */
void getMemoryUsage(double &vm_usage, double &resident_set) {
    vm_usage = 0.0;
    resident_set = 0.0;

    std::ifstream file("/proc/self/stat");
    std::string line;
    std::getline(file, line);

    std::istringstream iss(line);

    if (std::vector<std::string> fields((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>())
        ; fields.size() >= 24) {
        unsigned long vsize = std::stoul(fields[22]);
        long rss = std::stol(fields[23]);
        long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024;

        vm_usage = vsize / 1024.0;
        resident_set = rss * page_size_kb;
    }
}

/**
 * @brief Gets the size of a file.
 *
 * This function retrieves the size of the specified file in bytes.
 *
 * @param filename The name of the file whose size is to be determined.
 * @return The size of the file in bytes, or -1 if the file could not be accessed.
 */
long getFileSize(const std::string &filename) {
    struct stat stat_buf{};
    return stat(filename.c_str(), &stat_buf) == 0 ? stat_buf.st_size : -1;
}

int main(const int argc, char *argv[]) {
    if (argc < 2) {
        spdlog::error("Usage: {} <filename> [language]", argv[0]);
        return EXIT_FAILURE;
    }

    std::string filename = argv[1];
    std::string language = (argc >= 3) ? argv[2] : "";

    // Check if the file exists and get file size
    const long filesize = getFileSize(filename);
    if (filesize == -1) {
        spdlog::error("File '{}' not found or could not be accessed.", filename);
        return EXIT_FAILURE;
    }
    double filesize_mib = static_cast<double>(filesize) / (1024.0 * 1024.0);
    spdlog::info("File '{}' is present with size {:.2f} MiB.", filename, filesize_mib);

    double vm_usage, resident_set;

    // Before parser allocation
    getMemoryUsage(vm_usage, resident_set);
    spdlog::info("Before parser allocation - Virtual Memory: {} KB, Resident set size: {} KB", vm_usage, resident_set);

    try {
        spdlog::info("Initializing AppStreamParser with file: '{}' and language: '{}'", filename, language);

        auto parser = std::make_unique<AppStreamParser>(filename, language);

        // After parser allocation
        getMemoryUsage(vm_usage, resident_set);
        spdlog::info("After parser allocation - Virtual Memory: {} KB, Resident set size: {} KB", vm_usage,
                     resident_set);

        spdlog::info("Parsing completed. Total components: {}", parser->getTotalComponentCount());

        // After parsing
        getMemoryUsage(vm_usage, resident_set);
        spdlog::info("After parsing - Virtual Memory: {} KB, Resident set size: {} KB", vm_usage, resident_set);

        const auto categories = parser->getUniqueCategories();
        spdlog::info("Unique Categories:");
        for (const auto &category: categories) {
            spdlog::info("- {}", category);
        }

        // After getting unique categories
        getMemoryUsage(vm_usage, resident_set);
        spdlog::info("After getting unique categories - Virtual Memory: {} KB, Resident set size: {} KB", vm_usage,
                     resident_set);

        const auto keywords = parser->getUniqueKeywords();
        spdlog::info("Unique Keywords:");
        for (const auto &keyword: keywords) {
            spdlog::info("- {}", keyword);
        }

        // After getting unique keywords
        getMemoryUsage(vm_usage, resident_set);
        spdlog::info("After getting unique keywords - Virtual Memory: {} KB, Resident set size: {} KB", vm_usage,
                     resident_set);

        // Example searches
        const std::string sampleCategory = "utility";
        const auto componentsByCategory = parser->searchByCategory(sampleCategory);
        spdlog::info("Components in category '{}', ({}):", sampleCategory, sampleCategory.size());
        for (const auto &component: componentsByCategory) {
            component->Dump();
        }

        // After searching by category
        getMemoryUsage(vm_usage, resident_set);
        spdlog::info("After searching by category - Virtual Memory: {} KB, Resident set size: {} KB", vm_usage,
                     resident_set);

        const std::string sampleKeyword = "editor";
        const auto componentsByKeyword = parser->searchByKeyword(sampleKeyword);
        spdlog::info("Components with keyword '{}', ({}):", sampleKeyword, componentsByKeyword.size());
        for (const auto &comp: componentsByKeyword) {
            comp->Dump();
        }

        const auto components = parser->getComponents();
        //        for (const auto &[fst, snd]: components) {
        //            printComponent(fst, snd);
        //        }
        spdlog::info("Component Count: {}", components.size());

        // After searching by keyword
        getMemoryUsage(vm_usage, resident_set);
        spdlog::info("Before sorting - Virtual Memory: {} KB, Resident set size: {} KB", vm_usage,
                     resident_set);
        const auto sortedById = parser->getSortedComponents(AppStreamParser::SortOption::BY_ID);
        // After searching by keyword
        getMemoryUsage(vm_usage, resident_set);
        spdlog::info("After sorting - Virtual Memory: {} KB, Resident set size: {} KB", vm_usage,
                     resident_set);

        // After searching by keyword
        getMemoryUsage(vm_usage, resident_set);
        spdlog::info("After searching by keyword - Virtual Memory: {} KB, Resident set size: {} KB", vm_usage,
                     resident_set);

        parser.reset();

        getMemoryUsage(vm_usage, resident_set);
        spdlog::info("After reseting parser - Virtual Memory: {} KB, Resident set size: {} KB", vm_usage,
                     resident_set);
    } catch (const std::exception &e) {
        spdlog::error("Exception occurred: {}", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
