#pragma once
#include "REX/W32/OLE32.h"
#include "REX/W32/SHELL32.h"

// https://github.dev/digital-apple/ad-mortem-permadeath/ <3
class Data
{
public:
    static auto GetSingleton() -> Data*;

    auto GetSaveFileDirectory() -> std::optional<std::filesystem::path>;

    static bool DeleteSaveFile(const RE::BSFixedString& fileName);

private:
    Data() = default;
    Data(const Data&) = delete;
    Data(Data&&) = delete;

    ~Data() = default;

    Data& operator=(const Data&) = delete;
    Data& operator=(Data&&) = delete;

    std::optional<std::filesystem::path> save_files;
};