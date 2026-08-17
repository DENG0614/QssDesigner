# QssDesigner

基于 Qt C++ 的可视化界面设计器 + QSS 样式系统。**设计器自身的界面由
`forms/mainwindow.ui` 定义**（uic 编译进工程，可直接用 Qt Designer 打开拖拽
修改），界面全部中文化。左侧控件面板（接近 Qt Designer 全套控件）拖拽控件到
中央画布，自由移动/缩放/对齐；右侧属性面板编辑控件属性；底部 QSS 编辑器实时
调整样式；一键导出 Qt Designer `.ui` 文件、`.qss` 样式文件和可直接编译的
Qt C++ 工程。

兼容 Qt 5.6.3 (MSVC 2013) 与 Qt 5.15.2。

---

## 1. 功能总览

- **可视化拖拽布局**
- 左侧控件库为**分组树**（按钮/输入控件/滑块进度/列表表格树/容器/显示控件），
    覆盖 QssStylesheetEditor v1.8 预览面板中出现过的全部控件类型（图标模式列表、
    文件系统树、图片标签等变体也包含在内），共 48 项，**每一项都可拖拽或双击
    放到画布**，图标使用 Qt 标准图标。
  - 基于 `QGraphicsScene/QGraphicsView`，控件以 `QGraphicsProxyWidget` 嵌入真实 QWidget。
  - 鼠标多选（橡皮筋框选）、移动、8 方向缩放手柄调整大小、方向键微调。
  - 对齐：左/右/上/下/水平居中/垂直居中/等宽/等高；层级：置顶/置底；删除/复制/粘贴。
  - Ctrl+滚轮缩放画布。
- **集成 QSS 样式库**
  - 内置样式来自 `E:\qt_project\qss\QSS-master` 的 9 套主题：
    Ubuntu、ElegantDark、MaterialDark、ConsoleStyle、AMOLED、Aqua、ManjaroMix、
    NeonButtons、MacOS。
  - 并补充 QssStylesheetEditor_v1.8 的 default / dark 皮肤，以及 3 套带 `$变量`
    的 .qsst 模板（default / light / dark）。
  - 下拉菜单选择样式，即时应用到画布上所有控件（所见即所得）。
  - `$name = #color;` 变量语法自动转换（复用 QssStylesheetEditor 的 Qsst 处理器）。
  - `url(...)` 图片路径自动修复：Linux 主题图标（如 MacOS 引用的
    `/usr/share/icons/Adwaita/...`）自动重定向到内置 `styles/QSS_IMG` 图标。
  - 顶部“样式：”下拉框默认选中 **Aqua** 主题（找不到样式库时会自动向
    exe 同级、上级目录、工作目录搜索 `styles` 文件夹）。
- **控件预览面板（对应 QssStylesheetEditor v1.8 右侧 Preview）**
  - 画布右侧“控件预览”停靠面板，**按 v1.8 原始预览逐页移植，7 个页签**：
    - **常用组件**：Text 表单（用户名/数据/密码/网址/图标标签/自动补全/日期掩码/
      Mac 掩码/字符掩码/校验/正则校验/只读）、Button、Checkable、Selectable、
      TextEdit 分组
    - **特别组件**：Dialog（消息/提问/输入/颜色/警告/字体/错误/打开/关于/保存，
      点击弹真实对话框）、DateTime（日期/时间/日期时间/日历）、Sliders（滑块+
      进度、R/G/B 调色）、Meters（旋钮）
    - **绘制组件**：DrawGraphics（画笔/画刷/线性/径向/锥形渐变）、Pictures
    - **布局组件**：QHBoxLayout / QGridLayout / QFormLayout / Spliter MDI Dock
    - **容器组件**：QGroupBox、StackLayout、ToolBox、TabWidget（四方向页签）
    - **复杂组件**：列表/图标列表、表格/带表头表格、树（彩色节点）/文件系统树
    - **自定义**：完整主窗口（菜单栏+工具栏+状态栏+停靠+中央表单），中央控件可
      Ctrl+拖拽，**从菜单栏/工具栏/状态栏/停靠 Ctrl+拖拽可把整个主窗口拖到画布**
      （控件库“容器”分组也有“完整主窗口”）
  - **选择或编辑任何 QSS 时全局生效**（`qApp->setStyleSheet`）：整个程序界面——
    菜单、工具栏、停靠、对话框、预览、画布——统一换肤，所见即所得（同 v1.8 的
    整体换肤方式）。
  - **预览面板中的每个控件都可以直接拖到中央画布**：
    - **按住 Ctrl 再拖动**发起拖拽（普通点击/拖动保留控件原生行为：文本选择、
      表格选择、滚动、按钮点击、调色标签等互不冲突）；
    - 复合控件（表格/树/标签页/MDI/日历等）内部任意位置均可拖动（过滤器递归安装）；
    - **带标签的控件（常用组件 Text 表单、布局组件表单、自定义表单等）都连同
      前面的标签成组拖出**；
    - 拖出的控件保留预览属性（文本/禁用/选中/数值/条目/图标/输入掩码/表格行列/
      局部样式/宽高尺寸等）并立即应用当前 QSS，所见即所得；
    - 拖出后**立即应用当前 QSS**（画布记住当前样式，新控件落画布即换肤）；
    - 复制/粘贴的控件同样自动套用当前样式；
    - 对话框按钮拖出后点击仍可弹出对应对话框。
  - 拖拽载荷为 JSON（type/variant/props），控件库与预览面板统一由 DesignView 创建。
