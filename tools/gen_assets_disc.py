# -*- coding: utf-8 -*-
"""音乐页出图：黑胶唱片（旋转用）+ 静态唱盘底 + 方形专辑封面。

口径（沿 image-gen-standard）：
- SS=4 超采样绘制 → Image.BOX（面积平均，禁 LANCZOS/BICUBIC 负瓣振铃）
- 圆形边界 α = 覆盖率（超采样二值 mask → BOX 平均），**不用二值 mask 直接当 α**
"""
import os, math
import numpy as np
from PIL import Image, ImageDraw, ImageFilter

SS = 4
HERE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.join(HERE, 'resources', 'images')
os.makedirs(OUT, exist_ok=True)

D = 300          # 控件尺寸
R_DISC = 148     # 唱片半径
R_LABEL = 84     # 标签半径
R_HOLE = 8


def down(rgb, mask):
    """超采样 → 面积平均；返回 RGBA（α=覆盖率）。"""
    rgb_s = rgb.resize((D, D), Image.BOX)
    a = mask.resize((D, D), Image.BOX)
    rgba = rgb_s.convert('RGBA')
    rgba.putalpha(a)
    return rgba


def new_canvas(bg):
    img = Image.new('RGB', (D * SS, D * SS), bg)
    m = Image.new('L', (D * SS, D * SS), 0)
    return img, m


def ell(d, m, box, fill):
    d.ellipse(box, fill=fill)
    m.ellipse(box, fill=255)


def make_cover(idx):
    """方形专辑封面 176x176（程序化，风格各异）。"""
    W = 176
    S = 4
    img = Image.new('RGB', (W * S, W * S))
    px = img.load()

    pal = [
        ((24, 12, 58), (208, 70, 120), (255, 196, 96)),    # 合成波
        ((250, 236, 212), (228, 87, 46), (26, 92, 110)),   # 包豪斯
        ((14, 18, 40), (36, 128, 168), (140, 236, 224)),   # 深海
    ][idx]
    (c0, c1, c2) = pal
    n = W * S
    for y in range(n):
        t = y / (n - 1.0)
        r = int(c0[0] + (c1[0] - c0[0]) * t)
        g = int(c0[1] + (c1[1] - c0[1]) * t)
        b = int(c0[2] + (c1[2] - c0[2]) * t)
        for x in range(n):
            px[x, y] = (r, g, b)
    d = ImageDraw.Draw(img)
    if idx == 0:
        cx, cy, rr = 0.5 * n, 0.58 * n, 0.22 * n
        d.ellipse([cx - rr, cy - rr, cx + rr, cy + rr], fill=c2)
        d.rectangle([0, 0.60 * n, n, n], fill=(14, 8, 34))
        for i in range(5):
            y = 0.65 * n + i * 0.075 * n
            d.line([0, y, n, y], fill=(60, 220, 230), width=4 * S)
    elif idx == 1:
        d.ellipse([0.06 * n, 0.06 * n, 0.56 * n, 0.96 * n], fill=c1)
        d.ellipse([0.60 * n, 0.12 * n, 0.98 * n, 0.60 * n], fill=c2)
        d.pieslice([0.30 * n, 0.44 * n, 0.90 * n, 1.06 * n], 180, 360, fill=(240, 166, 90))
    else:
        cx, cy = 0.42 * n, 0.72 * n
        for rx, ry, w in [(0.62, 0.38, 5), (0.80, 0.50, 5), (0.98, 0.62, 5)]:
            d.ellipse([cx - rx * n, cy - ry * n, cx + rx * n, cy + ry * n], outline=c2, width=w * S)
        d.ellipse([0.66 * n, 0.10 * n, 0.86 * n, 0.30 * n], fill=(246, 214, 130))
    return img.resize((W, W), Image.BOX)


