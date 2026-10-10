/*
 * CoverCache.hpp - 封面小工具（CloudMusic-F133）
 *
 * 列表封面/播放页封面都要「从局域网代理下到设备本地，再交给控件当背景图」。
 * 统一在这里管：落盘位置固定 /tmp/cm_cov_<名字>.png（同名覆盖写，不堆文件），
 * 换列表/换页时用 coverCacheClear() 把上一批删掉，保证 /tmp（126MB tmpfs）不越积越多。
 * 只在 TaskRunner 的工作线程里调用（阻塞下载）；返回值是设备侧绝对路径（失败回空串）。
 */
#ifndef _CM_CORE_COVER_CACHE_HPP_
#define _CM_CORE_COVER_CACHE_HPP_

#include <string>

namespace cmcore {

/** 设备侧缓存目录（/tmp，用完即清） */
#define CM_COVER_DIR   "/tmp"

/** 下载 remoteUrl 封面到 /tmp/cm_cov_<name>.png（size 为 PC 侧转 PNG 的目标边长，0 = 默认） */
std::string coverCacheFetch(const std::string &remoteUrl, const std::string &name, int size);

/** 删掉 /tmp/cm_cov_<name>.png（不存在也算成功） */
void coverCacheClear(const std::string &name);

/** 本地缓存文件是否已存在且非空 */
bool coverCacheExists(const std::string &name);

/** 取本地缓存路径（不下载） */
std::string coverCachePath(const std::string &name);

} /* namespace cmcore */

#endif /* _CM_CORE_COVER_CACHE_HPP_ */
