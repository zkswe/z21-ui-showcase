/*
 * Http.hpp - 极简 libcurl HTTP 客户端（CloudMusic-F133）
 *
 * 为什么自己写一层：本工程只需要「同步 GET 一小段文本/二进制 + 超时 + 统计」，
 * libcurl 直接调用即可（curl 8.12.1 包已进 Manifest）。控制Test-F133 的
 * src/core/HttpStream.cc 走的是流式拉流（带落盘/断流细化），本工程用不上，
 * 这里按「一次性取完，带整体超时」收敛成 60 行，接口更小。
 *
 * 全部在调用者线程里同步执行 —— 因此**只在工作线程里调用**，UI 线程不许调。
 */
#ifndef _CM_CORE_HTTP_HPP_
#define _CM_CORE_HTTP_HPP_

#include <string>

namespace cmcore {

struct HttpResult {
    bool ok;
    int httpCode;               /**< HTTP 状态码；-1 = 连接/协议层失败 */
    std::string body;           /**< 响应体（二进制安全） */
    std::string err;            /**< 失败原因（人类可读，含 curl 原文） */
    long long ms;               /**< 耗时（连接+首字节+读完） */

    HttpResult() : ok(false), httpCode(-1), ms(0) {}
};

/** 同步 GET（整体超时 timeoutMs；连接超时取 min(4000, timeoutMs)） */
HttpResult httpGet(const std::string &url, int timeoutMs = 8000);

/** 下载到文件（覆盖写）；返回是否成功 + 落盘字节数 */
bool httpDownload(const std::string &url, const std::string &path,
                  long long *bytesOut, std::string *err, int timeoutMs = 15000);

/** curl_global_init 只做一次（进程内幂等） */
void httpGlobalInit();

/** URL 查询串转义（curl_easy_escape） */
std::string urlEncode(const std::string &s);

} /* namespace cmcore */

#endif /* _CM_CORE_HTTP_HPP_ */