def make_disc(idx, cover):
    img, mask = new_canvas((21, 21, 26))
    d = ImageDraw.Draw(img)
    m = ImageDraw.Draw(mask)
    C = D * SS / 2.0
    # 盘体 + 边缘
    ell(d, m, [C - R_DISC * SS, C - R_DISC * SS, C + R_DISC * SS, C + R_DISC * SS], (21, 21, 26))
    d.ellipse([C - R_DISC * SS, C - R_DISC * SS, C + R_DISC * SS, C + R_DISC * SS],
              outline=(52, 52, 60), width=2 * SS)
    # 纹路（同心，本身对称）——用亮度微变画"沟槽"
    for k in range(6, 60):
        r = (R_LABEL + 4 + (R_DISC - R_LABEL - 8) * k / 60.0) * SS
        sh = 30 + (4 if k % 3 == 0 else 0)
        d.ellipse([C - r, C - r, C + r, C + r], outline=(sh, sh, sh + 4), width=1 * SS)
    # 非对称亮弧 —— 让旋转看得出来（两段，不同半径）
    d.arc([C - 118 * SS, C - 118 * SS, C + 118 * SS, C + 118 * SS], 205, 262, fill=(96, 96, 112), width=4 * SS)
    d.arc([C - 134 * SS, C - 134 * SS, C + 134 * SS, C + 134 * SS], 116, 152, fill=(74, 74, 88), width=3 * SS)
    # 标签（圆形裁封面）+ 描边
    lab = cover.resize((2 * R_LABEL * SS, 2 * R_LABEL * SS), Image.LANCZOS).convert('RGB')
    lm = Image.new('L', (2 * R_LABEL * SS, 2 * R_LABEL * SS), 0)
    ImageDraw.Draw(lm).ellipse([0, 0, 2 * R_LABEL * SS - 1, 2 * R_LABEL * SS - 1], fill=255)
    img.paste(lab, (int(C - R_LABEL * SS), int(C - R_LABEL * SS)), lm)
    d.ellipse([C - R_LABEL * SS, C - R_LABEL * SS, C + R_LABEL * SS, C + R_LABEL * SS],
              outline=(188, 188, 198), width=2 * SS)
    # 中心孔
    d.ellipse([C - R_HOLE * SS, C - R_HOLE * SS, C + R_HOLE * SS, C + R_HOLE * SS], fill=(12, 12, 15))
    d.ellipse([C - (R_HOLE + 3) * SS, C - (R_HOLE + 3) * SS, C + (R_HOLE + 3) * SS, C + (R_HOLE + 3) * SS],
              outline=(70, 70, 80), width=1 * SS)
    out = down(img, mask)
    return out.filter(ImageFilter.GaussianBlur(0.4))


def make_platter():
    """静态唱盘底：深色圆盘 + 顶部左侧高光（不随唱片转）。"""
    img, mask = new_canvas((16, 16, 20))
    d = ImageDraw.Draw(img)
    m = ImageDraw.Draw(mask)
    C = D * SS / 2.0
    R = R_DISC + 4
    ell(d, m, [C - R * SS, C - R * SS, C + R * SS, C + R * SS], (30, 30, 36))
    d.ellipse([C - R * SS, C - R * SS, C + R * SS, C + R * SS], outline=(58, 58, 68), width=2 * SS)
    # 静态高光弧（左上）
    for w, a in [(26, (86, 86, 100)), (14, (120, 120, 136))]:
        d.arc([C - (R - 6) * SS, C - (R - 6) * SS, C + (R - 6) * SS, C + (R - 6) * SS], 190, 250,
              fill=a, width=w)
    return down(img, mask).filter(ImageFilter.GaussianBlur(0.5))


def main():
    for i in range(3):
        cov = make_cover(i)
        cov.save(os.path.join(OUT, 'album_%02d.png' % (i + 1)))
        make_disc(i, cov).save(os.path.join(OUT, 'disc_%d.png' % (i + 1)))
        print('disc_%d.png' % (i + 1))
    make_platter().save(os.path.join(OUT, 'platter_300.png'))
    print('platter_300.png')


if __name__ == '__main__':
    main()
