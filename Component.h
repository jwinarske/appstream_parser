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

#ifndef COMPONENT_H
#define COMPONENT_H

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>


class Component {
public:
    enum class BundleType {
        UNKNOWN = 0,
        PACKAGE,
        LIMBA,
        FLATPAK,
        APPIMAGE,
        SNAP,
        TARBALL,
        CABINET,
        LINGLONG
    };

    struct Bundle {
        std::string id;
        BundleType type;
    };

    enum class IconType {
        UNKNOWN = 0,
        STOCK,
        CACHED,
        LOCAL,
        URL,
        REMOTE
    };

    struct Icon {
        IconType type;
        std::string value;
        std::optional<int> width;
        std::optional<int> height;
        std::optional<int> scale;
    };

    enum class CompulsoryForDesktop {
        UNKNOWN = 0,
        COSMIC,
        GNOME,
        GNOME_Classic,
        GNOME_Flashback,
        KDE,
        LXDE,
        LXQt,
        MATE,
        Razor,
        ROX,
        TDE,
        Unity,
        XFCE,
        EDE,
        Cinnamon,
        Pantheon,
        DDE,
        Endless,
        Old
    };

    enum class UrlType {
        UNKNOWN = 0,
        HOMEPAGE,
        BUGTRACKER,
        FAQ,
        HELP,
        DONATION,
        TRANSLATE,
        CONTACT,
        VCS_BROWSER,
        CONTRIBUTE
    };

    enum class LaunchableType {
        UNKNOWN,
        DESKTOP_ID,
        SERVICE,
        COCKPIT_MANIFEST,
        URL
    };

    enum class ReleaseType {
        UNKNOWN = 0,
        STABLE,
        DEVELOPMENT,
        SNAPSHOT
    };

    enum class ReleaseUrgency {
        UNKNOWN = 0,
        LOW,
        MEDIUM,
        HIGH,
        CRITICAL
    };

    enum class IssueType {
        UNKNOWN,
        GENERIC,
        CVE
    };

    struct Artifact {
        std::string location;
        std::unordered_map<std::string, std::string> checksum;
        std::unordered_map<std::string, size_t> size;
    };

    struct Issue {
        IssueType type;
        std::string url;
        std::string value;
    };

    struct Release {
        ReleaseType type;
        std::string version;
        std::string date;
        std::string timestamp;
        std::string date_eol;
        ReleaseUrgency urgency;
        std::string description;
        std::string url;
        std::vector<Issue> issues;
        std::vector<Artifact> artifacts;
    };

    std::string id;
    std::string pkgname;
    std::string source_pkgname;
    std::string name;
    std::string summary;
    std::string projectLicense;
    std::string description;

    struct {
        std::string homepage;
        std::string bugtracker;
        std::string faq;
        std::string help;
        std::string donation;
        std::string translate;
        std::string contact;
        std::string vcs_browser;
        std::string contribute;
        std::string unknown;
    } url;

    std::string project_group;
    std::vector<Icon> icons;
    std::vector<CompulsoryForDesktop> compulsory_for_desktop;

    struct {
        std::string id;
        std::string name;
    } developer;

    struct {
        LaunchableType type;
        std::string desktop_id;
        std::string service;
        std::string cockpit_manifest;
        std::string url;
    } launchable;

    std::string media_baseurl;
    std::string architecture;
    Bundle bundle;
    std::string content_rating;
    std::string agreement;
    std::vector<std::string> keywords;
    std::vector<std::string> categories;
    std::vector<std::string> suggests;
    std::vector<Release> releases;
    std::vector<std::string> supportedLanguages;

    void Dump() const;

    void addSupportedLanguage(const std::string &language);

    static BundleType stringToBundleType(const std::string &typeStr);

    static IconType stringToIconType(const std::string &typeStr);

    static CompulsoryForDesktop stringToCompulsoryForDesktop(const std::string &desktopString);

    static UrlType stringToUrlType(const std::string &typeStr);

    static LaunchableType stringToLaunchableType(const std::string &typeStr);

    static ReleaseType stringToReleaseType(const std::string &typeStr);

    static ReleaseUrgency stringToReleaseUrgency(const std::string &typeStr);

    static IssueType stringToIssueType(const std::string &typeStr);

    static std::string bundleTypeToString(BundleType type);

    static std::string iconTypeToString(IconType type);

    static std::string compulsoryForDesktopToString(CompulsoryForDesktop desktopEnum);

    static std::string releaseTypeToString(ReleaseType type);

    static std::string releaseUrgencyToString(ReleaseUrgency type);

    static std::string issueTypeToString(IssueType type);
};

#endif // COMPONENT_H