- **无刷屏警告**
  - 修正内置 AMOLED.qss 的非法属性 `border-bottom-radius`（Qt 不识别，改为
    `border-bottom-left/right-radius`），并安装消息过滤器屏蔽 QSS 解析器的
    “Unknown property” 逐条警告，控制台不再刷屏。
  - 修正 Aqua.qss 的 4 参数 `rgb(0,113,255,255)`（改为 `rgba(...)`），消除
    `QCssParser::parseColorValue` 警告。
- **设计器自身界面 .ui 化**
  - 主窗口、菜单、工具栏、三个停靠面板、状态栏全部定义在
    `forms/mainwindow.ui` 中（自定义控件通过 `<customwidget>` 提升），
    用 Qt Designer 打开即可拖拽调整设计器自己的界面，改完重新 qmake/make 生效。
  - 源码统一使用 `#pragma execution_character_set("utf-8")` + UTF-8 BOM，
    MSVC 2013/2015 与新版 MSVC、MinGW 均可正确编译中文。
- **样式自定义与保存**
  - 底部 QSS 编辑器带语法高亮，编辑即实时刷新预览（400ms 防抖）。
  - “Save Custom Style” 将当前样式保存到 `styles/custom/`，自动加入样式库。
- **一键导出**
  - **Export UI**：导出 Qt Designer 4.0 格式 `.ui`（`uic` 可直接编译验证，
    Qt Designer 可直接打开继续编辑），支持 GroupBox 子控件嵌套与相对坐标。
  - **Export Project**：导出完整 Qt C++ 工程（`.pro`、`main.cpp`、
    `mainwindow.cpp/h`、`.ui`、`.qss`，以及 MacOS 主题所需的 QSS_IMG 图标目录），
    使用 `QUiLoader` 加载 .ui、读取 .qss 应用样式，`qmake && make` 即可编译运行。

---

## 2. 项目结构

```
E:\qt_project\QssDesigner\
├── QssDesigner.pro            # qmake 工程文件（QT += core gui widgets, CONFIG += c++11）
├── README.md
├── forms\
│   └── mainwindow.ui          # 设计器自身界面（uic 编译，Qt Designer 可打开编辑）
├── styles\                    # 内置样式库（运行时可扩展）
│   ├── AMOLED.qss ... Ubuntu.qss   # QSS-master 的 9 套主题
│   ├── default.qss / dark.qss      # QssStylesheetEditor v1.8 皮肤
│   ├── qsst\default.qsst ...       # 带 $变量 的模板样式
│   ├── custom\                     # 用户自定义样式保存目录（自动创建）
│   └── QSS_IMG\                    # MacOS 主题图标
├── src\
│   ├── main.cpp               # 入口（支持 --selftest <dir> 无头自检/导出测试）
│   ├── mainwindow.h/cpp       # 主窗口：菜单/工具栏/Dock/样式联动/导出动作
│   ├── widgetpalette.h/cpp    # 左侧控件面板（拖拽源，自定义 mime 载荷）
│   ├── designitem.h/cpp       # 画布图元：QGraphicsProxyWidget 子类 + 缩放手柄
│   ├── designscene.h/cpp      # 设计场景：添加/多选/对齐/层级/复制粘贴/样式应用
│   ├── designview.h/cpp       # 画布视图：拖放接收、橡皮筋多选、缩放、方向键微调
│   ├── propertypanel.h/cpp    # 右侧属性面板（objectName/geometry/文本/枚举等）
│   ├── stylemanager.h/cpp     # 样式库扫描、$变量转换、url 修复、自定义样式保存
│   ├── qsseditor.h/cpp        # QSS 源码编辑器（高亮 + Tab 缩进）
│   ├── previewwidget.h/cpp    # 控件预览面板（全控件样式画廊，右侧停靠）
│   ├── widgetdrag.h/cpp       # 统一控件拖拽（控件库/预览面板共用，拖到画布创建）
│   ├── qsshighlighter.h/cpp   # QSS 语法高亮（移植自 QsciLexerQSS 的配色与规则）
│   ├── qsst.h/cpp             # $变量 处理器（移植自 QssStylesheetEditor v1.8）
│   ├── uiwriter.h/cpp         # .ui XML 导出（Qt Designer 4.0 格式）
│   └── projectwriter.h/cpp    # Qt C++ 工程导出（.pro/main.cpp/mainwindow.cpp/h）
└── build\                     # 本地构建产物（Qt 5.15.2 MinGW 验证）
```

