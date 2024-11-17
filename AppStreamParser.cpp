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
#include <libxml/parser.h>
#include <spdlog/spdlog.h>
#include <set>
#include <cassert>
#include <cstring>
#include <algorithm>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iomanip>
#include <unistd.h>
#include <unordered_set>

// Define the chunk size
constexpr size_t CHUNK_SIZE = 1024;

int convertToInt(const std::string &str) {
    try {
        const int result = std::stoi(str);
        return result;
    } catch (const std::invalid_argument &e) {
        spdlog::error("Invalid argument: could not convert to int: {}", str);
        throw;
    } catch (const std::out_of_range &e) {
        spdlog::error("Out of range: value is too large to fit in an int:  {}", str);
        throw;
    }
}

size_t convertToSizeT(const char *str) {
    char *endPtr;
    errno = 0;
    const long long result = std::strtoll(str, &endPtr, 10);

    // Check for various possible errors
    if ((errno == ERANGE && (result == LLONG_MAX || result == LLONG_MIN)) ||
        (errno != 0 && result == 0)) {
        std::perror("Conversion error");
        return 0; // Or handle the error case more gracefully
    }

    if (endPtr == str) {
        spdlog::error("Invalid argument: could not convert to size_t: {}", str);
        return 0; // Or handle the error case more gracefully
    }

    if (result < 0 || static_cast<uint64_t>(result) > std::numeric_limits<size_t>::max()) {
        spdlog::error("Value out of range: {}", str);
        return 0; // Or handle the error case more gracefully
    }

    return result;
}

// Function to parse a const char* as a Unix timestamp and convert to ISO 8601
std::string unixEpochToISO8601(const char *epochStr) {
    // Convert the input const char* to a time_t
    const std::time_t epoch = std::stoll(epochStr);

    // Convert epoch to a time_point
    const std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(epoch);

    // Convert time_point to std::time_t to use std::gmtime
    const std::time_t time = std::chrono::system_clock::to_time_t(tp);
    const std::tm tm = *std::gmtime(&time);

    // Use stringstream to format the time as ISO 8601
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S") << 'Z';

    return ss.str();
}

