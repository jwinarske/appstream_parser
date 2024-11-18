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

constexpr char kPackage[] = "package";
constexpr char kLimba[] = "limba";
constexpr char kFlatpak[] = "flatpak";
constexpr char kAppimage[] = "appimage";
constexpr char kSnap[] = "snap";
constexpr char kTarball[] = "tarball";
constexpr char kCabinet[] = "cabinet";
constexpr char kLinglong[] = "linglong";
constexpr char kUnknown[] = "unknown";

constexpr char kStock[] = "stock";
constexpr char kCached[] = "cached";
constexpr char kLocal[] = "local";
constexpr char kUrl[] = "url";
constexpr char kRemote[] = "remote";

constexpr char kCosmic[] = "COSMIC";
constexpr char kGnome[] = "GNOME";
constexpr char kGnomeClassic[] = "GNOME-Classic";
constexpr char kGnomeFlashback[] = "GNOME-Flashback";
constexpr char kKde[] = "KDE";
constexpr char kLxde[] = "LXDE";
constexpr char kLxqt[] = "LXQt";
constexpr char kMate[] = "MATE";
constexpr char kRazor[] = "Razor";
constexpr char kRox[] = "ROX";
constexpr char kTde[] = "TDE";
constexpr char kUnity[] = "Unity";
constexpr char kXfce[] = "XFCE";
constexpr char kEde[] = "EDE";
constexpr char kCinnamon[] = "Cinnamon";
constexpr char kPantheon[] = "Pantheon";
constexpr char kDde[] = "DDE";
constexpr char kEndless[] = "Endless";
constexpr char kOld[] = "Old";

constexpr char kHomepage[] = "homepage";
constexpr char kBugtracker[] = "bugtracker";
constexpr char kFaq[] = "faq";
constexpr char kHelp[] = "help";
constexpr char kDonation[] = "donation";
constexpr char kTranslate[] = "translate";
constexpr char kContact[] = "contact";
constexpr char kVcsBrowser[] = "vcs-browser";
constexpr char kContribute[] = "contribute";

constexpr char kDesktopId[] = "desktop-id";
constexpr char kService[] = "service";
constexpr char kCockpitManifest[] = "cockpit-manifest";

constexpr char kStable[] = "stable";
constexpr char kDevelopment[] = "development";
constexpr char kSnapshot[] = "snapshot";

constexpr char kLow[] = "low";
constexpr char kMedium[] = "medium";
constexpr char kHigh[] = "high";
constexpr char kCritical[] = "critical";

constexpr char kGeneric[] = "generic";
constexpr char kCve[] = "cve";

Component::BundleType Component::stringToBundleType(const std::string &typeStr) {
    if (typeStr == kPackage) return BundleType::PACKAGE;
    if (typeStr == kLimba) return BundleType::LIMBA;
    if (typeStr == kFlatpak) return BundleType::FLATPAK;
    if (typeStr == kAppimage) return BundleType::APPIMAGE;
    if (typeStr == kSnap) return BundleType::SNAP;
    if (typeStr == kTarball) return BundleType::TARBALL;
    if (typeStr == kCabinet) return BundleType::CABINET;
    if (typeStr == kLinglong) return BundleType::LINGLONG;
    return BundleType::UNKNOWN;
}

std::string Component::bundleTypeToString(const BundleType type) {
    switch (type) {
        case BundleType::PACKAGE: return kPackage;
        case BundleType::LIMBA: return kLimba;
        case BundleType::FLATPAK: return kFlatpak;
        case BundleType::APPIMAGE: return kAppimage;
        case BundleType::SNAP: return kSnap;
        case BundleType::TARBALL: return kTarball;
        case BundleType::CABINET: return kCabinet;
        case BundleType::LINGLONG: return kLinglong;
        default: return kUnknown;
    }
}

Component::IconType Component::stringToIconType(const std::string &typeStr) {
    if (typeStr == kStock) return IconType::STOCK;
    if (typeStr == kCached) return IconType::CACHED;
    if (typeStr == kLocal) return IconType::LOCAL;
    if (typeStr == kUrl) return IconType::URL;
    if (typeStr == kRemote) return IconType::REMOTE;
    return IconType::UNKNOWN;
}

std::string Component::iconTypeToString(const IconType type) {
    switch (type) {
        case IconType::STOCK: return kStock;
        case IconType::CACHED: return kCached;
        case IconType::LOCAL: return kLocal;
        case IconType::URL: return kUrl;
        case IconType::REMOTE: return kRemote;
        default: return kUnknown;
    }
}

