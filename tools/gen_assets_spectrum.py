# -*- coding: utf-8 -*-
"""从 MP3 离线算频谱（真实音频分析）→ 紧凑二进制给设备按播放位置回放。

为什么要离线算：设备上 ZKMediaPlayer 不暴露 PCM，没法在 App 里做 FFT；
本工程三首曲子是我们自己合成的，**在 PC 侧算出真实频谱**，运行时按 `getCurrentPosition()`
索引帧回放即可 —— 频谱与真实音频严格同步（拖进度也会跟着跳），且设备端零 FFT 开销。

文件格式：'ZKS1' + u16 bands + u16 fps100 + u32 frames + u32 reserved，随后 frames*bands 个 uint8。
"""
import os, struct, subprocess
import numpy as np

FF = r'C:\Users\zkswe\AppData\Local\Microsoft\WinGet\Packages\Gyan.FFmpeg.Essentials_Microsoft.Winget.Source_8wekyb3d8bbwe\ffmpeg-8.1.1-essentials_build\bin\ffmpeg.exe'
SR = 44100
NFFT = 2048
HOP = 1102           # ≈ 40 fps
BANDS = 28
F_LO, F_HI = 60.0, 16000.0
PROJ = r'C:\Users\zkswe\.openclaw\workspace\projects\UIShowcase-Z21'
OUT = os.path.join(PROJ, 'resources', 'spectrum')
os.makedirs(OUT, exist_ok=True)


def decode(path):
    raw = subprocess.run([FF, '-v', 'error', '-i', path, '-f', 'f32le', '-ac', '1', '-ar', str(SR), '-'],
                         capture_output=True, check=True).stdout
    return np.frombuffer(raw, dtype=np.float32)


def spectrum(x):
    n = 1 + (len(x) - NFFT) // HOP
    idx = np.arange(NFFT)[None, :] + HOP * np.arange(n)[:, None]
    win = np.hanning(NFFT).astype(np.float32)
    X = np.abs(np.fft.rfft(x[idx] * win, axis=1))
    freqs = np.fft.rfftfreq(NFFT, 1.0 / SR)
    edges = np.geomspace(F_LO, F_HI, BANDS + 1)
    out = np.zeros((n, BANDS), np.float32)
    for i in range(BANDS):
        m = (freqs >= edges[i]) & (freqs < edges[i + 1])
        if m.any():
            out[:, i] = X[:, m].mean(axis=1)
    # 转 dB → 每条频带按自身 97 分位归一（低/高频都活起来）→ 42dB 动态范围
    db = 20.0 * np.log10(out + 1e-9)
    ref = np.percentile(db, 97, axis=0)
    v = np.clip((db - (ref - 42.0)) / 42.0, 0.0, 1.0) ** 0.85
    k = np.hanning(5); k /= k.sum()
    v = np.stack([np.convolve(v[:, i], k, mode='same') for i in range(BANDS)], axis=1)
    return v


def main():
    fps100 = int(round(100.0 * SR / HOP))
    for i in (1, 2, 3):
        mp3 = os.path.join(PROJ, 'resources', 'music', 'demo_%02d.mp3' % i)
        v = spectrum(decode(mp3))
        data = (v * 255.0 + 0.5).astype(np.uint8)
        path = os.path.join(OUT, 'spec_%d.bin' % i)
        with open(path, 'wb') as f:
            f.write(b'ZKS1')
            f.write(struct.pack('<HHII', BANDS, fps100, data.shape[0], 0))
            f.write(data.tobytes())
        print('spec_%d.bin  frames=%-5d bands=%d fps=%.2f  %.1f KB  (能量 mean %.3f max %.3f)'
              % (i, data.shape[0], BANDS, fps100 / 100.0, os.path.getsize(path) / 1024.0,
                 v.mean(), v.max()))


if __name__ == '__main__':
    main()