void AppStreamParser::startElementCallback(void *user_data, const xmlChar *name, const xmlChar **attrs) {
    auto *parser = static_cast<AppStreamParser *>(user_data);
    const auto tag = reinterpret_cast<const char *>(name);
    parser->state_.currentElement = reinterpret_cast<const char *>(name);
    parser->state_.currentData.clear();

    if (strcmp(tag, "component") == 0) {
        parser->state_.insideComponent = true;
        parser->state_.currentComponent = Component();
        return;
    }

    if (strcmp(tag, "releases") == 0) {
        parser->state_.insideReleases = true;
        return;
    }

    if (parser->state_.insideReleases) {
        if (strcmp(tag, "release") == 0) {
            parser->state_.currentRelease = Component::Release();
            // spec defaults
            parser->state_.currentRelease.type = Component::ReleaseType::STABLE;
            parser->state_.currentRelease.urgency = Component::ReleaseUrgency::MEDIUM;
            if (attrs) {
                for (int i = 0; attrs[i]; i += 2) {
                    const auto key = reinterpret_cast<const char *>(attrs[i]);
                    const auto value = reinterpret_cast<const char *>(attrs[i + 1]);
                    long unsigned int value_len = xmlStrlen(attrs[i + 1]);
                    if (strcmp(key, "type") == 0) {
                        parser->state_.currentRelease.type = Component::stringToReleaseType({value, value_len});
                    } else if (strcmp(key, "version") == 0) {
                        parser->state_.currentRelease.version = {value, value_len};
                    } else if (strcmp(key, "date") == 0) {
                        parser->state_.currentRelease.date = {value, value_len};
                    } else if (strcmp(key, "timestamp") == 0) {
                        parser->state_.currentRelease.timestamp = unixEpochToISO8601(value);
                    } else if (strcmp(key, "date_eol") == 0) {
                        parser->state_.currentRelease.date_eol = {value, value_len};
                    } else if (strcmp(key, "urgency") == 0) {
                        parser->state_.currentRelease.urgency = Component::stringToReleaseUrgency({value, value_len});
                    }
                }
            }
            return;
        }
        if (strcmp(tag, "issues") == 0) {
            parser->state_.insideIssues = true;
            return;
        }
        if (strcmp(tag, "issue") == 0) {
            parser->state_.currentIssue = Component::Issue();
            // spec default
            if (attrs) {
                for (int i = 0; attrs[i]; i += 2) {
                    const auto key = reinterpret_cast<const char *>(attrs[i]);
                    if (strcmp(key, "type") == 0) {
                        const auto value = reinterpret_cast<const char *>(attrs[i + 1]);
                        long unsigned int value_len = xmlStrlen(attrs[i + 1]);
                        parser->state_.currentIssue.type = Component::stringToIssueType({value, value_len});
                    }
                    if (strcmp(key, "url") == 0) {
                        const auto value = reinterpret_cast<const char *>(attrs[i + 1]);
                        long unsigned int value_len = xmlStrlen(attrs[i + 1]);
                        parser->state_.currentIssue.url = {value, value_len};
                    }
                }
            }
            return;
        }
        if (strcmp(tag, "artifact") == 0) {
            parser->state_.insideArtifact = true;
            parser->state_.currentArtifact = Component::Artifact();
        }
    }

    if (strcmp(tag, "icon") == 0) {
        parser->state_.currentIcon = Component::Icon();
        if (attrs) {
            for (int i = 0; attrs[i]; i += 2) {
                const auto key = reinterpret_cast<const char *>(attrs[i]);
                const auto value = reinterpret_cast<const char *>(attrs[i + 1]);
                long unsigned int value_len = xmlStrlen(attrs[i + 1]);
                if (strcmp(key, "type") == 0) {
                    parser->state_.currentIcon.type = Component::stringToIconType({value, value_len});
                } else if (strcmp(key, "width") == 0) {
                    parser->state_.currentIcon.width = convertToInt({value, value_len});
                } else if (strcmp(key, "height") == 0) {
                    parser->state_.currentIcon.height = convertToInt({value, value_len});
                } else if (strcmp(key, "scale") == 0) {
                    parser->state_.currentIcon.scale = convertToInt({value, value_len});
                }
            }
        }
        return;
    }

    if (attrs) {
        for (int i = 0; attrs[i]; i += 2) {
            const auto key = reinterpret_cast<const char *>(attrs[i]);
            const auto value = reinterpret_cast<const char *>(attrs[i + 1]);
            long unsigned int value_len = xmlStrlen(attrs[i + 1]);
            if (strcmp(key, "xml:lang") == 0) {
                if (const std::string lang(value);
                    !parser->state_.language.empty() && lang != parser->state_.language) {
                    parser->state_.currentElement = {};
                }
                break;
            }
            if (strcmp(tag, "developer") == 0 && strcmp(key, "id") == 0) {
                parser->state_.currentComponent.developer.id = value;
                parser->state_.currentDeveloper = true;
                break;
            }
            if (strcmp(tag, "bundle") == 0 && strcmp(key, "type") == 0) {
                parser->state_.currentComponent.bundle.type = Component::stringToBundleType({value, value_len});
                break;
            }
            if (strcmp(tag, "url") == 0 && strcmp(key, "type") == 0) {
                parser->state_.urlType = Component::stringToUrlType({value, value_len});
                break;
            }
            if (strcmp(tag, "launchable") == 0 && strcmp(key, "type") == 0) {
                parser->state_.launchableType = Component::stringToLaunchableType({value, value_len});
                break;
            }
        }
    }
}

