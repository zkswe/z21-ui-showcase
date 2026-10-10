/*
 * ApiTypes.hpp - 网易云音乐数据模型（CloudMusic-F133）
 *
 * 只放数据结构，逻辑在 NeteaseApi / MusicPlayer。
 * 字段口径取自 NeteaseCloudMusicApi 4.32.0 的 JSON（/search、/playlist/detail、
 * /personalized、/toplist、/song/url、/lyric）。
 */
#ifndef _CM_CORE_API_TYPES_HPP_
#define _CM_CORE_API_TYPES_HPP_

#include <string>
#include <vector>

namespace cmcore {

/** 一首歌（列表/播放都用它） */
struct Song {
    long long id;               /**< 网易云歌曲 id */
    std::string name;
    std::string artists;        /**< 逗号拼接的歌手名 */
    std::string album;
    long long durationMs;       /**< 时长 ms（0 = 未知） */
    std::string coverUrl;       /**< 专辑封面 URL（jpg） */
    int fee;                    /**< 0 免费 / 1 VIP / 4 购买专辑 等（原样保留） */
    std::string streamUrl;      /**< 非空 = 直接播这个地址（格式自检的本地样例用；线上曲留空） */

    Song() : id(0), durationMs(0), fee(0) {}
};

/** 歌单 / 榜单（首页卡片用） */
struct PlaylistInfo {
    long long id;
    std::string name;
    std::string coverUrl;
    long long playCount;        /**< 播放量（原值，UI 自行缩写成「万/亿」） */
    std::string tag;            /**< 榜单的 updateFrequency / 歌单的 tag，可为空 */

    PlaylistInfo() : id(0), playCount(0) {}
};

/** 一行歌词 */
struct LyricLine {
    long long ms;               /**< 该行开始时间 ms */
    std::string text;

    LyricLine() : ms(0) {}
};

} /* namespace cmcore */

#endif /* _CM_CORE_API_TYPES_HPP_ */
