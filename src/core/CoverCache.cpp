/*
 * CoverCache.cpp - 封面缓存实现
 */
#include "core/CoverCache.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <base/log.h>

#include "core/Http.hpp"

namespace cmcore {

std::string coverCachePath(const std::string &name) {
    return std::string(CM_COVER_DIR) + "/cm_cov_" + name + ".png";
}

bool coverCacheExists(const std::string &name) {
    const std::string p = coverCachePath(name);
    struct stat st;
    if (stat(p.c_str(), &st) != 0) {
        return false;
    }
    return st.st_size > 512;
}

void coverCacheClear(const std::string &name) {
    const std::string p = coverCachePath(name);
    if (remove(p.c_str()) == 0) {
        LOGD_TRACE("封面缓存删除 %s", p.c_str());
    }
}

std::string coverCacheFetch(const std::string &remoteUrl, const std::string &name, int size) {
    if (remoteUrl.empty()) {
        return std::string();
    }
    std::string url = remoteUrl;
    if (size > 0) {
        url += (url.find('?') == std::string::npos ? "?" : "&");
        url += "size=" + std::to_string((long long) size);
    }
    const std::string path = coverCachePath(name);
    long long bytes = 0;
    std::string err;
    if (!httpDownload(url, path, &bytes, &err, 12000)) {
        LOGW_TRACE("封面下载失败 %s: %s", name.c_str(), err.c_str());
        return std::string();
    }
    LOGD_TRACE("封面下载 ok %s %lldB -> %s", name.c_str(), bytes, path.c_str());
    return path;
}

} /* namespace cmcore */
