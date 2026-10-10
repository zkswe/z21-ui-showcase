/* fbcap.c — 抓 framebuffer 一块区域并转 RGB565LE（设备端，静态编译）
 * 用法: fbcap <out> <panY> <x0> <y0> <w> <h>
 *   屏幕 1024x600；panY = /sys/class/graphics/fb0/pan 的第一个数（可见缓冲起始行）
 * 输出: w*h*2 字节，小端 565
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc < 7) { fprintf(stderr, "usage: fbcap out panY x0 y0 w h\n"); return 2; }
    const char *out = argv[1];
    int panY = atoi(argv[2]), x0 = atoi(argv[3]), y0 = atoi(argv[4]);
    int w = atoi(argv[5]), h = atoi(argv[6]);
    const int stride = 1024 * 4;

    int fd = open("/dev/fb0", O_RDONLY);
    if (fd < 0) { perror("open fb0"); return 3; }
    FILE *f = fopen(out, "wb");
    if (!f) { perror("fopen out"); return 4; }

    unsigned short *o = (unsigned short *)malloc((size_t)w * 2);
    unsigned char *row = (unsigned char *)malloc((size_t)w * 4);
    if (!o || !row) return 5;

    for (int y = 0; y < h; y++) {
        long off = (long)(panY + y0 + y) * stride + (long)x0 * 4;
        if (lseek(fd, off, SEEK_SET) != off) break;
        if (read(fd, row, (size_t)w * 4) != (ssize_t)w * 4) break;
        for (int x = 0; x < w; x++) {
            unsigned char b = row[x * 4 + 0], g = row[x * 4 + 1], r = row[x * 4 + 2];
            o[x] = (unsigned short)(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
        }
        fwrite(o, 2, (size_t)w, f);
    }
    fclose(f);
    close(fd);
    free(o);
    free(row);
    return 0;
}
