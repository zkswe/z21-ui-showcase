<div align="center">

# Z21 UI Showcase · 一套「控件全跑通」的大屏示例

**1024×600 触摸大屏 · 8 个页面 · 从按钮到实时波形，全部是 FlyThings 现成控件**

[![Platform](https://img.shields.io/badge/platform-Z21-blue)](#平台能力)
[![UI](https://img.shields.io/badge/UI-FlyThings%20UI%20(自研)-orange)](#平台能力)
[![Built by AI](https://img.shields.io/badge/built%20by-AI%20%2B%20MCP-9cf)](#二次开发--ai-开发)
[![MCP](https://img.shields.io/badge/MCP-v0.27.201--open-green)](https://github.com/zkswe/mcp_release)

**GitHub 主仓**：https://github.com/zkswe/z21-ui-showcase ｜ **Gitee 镜像**：https://gitee.com/Kwolve/z21-ui-showcase

<img src="showcase/overview.png" width="100%">

</div>

---

## 这是什么

一块 10 寸级的触摸屏上，把**一本 UI 控件手册**跑给你看：

开关能拨、滑块能拖、选择器能滚、日历能翻月、波形在实时重绘、封面墙能滑——**全都不是画上去的图，是真实控件、真实交互**。
这份示例就是**评估一块屏能做什么**的最快方式：插上电，八页翻一遍，控件能力一目了然。

> 这套界面的代码，是**用 AI（MCP 工具链）从 HTML 原型生成的**，不是手搓控件。见[「二次开发 = AI 开发」](#二次开发--ai-开发)。

---

## 为什么选它

| 卖点 | 对你意味着什么 |
|---|---|
| **控件不是画的，是真的** | 开关/复选/单选/滑块/步进/滚轮选择器/日历/列表/画板全可点可拖 → 拿到屏就能验证触控手感，不用先写代码 |
| **动态数据 + 图片缓存** | 封面墙是 **listview 2 列网格 + ImageCache**：条目运行时添加、回页不重复解码 → 你的产品列表可以很长、很流畅 |
| **实时重绘能力** | 图表页波形按帧自绘 → 仪表盘/监护/工控那类「要动起来」的界面直接可用 |
| **整机 UI 一次跑通** | 8 页含导航/页面切换动画/状态栏 → 少走「每个控件都从零试」的弯路 |
| **AI 可继续开发** | 界面与逻辑都能让 AI 接着改（改一行 HTML/json 就出图）→ 不养 GUI 团队也能维护（见下节） |
| **示例即文档** | 对照 8 页截图与 3 段 GIF，一眼看清「这块屏能做到什么程度」 |

---

## 真机效果

全部截图与动效均来自**真机**（Z21，1024×600）。

### 页面切换动画
页面进入时的滑入编排（标题 / 卡片 / 导航栏错峰滑动）：

![页面切换动画](showcase/transition.gif)

### 列表滑动（封面墙 = listview 2 列网格）
动态数据 + 图片缓存，可流畅滑动与回弹：

![列表滑动](showcase/scroll.gif)

### 实时波形（按帧重绘）
图表页波形为运行时自绘：

![实时波形](showcase/waveform.gif)

### 黑胶唱片旋转（真实音频播放）
音乐页在**真机上真播音频**：唱片随播放匀速旋转、暂停即停；进度条与歌词都跟着**真实播放位置**走：

![音乐播放器](showcase/music.gif)

> 抓帧说明：设备端逐帧抓屏后合成。抓帧用 **RGB565 + 只抓感兴趣区域**（工具源码 `tools/fbcap.c`）：全屏 565 约 8.7 fps、裁区域后 9～14 fps。GIF 为抓帧回放，非屏幕实时帧率。

### 八个页面

| 页面 | 能做什么 | 截图 |
|---|---|---|
| **首页 / 仪表盘** | 卡片、指标块、进度条、状态栏图标体系 | <img src="showcase/pages/home.png" width="300"> |
| **控件族** | 按钮（主/描边/文本/禁用）、开关、复选、单选、滑块、步进、提示条、角标、输入框（系统键盘 + 拼音） | <img src="showcase/pages/widgets.png" width="300"> |
| **图表** | 实时折线/波形、环形进度、分组柱状（自绘重绘） | <img src="showcase/pages/charts.png" width="300"> |
| **日期时间** | 数字钟、表盘指针、滚轮选择器（年月日/时分秒）、日历（可翻月/可选中） | <img src="showcase/pages/datetime.png" width="300"> |
| **列表 / 封面墙** | listview 三件套；封面墙为 2 列网格 listview（动态数据 + 图片缓存） | <img src="showcase/pages/list.png" width="300"> |
| **画板** | 自绘 painter：手指绘制、粗细/颜色切换、撤销 / 清空 / 保存 | <img src="showcase/pages/image.png" width="300"> |
| **音乐** | 黑胶唱片旋转 + **真实音频播放**：本地 MP3、暂停/切歌/拖动进度/静音，歌词按时间同步 | <img src="showcase/pages/music.png" width="300"> |
| **设置 / 关于** | 开关组、分段选择、诊断信息、保存 / 重置 | <img src="showcase/pages/settings.png" width="300"> |

---

## 平台能力

| 项 | 说明 |
|---|---|
| 平台 | **Z21**（FlyThings OS） |
| 屏幕 | **1024 × 600** 触摸屏 |
| UI 系统 | **FlyThings UI**（自研 GUI 交互系统，设备侧产物 `zkgui` / `libzkgui.so`） |
| 系统底座 | 基于 **Linux** 的操作系统（内核 + 自研系统框架 + GUI）；应用是普通 Linux 进程 |
| 应用形态 | 界面由 `ui/*.ftu` 描述、逻辑在 `src/logic/*.cc`；`fsc build` 编译成 `libzkgui.so` 推到设备运行 |
| 输入 | 电容触摸（本机示例含触摸注入验证脚本）；文本输入走**系统内置键盘 + 拼音** |
| 网络/外设 | 依赖组件提供 WiFi（`zknet`）、GPIO/背光/ADC/PWM 等硬件 API（`zkhardware`） |

> 整机型号、主控、供电、接口与存储等**整机规格**请见随硬件提供的规格书，或直接联系我们（见文末）。

---

## 用到的组件能力

本示例跑在一条完整的组件链上（`Manifest.xml` 声明，`fsc install` 自动拉取）：

| 组件 | 提供的能力 | 对本项目意味着什么 |
|---|---|---|
| `easyui` | **FlyThings UI 控件库**（按钮/开关/滑块/列表/图表/画板/滚轮…） | 8 页控件全部来自它；不需要自己画控件 |
| `zknet` | 网络能力（WiFi/连接状态） | 设置页的「无线网络」与系统设置页跳转 |
| `zkhardware` | 硬件 API（GPIO/背光/ADC/PWM/蜂鸣器） | 背光、按键/指示灯这类硬件交互的落点 |
| `base-utility` | 基础库（文件/功能/日志等） | 生成代码与业务代码的公共底座 |
| `png` / `jpeg` | PNG / JPEG 解码 | 切图资源与照片类素材的加载 |
| `z` | 压缩库 | 网络/升级链路依赖 |
| `curl` + `openssl` + `cares` | HTTP(S) 客户端 / TLS / 异步 DNS | 需要联网拉数据（列表/封面/接口）时的现成通道 |

> 复用组件（`components/`：BLE 门面、字库、图标库、自定义控件包等）见 MCP 仓库的 `components/` 目录。

---

## 二次开发 = AI 开发

**这套界面是 AI 生成的**：从 HTML 交互原型 → 生成 `ui/*.json` → 打包成 `ui/*.ftu` → `fsc build` 编译 → 推到真机。
你要做的是**说清楚界面长什么样**，剩下交给工具链（MCP）。

| 能力 | 怎么做 |
|---|---|
| **改界面** | 改一行 HTML/json → 出确认稿 → 打包编译（不用手写控件代码） |
| **加一个页面** | 在同工程加一个「整屏 window」或用 HTML 原型生成新 json（**一页一 Activity** 只在跨业务域时拆） |
| **加一个游戏/子页/弹窗** | 按现有页面结构复制改内容；弹窗=同页里一个整屏 window |
| **查控件怎么用** | 全离线知识库检索（**不需要 API Key**），或直接问 AI；官方文档站 `developer.flythings.cn` |
| **验证效果** | 拖拽式 UI 编辑器 → 写回 json；像素 diff 验收；真机抓屏对比 |
| **上真机** | `fsc build` 编译 + `fsc launch` 推送到设备（本示例的真机截图就是这么来的） |

**一个开发回环 = 3 步**：

```
① 描述/改原型（HTML 或 json）  →  ② 生成/预览确认稿  →  ③ 编译推到真机看效果
```

**MCP 工具链（AI 开发入口）**

| 项 | 地址 |
|---|---|
| GitHub 主仓 | https://github.com/zkswe/mcp_release |
| Gitee 镜像 | https://gitee.com/Kwolve/flythingsmcp_release |
| 当前版本 | `0.27.201-open` |

一句话安装：把上面的仓库 `git clone` 下来，按仓库 README 用**仓库路径**跑 `mcp_server.py`（`pip install` 不包含知识库/模板/工具链等数据文件）。

---

## 上手（几步跑起来）

1. **看效果**：直接看本 README 的截图与 GIF（无需环境）。
2. **跑起来**：用 FlyThings IDE 打开本工程 → 编译，或命令行：
   ```bash
   fsc install                 # 拉取 Manifest 里的依赖组件
   fsc build -p Z21            # 编译 → libzkgui.so
   fsc launch -p Z21 -s <设备IP>:5555   # 推到真机运行
   ```
3. **改界面**：改 `ui_dev/*.json`（布局源）→ 打包成 `ui/*.ftu` → 重新 build/launch。
   > 目录约定：`ui/` 只放设备加载的 `*.ftu` + `images/`；开发期的 `*.json` / 预览稿 在 `ui_dev/`。

---

## 购买与联系

- **公司**：深圳中科世为科技有限公司（zkswe）
- **官网**：https://www.zkswe.com
- **电话**：0755-23019045
- **地址**：广东省深圳市宝安区西乡街道共乐社区凤凰智谷 A 座 1407 室

> 需要整机 / 核心板 / 定制 UI，或要一份「你的需求 → 我们能做到的样子」的评估，欢迎联系我们。

---

## 技术细节

### 目录结构

```
├─ ui/          设备实际加载的 *.ftu（编译产物）+ images/
├─ ui_dev/      开发期：布局源 *.json、页面预览稿
├─ src/         应用源码
│  ├─ logic/    页面逻辑（*.cc，按页面）
│  ├─ core/     业务代码（本示例含音乐/网络等模块）
│  ├─ ui/ zk/   自研页面动画、导航等
│  └─ uart/     串口协议框架（模板）
├─ resources/   图片等资源（json 里按 images/xxx.png 引用）
├─ font/        工程字库
├─ showcase/    本 README 用到的真机截图与 GIF
├─ tools/       抓帧工具源码（fbcap.c）
├─ Manifest.xml 依赖组件声明
└─ package.properties / .project / .cproject / .settings  IDE 与工程配置
```

### 关键实现点
- **动态列表**：`listview` + 适配器三回调（计数 / 填充 / 点击），数据运行时构建；封面墙用 **2 列网格** + **ImageCache**（按路径缓存已解码位图，回页不重复解码），取图尺寸严格等于显示盒。
- **动画**：页面进入用统一的滑入编排（错峰 duration/delay）。
- **自绘**：图表/波形按帧重绘；画板用 painter 记录笔画。
- **音乐播放**：`ZKMediaPlayer` 真播。Z21 是 SigmaStar 平台，音频输出走 **MI_AO** 通道（不是 ALSA）；唱片用 `pointer` 控件逐帧累加角度实现匀速旋转，暂停即停；进度与歌词取自播放器的真实位置；三首素材为仓库内程序化合成的 demo 曲（`resources/music/`），随资源一起部署到设备。
- **资源出图**：图标与封面按控件盒尺寸程序化生成（圆角走覆盖率 α，缩放用面积平均，避免边缘振铃）。

### 已知限制
- 示例重点在「控件与交互能力」的覆盖面，业务数据多为演示数据。
- 音乐页的音频素材是**程序化合成的 demo 曲**（无 vocals、无版权问题），歌词为演示文本并按时间同步；把 `resources/music/` 换成自己的 MP3 即可播真实歌曲。
- GIF 为抓帧回放（非实时帧率）；真机帧率以设备实测为准。

### 许可
本仓库为**示例代码**，供评估与二次开发参考；商用与再分发请联系我们确认授权。

<div align="center">

**深圳中科世为科技有限公司 · FlyThings OS / FlyThings UI**

</div>
