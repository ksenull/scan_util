#include "Detect.h"

DetectRequirement Detect::JsRequirement {
        .Pattern = "<script>evil_script()</script>",
        .Extension = ".js"
};

DetectRequirement Detect::UnixRequirement {
    .Pattern = "rm -rf ~/Documents",
    .Extension = ""
};

DetectRequirement Detect::MacosRequirement {
    .Pattern = "system(\"launchctl load /Library/LaunchAgents/com.malware.agent\")",
    .Extension = ""
};
std::unordered_map<DetectType, DetectRequirement> Detect::DetectTypeToRequirement {
        {DetectType::Js, Detect::JsRequirement},
        {DetectType::Unix, Detect::UnixRequirement},
        {DetectType::Macos, Detect::MacosRequirement}
};