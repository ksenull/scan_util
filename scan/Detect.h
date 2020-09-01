#pragma once

#include <string>
#include <unordered_map>


enum class DetectType {
    Js,
    Unix,
    Macos,
    None
};

static const std::unordered_map<DetectType, std::pair<std::string, std::string>> DetectRules = {
        {DetectType::Js, {"<script>evil_script()</script>", ".js"}},
        {DetectType::Unix, {"rm -rf ~/Documents", ""}},
        {DetectType::Macos, {"system(\"launchctl load /Library/LaunchAgents/com.malware.agent\")", ""}},
        {DetectType::None, {"", ""}}
};

struct Detect {
public:
    DetectType Type = DetectType::None;

    [[nodiscard]] std::string GetPattern() const {  // TODO: remove, needed only for inner mechanisms
        return DetectRules.at(Type).first;
    }

    [[nodiscard]] std::string GetExtension() const { // TODO: remove, needed only for inner mechanisms
        return DetectRules.at(Type).second;
    }
};


