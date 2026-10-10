/*
 * NeteaseApi.cpp - 网易云音乐接口客户端实现
 *
 * JSON 用本工程自带的 core/Json.hpp（极简 DOM）。为什么不用 easyui 里的 jsoncpp：
 *   该构建没有 JSON_HAS_INT64，链接时 Json::Value::Value(long)/asInt64() 符号缺失
 *   （真机链接实测失败）——自写解析器零依赖、口径可控。
 * 只取需要的字段，解析完立刻释放对象树，避免大响应吃内存。
 */
#include "core/NeteaseApi.hpp"

#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <base/log.h>

#include "core/Http.hpp"
#include "core/Json.hpp"
#include "core/Lyric.hpp"

namespace cmcore {

/** 默认 API 地址（PC 上的局域网代理；设置页可改并持久化） */
#define CM_DEFAULT_API_BASE   "http://192.168.1.188:3301"

NeteaseApi::NeteaseApi()
    : mBase(CM_DEFAULT_API_BASE), mLastCode(-1), mLastMs(0), mLastBytes(0),
      mOkCount(0), mFailCount(0) {
}

NeteaseApi &NeteaseApi::instance() {
    static NeteaseApi sInst;
    return sInst;
}

void NeteaseApi::setBaseUrl(const std::string &base) {
    std::string b = base;
    while (!b.empty() && (b[b.size() - 1] == '/' || b[b.size() - 1] == ' ')) {
        b.erase(b.size() - 1);
    }
    if (b.empty()) {
        b = CM_DEFAULT_API_BASE;
    }
    mBase = b;
    LOGD_TRACE("api base -> %s", mBase.c_str());
}

std::string NeteaseApi::baseUrl() const {
    return mBase;
}

std::string NeteaseApi::mediaUrl(long long songId, int br) const {
    char buf[256];
    snprintf(buf, sizeof(buf), "%s/media?id=%lld&br=%d", mBase.c_str(), songId, br);
    return std::string(buf);
}

bool NeteaseApi::getJson(const std::string &path, const std::string &query,
                         std::string *out, std::string *err) {
    std::string url = mBase + "/api/" + path;
    if (!query.empty()) {
        url += "?" + query;
    }
    HttpResult r = httpGet(url, 8000);
    mLastCode = r.httpCode;
    mLastMs = r.ms;
    mLastBytes = (long long) r.body.size();
    if (!r.ok) {
        ++mFailCount;
        if (err) {
            *err = r.httpCode > 0 ? ("HTTP " + std::to_string((long long) r.httpCode)) : r.err;
        }
        LOGE_TRACE("api %s 失败: %s (%lldms)", path.c_str(), (err ? err->c_str() : ""), r.ms);
        return false;
    }
    ++mOkCount;
    if (out) {
        *out = r.body;
    }
    LOGD_TRACE("api %s ok %lldms %lldB", path.c_str(), r.ms, (long long) r.body.size());
    return true;
}

static bool parseJson(const std::string &text, JVal *root, std::string *err) {
    return jsonParse(text, root, err);
}

static std::string joinArtists(const JVal &v) {
    std::string s;
    if (!v.isArr()) {
        return s;
    }
    for (size_t i = 0; i < v.size(); ++i) {
        const std::string n = v.at(i).strOf("name");
        if (n.empty()) {
            continue;
        }
        if (!s.empty()) {
            s += " / ";
        }
        s += n;
    }
    return s;
}

static Song songFromJson(const JVal &s) {
    Song o;
    o.id = s.i64Of("id", 0);
    o.name = s.strOf("name");
    o.durationMs = s.i64Of("duration", 0);
    if (o.durationMs == 0) {
        o.durationMs = s.i64Of("dt", 0);          /* 详情/歌单接口用 dt */
    }
    o.fee = s.i32Of("fee", 0);
    o.artists = joinArtists(s.get("artists"));
    if (o.artists.empty()) {
        o.artists = joinArtists(s.get("ar"));     /* 歌单接口用 ar */
    }
    const JVal &al = s.get("album").isNull() ? s.get("al") : s.get("album");
    if (!al.isNull()) {
        o.album = al.strOf("name");
        o.coverUrl = al.strOf("picUrl");
    }
    return o;
}

bool NeteaseApi::search(const std::string &keywords, int limit,
                        std::vector<Song> *out, std::string *err) {
    if (out) out->clear();
    std::string body;
    std::string q = "keywords=" + urlEncode(keywords) + "&limit=" + std::to_string((long long) limit)
                    + "&type=1";
    if (!getJson("search", q, &body, err)) {
        return false;
    }
    JVal root;
    if (!parseJson(body, &root, err)) {
        ++mFailCount;
        return false;
    }
    const JVal &songs = root.get("result").get("songs");
    if (!songs.isArr()) {
        if (err) *err = "搜索无结果字段";
        return false;
    }
    for (size_t i = 0; i < songs.size(); ++i) {
        if (out) out->push_back(songFromJson(songs.at(i)));
    }
    LOGD_TRACE("search 命中 %d 首", out ? (int) out->size() : 0);
    return true;
}

bool NeteaseApi::personalized(int limit, std::vector<PlaylistInfo> *out, std::string *err) {
    if (out) out->clear();
    std::string body;
    if (!getJson("personalized", "limit=" + std::to_string((long long) limit), &body, err)) {
        return false;
    }
    JVal root;
    if (!parseJson(body, &root, err)) {
        ++mFailCount;
        return false;
    }
    const JVal &res = root.get("result");
    if (!res.isArr()) {
        if (err) *err = "推荐接口无 result";
        return false;
    }
    for (size_t i = 0; i < res.size(); ++i) {
        PlaylistInfo p;
        p.id = res.at(i).i64Of("id", 0);
        p.name = res.at(i).strOf("name");
        p.coverUrl = res.at(i).strOf("picUrl");
        p.playCount = res.at(i).i64Of("playCount", 0);
        if (out) out->push_back(p);
    }
    LOGD_TRACE("personalized -> %d 个歌单", out ? (int) out->size() : 0);
    return true;
}

bool NeteaseApi::toplists(std::vector<PlaylistInfo> *out, std::string *err) {
    if (out) out->clear();
    std::string body;
    if (!getJson("toplist", "", &body, err)) {
        return false;
    }
    JVal root;
    if (!parseJson(body, &root, err)) {
        ++mFailCount;
        return false;
    }
    const JVal &lst = root.get("list");
    if (!lst.isArr()) {
        if (err) *err = "榜单接口无 list";
        return false;
    }
    for (size_t i = 0; i < lst.size(); ++i) {
        PlaylistInfo p;
        p.id = lst.at(i).i64Of("id", 0);
        p.name = lst.at(i).strOf("name");
        p.coverUrl = lst.at(i).strOf("coverImgUrl");
        p.playCount = lst.at(i).i64Of("playCount", 0);
        p.tag = lst.at(i).strOf("updateFrequency");
        if (out) out->push_back(p);
    }
    LOGD_TRACE("toplist -> %d 个榜", out ? (int) out->size() : 0);
    return true;
}

bool NeteaseApi::playlistTracks(long long playlistId, int limit,
                                std::vector<Song> *out, std::string *err) {
    if (out) out->clear();
    std::string body;
    /* playlist/track/all 支持 limit/offset（比 playlist/detail 一次吐全部省内存） */
    std::string q = "id=" + std::to_string(playlistId) + "&limit="
                    + std::to_string((long long) limit) + "&offset=0";
    if (!getJson("playlist/track/all", q, &body, err)) {
        return false;
    }
    JVal root;
    if (!parseJson(body, &root, err)) {
        ++mFailCount;
        return false;
    }
    const JVal &songs = root.get("songs");
    if (!songs.isArr()) {
        if (err) *err = "歌单曲目字段缺失";
        return false;
    }
    for (size_t i = 0; i < songs.size(); ++i) {
        if (out) out->push_back(songFromJson(songs.at(i)));
    }
    LOGD_TRACE("playlist %lld -> %d 首", playlistId, out ? (int) out->size() : 0);
    return true;
}

bool NeteaseApi::lyric(long long songId, std::vector<LyricLine> *out, std::string *err) {
    if (out) out->clear();
    std::string body;
    if (!getJson("lyric", "id=" + std::to_string(songId), &body, err)) {
        return false;
    }
    JVal root;
    if (!parseJson(body, &root, err)) {
        ++mFailCount;
        return false;
    }
    const std::string lrc = root.get("lrc").strOf("lyric");
    if (lrc.empty()) {
        if (err) *err = "无歌词";
        return false;
    }
    lyricParse(lrc, out);
    LOGD_TRACE("lyric %lld -> %d 行", songId, out ? (int) out->size() : 0);
    return true;
}

std::string formatPlayCount(long long n) {
    char buf[64];
    if (n < 10000) {
        snprintf(buf, sizeof(buf), "%lld", n);
        return std::string(buf);
    }
    if (n < 100000000LL) {
        snprintf(buf, sizeof(buf), "%.1f万", (double) n / 10000.0);
        return std::string(buf);
    }
    snprintf(buf, sizeof(buf), "%.1f亿", (double) n / 100000000.0);
    return std::string(buf);
}

std::string formatTime(long long ms) {
    if (ms < 0) {
        ms = 0;
    }
    const long long total = ms / 1000;
    char buf[32];
    snprintf(buf, sizeof(buf), "%02lld:%02lld", total / 60, total % 60);
    return std::string(buf);
}

} /* namespace cmcore */
