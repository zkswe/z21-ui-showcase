# -*- coding: utf-8 -*-
"""程序化合成 3 首 demo 曲（无版权素材）→ WAV，再交 ffmpeg 转 MP3。
每首：和弦进行 + 贝斯 + 琶音/主旋律 + 轻打击，ADSR + 单极低通，软限幅。
"""
import os, wave, subprocess, struct
import numpy as np

SR = 44100
OUT = r'C:\Users\zkswe\.openclaw\workspace\projects\UIShowcase-Z21\resources\music'
FF = r'C:\Users\zkswe\AppData\Local\Microsoft\WinGet\Packages\Gyan.FFmpeg.Essentials_Microsoft.Winget.Source_8wekyb3d8bbwe\ffmpeg-8.1.1-essentials_build\bin\ffmpeg.exe'
os.makedirs(OUT, exist_ok=True)

NOTE = {}
for i, n in enumerate(['C','C#','D','D#','E','F','F#','G','G#','A','A#','B']):
    for octv in range(0, 9):
        NOTE['%s%d' % (n, octv)] = 440.0 * 2 ** ((i - 9) / 12.0 + (octv - 4))


def freq(name):
    return NOTE[name]


def adsr(n, a=0.01, d=0.1, s=0.7, r=0.2):
    ae, de = int(a * SR), int(d * SR)
    re = int(r * SR)
    se = max(0, n - ae - de - re)
    env = np.concatenate([
        np.linspace(0, 1, max(1, ae)),
        np.linspace(1, s, max(1, de)),
        np.full(se, s),
        np.linspace(s, 0, max(1, re)),
    ])
    if len(env) < n:
        env = np.pad(env, (0, n - len(env)), 'edge')
    return env[:n]


def tone(f, dur, kind='sine', a=0.01, d=0.1, s=0.7, r=0.25, detune=0.0):
    n = int(dur * SR)
    t = np.arange(n) / SR
    if kind == 'saw':
        w = 2.0 * ((f * t) % 1.0) - 1.0
    elif kind == 'square':
        w = np.sign(np.sin(2 * np.pi * f * t)) * 0.6
    elif kind == 'tri':
        w = 2 * np.abs(2 * ((f * t) % 1) - 1) - 1
    else:
        w = np.sin(2 * np.pi * f * t)
        w = w + 0.28 * np.sin(2 * np.pi * 2 * f * t) + 0.12 * np.sin(2 * np.pi * 3 * f * t)
    if detune:
        w = w + 0.5 * np.sin(2 * np.pi * (f * (1 + detune)) * t)
    return w * adsr(n, a, d, s, r)


def lp(x, cutoff):
    a = 1 - np.exp(-2 * np.pi * cutoff / SR)
    y = np.empty_like(x)
    acc = 0.0
    for i in range(len(x)):
        acc += a * (x[i] - acc)
        y[i] = acc
    return y


def lp_fast(x, cutoff):
    """单极 IIR 的向量化近似（分块递推）。"""
    a = 1 - np.exp(-2 * np.pi * cutoff / SR)
    y = np.empty_like(x)
    acc = 0.0
    for i in range(0, len(x), 512):
        blk = x[i:i + 512]
        for j in range(len(blk)):
            acc += a * (blk[j] - acc)
            y[i + j] = acc
    return y


def kick(dur=0.18):
    n = int(dur * SR)
    t = np.arange(n) / SR
    f = 120 * np.exp(-t * 28) + 42
    return np.sin(2 * np.pi * np.cumsum(f) / SR) * np.exp(-t * 14) * 0.9


def hat(dur=0.05, seed=0):
    n = int(dur * SR)
    rng = np.random.default_rng(seed)
    return rng.standard_normal(n) * np.exp(-np.arange(n) / SR * 90) * 0.18


def mix(buf, sig, at):
    i = int(at * SR)
    if i >= len(buf):
        return
    j = min(len(buf), i + len(sig))
    buf[i:j] += sig[:j - i]


def soft_clip(x, g=1.0):
    x = x * g
    return np.tanh(x * 1.25) / np.tanh(1.25)


