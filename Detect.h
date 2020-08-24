#pragma once

#include <string>
#include <unordered_map>


enum class DetectType {
    Js,
    Unix,
    Macos,
    Unknown
};

struct DetectRequirement {
    std::string Pattern;
    std::string Extension;
};

struct Detect {
public:
    DetectType Type = DetectType::Unknown;
    static DetectRequirement JsRequirement;
    static DetectRequirement UnixRequirement;
    static DetectRequirement MacosRequirement;
    static std::unordered_map<DetectType, DetectRequirement> DetectTypeToRequirement;
};
