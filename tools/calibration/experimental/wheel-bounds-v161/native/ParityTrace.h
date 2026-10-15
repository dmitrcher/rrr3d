#ifndef MOTORROCK_PARITY_TRACE_H
#define MOTORROCK_PARITY_TRACE_H
#pragma push_macro("new")
#pragma push_macro("malloc")
#pragma push_macro("free")
#undef new
#undef malloc
#undef free
// C++98 compatible: shared by the VC2008 reference and the native Mac port.
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <locale>
#include <sstream>
#include <streambuf>
#include <string>
#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#include <unistd.h>
#endif

namespace motorrock { namespace parity {
inline bool enabled() { const char* p = std::getenv("RRR3D_TRACE_FILE"); return p && *p; }
inline double monotonicMs() {
#ifdef _WIN32
    LARGE_INTEGER now, frequency;
    QueryPerformanceCounter(&now); QueryPerformanceFrequency(&frequency);
    return double(now.QuadPart) * 1000.0 / double(frequency.QuadPart);
#else
    timespec t; clock_gettime(CLOCK_MONOTONIC, &t);
    return double(t.tv_sec) * 1000.0 + double(t.tv_nsec) / 1000000.0;
#endif
}
inline std::string quote(const std::string& s) {
    std::ostringstream o; o << '"';
    for (size_t i = 0; i < s.size(); ++i) {
        const unsigned char c = (unsigned char)s[i];
        if (c == '"' || c == '\\') o << '\\' << c;
        else if (c < 32 || c >= 127) o << "\\u00" << std::hex << std::setw(2) << std::setfill('0') << (unsigned)c << std::dec;
        else o << c;
    }
    o << '"'; return o.str();
}
inline void event(const char* name, const std::string& detail, const std::string& sample = "") {
    if (!enabled()) return;
    const std::time_t now = std::time(0);
    std::tm utc;
#ifdef _WIN32
    gmtime_s(&utc, &now);
    const unsigned long pid = GetCurrentProcessId();
    const char* platform = "windows-physx-d3d9";
#else
    gmtime_r(&now, &utc);
    const unsigned long pid = (unsigned long)getpid();
    const char* platform = "mac-jolt-metal";
#endif
    char stamp[32]; std::strftime(stamp, sizeof(stamp), "%Y-%m-%dT%H:%M:%SZ", &utc);
    std::ostringstream row; row.imbue(std::locale::classic());
    row << "{\"utc\":" << quote(stamp) << ",\"monotonic_ms\":" << std::fixed << std::setprecision(3) << monotonicMs()
        << ",\"pid\":" << pid << ",\"platform\":" << quote(platform)
        << ",\"event\":" << quote(name) << ",\"detail\":" << quote(detail);
    if (!sample.empty()) row << ",\"sample\":" << sample;
    row << "}\n";
    // One append per complete JSON record; never redirect or consume game input.
    FILE* file = std::fopen(std::getenv("RRR3D_TRACE_FILE"), "ab");
    if (file) { const std::string value = row.str(); std::fwrite(value.data(), 1, value.size(), file); std::fclose(file); }
}
inline unsigned seedFromEnvironment(unsigned fallback) {
    const char* text = std::getenv("RRR3D_SEED");
    if (!text || !*text) return fallback;
    char* end = 0; const unsigned long seed = std::strtoul(text, &end, 10);
    return end && !*end ? (unsigned)seed : fallback;
}
// Preserve the existing console while recording complete load/progress lines.
class ConsoleTrace : public std::streambuf {
    std::ostream& stream_; std::streambuf* original_; std::string line_;
    ConsoleTrace(const ConsoleTrace&); ConsoleTrace& operator=(const ConsoleTrace&);
    void flushLine() { if (!line_.empty()) { event("log", line_); line_.clear(); } }
protected:
    virtual int_type overflow(int_type c) {
        if (traits_type::eq_int_type(c, traits_type::eof())) return traits_type::not_eof(c);
        const char value = traits_type::to_char_type(c);
        if (value == '\n') flushLine(); else if (value != '\r') line_ += value;
        return original_->sputc(value);
    }
    virtual int sync() { flushLine(); return original_->pubsync(); }
public:
    explicit ConsoleTrace(std::ostream& stream) : stream_(stream), original_(stream.rdbuf(this)) {}
    ~ConsoleTrace() { flushLine(); stream_.rdbuf(original_); }
};
} }
#pragma pop_macro("free")
#pragma pop_macro("malloc")
#pragma pop_macro("new")
#endif