void AppStreamParser::endElementCallback(void *user_data, const xmlChar *name) {
    auto *parser = static_cast<AppStreamParser *>(user_data);
    const std::string currentElement(reinterpret_cast<const char *>(name));

    if (parser->state_.insideComponent) {
        if (currentElement == "id") {
            parser->state_.currentComponent.id = parser->state_.currentData;
        } else if (currentElement == "pkgname") {
            parser->state_.currentComponent.pkgname = parser->state_.currentData;
        } else if (currentElement == "source_pkgname") {
            parser->state_.currentComponent.source_pkgname = parser->state_.currentData;
        } else if (currentElement == "name") {
            if (parser->state_.currentDeveloper) {
                parser->state_.currentComponent.developer.name = parser->state_.currentData;
            } else {
                parser->state_.currentComponent.name = parser->state_.currentData;
            }
        } else if (currentElement == "project_license") {
            parser->state_.currentComponent.projectLicense = parser->state_.currentData;
        } else if (currentElement == "summary") {
            parser->state_.currentComponent.summary = parser->state_.currentData;
        } else if (currentElement == "description") {
            if (parser->state_.insideReleases) {
                parser->state_.currentRelease.description = parser->state_.currentData;
            } else {
                parser->state_.currentComponent.description = parser->state_.currentData;
            }
        } else if (currentElement == "url") {
            if (parser->state_.insideReleases) {
                parser->state_.currentRelease.url = parser->state_.currentData;
            } else {
                if (parser->state_.urlType == Component::UrlType::HELP) {
                    parser->state_.currentComponent.url.help = parser->state_.currentData;
                } else if (parser->state_.urlType == Component::UrlType::CONTACT) {
                    parser->state_.currentComponent.url.contact = parser->state_.currentData;
                } else if (parser->state_.urlType == Component::UrlType::DONATION) {
                    parser->state_.currentComponent.url.donation = parser->state_.currentData;
                } else if (parser->state_.urlType == Component::UrlType::HOMEPAGE) {
                    parser->state_.currentComponent.url.homepage = parser->state_.currentData;
                } else if (parser->state_.urlType == Component::UrlType::TRANSLATE) {
                    parser->state_.currentComponent.url.translate = parser->state_.currentData;
                } else if (parser->state_.urlType == Component::UrlType::FAQ) {
                    parser->state_.currentComponent.url.faq = parser->state_.currentData;
                } else if (parser->state_.urlType == Component::UrlType::BUGTRACKER) {
                    parser->state_.currentComponent.url.bugtracker = parser->state_.currentData;
                } else if (parser->state_.urlType == Component::UrlType::CONTRIBUTE) {
                    parser->state_.currentComponent.url.contribute = parser->state_.currentData;
                } else if (parser->state_.urlType == Component::UrlType::VCS_BROWSER) {
                    parser->state_.currentComponent.url.vcs_browser = parser->state_.currentData;
                } else {
                    parser->state_.currentComponent.url.unknown = parser->state_.currentData;
                }
            }
        } else if (currentElement == "project_group") {
            parser->state_.currentComponent.project_group = parser->state_.currentData;
        } else if (currentElement == "compulsory_for_desktop") {
            parser->state_.currentComponent.compulsory_for_desktop.push_back(
                Component::stringToCompulsoryForDesktop(parser->state_.currentData));
        } else if (currentElement == "developer") {
            parser->state_.currentDeveloper = false;
        } else if (currentElement == "launchable") {
            if (parser->state_.launchableType == Component::LaunchableType::URL) {
                parser->state_.currentComponent.launchable.url = parser->state_.currentData;
            } else if (parser->state_.launchableType == Component::LaunchableType::SERVICE) {
                parser->state_.currentComponent.launchable.service = parser->state_.currentData;
            } else if (parser->state_.launchableType == Component::LaunchableType::DESKTOP_ID) {
                parser->state_.currentComponent.launchable.desktop_id = parser->state_.currentData;
            } else if (parser->state_.launchableType == Component::LaunchableType::COCKPIT_MANIFEST) {
                parser->state_.currentComponent.launchable.cockpit_manifest = parser->state_.currentData;
            } else {
                spdlog::error("Unknown launchable type: {}", parser->state_.currentData);
            }
        } else if (currentElement == "artifact") {
            parser->state_.insideArtifact = false;
            parser->state_.currentRelease.artifacts.push_back(parser->state_.currentArtifact);
        } else if (parser->state_.insideArtifact && currentElement == "location") {
            parser->state_.currentArtifact.location = parser->state_.currentData;
        } else if (parser->state_.insideArtifact && currentElement == "checksum") {
            parser->state_.currentArtifact.checksum[parser->state_.currentArtifactChecksumKey] = parser->state_.
                    currentData;
        } else if (parser->state_.insideArtifact && currentElement == "size") {
            parser->state_.currentArtifact.size[parser->state_.currentArtifactSizeKey] = convertToSizeT(
                parser->state_.currentData.c_str());
        } else if (currentElement == "bundle") {
            parser->state_.currentComponent.bundle.id = parser->state_.currentData;
        } else if (currentElement == "content_rating") {
            parser->state_.currentComponent.content_rating = parser->state_.currentData;
        } else if (currentElement == "agreement") {
            parser->state_.currentComponent.agreement = parser->state_.currentData;
        } else if (currentElement == "keyword") {
            parser->state_.currentComponent.keywords.push_back(parser->state_.currentData);
        } else if (currentElement == "category") {
            parser->state_.currentComponent.categories.push_back(parser->state_.currentData);
        } else if (currentElement == "icon") {
            parser->state_.currentIcon.value = parser->state_.currentData;
            parser->state_.currentComponent.icons.push_back(parser->state_.currentIcon);
        } else if (currentElement == "suggest") {
            parser->state_.currentComponent.suggests.push_back(parser->state_.currentData);
        } else if (currentElement == "media_baseurl") {
            parser->state_.currentComponent.media_baseurl = parser->state_.currentData;
        } else if (currentElement == "architecture") {
            parser->state_.currentComponent.architecture = parser->state_.currentData;
        } else if (currentElement == "releases") {
            parser->state_.insideReleases = false;
        } else if (currentElement == "release") {
            parser->state_.currentComponent.releases.push_back(parser->state_.currentRelease);
        } else if (currentElement == "issues") {
            parser->state_.insideIssues = false;
        } else if (currentElement == "issue") {
            parser->state_.currentRelease.issues.push_back(parser->state_.currentIssue);
        } else if (currentElement == "language") {
            parser->state_.currentComponent.addSupportedLanguage(parser->state_.currentData);
        } else if (currentElement == "component") {
            parser->state_.insideComponent = false;
            assert(!parser->state_.currentComponent.id.empty());
            if (!parser->components_.count(parser->state_.currentComponent.id)) {
                parser->components_[parser->state_.currentComponent.id] = parser->state_.currentComponent;
            } else {
                SPDLOG_WARN("Duplicate: [{}]", parser->state_.currentComponent.id);
            }
        }
    }

    parser->state_.currentData.clear();
    parser->state_.currentElement = {};
    parser->state_.currentIcon = {};
}