---

## 3. 核心模块设计

### 3.1 画布：`DesignScene` + `DesignView` + `DesignItem`

```
WidgetPalette ──(拖拽, mime="application/x-qssdesigner-widget")──▶ DesignView
                                                                    │ mapToScene
                                                                    ▼
DesignScene::addWidget(type, pos) ──▶ DesignItem(QGraphicsProxyWidget)
                                        ├─ 内部包裹真实 QWidget（QPushButton 等）
                                        ├─ 选中时绘制虚线边框 + 8 个缩放手柄
                                        ├─ 手柄拖拽 → setPos + widget()->resize()
                                        └─ 支持 多选/移动/对齐/层级/复制粘贴
```

- `DesignItem` 定义了自定义 `type()`（`UserType + 1`），保证
  `qgraphicsitem_cast<DesignItem *>` 不会误判场景内部自动创建的代理图元。
- 控件属性通过 `exportProperties()` / `applyProperty()` 双向读写，导出和属性
  面板共用同一套数据。
- 样式应用：`DesignScene::applyStyleSheet()` 遍历所有 `DesignItem`，对每个
  真实 QWidget 调用 `setStyleSheet()`，QSS 会级联到控件子树，实现所见即所得。

### 3.2 样式系统：`StyleManager` + `Qsst`

```
styles\*.qss / *.qsst ──扫描──▶ StyleManager::styles()
                                    │ 选择样式
                                    ▼
                          prepareForPreview(name, text)
                                    │ 1) .qsst → Qsst::loadVars/convertQss ($变量替换)
                                    │ 2) fixUrls()（/usr/... 图标 → styles/QSS_IMG）
                                    ▼
                          DesignScene::applyStyleSheet(qss)
```

`Qsst` 完整移植了 QssStylesheetEditor v1.8 的变量语法：

- 定义：`$background = #FDFDFD;`
- 引用：`background-color: $background;`
- 透明度：`$selected%70` → 生成 8 位 `#AARRGGBB`
- 代码块 `~~~ ... ~~~` / `` ``` ... ``` `` 在转换时移除

### 3.3 导出：`UiWriter` + `ProjectWriter`

`UiWriter` 用 `QXmlStreamWriter` 生成标准 Qt Designer 4.0 格式：

```xml
<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>MainWindow</class>
 <widget class="QMainWindow" name="MainWindow">
  <property name="geometry"><rect>...</rect></property>
  <widget class="QWidget" name="centralwidget">
   <widget class="QPushButton" name="pushButton1">
    <property name="geometry"><rect>...</rect></property>
    <property name="text"><string>Push Button</string></property>
   </widget>
  </widget>
 </widget>
 <resources/>
 <connections/>
</ui>
```

控件属性按 Designer 规则输出（`rect/string/bool/number/enum` 元素、`<item>` 列表、
`Qt::Horizontal` 枚举等），`uic` 与 Qt Designer 均能正确解析；GroupBox 内的控件
自动嵌套并以相对坐标导出。

`ProjectWriter` 导出的工程使用 `QUiLoader` 加载 `.ui`、`QFile` 读取 `.qss`
并 `setStyleSheet()`，保证两个 Qt 版本下 `qmake && make` 直接可编译运行。

---

## 4. Qt 版本兼容性说明（5.6.3 MSVC2013 ↔ 5.15.2）