Component::CompulsoryForDesktop Component::stringToCompulsoryForDesktop(const std::string &desktopString) {
    static const std::unordered_map<std::string, CompulsoryForDesktop> stringToEnumMap = {
        {kCosmic, CompulsoryForDesktop::COSMIC}, {kGnome, CompulsoryForDesktop::GNOME},
        {kGnomeClassic, CompulsoryForDesktop::GNOME_Classic}, {kGnomeFlashback, CompulsoryForDesktop::GNOME_Flashback},
        {kKde, CompulsoryForDesktop::KDE}, {kLxde, CompulsoryForDesktop::LXDE},
        {kLxqt, CompulsoryForDesktop::LXQt}, {kMate, CompulsoryForDesktop::MATE},
        {kRazor, CompulsoryForDesktop::Razor}, {kRox, CompulsoryForDesktop::ROX},
        {kTde, CompulsoryForDesktop::TDE}, {kUnity, CompulsoryForDesktop::Unity},
        {kXfce, CompulsoryForDesktop::XFCE}, {kEde, CompulsoryForDesktop::EDE},
        {kCinnamon, CompulsoryForDesktop::Cinnamon}, {kPantheon, CompulsoryForDesktop::Pantheon},
        {kDde, CompulsoryForDesktop::DDE}, {kEndless, CompulsoryForDesktop::Endless},
        {kOld, CompulsoryForDesktop::Old}
    };

    if (const auto it = stringToEnumMap.find(desktopString); it != stringToEnumMap.end()) {
        return it->second;
    }
    return CompulsoryForDesktop::UNKNOWN;
}

std::string Component::compulsoryForDesktopToString(const CompulsoryForDesktop desktopEnum) {
    static const std::unordered_map<CompulsoryForDesktop, std::string> enumToStringMap = {
        {CompulsoryForDesktop::COSMIC, kCosmic}, {CompulsoryForDesktop::GNOME, kGnome},
        {CompulsoryForDesktop::GNOME_Classic, kGnomeClassic}, {CompulsoryForDesktop::GNOME_Flashback, kGnomeFlashback},
        {CompulsoryForDesktop::KDE, kKde}, {CompulsoryForDesktop::LXDE, kLxde},
        {CompulsoryForDesktop::LXQt, kLxqt}, {CompulsoryForDesktop::MATE, kMate},
        {CompulsoryForDesktop::Razor, kRazor}, {CompulsoryForDesktop::ROX, kRox},
        {CompulsoryForDesktop::TDE, kTde}, {CompulsoryForDesktop::Unity, kUnity},
        {CompulsoryForDesktop::XFCE, kXfce}, {CompulsoryForDesktop::EDE, kEde},
        {CompulsoryForDesktop::Cinnamon, kCinnamon}, {CompulsoryForDesktop::Pantheon, kPantheon},
        {CompulsoryForDesktop::DDE, kDde}, {CompulsoryForDesktop::Endless, kEndless},
        {CompulsoryForDesktop::Old, kOld}
    };

    if (const auto it = enumToStringMap.find(desktopEnum); it != enumToStringMap.end()) {
        return it->second;
    }
    return kUnknown;
}

Component::UrlType Component::stringToUrlType(const std::string &typeStr) {
    if (typeStr == kHomepage) return UrlType::HOMEPAGE;
    if (typeStr == kBugtracker) return UrlType::BUGTRACKER;
    if (typeStr == kFaq) return UrlType::FAQ;
    if (typeStr == kHelp) return UrlType::HELP;
    if (typeStr == kDonation) return UrlType::DONATION;
    if (typeStr == kTranslate) return UrlType::TRANSLATE;
    if (typeStr == kContact) return UrlType::CONTACT;
    if (typeStr == kVcsBrowser) return UrlType::VCS_BROWSER;
    if (typeStr == kContribute) return UrlType::CONTRIBUTE;
    return UrlType::UNKNOWN;
}

Component::LaunchableType Component::stringToLaunchableType(const std::string &typeStr) {
    if (typeStr == kDesktopId) return LaunchableType::DESKTOP_ID;
    if (typeStr == kService) return LaunchableType::SERVICE;
    if (typeStr == kCockpitManifest) return LaunchableType::COCKPIT_MANIFEST;
    if (typeStr == kUrl) return LaunchableType::URL;
    return LaunchableType::UNKNOWN;
}

Component::ReleaseType Component::stringToReleaseType(const std::string &typeStr) {
    if (typeStr == kStable) return ReleaseType::STABLE;
    if (typeStr == kDevelopment) return ReleaseType::DEVELOPMENT;
    if (typeStr == kSnapshot) return ReleaseType::SNAPSHOT;
    return ReleaseType::UNKNOWN;
}

Component::ReleaseUrgency Component::stringToReleaseUrgency(const std::string &typeStr) {
    if (typeStr == kLow) return ReleaseUrgency::LOW;
    if (typeStr == kMedium) return ReleaseUrgency::MEDIUM;
    if (typeStr == kHigh) return ReleaseUrgency::HIGH;
    if (typeStr == kCritical) return ReleaseUrgency::CRITICAL;
    return ReleaseUrgency::UNKNOWN;
}

