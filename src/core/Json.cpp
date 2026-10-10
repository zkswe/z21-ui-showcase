/*
 * Json.cpp - 极简 JSON DOM 实现（递归下降）
 */
#include "core/Json.hpp"

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace cmcore {

static JVal sNull;

JVal::JVal() : mType(T_NULL), mNum(0.0), mBool(false) {
}

JVal::~JVal() {
}

size_t JVal::size() const {
    if (mType == T_ARR) {
        return mArr.size();
    }
    if (mType == T_OBJ) {
        return mObj.size();
    }
    return 0;
}

const JVal &JVal::at(size_t i) const {
    if (mType == T_ARR && i < mArr.size()) {
        return mArr[i];
    }
    if (mType == T_OBJ && i < mObj.size()) {
        return mObj[i].second;
    }
    return sNull;
}

const std::string &JVal::keyAt(size_t i) const {
    if (mType == T_OBJ && i < mObj.size()) {
        return mObj[i].first;
    }
    return sNull.mStr;
}

const JVal &JVal::get(const char *k) const {
    if (mType != T_OBJ || k == NULL) {
        return sNull;
    }
    for (size_t i = 0; i < mObj.size(); ++i) {
        if (mObj[i].first == k) {
            return mObj[i].second;
        }
    }
    return sNull;
}

const std::string &JVal::str() const {
    return mType == T_STR ? mStr : sNull.mStr;
}

double JVal::num(double def) const {
    return mType == T_NUM ? mNum : def;
}

long long JVal::i64(long long def) const {
    if (mType == T_NUM) {
        return (long long) mNum;
    }
    if (mType == T_STR) {
        return atoll(mStr.c_str());
    }
    return def;
}

int JVal::i32(int def) const {
    return (int) i64(def);
}

bool JVal::boolean(bool def) const {
    if (mType == T_BOOL) {
        return mBool;
    }
    if (mType == T_NUM) {
        return mNum != 0.0;
    }
    return def;
}

std::string JVal::strOf(const char *k, const std::string &def) const {
    const JVal &v = get(k);
    return v.mType == T_STR ? v.mStr : def;
}

long long JVal::i64Of(const char *k, long long def) const {
    const JVal &v = get(k);
    return v.mType == T_NULL ? def : v.i64(def);
}

int JVal::i32Of(const char *k, int def) const {
    return (int) i64Of(k, def);
}

bool JVal::boolOf(const char *k, bool def) const {
    const JVal &v = get(k);
    return v.mType == T_NULL ? def : v.boolean(def);
}

void JVal::setStr(const std::string &v) {
    mType = T_STR;
    mStr = v;
}

void JVal::setI64(long long v) {
    mType = T_NUM;
    mNum = (double) v;
}

void JVal::setInt(int v) {
    setI64((long long) v);
}

void JVal::setBool(bool v) {
    mType = T_BOOL;
    mBool = v;
}

void JVal::setArr() {
    mType = T_ARR;
}

void JVal::setObj() {
    mType = T_OBJ;
}

JVal &JVal::append(const JVal &v) {
    if (mType != T_ARR) {
        setArr();
    }
    mArr.push_back(v);
    return mArr.back();
}

JVal &JVal::addMember(const std::string &k, const JVal &v) {
    if (mType != T_OBJ) {
        setObj();
    }
    mObj.push_back(std::make_pair(k, v));
    return mObj.back().second;
}

std::string jsonEscape(const std::string &s) {
    std::string o;
    o.reserve(s.size() + 8);
    for (size_t i = 0; i < s.size(); ++i) {
        const unsigned char c = (unsigned char) s[i];
        switch (c) {
        case '"': o += "\\\""; break;
        case '\\': o += "\\\\"; break;
        case '\n': o += "\\n"; break;
        case '\r': o += "\\r"; break;
        case '\t': o += "\\t"; break;
        default:
            if (c < 0x20) {
                char buf[8];
                snprintf(buf, sizeof(buf), "\\u%04x", c);
                o += buf;
            } else {
                o += (char) c;
            }
        }
    }
    return o;
}

static void numToStr(double v, std::string *out) {
    char buf[40];
    if (v == (double) (long long) v && fabs(v) < 1e15) {
        snprintf(buf, sizeof(buf), "%lld", (long long) v);
    } else {
        snprintf(buf, sizeof(buf), "%.6f", v);
    }
    *out += buf;
}

void JVal::dumpTo(std::string *out) const {
    switch (mType) {
    case T_NULL:
        *out += "null";
        break;
    case T_BOOL:
        *out += mBool ? "true" : "false";
        break;
    case T_NUM:
        numToStr(mNum, out);
        break;
    case T_STR:
        *out += "\"";
        *out += jsonEscape(mStr);
        *out += "\"";
        break;
    case T_ARR: {
        *out += "[";
        for (size_t i = 0; i < mArr.size(); ++i) {
            if (i > 0) {
                *out += ",";
            }
            mArr[i].dumpTo(out);
        }
        *out += "]";
        break;
    }
    case T_OBJ: {
        *out += "{";
        for (size_t i = 0; i < mObj.size(); ++i) {
            if (i > 0) {
                *out += ",";
            }
            *out += "\"";
            *out += jsonEscape(mObj[i].first);
            *out += "\":";
            mObj[i].second.dumpTo(out);
        }
        *out += "}";
        break;
    }
    default:
        *out += "null";
        break;
    }
}

std::string JVal::dump() const {
    std::string o;
    dumpTo(&o);
    return o;
}

/* ---------------- 解析器 ---------------- */
namespace {

class Parser {
public:
    Parser(const std::string &t) : mT(t), mP(0), mDepth(0) {
    }

    bool parse(JVal *out, std::string *err) {
        skipWs();
        if (!parseValue(out, err)) {
            return false;
        }
        skipWs();
        if (mP != mT.size()) {
            return fail(err, "尾部有多余字符");
        }
        return true;
    }

private:
    bool fail(std::string *err, const char *why) {
        if (err != NULL) {
            char buf[192];
            snprintf(buf, sizeof(buf), "JSON 解析失败（位置 %d）: %s", (int) mP, why);
            *err = buf;
        }
        return false;
    }

    void skipWs() {
        while (mP < mT.size()) {
            const char c = mT[mP];
            if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
                ++mP;
            } else {
                break;
            }
        }
    }

    bool parseValue(JVal *out, std::string *err) {
        if (++mDepth > 64) {
            return fail(err, "嵌套过深");
        }
        skipWs();
        if (mP >= mT.size()) {
            return fail(err, "意外结束");
        }
        const char c = mT[mP];
        bool ok = false;
        if (c == '{') {
            ok = parseObject(out, err);
        } else if (c == '[') {
            ok = parseArray(out, err);
        } else if (c == '"') {
            std::string s;
            ok = parseString(&s, err);
            if (ok) {
                out->setStr(s);
            }
        } else if (c == 't' || c == 'f' || c == 'n') {
            ok = parseLiteral(out, err);
        } else {
            ok = parseNumber(out, err);
        }
        --mDepth;
        return ok;
    }

    bool parseLiteral(JVal *out, std::string *err) {
        if (mT.compare(mP, 4, "true") == 0) {
            mP += 4;
            out->setBool(true);
            return true;
        }
        if (mT.compare(mP, 5, "false") == 0) {
            mP += 5;
            out->setBool(false);
            return true;
        }
        if (mT.compare(mP, 4, "null") == 0) {
            mP += 4;
            out->setStr(std::string());
            *out = JVal();
            return true;
        }
        return fail(err, "非法字面量");
    }

    bool parseNumber(JVal *out, std::string *err) {
        const size_t start = mP;
        if (mP < mT.size() && (mT[mP] == '-' || mT[mP] == '+')) {
            ++mP;
        }
        bool any = false;
        while (mP < mT.size() && isdigit((unsigned char) mT[mP])) {
            ++mP;
            any = true;
        }
        if (mP < mT.size() && mT[mP] == '.') {
            ++mP;
            while (mP < mT.size() && isdigit((unsigned char) mT[mP])) {
                ++mP;
                any = true;
            }
        }
        if (any && mP < mT.size() && (mT[mP] == 'e' || mT[mP] == 'E')) {
            ++mP;
            if (mP < mT.size() && (mT[mP] == '-' || mT[mP] == '+')) {
                ++mP;
            }
            while (mP < mT.size() && isdigit((unsigned char) mT[mP])) {
                ++mP;
            }
        }
        if (!any) {
            return fail(err, "非法数字");
        }
        const std::string n = mT.substr(start, mP - start);
        *out = JVal();
        out->setI64((long long) atoll(n.c_str()));
        return true;
    }

    bool parseString(std::string *out, std::string *err) {
        if (mP >= mT.size() || mT[mP] != '"') {
            return fail(err, "字符串缺少引号");
        }
        ++mP;
        out->clear();
        while (mP < mT.size()) {
            const char c = mT[mP++];
            if (c == '"') {
                return true;
            }
            if (c != '\\') {
                *out += c;
                continue;
            }
            if (mP >= mT.size()) {
                return fail(err, "转义未结束");
            }
            const char e = mT[mP++];
            switch (e) {
            case '"': *out += '"'; break;
            case '\\': *out += '\\'; break;
            case '/': *out += '/'; break;
            case 'b': *out += '\b'; break;
            case 'f': *out += '\f'; break;
            case 'n': *out += '\n'; break;
            case 'r': *out += '\r'; break;
            case 't': *out += '\t'; break;
            case 'u': {
                if (mP + 4 > mT.size()) {
                    return fail(err, "\\u 转义不完整");
                }
                unsigned cp = 0;
                for (int i = 0; i < 4; ++i) {
                    const char h = mT[mP + i];
                    cp <<= 4;
                    if (h >= '0' && h <= '9') {
                        cp |= (unsigned) (h - '0');
                    } else if (h >= 'a' && h <= 'f') {
                        cp |= (unsigned) (h - 'a' + 10);
                    } else if (h >= 'A' && h <= 'F') {
                        cp |= (unsigned) (h - 'A' + 10);
                    } else {
                        return fail(err, "\\u 后不是十六进制");
                    }
                }
                mP += 4;
                /* UTF-16 代理对（很少见，网易云歌词里偶有） */
                if (cp >= 0xD800 && cp <= 0xDBFF && mP + 6 <= mT.size()
                        && mT[mP] == '\\' && mT[mP + 1] == 'u') {
                    unsigned lo = 0;
                    bool ok = true;
                    for (int i = 0; i < 4; ++i) {
                        const char h = mT[mP + 2 + i];
                        lo <<= 4;
                        if (h >= '0' && h <= '9') {
                            lo |= (unsigned) (h - '0');
                        } else if (h >= 'a' && h <= 'f') {
                            lo |= (unsigned) (h - 'a' + 10);
                        } else if (h >= 'A' && h <= 'F') {
                            lo |= (unsigned) (h - 'A' + 10);
                        } else {
                            ok = false;
                            break;
                        }
                    }
                    if (ok && lo >= 0xDC00 && lo <= 0xDFFF) {
                        cp = 0x10000 + ((cp - 0xD800) << 10) + (lo - 0xDC00);
                        mP += 6;
                    }
                }
                appendUtf8(out, cp);
                break;
            }
            default:
                return fail(err, "未知转义");
            }
        }
        return fail(err, "字符串未闭合");
    }

    static void appendUtf8(std::string *out, unsigned cp) {
        if (cp < 0x80) {
            *out += (char) cp;
        } else if (cp < 0x800) {
            *out += (char) (0xC0 | (cp >> 6));
            *out += (char) (0x80 | (cp & 0x3F));
        } else if (cp < 0x10000) {
            *out += (char) (0xE0 | (cp >> 12));
            *out += (char) (0x80 | ((cp >> 6) & 0x3F));
            *out += (char) (0x80 | (cp & 0x3F));
        } else {
            *out += (char) (0xF0 | (cp >> 18));
            *out += (char) (0x80 | ((cp >> 12) & 0x3F));
            *out += (char) (0x80 | ((cp >> 6) & 0x3F));
            *out += (char) (0x80 | (cp & 0x3F));
        }
    }

    bool parseArray(JVal *out, std::string *err) {
        ++mP;                    /* '[' */
        out->setArr();
        skipWs();
        if (mP < mT.size() && mT[mP] == ']') {
            ++mP;
            return true;
        }
        while (true) {
            JVal v;
            if (!parseValue(&v, err)) {
                return false;
            }
            out->append(v);
            skipWs();
            if (mP >= mT.size()) {
                return fail(err, "数组未闭合");
            }
            if (mT[mP] == ',') {
                ++mP;
                continue;
            }
            if (mT[mP] == ']') {
                ++mP;
                return true;
            }
            return fail(err, "数组内需要 , 或 ]");
        }
    }

    bool parseObject(JVal *out, std::string *err) {
        ++mP;                    /* '{' */
        out->setObj();
        skipWs();
        if (mP < mT.size() && mT[mP] == '}') {
            ++mP;
            return true;
        }
        while (true) {
            skipWs();
            std::string k;
            if (!parseString(&k, err)) {
                return false;
            }
            skipWs();
            if (mP >= mT.size() || mT[mP] != ':') {
                return fail(err, "对象成员缺少 :");
            }
            ++mP;
            JVal v;
            if (!parseValue(&v, err)) {
                return false;
            }
            out->addMember(k, v);
            skipWs();
            if (mP >= mT.size()) {
                return fail(err, "对象未闭合");
            }
            if (mT[mP] == ',') {
                ++mP;
                continue;
            }
            if (mT[mP] == '}') {
                ++mP;
                return true;
            }
            return fail(err, "对象内需要 , 或 }");
        }
    }

    const std::string &mT;
    size_t mP;
    int mDepth;
};

} /* anonymous namespace */

bool jsonParse(const std::string &text, JVal *out, std::string *err) {
    if (out == NULL) {
        return false;
    }
    Parser p(text);
    return p.parse(out, err);
}

} /* namespace cmcore */