### 4.1 语言标准
- 全工程只使用 C++11（`CONFIG += c++11`），不使用 C++14/17 特性（无泛型
  lambda、无 `auto` 返回类型推导、无 `make_unique` 等），MSVC 2013/2015 与新
  版 MSVC 均可编译。
- 源码为纯 ASCII（界面文案走 `tr()`），规避 MSVC2013 对无 BOM UTF-8 源文件的
  编码问题。

### 4.2 API 差异处理（均已在代码中条件编译）

| 场景 | Qt 5.6.3 | Qt 5.15.2 | 处理方式 |
|---|---|---|---|
| Tab 宽度 | `setTabStopWidth()` | `setTabStopDistance()` | `#if QT_VERSION >= 5.10` |
| 字符宽度 | `QFontMetrics::width()` | `horizontalAdvance()` | `#if QT_VERSION >= 5.11` |
| `QSet::toList()` | 可用 | 已弃用 | 统一用 `values()`（两版都有） |
| 菜单加动作 | 函数指针重载（5.6 新增） | 支持 | 直接使用 |
| 标准快捷键 | `QKeySequence::ZoomIn` 等 5.6 不保证 | 支持 | 显式 `Ctrl++ / Ctrl+-` |
| `QGraphicsProxyWidget` | 继承 QGraphicsWidget | 同 | `setMinimumSize` 两版可用 |
| 轮滚角度 | `delta()` | `angleDelta()` | 统一 `angleDelta()`（5.0+ 都有） |
| `QUiLoader` | qttools 自带 | qttools 自带 | 导出工程 `QT += uitools` |
| 中文字符串 | 需 UTF-8 BOM + `#pragma execution_character_set("utf-8")` | 原生 UTF-8 | 源码统一 BOM + pragma；uic 生成的 ui_*.h 用八进制转义，天然 ASCII 安全 |

### 4.3 构建

Qt 5.15.2 (MinGW 或 MSVC)：
```
qmake QssDesigner.pro
mingw32-make   (或 nmake / jom)
```

Qt 5.6.3 (MSVC 2013)：
```
set PATH=E:\Qt\Qt5.6.3_64\5.6.3\msvc2013_64\bin;%PATH%
qmake QssDesigner.pro -spec win32-msvc2013
nmake
```

运行前把 `styles\` 目录放到可执行文件同目录（或工作目录），程序会自动查找。

---

## 5. 使用说明

1. 启动后在左侧“控件库”分组树中选择控件拖到画布；双击控件也可添加到画布中央。
2. 单击选中控件（Ctrl/Shift 多选），拖拽移动，拖动边缘手柄缩放；方向键微调。
3. 菜单 Edit → Align / Order 进行对齐与层级调整。
4. 右侧“Properties”面板修改 objectName、坐标、文本、勾选状态、数值范围等。
5. 工具栏选择样式（如 QSS-master / AMOLED），画布立即刷新；底部 QSS 编辑器
   可直接修改并实时预览；修改后可 “Save Custom Style” 存为自定义样式。
   右侧“控件预览”面板同步展示所有控件类型的换肤效果。
6. File → Export UI 导出 `.ui`；File → Export Qt Project 导出完整工程。
   导出工程后：`qmake && mingw32-make`（或 nmake），运行生成的 exe。
7. 想调整设计器自己的界面：用 Qt Designer 打开 `forms/mainwindow.ui`，
   拖动菜单/工具栏/停靠面板/状态栏布局后保存，重新 `qmake && make` 即可。

---

## 6. 已知限制

- 画布为绝对坐标布局（与 Qt Designer 的 "Widget Box" 自由布局一致），不生成
  layout 管理（QHBoxLayout 等）；GroupBox 内的控件会嵌套导出。
- `QTabWidget` 的页签内控件暂按顶层绝对坐标导出（Designer 可打开，可手动调整）。
- QSS 高亮为规则移植，极端写法（嵌套字符串等）可能着色不精确，不影响功能。
- 样式编辑器的“回退/重做”与控件属性编辑“撤销”尚未接入统一 undo 栈。

---

## 7. 样式来源与版权

- `styles/` 下 QSS-master 的 9 套主题来自
  [GTRONICK/QSS](https://github.com/GTRONICK/QSS)，协议见
  `styles/LICENSE_QSS-master.txt`。
- `default.qss`、`dark.qss` 及 `qsst/` 模板来自
  [hustlei/QssStylesheetEditor](https://github.com/hustlei/QssStylesheetEditor)
  v1.8。
