/*
 * Json.hpp - 极简 JSON DOM 解析（CloudMusic-F133）
 *
 * 为什么不用现成库：
 *   easyui 自带的 jsoncpp（include/json/json.h）是**无 JSON_HAS_INT64 的旧构建**，
 *   链接时符号缺失（`Json::Value::Value(long)` / `asInt64()` undefined）——
 *   真机链接失败（本工程实测）。独立包里的 rapidjson 需要额外依赖包。
 *   本工程只需要「取几个字段」，自己写 200 行递归下降解析器最稳、零依赖。
 *
 * 支持：对象/数组/字符串(含 \uXXXX -> UTF-8)/数字(整数与浮点)/true/false/null；
 *       不理解的东西直接报错（不静默吞）。DOM 用 vector 保序（网易云接口字段顺序稳定，
 *       且我们只按名取），内存随解析完的文本一起释放。
 * ⚠️ 只在工作线程里解析（大响应不要放 UI 线程）。
 */
#ifndef _CM_CORE_JSON_HPP_
#define _CM_CORE_JSON_HPP_

#include <stdint.h>

#include <string>
#include <utility>
#include <vector>

namespace cmcore {

class JVal {
public:
    enum Type {
        T_NULL = 0,
        T_BOOL,
        T_NUM,
        T_STR,
        T_ARR,
        T_OBJ,
    };

    JVal();
    ~JVal();

    Type type() const { return mType; }
    bool isNull() const { return mType == T_NULL; }
    bool isArr() const { return mType == T_ARR; }
    bool isObj() const { return mType == T_OBJ; }
    /** 数组/对象元素个数 */
    size_t size() const;
    /** 数组元素（越界返回静态 null） */
    const JVal &at(size_t i) const;
    /** 对象键名（越界返回空串） */
    const std::string &keyAt(size_t i) const;
    /** 对象取键（不存在返回静态 null） */
    const JVal &get(const char *k) const;

    /* 取值（类型不符回默认值，绝不抛） */
    const std::string &str() const;
    double num(double def = 0.0) const;
    long long i64(long long def = 0) const;
    int i32(int def = 0) const;
    bool boolean(bool def = false) const;

    /* 便捷：先 get 再取值 */
    std::string strOf(const char *k, const std::string &def = "") const;
    long long i64Of(const char *k, long long def = 0) const;
    int i32Of(const char *k, int def = 0) const;
    bool boolOf(const char *k, bool def = false) const;

    /* 构建（AppState 落盘用） */
    void setStr(const std::string &v);
    void setI64(long long v);
    void setInt(int v);
    void setBool(bool v);
    void setArr();
    void setObj();
    /** 追加数组元素 / 对象成员（返回可继续填充的新节点引用） */
    JVal &append(const JVal &v);
    JVal &addMember(const std::string &k, const JVal &v);
    /** 序列化为 JSON 文本（对象/数组/标量都行） */
    std::string dump() const;

private:
    Type mType;
    std::string mStr;
    double mNum;
    bool mBool;
    std::vector<JVal> mArr;
    std::vector<std::pair<std::string, JVal> > mObj;

    void dumpTo(std::string *out) const;
};

/** 解析 JSON 文本；失败返回 false 并给出位置与原因 */
bool jsonParse(const std::string &text, JVal *out, std::string *err);

/** JSON 字符串转义（写盘用） */
std::string jsonEscape(const std::string &s);

} /* namespace cmcore */

#endif /* _CM_CORE_JSON_HPP_ */