void AppStreamParser::charactersCallback(void *user_data, const xmlChar *ch, const int len) {
    spdlog::debug("{}", std::string_view(reinterpret_cast<const char *>(ch), len));
    auto *parser = static_cast<AppStreamParser *>(user_data);
    if (!parser->state_.currentElement.empty()) {
        parser->state_.currentData.append(reinterpret_cast<const char *>(ch), len);
    }
}

AppStreamParser::AppStreamParser(const std::string &filename, const std::string &language)
    : language_(language) {
    state_.language = language;
    parseFile(filename);
}

AppStreamParser::~AppStreamParser() {
    munmapFile();
}

void AppStreamParser::mmapFile(const std::string &filename) {
    int fd = open(filename.c_str(), O_RDONLY);
    if (fd == -1) {
        spdlog::error("Failed to open file: {}", filename);
        exit(EXIT_FAILURE);
    }

    struct stat sb{};
    if (fstat(fd, &sb) == -1) {
        spdlog::error("Failed to get file size: {}", filename);
        close(fd);
        exit(EXIT_FAILURE);
    }
    fileSize_ = sb.st_size;

    fileData_ = mmap(nullptr, fileSize_, PROT_READ, MAP_PRIVATE, fd, 0);
    // Close the file descriptor as it's no longer needed after mapping
    close(fd);
    if (fileData_ == MAP_FAILED) {
        spdlog::error("Failed to memory-map file: {}", filename);
        exit(EXIT_FAILURE);
    }
}

