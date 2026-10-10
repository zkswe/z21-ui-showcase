/*
 * Lyric.cpp - LRC 解析实现
 *
 * 注意：网易云的歌词文本里可能带 "[00:00.00] 作曲 : xxx" 这类元信息行，
 * 本实现按「有时间戳就收」处理（网易云 App 也是这么显示的），不特殊过滤。
 * 空文本（时间戳后无字）跳过。
 * 行内多个时间戳（如 "[00:12.34][01:20.00]副歌"）展开成多行。
 */
#include "core/Lyric.hpp"

#include <algorithm>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <base/log.h>

namespace cmcore {

/** 从 "[mm:ss.xx]" 里取毫秒；不合法返回 -1 */
static long long parseStamp(const char *p, size_t len) {
    if (len < 4 || p[0] != '[') {
        return -1;
    }
    const char *end = (const char *) memchr(p, ']', len);
    if (end == NULL) {
        return -1;
    }
    const std::string inside(p + 1, (size_t) (end - p - 1));
    const size_t colon = inside.find(':');
    if (colon == std::string::npos) {
        return -1;
    }
    std::string mm = inside.substr(0, colon);
    std::string rest = inside.substr(colon + 1);
    std::string ss = rest, frac;
    const size_t dot = rest.find('.');
    if (dot != std::string::npos) {
        ss = rest.substr(0, dot);
        frac = rest.substr(dot + 1);
    }
    if (mm.empty() || ss.empty()) {
        return -1;
    }
    for (size_t i = 0; i < mm.size(); ++i) {
        if (!isdigit((unsigned char) mm[i])) return -1;
    }
    for (size_t i = 0; i < ss.size(); ++i) {
        if (!isdigit((unsigned char) ss[i])) return -1;
    }
    long long ms = atoll(mm.c_str()) * 60000LL + atoll(ss.c_str()) * 1000LL;
    if (frac.size() == 1) {
        ms += atoll(frac.c_str()) * 100;
    } else if (frac.size() == 2) {
        ms += atoll(frac.c_str()) * 10;
    } else if (frac.size() >= 3) {
        ms += atoll(frac.substr(0, 3).c_str());
    }
    return ms;
}

static std::string trimAscii(const std::string &s) {
    size_t b = s.find_first_not_of(" \t");
    if (b == std::string::npos) {
        return std::string();
    }
    size_t e = s.find_last_not_of(" \t");
    return s.substr(b, e - b + 1);
}

int lyricParse(const std::string &lrc, std::vector<LyricLine> *out) {
    if (out == NULL) {
        return 0;
    }
    out->clear();
    std::vector<LyricLine> rows;
    size_t pos = 0;
    const size_t total = lrc.size();
    while (pos < total) {
        size_t nl = lrc.find('\n', pos);
        if (nl == std::string::npos) {
            nl = total;
        }
        std::string line = lrc.substr(pos, nl - pos);
        pos = nl + 1;
        if (!line.empty() && line[line.size() - 1] == '\r') {
            line.erase(line.size() - 1);
        }
        if (!line.empty()) {
            /* 行首连续的 [..] 时间戳 */
            std::vector<long long> stamps;
            size_t i = 0;
            while (i < line.size() && line[i] == '[') {
                const long long ms = parseStamp(line.c_str() + i, line.size() - i);
                if (ms < 0) {
                    break;
                }
                const char *close = strchr(line.c_str() + i, ']');
                if (close == NULL) {
                    break;
                }
                i = (size_t) (close - line.c_str()) + 1;
                stamps.push_back(ms);
            }
            if (!stamps.empty()) {
                const std::string text = trimAscii(line.substr(i));
                if (!text.empty()) {
                    for (size_t k = 0; k < stamps.size(); ++k) {
                        LyricLine ll;
                        ll.ms = stamps[k];
                        ll.text = text;
                        rows.push_back(ll);
                    }
                }
            }
        }
        if (nl == total) {
            break;
        }
    }
    std::stable_sort(rows.begin(), rows.end(),
                     [](const LyricLine &a, const LyricLine &b) { return a.ms < b.ms; });
    out->swap(rows);
    LOGD_TRACE("lrc 解析: %d 行", (int) out->size());
    return (int) out->size();
}

int lyricLineAt(const std::vector<LyricLine> &lines, long long ms) {
    int idx = -1;
    for (size_t i = 0; i < lines.size(); ++i) {
        if (lines[i].ms <= ms) {
            idx = (int) i;
        } else {
            break;
        }
    }
    return idx;
}

} /* namespace cmcore */
