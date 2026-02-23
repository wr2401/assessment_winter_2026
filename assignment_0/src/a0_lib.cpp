#include "rm_a0/a0_lib.hpp"

#include "rm_a0/a0_01_temperature.hpp"
#include "rm_a0/a0_02_leap_year.hpp"
#include "rm_a0/a0_03_range_sum.hpp"
#include "rm_a0/a0_04_vowel_count.hpp"
#include "rm_a0/a0_05_score_stats.hpp"
#include "rm_a0/a0_06_bigint.hpp"
#include "rm_a0/a0_07_log_analyzer.hpp"
#include "rm_a0/a0_08_raii_handle.hpp"

#include <cctype>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace rm_a0 {

// ==================== A0-01 Temperature ====================
// TODO: 完成下面函数的实现
double CelsiusToFahrenheit(double celsius) {
    return celsius * 9.0 / 5.0 + 32.0;
}

std::string FormatFahrenheit(double fahrenheit) {
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "%.2f\n", fahrenheit);
    return std::string(buffer);
}


std::string SolveTemperature(const std::string& input, bool& ok) {
    std::istringstream in(input);
    double celsius = 0.0;
    if (!(in >> celsius)) {
        ok = false;
        return {};
    }

    ok = true;
    return FormatFahrenheit(CelsiusToFahrenheit(celsius));
}

// ==================== A0-02 Leap Year ====================
// TODO: 完成下面函数的实现
bool IsLeapYear(int year) {
    if(year % 4 == 0&& year % 100 != 0 || year % 400 == 0) return true;
    return false;
}
// TODO: 完成下面函数的实现,不要新增行数，只修改返回值
std::string FormatLeapYearAnswer(bool is_leap_year) {
    (void)is_leap_year;
    return is_leap_year ? "YES\n" : "NO\n";
}

std::string SolveLeapYear(const std::string& input, bool& ok) {
    std::istringstream in(input);
    int year = 0;
    if (!(in >> year)) {
        ok = false;
        return {};
    }

    ok = true;
    return FormatLeapYearAnswer(IsLeapYear(year));
}

// ==================== A0-03 Range Sum ====================
// TODO: 完成下面函数的实现
long long RangeSum(long long l, long long r) {
    (void)l;
    (void)r;
    return (l + r) * (r - l + 1) / 2                                                                                                                                                                                                            ;
}

std::string SolveRangeSum(const std::string& input, bool& ok) {
    std::istringstream in(input);
    long long l = 0;
    long long r = 0;
    if (!(in >> l >> r)) {
        ok = false;
        return {};
    }
    ok = true;
    std::ostringstream out;
    out << RangeSum(l, r) << "\n";
    return out.str();
}


// ==================== A0-04 Vowel Count ====================

namespace {
    namespace a0_04_detail {

        bool IsVowelChar(unsigned char c) {
            // TODO: 完成下面函数的实现
            (void)c;
            c=tolower(c);
            if(c=='a'||c=='e'||c=='i'||c=='o'||c=='u') return true;
            return false;
        }

    } // namespace a0_04_detail
} // namespace

std::size_t CountVowels(const std::string& line) {
    std::size_t count = 0;
    // TODO: 完成下面函数的实现
    for(char c:line){
        if(a0_04_detail::IsVowelChar(c)) count++;
    }
    return count;
}

std::string SolveVowelCount(const std::string& input, bool& ok) {
    std::istringstream in(input);
    std::string line;
    if (!std::getline(in, line)) {
        ok = false;
        return {};
    }
    ok = true;
    std::ostringstream out;
    out << CountVowels(line) << "\n";
    return out.str();
}

// ==================== A0-05 Score Stats ====================