std::string Component::releaseUrgencyToString(const ReleaseUrgency type) {
    switch (type) {
        case ReleaseUrgency::LOW: return kLow;
        case ReleaseUrgency::MEDIUM: return kMedium;
        case ReleaseUrgency::HIGH: return kHigh;
        case ReleaseUrgency::CRITICAL: return kCritical;
        default: return kUnknown;
    }
}

Component::IssueType Component::stringToIssueType(const std::string &typeStr) {
    if (typeStr == kGeneric) return IssueType::GENERIC;
    if (typeStr == kCve) return IssueType::CVE;
    return IssueType::UNKNOWN;
}

std::string Component::releaseTypeToString(const ReleaseType type) {
    switch (type) {
        case ReleaseType::STABLE: return kStable;
        case ReleaseType::SNAPSHOT: return kSnapshot;
        case ReleaseType::DEVELOPMENT: return kDevelopment;
        default: return kUnknown;
    }
}

std::string Component::issueTypeToString(const IssueType type) {
    switch (type) {
        case IssueType::GENERIC: return kGeneric;
        case IssueType::CVE: return kCve;
        default: return kUnknown;
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
    if (!pkgname.empty()) spdlog::info("\tpkgname: {}", pkgname);
    if (!source_pkgname.empty()) spdlog::info("\tsource_pkgname: {}", source_pkgname);
    if (!description.empty()) spdlog::info("\tdescription: {}", description);
    if (!url.homepage.empty()) spdlog::info("\thomepage: {}", url.homepage);
    if (!url.bugtracker.empty()) spdlog::info("\tbugtracker: {}", url.bugtracker);
    if (!url.faq.empty()) spdlog::info("\tfaq: {}", url.faq);
    if (!url.help.empty()) spdlog::info("\thelp: {}", url.help);
    if (!url.donation.empty()) spdlog::info("\tdonation: {}", url.donation);
    if (!url.translate.empty()) spdlog::info("\ttranslate: {}", url.translate);
    if (!url.contact.empty()) spdlog::info("\tcontact: {}", url.contact);
    if (!url.vcs_browser.empty()) spdlog::info("\tvcs_browser: {}", url.vcs_browser);
    if (!url.contribute.empty()) spdlog::info("\tcontribute: {}", url.contribute);
    if (!url.unknown.empty()) spdlog::info("\tunknown: {}", url.unknown);
    if (!project_group.empty()) spdlog::info("\tproject_group: {}", project_group);
    if (!developer.id.empty()) spdlog::info("\tdeveloper id: {}", developer.id);
    if (!developer.name.empty()) spdlog::info("\tdeveloper name: {}", developer.name);
    if (!launchable.desktop_id.empty()) spdlog::info("\tlaunchable desktop_id: {}", launchable.desktop_id);
    if (!launchable.service.empty()) spdlog::info("\tlaunchable service: {}", launchable.service);
    if (!launchable.cockpit_manifest.empty())
        spdlog::info("\tlaunchable cockpit_manifest: {}",
                     launchable.cockpit_manifest);
    if (!launchable.url.empty()) spdlog::info("\tlaunchable url: {}", launchable.url);
    for (const auto &comp: compulsory_for_desktop) {
        spdlog::info("compulsory_for_desktop: {}", compulsoryForDesktopToString(comp));
    }
    for (const auto &[type, value, width, height, scale]: icons) {
        spdlog::info("\ticon");
        spdlog::info("\t\ttype: {}", iconTypeToString(type));
        spdlog::info("\t\tvalue: {}", value);
        if (width) spdlog::info("\t\twidth: {}", *width);
        if (height) spdlog::info("\t\theight: {}", *height);
        if (scale) spdlog::info("\t\tscale: {}", *scale);
    }
    for (const auto &[type, version, date, timestamp, date_eol, urgency, description, url, issues, artifacts]:
         releases) {
        spdlog::info("\trelease");
        spdlog::info("\t\ttype: {}", releaseTypeToString(type));
        spdlog::info("\t\tversion: {}", version);
        if (!date.empty()) spdlog::info("\t\tdate: {}", date);
        if (!timestamp.empty()) spdlog::info("\t\ttimestamp: {}", timestamp);
        if (!date_eol.empty()) spdlog::info("\t\tdate_eol: {}", date_eol);
        spdlog::info("\t\turgency: {}", releaseUrgencyToString(urgency));
        if (!description.empty()) spdlog::info("\t\tdescription: {}", description);
        if (!url.empty()) spdlog::info("\t\turl: {}", url);
        for (const auto &[type, url, value]: issues) {
            spdlog::info("\t\tissue");
            spdlog::info("\t\t\ttype: {}", issueTypeToString(type));
            if (!url.empty()) spdlog::info("\t\t\turl: {}", url);
            if (!value.empty()) spdlog::info("\t\t\tvalue: {}", value);
        }
        for (const auto &[location, checksum, size]: artifacts) {
            spdlog::info("\t\tartifact");
            if (!location.empty()) spdlog::info("\t\t\tlocation: {}", location);
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
