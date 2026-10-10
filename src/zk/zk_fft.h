#ifndef ZK_FFT_H_
#define ZK_FFT_H_

#include <math.h>

/* 轻量 radix-2 FFT（仅本工程频谱用；N 必须是 2 的幂，且 <= 2048）
 * 实输入 → 只取前 N/2+1 个 bin 的幅度，够画频谱了。 */
class ZkFft {
public:
    explicit ZkFft(int n = 1024) : mN(n), mBits(0) {
        while ((1 << mBits) < mN) ++mBits;
        mRev = new int[mN];
        mCos = new float[mN / 2];
        mSin = new float[mN / 2];
        for (int i = 0; i < mN; ++i) {
            int r = 0;
            for (int b = 0; b < mBits; ++b) {
                if (i & (1 << b)) r |= 1 << (mBits - 1 - b);
            }
            mRev[i] = r;
        }
        const float kPi = 3.14159265358979f;
        for (int i = 0; i < mN / 2; ++i) {
            mCos[i] = cosf(-2.0f * kPi * i / mN);
            mSin[i] = sinf(-2.0f * kPi * i / mN);
        }
    }

    ~ZkFft() {
        delete[] mRev;
        delete[] mCos;
        delete[] mSin;
    }

    /* x: 实输入（长度 mN）；win: 窗（长度 mN，可为 NULL）；mag: 输出幅度（长度 mN/2+1） */
    void magnitude(const float *x, const float *win, float *mag) {
        for (int i = 0; i < mN; ++i) {
            mRe[i] = win ? x[i] * win[i] : x[i];
            mIm[i] = 0.0f;
        }
        for (int i = 0; i < mN; ++i) {          /* 位反转重排 */
            int j = mRev[i];
            if (j > i) {
                float t = mRe[i]; mRe[i] = mRe[j]; mRe[j] = t;
                t = mIm[i]; mIm[i] = mIm[j]; mIm[j] = t;
            }
        }
        for (int len = 2; len <= mN; len <<= 1) {  /* 蝶形 */
            int half = len >> 1;
            int step = mN / len;
            for (int i = 0; i < mN; i += len) {
                for (int j = 0; j < half; ++j) {
                    float wr = mCos[j * step];
                    float wi = mSin[j * step];
                    int a = i + j;
                    int b = a + half;
                    float xr = mRe[b] * wr - mIm[b] * wi;
                    float xi = mRe[b] * wi + mIm[b] * wr;
                    mRe[b] = mRe[a] - xr;
                    mIm[b] = mIm[a] - xi;
                    mRe[a] += xr;
                    mIm[a] += xi;
                }
            }
        }
        for (int i = 0; i <= mN / 2; ++i) {
            mag[i] = sqrtf(mRe[i] * mRe[i] + mIm[i] * mIm[i]);
        }
    }

    int size() const { return mN; }

private:
    int   mN;
    int   mBits;
    int  *mRev;
    float *mCos;
    float *mSin;
    float mRe[2048];
    float mIm[2048];
};

/* Hann 窗（频谱用，两端到零，减少泄漏） */
static inline void zkHann(float *w, int n) {
    const float kPi = 3.14159265358979f;
    for (int i = 0; i < n; ++i) {
        w[i] = 0.5f - 0.5f * cosf(2.0f * kPi * i / (n - 1));
    }
}

#endif /* ZK_FFT_H_ */