ScoreStatsResult ComputeScoreStats(const std::string& input, bool& ok) {
    ok = false;
    std::istringstream in(input);
    // TODO: 完成下面函数的实现
    int n;
    in >> n;
    
    std::string name;
    int score;
    int sum = 0;
    int max_score = -1;
    std::string max_name;
    
    for (int i = 0; i < n; i++) {
        in >> name >> score;    
        sum += score;
        if (score > max_score) {
            max_score = score;
            max_name = name;
        }
    }
    
    double avg = static_cast<double>(sum) / n;
    ok = true;
    return ScoreStatsResult{max_name, max_score, avg};
}  

std::string SolveScoreStats(const std::string& input, bool& ok) {
    auto res = ComputeScoreStats(input, ok);
    if (!ok) {
        return {};
    }

    std::ostringstream out;
    out << "top=" << res.top_name << " " << res.top_score << "\n";
    out << "avg=" << std::fixed << std::setprecision(2) << res.avg << "\n";
    return out.str();
}

// ==================== A0-06 BigInt ====================

// TODO: 参考hpp完成类实现
/*
at a0_06_bigint.hpp:
class BigInt {
public:
  BigInt();

  // Constructs from a non-negative decimal string.
  explicit BigInt(const std::string &s);

  friend BigInt operator+(const BigInt &a, const BigInt &b);
  friend std::ostream &operator<<(std::ostream &os, const BigInt &x);

private:
  // Little-endian digits, each 0..9.
  std::vector<int> digits_;
};
*/
/*class BigInt {
public:
  BigInt(){ digits_.push_back(0);}

  // Constructs from a non-negative decimal string.
  explicit BigInt(const std::string &s){
        for (int i = s.size() - 1; i >= 0; i--) {
            digits_.push_back(s[i] - '0');
        }
  }

  friend BigInt operator+(const BigInt &a, const BigInt &b){
    BigInt result;
    result.digits_.clear();
    int carry = 0;
    size_t max_size = std::max(a.digits_.size(), b.digits_.size());
    for (size_t i = 0; i < max_size || carry; i++) {
        int digit_a = i < a.digits_.size() ? a.digits_[i] : 0;
        int digit_b = i < b.digits_.size() ? b.digits_[i] : 0;
        int sum = digit_a + digit_b + carry;
        result.digits_.push_back(sum % 10);
        carry = sum / 10;
    }
    return result;
  }
  friend std::ostream &operator<<(std::ostream &os, const BigInt &x){
        for (int i = x.digits_.size() - 1; i >= 0; i--) {
            os << x.digits_[i];
        }
        return os;
  }

private:
  // Little-endian digits, each 0..9.
  std::vector<int> digits_;
};*/
std::string SolveBigIntAdd(const std::string& input, bool& ok) {
    std::istringstream in(input);
    std::string a;
    std::string b;
    std::ostringstream out;
    if (!std::getline(in, a)) {
        ok = false;
        return {};
    }
    if (!std::getline(in, b)) {
        ok = false;
        return {};
    }

    ok = true;
    BigInt big_a(a);
    BigInt big_b(b);
    BigInt sum = big_a + big_b;
    out << sum << "\n";
    return out.str();
}


// ==================== A0-07 Log Analyzer ====================
/*
at a0_07_log_analyzer.hpp:
struct LogStats {
  long long info = 0;
  long long warn = 0;
  long long error = 0;
  double avg_ms = 0.0;
  std::string max_level;
  long long max_ms = 0;
};
*/
LogStats AnalyzeLogFile(const std::string& path, bool& ok) {
    std::ifstream file(path);
    if (!file.is_open()) {
        ok = false;
        return {};
    }
    LogStats stats;
    std::string line;
    int count=0;
    while(std::getline(file, line)) {
        if(line.empty()) continue;
        std::istringstream in(line);
        std::string level;
        long long ms;
        if (!(in >> level >> ms)) continue;
        if(level == "INFO") stats.info++;
        else if(level == "WARN") stats.warn++;
        else if(level == "ERROR") stats.error++;
        else continue;
        stats.avg_ms += ms;
        if(ms > stats.max_ms) {
            stats.max_ms = ms;
            stats.max_level = level;
        }
        count++;
    }
    if(count == 0) ok = false;
    else stats.avg_ms /= count;
    
    return stats;
}

