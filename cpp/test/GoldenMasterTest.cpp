#include <gtest/gtest.h>

#include "TexttestSimulator.h"

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

#if __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#error "A C++17 filesystem implementation is required"
#endif

#ifndef GOLDEN_DIR
#define GOLDEN_DIR "test/golden"
#endif

namespace {

constexpr const char* kGoldenFileName = "texttest_31_days.expected.txt";
constexpr const char* kReceivedFileName = "texttest_31_days.received.txt";
constexpr int kSimulationDays = 31;

std::string normalizeNewlines(std::string text) {
    std::string normalized;
    normalized.reserve(text.size());

    for (std::size_t index = 0; index < text.size(); ++index) {
        if (text[index] == '\r') {
            if (index + 1 < text.size() && text[index + 1] == '\n') {
                ++index;
            }
            normalized.push_back('\n');
        } else {
            normalized.push_back(text[index]);
        }
    }

    return normalized;
}

std::string readFile(const fs::path& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input.is_open()) {
        return {};
    }

    std::ostringstream buffer;
    buffer << input.rdbuf();
    return normalizeNewlines(buffer.str());
}

void writeFile(const fs::path& path, const std::string& contents) {
    fs::create_directories(path.parent_path());
    std::ofstream output(path, std::ios::binary | std::ios::trunc);
    ASSERT_TRUE(output.is_open()) << "Failed to write file: " << path.string();
    output << contents;
}

fs::path goldenFilePath() {
    return fs::path(GOLDEN_DIR) / kGoldenFileName;
}

fs::path receivedFilePath() {
    return fs::path(GOLDEN_DIR) / kReceivedFileName;
}

bool shouldUpdateGolden() {
    const char* updateFlag = std::getenv("UPDATE_GOLDEN");
    return updateFlag != nullptr && updateFlag[0] != '\0' && updateFlag[0] != '0';
}

}  // namespace

TEST(GoldenMasterTest, ThirtyOneDayTexttestOutputMatchesApprovedFile) {
    // Given
    const std::string actual = normalizeNewlines(runTexttestSimulation(kSimulationDays));
    const fs::path goldenPath = goldenFilePath();

    // When updating the approved output intentionally
    if (shouldUpdateGolden()) {
        writeFile(goldenPath, actual);
        GTEST_SKIP() << "Golden master updated at " << goldenPath.string()
                     << ". Re-run tests without UPDATE_GOLDEN.";
    }

    // When
    const std::string expected = readFile(goldenPath);

    // Then
    ASSERT_FALSE(expected.empty())
        << "Missing or empty golden file: " << goldenPath.string()
        << ". Run: cmake --build build --target update-golden-master";

    if (expected != actual) {
        writeFile(receivedFilePath(), actual);
        FAIL() << "Golden master mismatch.\n"
               << "  expected: " << goldenPath.string() << '\n'
               << "  received: " << receivedFilePath().string() << '\n'
               << "Review the diff, then update only if the behavior change is intended.";
    }
}
