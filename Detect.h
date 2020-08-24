#pragma once

#include <string>
#include <unordered_map>


enum class DetectType {
    Js,
    Unix,
    Macos,
    None
};


struct Detect {
public:
    DetectType Type = DetectType::None;
    std::string Pattern;
    std::string Extension;
};


static std::unordered_map<DetectType, Detect> DetectTypeToDetect {
        {DetectType::Js, Detect{
                .Type = DetectType::Js,
                .Pattern = "<script>evil_script()</script>",
                .Extension = ".js"
        }},
        {DetectType::Unix, Detect{
                .Type = DetectType::Unix,
                .Pattern = "rm -rf ~/Documents",
                .Extension = ""
        }},
        {DetectType::Macos, Detect{
                .Type = DetectType::Macos,
                .Pattern = "system(\"launchctl load /Library/LaunchAgents/com.malware.agent\")",
                .Extension = ""
        }}
};;
