#pragma once

#include <iosfwd>
#include <string>
#include <vector>

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


// Reads two lines A and B, returns (A+B)+"\n".
// If parsing fails, ok=false and returns empty string.
std::string SolveBigIntAdd(const std::string &input, bool &ok);

} // namespace rm_a0