void AppStreamParser::munmapFile() {
    if (fileData_ && fileData_ != MAP_FAILED) {
        munmap(fileData_, fileSize_);
        fileData_ = nullptr;
    }
}

void AppStreamParser::parseFile(const std::string &filename) {
    mmapFile(filename);

    xmlSAXHandler saxHandler = {
        .startElement = startElementCallback,
        .endElement = endElementCallback,
        .characters = charactersCallback,
    };

    spdlog::info("Parsing file: {}", filename);

    std::unique_ptr<xmlParserCtxt, decltype(&xmlFreeParserCtxt)> ctxt(
        xmlCreatePushParserCtxt(&saxHandler, this, static_cast<const char *>(fileData_), 4, filename.c_str()),
        xmlFreeParserCtxt);

    size_t offset = 4;
    while (offset < fileSize_) {
        const size_t chunkSize = std::min(CHUNK_SIZE, fileSize_ - offset);
        if (int ret = xmlParseChunk(ctxt.get(), static_cast<const char *>(fileData_) + offset,
                                    static_cast<int>(chunkSize), 0);
            ret != 0) {
            spdlog::error("Failed to parse XML, error code: {}", ret);
            munmapFile();
            exit(EXIT_FAILURE);
        }
        offset += chunkSize;
    }

    // Send an EOF indication
    if (int ret = xmlParseChunk(ctxt.get(), nullptr, 0, 1); ret != 0) {
        spdlog::error("Failed to parse XML, error code: {}", ret);
        munmapFile();
        exit(EXIT_FAILURE);
    }

    munmapFile();
}

std::vector<std::string> AppStreamParser::getUniqueCategories() {
    std::unordered_set<std::string> uniqueCategories;

    for (const auto &[key, component]: components_) {
        uniqueCategories.insert(component.categories.begin(), component.categories.end());
    }

    return {uniqueCategories.begin(), uniqueCategories.end()};
}

std::vector<std::string> AppStreamParser::getUniqueKeywords() {
    std::unordered_set<std::string> uniqueKeywords;

    for (const auto &[key, component]: components_) {
        uniqueKeywords.insert(component.keywords.begin(), component.keywords.end());
    }

    return {uniqueKeywords.begin(), uniqueKeywords.end()};
}

std::vector<Component> AppStreamParser::getSortedComponents(const SortOption option) {
    // Convert unordered_map to vector for sorting
    std::vector<Component> sortedComponents;
    sortedComponents.reserve(components_.size());
    for (const auto &[key, component]: components_) {
        sortedComponents.push_back(component);
    }

    // Sort based on the option
    switch (option) {
        case SortOption::BY_ID:
            std::sort(sortedComponents.begin(), sortedComponents.end(), [](const Component &a, const Component &b) {
                return a.id < b.id;
            });
            break;
        case SortOption::BY_NAME:
            std::sort(sortedComponents.begin(), sortedComponents.end(), [](const Component &a, const Component &b) {
                return a.name < b.name;
            });
            break;
        default:
            throw std::invalid_argument("Invalid sort option");
    }

    return sortedComponents;
}

std::vector<Component> AppStreamParser::searchByCategory(const std::string &category) {
    std::vector<Component> result;

    for (const auto &[key, component]: components_) {
        if (std::find(component.categories.begin(), component.categories.end(), category) != component.categories.
            end()) {
            result.push_back(component);
        }
    }

    return result;
}

std::vector<Component> AppStreamParser::searchByKeyword(const std::string &keyword) {
    std::vector<Component> result;

    for (const auto &[key, component]: components_) {
        if (std::find(component.keywords.begin(), component.keywords.end(), keyword) != component.keywords.end()) {
            result.push_back(component);
        }
    }

    return result;
}

size_t AppStreamParser::getTotalComponentCount() const {
    return components_.size();
}

const std::map<std::string, Component> &AppStreamParser::getComponents() const {
    return components_;
}
