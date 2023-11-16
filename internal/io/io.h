#pragma once
#include <string>

namespace WindowManager {
        [[nodiscard]] std::string readFile(const char* filepath) noexcept;
}
