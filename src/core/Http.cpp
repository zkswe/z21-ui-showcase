/*
 * Http.cpp - 极简 libcurl GET / 下载实现
 */
#include "core/Http.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <curl/curl.h>

#include <base/log.h>

namespace cmcore {

static long long nowMs() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long) tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void httpGlobalInit() {
    static bool sInit = false;
    if (!sInit) {
        curl_global_init(CURL_GLOBAL_DEFAULT);
        sInit = true;
    }
}

std::string urlEncode(const std::string &s) {
    httpGlobalInit();
    char *e = curl_easy_escape(NULL, s.c_str(), (int) s.size());
    if (e == NULL) {
        return s;
    }
    std::string out(e);
    curl_free(e);
    return out;
}

static size_t writeToBuf(char *ptr, size_t size, size_t nmemb, void *userdata) {
    std::string *buf = (std::string *) userdata;
    const size_t n = size * nmemb;
    buf->append(ptr, n);
    return n;
}

static size_t writeToFile(char *ptr, size_t size, size_t nmemb, void *userdata) {
    FILE *f = (FILE *) userdata;
    return fwrite(ptr, size, nmemb, f);
}

static void applyCommonOpts(CURL *c, int timeoutMs) {
    curl_easy_setopt(c, CURLOPT_CONNECTTIMEOUT_MS, (long) (timeoutMs < 4000 ? timeoutMs : 4000));
    curl_easy_setopt(c, CURLOPT_TIMEOUT_MS, (long) timeoutMs);
    curl_easy_setopt(c, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(c, CURLOPT_MAXREDIRS, 5L);
    curl_easy_setopt(c, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(c, CURLOPT_USERAGENT,
                     "Mozilla/5.0 (Linux; F133) AppleWebKit/537.36 cloudmusic-f133/1.0");
    curl_easy_setopt(c, CURLOPT_ACCEPT_ENCODING, "");
}

HttpResult httpGet(const std::string &url, int timeoutMs) {
    HttpResult r;
    httpGlobalInit();
    const long long t0 = nowMs();
    CURL *c = curl_easy_init();
    if (c == NULL) {
        r.err = "curl_easy_init 失败";
        return r;
    }
    char errbuf[CURL_ERROR_SIZE];
    errbuf[0] = '\0';
    curl_easy_setopt(c, CURLOPT_URL, url.c_str());
    curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, &writeToBuf);
    curl_easy_setopt(c, CURLOPT_WRITEDATA, &r.body);
    curl_easy_setopt(c, CURLOPT_ERRORBUFFER, errbuf);
    applyCommonOpts(c, timeoutMs);

    const CURLcode rc = curl_easy_perform(c);
    long code = 0;
    curl_easy_getinfo(c, CURLINFO_RESPONSE_CODE, &code);
    r.httpCode = (int) code;
    r.ms = nowMs() - t0;
    if (rc != CURLE_OK) {
        r.err = errbuf[0] != '\0' ? errbuf : curl_easy_strerror(rc);
        LOGE_TRACE("http GET 失败 %s (%dms) url=%s", r.err.c_str(), (int) r.ms, url.c_str());
    } else if (code != 200) {
        r.err = "HTTP " + std::to_string((long long) code);
    } else {
        r.ok = true;
    }
    curl_easy_cleanup(c);
    return r;
}

bool httpDownload(const std::string &url, const std::string &path,
                  long long *bytesOut, std::string *err, int timeoutMs) {
    httpGlobalInit();
    FILE *f = fopen(path.c_str(), "wb");
    if (f == NULL) {
        if (err) *err = "打不开目标文件：" + path;
        return false;
    }
    CURL *c = curl_easy_init();
    if (c == NULL) {
        fclose(f);
        if (err) *err = "curl_easy_init 失败";
        return false;
    }
    char errbuf[CURL_ERROR_SIZE];
    errbuf[0] = '\0';
    curl_easy_setopt(c, CURLOPT_URL, url.c_str());
    curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, &writeToFile);
    curl_easy_setopt(c, CURLOPT_WRITEDATA, f);
    curl_easy_setopt(c, CURLOPT_ERRORBUFFER, errbuf);
    applyCommonOpts(c, timeoutMs);
    const CURLcode rc = curl_easy_perform(c);
    long code = 0;
    curl_easy_getinfo(c, CURLINFO_RESPONSE_CODE, &code);
    curl_easy_cleanup(c);
    long long n = 0;
    if (f != NULL) {
        long pos = ftell(f);
        n = pos > 0 ? (long long) pos : 0;
        fclose(f);
    }
    if (bytesOut) *bytesOut = n;
    if (rc != CURLE_OK || code != 200) {
        if (err) {
            *err = rc != CURLE_OK ? (errbuf[0] ? errbuf : curl_easy_strerror(rc))
                                  : ("HTTP " + std::to_string((long long) code));
        }
        remove(path.c_str());            /* 半成品立刻删掉（不占 /tmp） */
        return false;
    }
    return true;
}

} /* namespace cmcore */
