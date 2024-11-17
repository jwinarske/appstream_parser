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

#include "Component.h"

#include "spdlog/spdlog.h"

Component::BundleType Component::stringToBundleType(const std::string &typeStr) {
    if (typeStr == "package") return BundleType::PACKAGE;
    if (typeStr == "limba") return BundleType::LIMBA;
    if (typeStr == "flatpak") return BundleType::FLATPAK;
    if (typeStr == "appimage") return BundleType::APPIMAGE;
    if (typeStr == "snap") return BundleType::SNAP;
    if (typeStr == "tarball") return BundleType::TARBALL;
    if (typeStr == "cabinet") return BundleType::CABINET;
    if (typeStr == "linglong") return BundleType::LINGLONG;
    return BundleType::UNKNOWN;
}

std::string Component::bundleTypeToString(const BundleType type) {
    switch (type) {
        case BundleType::PACKAGE: return "package";
        case BundleType::LIMBA: return "limba";
        case BundleType::FLATPAK: return "flatpak";
        case BundleType::APPIMAGE: return "appimage";
        case BundleType::SNAP: return "snap";
        case BundleType::TARBALL: return "tarball";
        case BundleType::CABINET: return "cabinet";
        case BundleType::LINGLONG: return "linglong";
        default: return "unknown";
    }
}

Component::IconType Component::stringToIconType(const std::string &typeStr) {
    if (typeStr == "stock") return Component::IconType::STOCK;
    if (typeStr == "cached") return Component::IconType::CACHED;
    if (typeStr == "local") return Component::IconType::LOCAL;
    if (typeStr == "url") return Component::IconType::URL;
    if (typeStr == "remote") return Component::IconType::REMOTE;
    return IconType::UNKNOWN;
}

std::string Component::iconTypeToString(const IconType type) {
    switch (type) {
        case IconType::STOCK: return "stock";
        case IconType::CACHED: return "cached";
        case IconType::LOCAL: return "local";
        case IconType::URL: return "url";
        case IconType::REMOTE: return "remote";
        default: return "unknown";
    }
}

Component::CompulsoryForDesktop Component::stringToCompulsoryForDesktop(const std::string &desktopString) {
    static const std::unordered_map<std::string, CompulsoryForDesktop> stringToEnumMap = {
        {"COSMIC", CompulsoryForDesktop::COSMIC},
        {"GNOME", CompulsoryForDesktop::GNOME},
        {"GNOME-Classic", CompulsoryForDesktop::GNOME_Classic},
        {"GNOME-Flashback", CompulsoryForDesktop::GNOME_Flashback},
        {"KDE", CompulsoryForDesktop::KDE},
        {"LXDE", CompulsoryForDesktop::LXDE},
        {"LXQt", CompulsoryForDesktop::LXQt},
        {"MATE", CompulsoryForDesktop::MATE},
        {"Razor", CompulsoryForDesktop::Razor},
        {"ROX", CompulsoryForDesktop::ROX},
        {"TDE", CompulsoryForDesktop::TDE},
        {"Unity", CompulsoryForDesktop::Unity},
        {"XFCE", CompulsoryForDesktop::XFCE},
        {"EDE", CompulsoryForDesktop::EDE},
        {"Cinnamon", CompulsoryForDesktop::Cinnamon},
        {"Pantheon", CompulsoryForDesktop::Pantheon},
        {"DDE", CompulsoryForDesktop::DDE},
        {"Endless", CompulsoryForDesktop::Endless},
        {"Old", CompulsoryForDesktop::Old}
    };

    if (const auto it = stringToEnumMap.find(desktopString); it != stringToEnumMap.end()) {
        return it->second;
    }
    return CompulsoryForDesktop::UNKNOWN;
}

