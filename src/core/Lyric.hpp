/*
 * Lyric.hpp - LRC 歌词解析（CloudMusic-F133）
 *
 * 输入是网易云 /lyric 返回的 lrc.lyric 文本，常见形态：
 *   [00:12.34] 歌词文本        （一毫秒精度两位）
 *   [00:12.340]歌词文本        （三位也有）
 *   [ti:xxx] [ar:xxx] [by:xxx] （标签行，丢弃）
 *   [00:12.34][01:20.00]重复段落（同一行多个时间戳 -> 展开成多行）
 * 输出按时间升序排序的行列表；空行/纯标签行丢弃。
 * 时间戳格式异常的行整行丢弃（不崩、不错位）。
 */
#ifndef _CM_CORE_LYRIC_HPP_
#define _CM_CORE_LYRIC_HPP_

#include <string>
#include <vector>

#include "core/ApiTypes.hpp"

namespace cmcore {

/** 解析 LRC -> 行列表（按 ms 升序）；返回解析出的行数 */
int lyricParse(const std::string &lrc, std::vector<LyricLine> *out);

/** 当前时间落在第几行（返回下标；<0 表示还没到第一行/无歌词） */
int lyricLineAt(const std::vector<LyricLine> &lines, long long ms);

} /* namespace cmcore */

#endif /* _CM_CORE_LYRIC_HPP_ */
