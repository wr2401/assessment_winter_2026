#pragma once

#include <cstdio>
#include <string>

namespace rm_a0 {

class FileHandle {
public:
  FileHandle() = default;
  FileHandle(const char *path, const char *mode);
  ~FileHandle();

  FileHandle(const FileHandle &) = delete;
  FileHandle &operator=(const FileHandle &) = delete;

  FileHandle(FileHandle &&other) noexcept;
  FileHandle &operator=(FileHandle &&other) noexcept;

  bool valid() const;
  FILE *get() const;

private:
  FILE *fp_ = nullptr;
};

bool CopyFile(const std::string &in_path, const std::string &out_path);

// Input: "in_path out_path".
// Output: "OK\n" on success else "FAIL\n".
// If parsing fails: ok=false and returns empty string.
std::string SolveRaiiCopy(const std::string &input, bool &ok);

} // namespace rm_a0