std::string Component::compulsoryForDesktopToString(const CompulsoryForDesktop desktopEnum) {
    static const std::unordered_map<CompulsoryForDesktop, std::string> enumToStringMap = {
        {CompulsoryForDesktop::COSMIC, "COSMIC"},
        {CompulsoryForDesktop::GNOME, "GNOME"},
        {CompulsoryForDesktop::GNOME_Classic, "GNOME-Classic"},
        {CompulsoryForDesktop::GNOME_Flashback, "GNOME-Flashback"},
        {CompulsoryForDesktop::KDE, "KDE"},
        {CompulsoryForDesktop::LXDE, "LXDE"},
        {CompulsoryForDesktop::LXQt, "LXQt"},
        {CompulsoryForDesktop::MATE, "MATE"},
        {CompulsoryForDesktop::Razor, "Razor"},
        {CompulsoryForDesktop::ROX, "ROX"},
        {CompulsoryForDesktop::TDE, "TDE"},
        {CompulsoryForDesktop::Unity, "Unity"},
        {CompulsoryForDesktop::XFCE, "XFCE"},
        {CompulsoryForDesktop::EDE, "EDE"},
        {CompulsoryForDesktop::Cinnamon, "Cinnamon"},
        {CompulsoryForDesktop::Pantheon, "Pantheon"},
        {CompulsoryForDesktop::DDE, "DDE"},
        {CompulsoryForDesktop::Endless, "Endless"},
        {CompulsoryForDesktop::Old, "Old"}
    };

    if (const auto it = enumToStringMap.find(desktopEnum); it != enumToStringMap.end()) {
        return it->second;
    }
    return "unknown";
}

Component::UrlType Component::stringToUrlType(const std::string &typeStr) {
    if (typeStr == "homepage") return UrlType::HOMEPAGE;
    if (typeStr == "bugtracker") return UrlType::BUGTRACKER;
    if (typeStr == "faq") return UrlType::FAQ;
    if (typeStr == "help") return UrlType::HELP;
    if (typeStr == "donation") return UrlType::DONATION;
    if (typeStr == "translate") return UrlType::TRANSLATE;
    if (typeStr == "contact") return UrlType::CONTACT;
    if (typeStr == "vcs-browser") return UrlType::VCS_BROWSER;
    if (typeStr == "contribute") return UrlType::CONTRIBUTE;
    return UrlType::UNKNOWN;
}

Component::LaunchableType Component::stringToLaunchableType(const std::string &typeStr) {
    if (typeStr == "desktop-id") return LaunchableType::DESKTOP_ID;
    if (typeStr == "service") return LaunchableType::SERVICE;
    if (typeStr == "cockpit-manifest") return LaunchableType::COCKPIT_MANIFEST;
    if (typeStr == "url") return LaunchableType::URL;
    return LaunchableType::UNKNOWN;
}

Component::ReleaseType Component::stringToReleaseType(const std::string &typeStr) {
    if (typeStr == "stable") return ReleaseType::STABLE;
    if (typeStr == "development") return ReleaseType::DEVELOPMENT;
    if (typeStr == "snapshot") return ReleaseType::SNAPSHOT;
    return ReleaseType::UNKNOWN;
}

Component::ReleaseUrgency Component::stringToReleaseUrgency(const std::string &typeStr) {
    if (typeStr == "low") return ReleaseUrgency::LOW;
    if (typeStr == "medium") return ReleaseUrgency::MEDIUM;
    if (typeStr == "high") return ReleaseUrgency::HIGH;
    if (typeStr == "critical") return ReleaseUrgency::CRITICAL;
    return ReleaseUrgency::UNKNOWN;
}

std::string Component::releaseUrgencyToString(const ReleaseUrgency type) {
    if (type == ReleaseUrgency::LOW) return "low";
    if (type == ReleaseUrgency::MEDIUM) return "medium";
    if (type == ReleaseUrgency::HIGH) return "high";
    if (type == ReleaseUrgency::CRITICAL) return "critical";
    return "unknown";
}

Component::IssueType Component::stringToIssueType(const std::string &typeStr) {
    if (typeStr == "generic") return IssueType::GENERIC;
    if (typeStr == "cve") return IssueType::CVE;
    return IssueType::UNKNOWN;
}

std::string Component::releaseTypeToString(const ReleaseType type) {
    switch (type) {
        case ReleaseType::STABLE: return "stable";
        case ReleaseType::SNAPSHOT: return "snapshot";
        case ReleaseType::DEVELOPMENT: return "development";
        default: return "unknown";
    }
}

std::string Component::issueTypeToString(const IssueType type) {
    switch (type) {
        case IssueType::GENERIC: return "generic";
        case IssueType::CVE: return "cve";
        default: return "unknown";
    }
}

void Component::addSupportedLanguage(const std::string &language) {
    supportedLanguages.push_back(language);
}