def make_track(bpm, bars, prog, lead_kind, bass_kind, lead_oct, arp_oct, drums, seed):
    beat = 60.0 / bpm
    total = bars * 4 * beat
    n = int(total * SR) + SR
    L = np.zeros(n); R = np.zeros(n)
    rng = np.random.default_rng(seed)

    for b in range(bars):
        root_name, chord, kind = prog[b % len(prog)]
        t0 = b * 4 * beat
        # --- pad / chord
        for k, nm in enumerate(chord):
            f = freq(nm)
            dur = 4 * beat * 1.02
            v = tone(f, dur, 'sine', a=0.25, d=0.3, s=0.55, r=0.6, detune=0.004)
            pan = 0.5 + (k - len(chord) / 2.0) * 0.22
            mix(L, v * (1 - pan) * 0.30, t0)
            mix(R, v * pan * 0.30, t0)
        # --- bass
        for bb in range(4):
            f = freq(root_name) / 2
            if bass_kind == 'walk' and bb % 2 == 1:
                f *= 1.5 if (b + bb) % 4 >= 2 else 1.25
            v = tone(f, beat * 0.92, 'tri', a=0.005, d=0.15, s=0.6, r=0.2)
            mix(L, v * 0.42, t0 + bb * beat)
            mix(R, v * 0.42, t0 + bb * beat)
        # --- lead / arp
        steps = 8
        for s in range(steps):
            idx = s % len(chord)
            nm = chord[idx]
            o = lead_oct if s % 2 == 0 else arp_oct
            f = freq(nm) * (2 ** o)
            dur = beat / 2 * 0.95
            v = tone(f, dur, lead_kind, a=0.004, d=0.08, s=0.45, r=0.16)
            at = t0 + s * beat / 2
            g = 0.30 if s % 2 == 0 else 0.18
            mix(L, v * g * 0.9, at)
            mix(R, v * g, at + 0.012)   # 轻微延迟做立体声宽
        # --- drums
        if drums:
            for bb in range(4):
                if bb in (0, 2) or (bb == 3 and b % 2 == 1):
                    mix(L, kick(), t0 + bb * beat); mix(R, kick(), t0 + bb * beat)
                for hh in range(2):
                    h = hat(seed=seed * 17 + b * 7 + bb * 2 + hh)
                    mix(L, h * (1 - hh * 0.5), t0 + bb * beat + hh * beat / 2)
                    mix(R, h * hh * 0.9, t0 + bb * beat + hh * beat / 2)

    L = lp_fast(L, 9000); R = lp_fast(R, 9000)
    L = soft_clip(L, 0.9); R = soft_clip(R, 0.9)
    # 首尾各 0.6s 淡入淡出
    f = int(0.6 * SR)
    L[:f] *= np.linspace(0, 1, f); R[:f] *= np.linspace(0, 1, f)
    L[-f:] *= np.linspace(1, 0, f); R[-f:] *= np.linspace(1, 0, f)
    return L, R


def write_wav(path, L, R):
    data = np.empty(len(L) * 2, dtype=np.int16)
    data[0::2] = (np.clip(L, -1, 1) * 32000).astype(np.int16)
    data[1::2] = (np.clip(R, -1, 1) * 32000).astype(np.int16)
    with wave.open(path, 'wb') as w:
        w.setnchannels(2); w.setsampwidth(2); w.setframerate(SR)
        w.writeframes(data.tobytes())


TRACKS = [
    ('demo_01', dict(bpm=96, bars=20, prog=[('C3', ['C4','E4','G4'], 'M'), ('G2', ['G3','B3','D4'], 'M'),
                                            ('A2', ['A3','C4','E4'], 'm'), ('F2', ['F3','A3','C4'], 'M')],
                     lead_kind='sine', bass_kind='walk', lead_oct=0, arp_oct=1, drums=True, seed=11)),
    ('demo_02', dict(bpm=132, bars=24, prog=[('A2', ['A3','C4','E4'], 'm'), ('F2', ['F3','A3','C4'], 'M'),
                                             ('C3', ['C4','E4','G4'], 'M'), ('G2', ['G3','B3','D4'], 'M')],
                     lead_kind='square', bass_kind='root', lead_oct=1, arp_oct=1, drums=True, seed=23)),
    ('demo_03', dict(bpm=80, bars=18, prog=[('D3', ['D4','F4','A4'], 'm'), ('B2', ['B3','D4','F#4'], 'm'),
                                            ('G2', ['G3','B3','D4'], 'M'), ('A2', ['A3','C#4','E4'], 'M')],
                     lead_kind='tri', bass_kind='root', lead_oct=0, arp_oct=1, drums=False, seed=37)),
]

for name, cfg in TRACKS:
    L, R = make_track(**cfg)
    wav = os.path.join(OUT, name + '.wav')
    mp3 = os.path.join(OUT, name + '.mp3')
    write_wav(wav, L, R)
    subprocess.run([FF, '-y', '-hide_banner', '-loglevel', 'error', '-i', wav,
                    '-codec:a', 'libmp3lame', '-b:a', '128k', '-ar', '44100', mp3], check=True)
    os.remove(wav)
    print('%-8s %5.1f s  %6.1f KB' % (name, len(L) / SR, os.path.getsize(mp3) / 1024.0))