std::string SolveLogAnalyzer(const std::string& input, bool& ok) {
    std::istringstream in(input);
    std::string path;
    if (!std::getline(in, path)) {
        ok = false;
        return {};
    }
    if (path.empty()) {
        ok = false;
        return "FAIL\n";
    }

    bool file_ok = false;
    LogStats s   = AnalyzeLogFile(path, file_ok);
    if (!file_ok) {
        ok = false;
        return "FAIL\n";
    }

    ok = true;
    std::ostringstream out;
    out << "INFO=" << s.info << "\n";
    out << "WARN=" << s.warn << "\n";
    out << "ERROR=" << s.error << "\n";
    out << "avg=" << std::fixed << std::setprecision(2) << s.avg_ms << "\n";
    out << "max=" << s.max_level << " " << s.max_ms << "\n";
    return out.str();
}

// ==================== A0-08 RAII Handle ====================

// TODO: 参考hpp完成类实现
/*
at a0_08_raii_handle.hpp:
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
*/
/*class FileHandle {
public:
  FileHandle() = default;
  FileHandle(const char *path, const char *mode)
    : fp_(fopen(path, mode)) {}

  ~FileHandle(){
    if (fp_) fclose(fp_);
}
  FileHandle(const FileHandle &) = delete;
  FileHandle &operator=(const FileHandle &) = delete;

  FileHandle(FileHandle &&other) noexcept
    : fp_(other.fp_) {
        other.fp_ = nullptr;
  }
  FileHandle &operator=(FileHandle &&other) noexcept{
        if (this != &other) {
            if (fp_) fclose(fp_);
            fp_ = other.fp_;
            other.fp_ = nullptr;
        }
        return *this;
    }

  bool valid() const{
    return fp_ != nullptr;
}
  FILE *get() const{
        return fp_;
      }

private:
  FILE *fp_ = nullptr;
};
*/
bool CopyFile(const std::string& in_path, const std::string& out_path) {

    ileHandle in(in_path.c_str(), "rb");
    if (!in.valid()) return false;

    FileHandle out(out_path.c_str(), "wb");
    if (!out.valid()) return false;

    char buffer[4096];
    size_t bytes;

    while ((bytes = fread(buffer, 1, sizeof(buffer), in.get())) > 0) {
        if (fwrite(buffer, 1, bytes, out.get()) != bytes) {
            return false;
        }
    }

    return !ferror(in.get());
    
}

std::string SolveRaiiCopy(const std::string& input, bool& ok) {
    std::istringstream in(input);
    std::string in_path;
    std::string out_path;
    if (!(in >> in_path >> out_path)) {
        ok = false;
        return {};
    }

    if (CopyFile(in_path, out_path)) {
        ok = true;
        return "OK\n";
    }

    ok = false;
    return "FAIL\n";
}

// ==================== A0-09 Text Pipeline====================

std::vector<std::string> RunTextPipeline(const std::string& pipeline, const std::vector<std::string>& lines, bool& ok) {
    (void)pipeline;
    (void)lines;
    ok = false;
    return {};
}

// ==================== A0-10 Rule Engine ====================

std::vector<long long> RunRuleEngine(
    const std::vector<std::string>& rule_specs,
    const std::vector<Event>& events,
    long long& total_any,
    bool& ok
) {
    (void)rule_specs;
    (void)events;
    total_any = 0;
    ok        = false;
    return {};
}

// ==================== A0-11 Command Dispatcher====================

std::string RunCommandDispatcher(const std::string& full_stdin, bool& ok) {
    (void)full_stdin;
    ok = false;
    return "FAIL\n";
}

} // namespace rm_a0