void Component::Dump() const {
    spdlog::info("id: {}", id);
    spdlog::info("\tname: {}", name);
    spdlog::info("\tproject_license: {}", projectLicense);
    spdlog::info("\tsummary: {}", summary);
    if (!pkgname.empty()) {
        spdlog::info("\tpkgname: {}", pkgname);
    }
    if (!source_pkgname.empty()) {
        spdlog::info("\tsource_pkgname: {}", source_pkgname);
    }
    if (!description.empty()) {
        spdlog::info("\tdescription: {}", description);
    }
    if (!url.homepage.empty()) {
        spdlog::info("\thomepage: {}", url.homepage);
    }
    if (!url.bugtracker.empty()) {
        spdlog::info("\tbugtracker: {}", url.bugtracker);
    }
    if (!url.faq.empty()) {
        spdlog::info("\tfaq: {}", url.faq);
    }
    if (!url.help.empty()) {
        spdlog::info("\thelp: {}", url.help);
    }
    if (!url.donation.empty()) {
        spdlog::info("\tdonation: {}", url.donation);
    }
    if (!url.translate.empty()) {
        spdlog::info("\ttranslate: {}", url.translate);
    }
    if (!url.contact.empty()) {
        spdlog::info("\tcontact: {}", url.contact);
    }
    if (!url.vcs_browser.empty()) {
        spdlog::info("\tvcs_browser: {}", url.vcs_browser);
    }
    if (!url.contribute.empty()) {
        spdlog::info("\tcontribute: {}", url.contribute);
    }
    if (!url.unknown.empty()) {
        spdlog::info("\tunknown: {}", url.unknown);
    }
    if (!project_group.empty()) {
        spdlog::info("\tproject_group: {}", project_group);
    }
    if (!developer.id.empty()) {
        spdlog::info("\tdeveloper id: {}", developer.id);
    }
    if (!developer.name.empty()) {
        spdlog::info("\tdeveloper name: {}", developer.name);
    }
    if (!launchable.desktop_id.empty()) {
        spdlog::info("\tlaunchable desktop_id: {}", launchable.desktop_id);
    }
    if (!launchable.service.empty()) {
        spdlog::info("\tlaunchable service: {}", launchable.service);
    }
    if (!launchable.cockpit_manifest.empty()) {
        spdlog::info("\tlaunchable cockpit_manifest: {}", launchable.cockpit_manifest);
    }
    if (!launchable.url.empty()) {
        spdlog::info("\tlaunchable url: {}", launchable.url);
    }
    for (const auto &comp: compulsory_for_desktop) {
        spdlog::info("compulsory_for_desktop: {}", Component::compulsoryForDesktopToString(comp));
    }
    for (const auto &[type, value, width, height, scale]: icons) {
        spdlog::info("\ticon");
        spdlog::info("\t\ttype: {}", Component::iconTypeToString(type));
        spdlog::info("\t\tvalue: {}", value);
        if (width.has_value()) {
            spdlog::info("\t\twidth: {}", width.value());
        }
        if (height.has_value()) {
            spdlog::info("\t\theight: {}", height.value());
        }
        if (scale.has_value()) {
            spdlog::info("\t\tscale: {}", scale.value());
        }
    }
    for (const auto &[type, version, date, timestamp, date_eol, urgency, description, url, issues, artifacts]:
         releases) {
        spdlog::info("\trelease");
        spdlog::info("\t\ttype: {}", releaseTypeToString(type));
        spdlog::info("\t\tversion: {}", version);
        if (!date.empty()) {
            spdlog::info("\t\tdate: {}", date);
        }
        if (!timestamp.empty()) {
            spdlog::info("\t\ttimestamp: {}", timestamp);
        }
        if (!date_eol.empty()) {
            spdlog::info("\t\tdate_eol: {}", date_eol);
        }
        spdlog::info("\t\turgency: {}", releaseUrgencyToString(urgency));
        if (!description.empty()) {
            spdlog::info("\t\tdescription: {}", description);
        }
        if (!url.empty()) {
            spdlog::info("\t\turl: {}", url);
        }
        for (const auto &[type, url, value]: issues) {
            spdlog::info("\t\tissue");
            spdlog::info("\t\t\ttype: {}", issueTypeToString(type));
            if (!url.empty()) {
                spdlog::info("\t\t\turl: {}", url);
            }
            if (!value.empty()) {
                spdlog::info("\t\t\tvalue: {}", value);
            }
        }
        for (const auto &[location, checksum, size]: artifacts) {
            spdlog::info("\t\tartifact");
            if (!location.empty()) {
                spdlog::info("\t\t\tlocation: {}", location);
            }
            if (!checksum.empty()) {
                for (const auto &[key, value]: checksum) {
                    spdlog::info("\t\t\tchecksum: {} = {}", key, value);
                }
            }
            if (!size.empty()) {
                for (const auto &[key, value]: size) {
                    spdlog::info("\t\t\tsize: {} = {}", key, value);
                }
            }
        }
    }
}
