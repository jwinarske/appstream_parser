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

#ifndef APPSTREAMPARSER_H
#define APPSTREAMPARSER_H

#include "Component.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <libxml/parser.h>


class AppStreamParser {
public:
    explicit AppStreamParser(const std::string &filename, const std::string &language);

    ~AppStreamParser();

    std::vector<std::string> getUniqueCategories();

    std::vector<std::string> getUniqueKeywords();

    std::vector<std::shared_ptr<Component> > searchByCategory(const std::string &category);

    std::vector<std::shared_ptr<Component> > searchByKeyword(const std::string &keyword);

    enum class SortOption { BY_ID, BY_NAME };

    std::vector<std::shared_ptr<Component> > getSortedComponents(SortOption option);

    [[nodiscard]] size_t getTotalComponentCount() const;

    [[nodiscard]] const std::map<std::string, std::shared_ptr<Component> > &getComponents() const;

private:
    static constexpr char kEmptyString[] = "";
    static constexpr char kReleaseTypeStable[] = "stable";
    static constexpr char kReleaseUrgencyMedium[] = "medium";
    static constexpr char kIssueTypeGeneric[] = "generic";

    std::map<std::string, std::shared_ptr<Component> > components_;
    std::string language_;

    struct ParsingState {
        bool insideComponent = false;
        bool insideReleases = false;
        bool insideIssues = false;
        bool insideArtifact = true;
        bool currentDeveloper = false;

        std::shared_ptr<Component> currentComponent;
        std::string currentElement;
        std::string currentData;

        Component::Icon currentIcon;
        Component::UrlType urlType;
        Component::LaunchableType launchableType;
        Component::Release currentRelease;
        Component::Issue currentIssue;
        Component::Artifact currentArtifact;

        std::string currentArtifactChecksumKey;
        std::string currentArtifactSizeKey;
        std::string language;
    };

    ParsingState state_;

    static void startElementCallback(void *user_data, const xmlChar *name, const xmlChar **attrs);

    static void endElementCallback(void *user_data, const xmlChar *name);

    static void charactersCallback(void *user_data, const xmlChar *ch, int len);

    size_t fileSize_ = 0;
    void *fileData_ = nullptr;

    void parseFile(const std::string &filename);

    void mmapFile(const std::string &filename);

    void munmapFile();
};

#endif // APPSTREAMPARSER_H
