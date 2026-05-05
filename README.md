# Dependancies
## C++
1. nlhomann-json
2. cpp-httplib

## Mac OS VSCode c_cpp_properties.json file
```json
{
    "configurations": [
        {
            "name": "Mac",
            "includePath": [
                "${workspaceFolder}/**",
                "/opt/homebrew/include",
                "/opt/homebrew/lib"
            ],
            "defines": [],
            "cStandard": "c17",
            "cppStandard": "c++17",
            "intelliSenseMode": "macos-clang-arm64"
        }
    ],
    "version": 4
}
```