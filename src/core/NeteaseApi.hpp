/*
 * NeteaseApi.hpp - 网易云音乐接口客户端（CloudMusic-F133）
 *
 * 设计口径（钟工：设备侧不实现网易云的加密签名）：
 *   · 加密/签名全在 PC 上跑：NeteaseCloudMusicApi 4.32.0（Node）监听 3300；
 *   · PC 上再有一层局域网代理（temp/cloudmusic/proxy.py，3301）：
 *         /api/<接口>?<参数>  透传出 JSON（本类只认这一个地址）
 *         /media?id=&br=      音频字节流（支持 Range，ffmpeg seek 用）
 *         /cover?id=          封面（PC 侧 JPEG 转 PNG，设备只吃 PNG）
 *   · 设备只发 http://<PC>:3301/api/xxx 的普通 GET —— 没有任何加密逻辑。
 *
 * 只在工作线程调用（同步阻塞）；返回值里带 httpCode/耗时，供设置页诊断读数用。
 */
#ifndef _CM_CORE_NETEASE_API_HPP_
#define _CM_CORE_NETEASE_API_HPP_

#include <string>
#include <vector>

#include "core/ApiTypes.hpp"

namespace cmcore {

class NeteaseApi {
public:
    static NeteaseApi &instance();

    /** API 基地址（形如 http://192.168.1.188:3301）；持久化在 AppState 里 */
    void setBaseUrl(const std::string &base);
    std::string baseUrl() const;

    /** 拼出音频流地址（交给 ffmpeg）：<base>/media?id=<id>&br=<br> */
    std::string mediaUrl(long long songId, int br) const;

    /* ---------------- 目标接口（够用即可） ---------------- */
    bool search(const std::string &keywords, int limit, std::vector<Song> *out, std::string *err);
    bool personalized(int limit, std::vector<PlaylistInfo> *out, std::string *err);
    bool toplists(std::vector<PlaylistInfo> *out, std::string *err);
    bool playlistTracks(long long playlistId, int limit, std::vector<Song> *out, std::string *err);
    bool lyric(long long songId, std::vector<LyricLine> *out, std::string *err);

    /** 最近一次请求的 HTTP 码 / 耗时 / 响应字节（诊断读数） */
    int lastHttpCode() const { return mLastCode; }
    long long lastMs() const { return mLastMs; }
    long long lastBytes() const { return mLastBytes; }
    /** 成功/失败计数（诊断） */
    long long okCount() const { return mOkCount; }
    long long failCount() const { return mFailCount; }

private:
    NeteaseApi();
    NeteaseApi(const NeteaseApi &);
    NeteaseApi &operator=(const NeteaseApi &);

    /** 发一次 GET（<base>/api/<path>?<query>），把 JSON 体写进 out */
    bool getJson(const std::string &path, const std::string &query,
                 std::string *out, std::string *err);

    std::string mBase;
    int mLastCode;
    long long mLastMs;
    long long mLastBytes;
    long long mOkCount;
    long long mFailCount;
};

/** 播放量缩写：12345 -> "1.2万"，123456789 -> "1.2亿"（首页卡片用） */
std::string formatPlayCount(long long n);

/** ms -> "mm:ss" */
std::string formatTime(long long ms);

} /* namespace cmcore */

#endif /* _CM_CORE_NETEASE_API_HPP_ */
