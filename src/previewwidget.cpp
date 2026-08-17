#pragma execution_character_set("utf-8")

#include "previewwidget.h"

#include "dialogs.h"
#include "widgetdrag.h"

#include <QApplication>
#include <QCalendarWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QCompleter>
#include <QCommandLinkButton>
#include <QConicalGradient>
#include <QDateTimeEdit>
#include <QDateEdit>
#include <QDial>
#include <QDockWidget>
#include <QDoubleSpinBox>
#include <QDoubleValidator>
#include <QFileSystemModel>
#include <QFormLayout>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLinearGradient>
#include <QLineEdit>
#include <QListWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMdiArea>
#include <QObject>
#include <QPainter>
#include <QProgressBar>
#include <QPushButton>
#include <QRadialGradient>
#include <QRadioButton>
#include <QRegularExpressionValidator>
#include <QScrollArea>
#include <QScrollBar>
#include <QSlider>
#include <QSpinBox>
#include <QSplitter>
#include <QStackedWidget>
#include <QStatusBar>
#include <QStringListModel>
#include <QStyle>
#include <QTabWidget>
#include <QTableView>
#include <QTableWidget>
#include <QTextEdit>
#include <QTimeEdit>
#include <QToolBar>
#include <QToolBox>
#include <QToolButton>
#include <QTreeView>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QTime>
#include <QDate>
#include <QDateTime>
#include <QDir>

namespace {

// 给预览控件挂上拖拽：Ctrl+拖拽 到画布
void makeDraggable(QWidget *w, const QString &type, int variant = 0)
{
    new WidgetDragFilter(w, type, QVariant(variant));
}

// 带标签的控件连同前面的标签一起拖出
void makeDraggableRow(QWidget *label, QWidget *editor, const QString &type, int variant = 0)
{
    QList<WidgetDragFilter::DragSource> sources;
    sources << WidgetDragFilter::DragSource(QStringLiteral("QLabel"), QVariant(0), label);
    sources << WidgetDragFilter::DragSource(type, QVariant(variant), editor);
    new WidgetDragFilter(label, sources);
    new WidgetDragFilter(editor, sources);
}

// 对话框按钮：点击弹出，Ctrl+拖拽 到画布后仍可弹出
void makeDraggableDialogButton(QWidget *tab, QPushButton *btn, const QString &action)
{
    QObject::connect(btn, &QPushButton::clicked, tab, [tab, action]() {
        showStandardDialog(tab, action);
    });
    QList<WidgetDragFilter::DragSource> sources;
    sources << WidgetDragFilter::DragSource(QStringLiteral("QPushButton"), QVariant(0),
                                            btn, action);
    new WidgetDragFilter(btn, sources);
}

QWidget *scrolled(QWidget *content)
{
    QScrollArea *area = new QScrollArea;
    area->setWidgetResizable(true);
    area->setWidget(content);
    return area;
}

// 绘制演示控件（移植自 v1.8 预览“绘制组件”页）
class DrawingDemo : public QWidget
{
public:
    explicit DrawingDemo(QWidget *parent = 0)
        : QWidget(parent)
    {
        setMinimumHeight(170);
    }

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE
    {
        QPainter p(this);
        QPen pen(Qt::black, 2, Qt::SolidLine);
        p.setPen(pen);
        p.drawLine(20, 15, 150, 15);
        pen.setStyle(Qt::DashLine);
        p.setPen(pen);
        p.drawLine(20, 35, 150, 35);
        pen.setStyle(Qt::DotLine);
        p.setPen(pen);
        p.drawLine(20, 55, 150, 55);
        pen.setStyle(Qt::DashDotLine);
        p.setPen(pen);
        p.drawLine(20, 75, 150, 75);
        pen.setStyle(Qt::DashDotDotLine);
        p.setPen(pen);
        p.drawLine(20, 95, 150, 95);
        pen.setColor(Qt::black);
        pen.setWidth(1);
        pen.setStyle(Qt::SolidLine);
        p.setPen(pen);
        p.setBrush(QBrush(Qt::SolidPattern));
        p.drawRect(180, 10, 40, 30);
        p.setBrush(QBrush(Qt::Dense5Pattern));
        p.drawRect(240, 10, 40, 30);
        p.setBrush(QBrush(Qt::Dense7Pattern));
        p.drawRect(300, 10, 40, 30);
        p.setBrush(QBrush(Qt::green, Qt::HorPattern));
        p.drawRect(180, 50, 40, 30);
        p.setBrush(QBrush(Qt::green, Qt::VerPattern));
        p.drawRect(240, 50, 40, 30);
        p.setBrush(QBrush(Qt::green, Qt::CrossPattern));
        p.drawRect(300, 50, 40, 30);
        p.setBrush(QBrush(Qt::blue, Qt::BDiagPattern));
        p.drawRect(180, 90, 40, 30);
        p.setBrush(QBrush(Qt::blue, Qt::FDiagPattern));
        p.drawRect(240, 90, 40, 30);
        p.setBrush(QBrush(Qt::blue, Qt::DiagCrossPattern));
        p.drawRect(300, 90, 40, 30);
        QLinearGradient lg(180, 130, 220, 160);
        lg.setColorAt(0, Qt::red);
        lg.setColorAt(1, Qt::blue);
        p.setBrush(QBrush(lg));
        p.drawRect(180, 130, 40, 30);
        QRadialGradient rg(260, 145, 20);
        rg.setColorAt(0, Qt::red);
        rg.setColorAt(0.5, Qt::yellow);
        rg.setColorAt(1, Qt::blue);
        p.setBrush(QBrush(rg));
        p.drawRect(240, 130, 40, 30);
        QConicalGradient cg(320, 145, 0);
        cg.setColorAt(0, Qt::red);
        cg.setColorAt(0.4, Qt::yellow);
        cg.setColorAt(0.8, Qt::blue);
        cg.setColorAt(1, Qt::red);
        p.setBrush(QBrush(cg));
        p.drawRect(300, 130, 40, 30);
    }
};

} // namespace

PreviewWidget::PreviewWidget(QWidget *parent)
    : QTabWidget(parent)
{
    addTab(scrolled(makeTabBasic()), QStringLiteral("常用组件"));
    addTab(scrolled(makeTabSpecial()), QStringLiteral("特别组件"));
    addTab(scrolled(makeTabDrawing()), QStringLiteral("绘制组件"));
    addTab(scrolled(makeTabLayout()), QStringLiteral("布局组件"));
    addTab(scrolled(makeTabContainer()), QStringLiteral("容器组件"));
    addTab(scrolled(makeTabAdvance()), QStringLiteral("复杂组件"));
    addTab(scrolled(makeTabCustom()), QStringLiteral("自定义"));
}

void PreviewWidget::applyStyleSheet(const QString &qss)
{
    setStyleSheet(qss);
}

QWidget *PreviewWidget::makeTabBasic()
{
    QWidget *w = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(w);
    layout->setSpacing(8);

    // Text 组（两个 QFormLayout 并排）
    QGroupBox *group1 = new QGroupBox(QStringLiteral("Text"));
    group1->setCheckable(true);
    QHBoxLayout *g1h = new QHBoxLayout(group1);
    QFormLayout *col1 = new QFormLayout;
    QFormLayout *col2 = new QFormLayout;
    g1h->addLayout(col1);
    g1h->addLayout(col2);
    layout->addWidget(group1);

    QLabel *lblUser = new QLabel(QStringLiteral("用户名(&U)："));
    QLineEdit *edUser = new QLineEdit(QStringLiteral("default"));
    lblUser->setBuddy(edUser);
    col1->addRow(lblUser, edUser);
    makeDraggableRow(lblUser, edUser, QStringLiteral("QLineEdit"));

    QLabel *lblData = new QLabel(QStringLiteral("数据1："));
    QLineEdit *edData = new QLineEdit;
    edData->setPlaceholderText(QStringLiteral("输入"));
    col1->addRow(lblData, edData);
    makeDraggableRow(lblData, edData, QStringLiteral("QLineEdit"));

    QLabel *lblPwd = new QLabel(QStringLiteral("<b>密码</b>："));
    QLineEdit *edPwd = new QLineEdit(QStringLiteral("******"));
    edPwd->setEchoMode(QLineEdit::Password);
    col1->addRow(lblPwd, edPwd);
    makeDraggableRow(lblPwd, edPwd, QStringLiteral("QLineEdit"));

    QLabel *lblLink = new QLabel(QStringLiteral("网址标签："));
    QLabel *lblLinkVal = new QLabel(QStringLiteral("..."));
    lblLinkVal->setOpenExternalLinks(true);
    col1->addRow(lblLink, lblLinkVal);
    makeDraggableRow(lblLink, lblLinkVal, QStringLiteral("QLabel"));

    QLabel *lblIcon = new QLabel(QStringLiteral("图标标签："));
    QLabel *lblIconVal = new QLabel;
    lblIconVal->setPixmap(QApplication::style()->standardIcon(QStyle::SP_ComputerIcon).pixmap(32, 32));
    col1->addRow(lblIcon, lblIconVal);
    makeDraggableRow(lblIcon, lblIconVal, QStringLiteral("QLabel"), 1);

    QLabel *lblComp = new QLabel(QStringLiteral("自动补全："));
    QLineEdit *edComp = new QLineEdit;
    QStringListModel *compModel = new QStringListModel(edComp);
    compModel->setStringList(QStringList() << QStringLiteral("@163.com") << QStringLiteral("@qq.com")
                                          << QStringLiteral("@gmail.com") << QStringLiteral("@hotmail.com")
                                          << QStringLiteral("@126.com"));
    QCompleter *comp = new QCompleter(compModel, edComp);
    edComp->setCompleter(comp);
    col1->addRow(lblComp, edComp);
    makeDraggableRow(lblComp, edComp, QStringLiteral("QLineEdit"));

    QLabel *lblMask1 = new QLabel(QStringLiteral("日期掩码："));
    QLineEdit *edMask1 = new QLineEdit;
    edMask1->setInputMask(QStringLiteral("0000-00-00"));
    col2->addRow(lblMask1, edMask1);
    makeDraggableRow(lblMask1, edMask1, QStringLiteral("QLineEdit"));

    QLabel *lblMask2 = new QLabel(QStringLiteral("Mac 地址掩码"));
    QLineEdit *edMask2 = new QLineEdit;
    edMask2->setInputMask(QStringLiteral("HH:HH:HH:HH:HH:HH;_"));
    col2->addRow(lblMask2, edMask2);
    makeDraggableRow(lblMask2, edMask2, QStringLiteral("QLineEdit"));

    QLabel *lblMask3 = new QLabel(QStringLiteral("字符掩码"));
    QLineEdit *edMask3 = new QLineEdit;
    edMask3->setInputMask(QStringLiteral("XXXXXX"));
    col2->addRow(lblMask3, edMask3);
    makeDraggableRow(lblMask3, edMask3, QStringLiteral("QLineEdit"));

    QLabel *lblValid = new QLabel(QStringLiteral("校验输入："));
    QLineEdit *edValid = new QLineEdit;
    QDoubleValidator *dv = new QDoubleValidator(0.0, 100.0, 2, edValid);
    edValid->setValidator(dv);
    col2->addRow(lblValid, edValid);
    makeDraggableRow(lblValid, edValid, QStringLiteral("QLineEdit"));

    QLabel *lblRe = new QLabel(QStringLiteral("正则表达式校验："));
    QLineEdit *edRe = new QLineEdit;
    edRe->setValidator(new QRegularExpressionValidator(
        QRegularExpression(QStringLiteral("[a-zA-Z0-9]+$")), edRe));
    col2->addRow(lblRe, edRe);
    makeDraggableRow(lblRe, edRe, QStringLiteral("QLineEdit"));

    QLabel *lblRo = new QLabel(QStringLiteral("只读："));
    QLineEdit *edRo = new QLineEdit(QStringLiteral("ReadOnly"));
    edRo->setReadOnly(true);
    col2->addRow(lblRo, edRo);
    makeDraggableRow(lblRo, edRo, QStringLiteral("QLineEdit"));

    // Button 组
    QGroupBox *group2 = new QGroupBox(QStringLiteral("Button"));
    group2->setCheckable(true);
    QVBoxLayout *g2 = new QVBoxLayout(group2);
    QHBoxLayout *row1 = new QHBoxLayout;
    QHBoxLayout *row2 = new QHBoxLayout;
    g2->addLayout(row1);
    g2->addLayout(row2);
    layout->addWidget(group2);
    QPushButton *b1 = new QPushButton(QStringLiteral("Button"));
    QPushButton *b2 = new QPushButton(QStringLiteral("IconBtn"));
    b2->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogOkButton));
    QPushButton *b3 = new QPushButton(QStringLiteral("Disabled"));
    b3->setEnabled(false);
    QPushButton *b4 = new QPushButton(QStringLiteral("Default"));
    b4->setDefault(true);
    QPushButton *b5 = new QPushButton(QStringLiteral("Switch"));
    b5->setCheckable(true);
    QToolButton *tb = new QToolButton;
    tb->setText(QStringLiteral("ToolButton"));
    tb->setPopupMode(QToolButton::MenuButtonPopup);
    QMenu *m = new QMenu(tb);
    m->addAction(QStringLiteral("action1"));
    m->addAction(QStringLiteral("action2"));
    m->addAction(QStringLiteral("action3"));
    tb->setMenu(m);
    QCommandLinkButton *clb = new QCommandLinkButton(QStringLiteral("LinkBtn"));
    row1->addWidget(b1);
    row1->addWidget(b2);
    row1->addWidget(b3);
    row1->addWidget(b4);
    row2->addWidget(b5);
    row2->addWidget(tb);
    row2->addWidget(clb);
    makeDraggable(b1, QStringLiteral("QPushButton"));
    makeDraggable(b2, QStringLiteral("QPushButton"));
    makeDraggable(b3, QStringLiteral("QPushButton"));
    makeDraggable(b4, QStringLiteral("QPushButton"));
    makeDraggable(b5, QStringLiteral("QPushButton"));
    makeDraggable(tb, QStringLiteral("QToolButton"));
    makeDraggable(clb, QStringLiteral("QCommandLinkButton"));

    // Checkable 组
    QGroupBox *group3 = new QGroupBox(QStringLiteral("Checkable"));
    group3->setCheckable(true);
    QVBoxLayout *g3 = new QVBoxLayout(group3);
    QHBoxLayout *c1 = new QHBoxLayout;
    QHBoxLayout *c2 = new QHBoxLayout;
    g3->addLayout(c1);
    g3->addLayout(c2);
    layout->addWidget(group3);
    QRadioButton *r1 = new QRadioButton(QStringLiteral("Radio"));
    QRadioButton *r2 = new QRadioButton(QStringLiteral("Iconradio"));
    r2->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogYesButton));
    QRadioButton *r3 = new QRadioButton(QStringLiteral("Iconradio"));
    r3->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogNoButton));
    QRadioButton *r4 = new QRadioButton(QStringLiteral("Disable"));
    r4->setEnabled(false);
    QCheckBox *ch1 = new QCheckBox(QStringLiteral("CheckBox"));
    QCheckBox *ch2 = new QCheckBox(QStringLiteral("CheckBox"));
    ch2->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogYesButton));
    QCheckBox *ch3 = new QCheckBox(QStringLiteral("TriState"));
    ch3->setTristate(true);
    ch3->setCheckState(Qt::PartiallyChecked);
    QCheckBox *ch4 = new QCheckBox(QStringLiteral("Disable"));
    ch4->setEnabled(false);
    c1->addWidget(r1);
    c1->addWidget(r2);
    c1->addWidget(r3);
    c1->addWidget(r4);
    c2->addWidget(ch1);
    c2->addWidget(ch2);
    c2->addWidget(ch3);
    c2->addWidget(ch4);
    makeDraggable(r1, QStringLiteral("QRadioButton"));
    makeDraggable(r2, QStringLiteral("QRadioButton"));
    makeDraggable(r3, QStringLiteral("QRadioButton"));
    makeDraggable(r4, QStringLiteral("QRadioButton"));
    makeDraggable(ch1, QStringLiteral("QCheckBox"));
    makeDraggable(ch2, QStringLiteral("QCheckBox"));
    makeDraggable(ch3, QStringLiteral("QCheckBox"));
    makeDraggable(ch4, QStringLiteral("QCheckBox"));

    // Selectable 组
    QGroupBox *group4 = new QGroupBox(QStringLiteral("Selectable"));
    group4->setCheckable(true);
    QHBoxLayout *g4 = new QHBoxLayout(group4);
    group4->setLayout(g4);
    layout->addWidget(group4);
    QSpinBox *s1 = new QSpinBox;
    s1->setValue(50);
    QDoubleSpinBox *s2 = new QDoubleSpinBox;
    s2->setRange(0, 1);
    s2->setValue(0.5);
    QComboBox *s3 = new QComboBox;
    s3->addItems(QStringList() << QStringLiteral("aaa") << QStringLiteral("bbb") << QStringLiteral("ccc"));
    s3->setEditable(true);
    s3->setCurrentIndex(2);
    QComboBox *s4 = new QComboBox;
    s4->addItems(QStringList() << QStringLiteral("aaa") << QStringLiteral("bbb") << QStringLiteral("ccc"));
    g4->addWidget(s1);
    g4->addWidget(s2);
    g4->addWidget(s3);
    g4->addWidget(s4);
    makeDraggable(s1, QStringLiteral("QSpinBox"));
    makeDraggable(s2, QStringLiteral("QDoubleSpinBox"));
    makeDraggable(s3, QStringLiteral("QComboBox"));
    makeDraggable(s4, QStringLiteral("QComboBox"));

    // TextEdit 组
    QGroupBox *group5 = new QGroupBox(QStringLiteral("TextEdit"));
    group5->setCheckable(true);
    QVBoxLayout *g5 = new QVBoxLayout(group5);
    group5->setLayout(g5);
    layout->addWidget(group5);
    QTextEdit *te = new QTextEdit(QStringLiteral("今人不见古时月，今月曾经照古人。古人今人若流水，共看明月皆如此。"));
    g5->addWidget(te);
    makeDraggable(te, QStringLiteral("QTextEdit"));

    layout->addStretch(1);
    return w;
}

QWidget *PreviewWidget::makeTabSpecial()
{
    QWidget *w = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(w);
    layout->setSpacing(8);

    // Dialog 组
    QGroupBox *g0 = new QGroupBox(QStringLiteral("Dialog"));
    QVBoxLayout *g0v = new QVBoxLayout(g0);
    QHBoxLayout *r1 = new QHBoxLayout;
    QHBoxLayout *r2 = new QHBoxLayout;
    g0v->addLayout(r1);
    g0v->addLayout(r2);
    layout->addWidget(g0);
    struct Dlg { const char *label; const char *action; };
    const Dlg dlgs[] = {
        { "消息对话框", "信息" },
        { "提问对话框", "提问" },
        { "输入对话框", "输入选择" },
        { "颜色对话框", "颜色" },
        { "警告对话框", "警告" },
        { "字体对话框", "字体" },
        { "错误对话框", "错误" },
        { "打开对话框", "打开文件" },
        { "关于对话框", "关于" },
        { "保存对话框", "保存文件" }
    };
    for (int i = 0; i < 10; ++i) {
        QPushButton *b = new QPushButton(QString::fromUtf8(dlgs[i].label));
        makeDraggableDialogButton(w, b, QString::fromUtf8(dlgs[i].action));
        (i < 5 ? r1 : r2)->addWidget(b);
    }

    // DateTime 组
    QGroupBox *g1 = new QGroupBox(QStringLiteral("DateTime"));
    g1->setCheckable(true);
    QHBoxLayout *g1h = new QHBoxLayout(g1);
    QVBoxLayout *dcol = new QVBoxLayout;
    g1h->addLayout(dcol);
    g1->setMaximumHeight(240);
    layout->addWidget(g1);
    QDateEdit *dt1 = new QDateEdit(QDate::currentDate());
    QTimeEdit *dt2 = new QTimeEdit(QTime::currentTime());
    QDateTimeEdit *dt3 = new QDateTimeEdit(QDateTime::currentDateTime());
    QDateTimeEdit *dt4 = new QDateTimeEdit(QDateTime::currentDateTime());
    dt4->setCalendarPopup(true);
    QCalendarWidget *cal = new QCalendarWidget;
    cal->setMaximumSize(QSize(250, 220));
    dcol->addWidget(dt1);
    dcol->addWidget(dt2);
    dcol->addWidget(dt3);
    dcol->addWidget(dt4);
    g1h->addWidget(cal);
    makeDraggable(dt1, QStringLiteral("QDateEdit"));
    makeDraggable(dt2, QStringLiteral("QTimeEdit"));
    makeDraggable(dt3, QStringLiteral("QDateTimeEdit"));
    makeDraggable(dt4, QStringLiteral("QDateTimeEdit"));
    makeDraggable(cal, QStringLiteral("QCalendarWidget"));

    // Sliders 组
    QGroupBox *g2 = new QGroupBox(QStringLiteral("Sliders"));
    g2->setCheckable(true);
    QVBoxLayout *g2v = new QVBoxLayout(g2);
    QHBoxLayout *srow = new QHBoxLayout;
    QHBoxLayout *scrow = new QHBoxLayout;
    g2v->addLayout(srow);
    g2v->addLayout(scrow);
    layout->addWidget(g2);
    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setMaximum(100);
    QProgressBar *progress = new QProgressBar;
    QObject::connect(slider, &QSlider::valueChanged, progress, &QProgressBar::setValue);
    slider->setValue(50);
    QScrollBar *sb1 = new QScrollBar(Qt::Horizontal);
    QScrollBar *sb2 = new QScrollBar(Qt::Horizontal);
    QScrollBar *sb3 = new QScrollBar(Qt::Horizontal);
    sb1->setMaximum(255);
    sb2->setMaximum(255);
    sb3->setMaximum(255);
    QLabel *clr = new QLabel(QStringLiteral("拖动滑块改变颜色"));
    clr->setAutoFillBackground(true);
    clr->setAlignment(Qt::AlignCenter);
    clr->setStyleSheet(QStringLiteral("background: rgba(0,0,0,100);"));
    auto applyClr = [sb1, sb2, sb3, clr]() {
        clr->setStyleSheet(QStringLiteral("background: rgba(%1,%2,%3,100);")
                               .arg(sb1->value()).arg(sb2->value()).arg(sb3->value()));
    };
    QObject::connect(sb1, &QScrollBar::valueChanged, applyClr);
    QObject::connect(sb2, &QScrollBar::valueChanged, applyClr);
    QObject::connect(sb3, &QScrollBar::valueChanged, applyClr);
    sb1->setValue(128);
    srow->addWidget(slider);
    srow->addWidget(progress);
    QVBoxLayout *sccol = new QVBoxLayout;
    sccol->addWidget(sb1);
    sccol->addWidget(sb2);
    sccol->addWidget(sb3);
    scrow->addLayout(sccol);
    scrow->addWidget(clr);
    makeDraggable(slider, QStringLiteral("QSlider"));
    makeDraggable(progress, QStringLiteral("QProgressBar"));
    makeDraggable(sb1, QStringLiteral("QScrollBar"));
    makeDraggable(sb2, QStringLiteral("QScrollBar"));
    makeDraggable(sb3, QStringLiteral("QScrollBar"));
    makeDraggable(clr, QStringLiteral("QLabel"));

    // Meters 组
    QGroupBox *g3 = new QGroupBox(QStringLiteral("Meters"));
    g3->setCheckable(true);
    QHBoxLayout *g3h = new QHBoxLayout(g3);
    layout->addWidget(g3);
    QDial *d1 = new QDial;
    QDial *d2 = new QDial;
    d2->setNotchesVisible(true);
    QObject::connect(d1, &QDial::valueChanged, d2, &QDial::setValue);
    g3h->addWidget(d1);
    g3h->addWidget(d2);
    makeDraggable(d1, QStringLiteral("QDial"));
    makeDraggable(d2, QStringLiteral("QDial"));

    layout->addStretch(1);
    return w;
}

QWidget *PreviewWidget::makeTabDrawing()
{
    QWidget *w = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(w);
    layout->setSpacing(8);

    QGroupBox *g1 = new QGroupBox(QStringLiteral("DrawGraphics"));
    g1->setCheckable(true);
    QHBoxLayout *g1h = new QHBoxLayout(g1);
    layout->addWidget(g1);
    DrawingDemo *draw = new DrawingDemo;
    g1h->addWidget(draw);

    QGroupBox *g2 = new QGroupBox(QStringLiteral("Pictures"));
    g2->setCheckable(true);
    QHBoxLayout *g2h = new QHBoxLayout(g2);
    layout->addWidget(g2);
    QLabel *pic1 = new QLabel;
    pic1->setPixmap(QApplication::style()->standardIcon(QStyle::SP_ComputerIcon).pixmap(64, 64));
    QLabel *cap1 = new QLabel(QStringLiteral("QLabel"));
    cap1->setAlignment(Qt::AlignHCenter);
    QVBoxLayout *pic1l = new QVBoxLayout;
    pic1l->addWidget(pic1);
    pic1l->addWidget(cap1);
    g2h->addLayout(pic1l);
    makeDraggable(pic1, QStringLiteral("QLabel"), 1);
    g2h->addStretch(1);

    class PicWidget : public QWidget
    {
    public:
        PicWidget(QWidget *parent = 0)
            : QWidget(parent)
        {
            setMinimumSize(100, 100);
        }
    protected:
        void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE
        {
            QPainter p(this);
            const QPixmap img = QApplication::style()->standardIcon(QStyle::SP_ComputerIcon)
                                    .pixmap(64, 64);
            p.drawPixmap(18, 18, img);
        }
    };
    PicWidget *pic2 = new PicWidget;
    QLabel *cap2 = new QLabel(QStringLiteral("QWidget Paint"));
    cap2->setAlignment(Qt::AlignHCenter);
    QVBoxLayout *pic2l = new QVBoxLayout;
    pic2l->addWidget(pic2);
    pic2l->addWidget(cap2);
    g2h->addLayout(pic2l);
    g2h->addStretch(1);

    layout->addStretch(1);
    return w;
}

QWidget *PreviewWidget::makeTabLayout()
{
    QWidget *w = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(w);
    layout->setSpacing(8);

    // QHBoxLayout 布局
    QGroupBox *g1 = new QGroupBox(QStringLiteral("QHBoxLayout 布局"));
    QHBoxLayout *g1h = new QHBoxLayout(g1);
    layout->addWidget(g1);
    for (int i = 1; i <= 4; ++i) {
        QPushButton *b = new QPushButton(QStringLiteral("按钮%1").arg(i));
        g1h->addWidget(b);
        makeDraggable(b, QStringLiteral("QPushButton"));
    }

    // QGridLayout 布局
    QGroupBox *g2 = new QGroupBox(QStringLiteral("QGridLayout 布局"));
    g2->setCheckable(true);
    QGridLayout *grid = new QGridLayout(g2);
    layout->addWidget(g2);
    QLabel *l1 = new QLabel(QStringLiteral("第一行数据："));
    QLabel *l2 = new QLabel(QStringLiteral("第二行数据："));
    QLabel *l3 = new QLabel(QStringLiteral("第三行数据："));
    QLineEdit *e1 = new QLineEdit;
    QLineEdit *e2 = new QLineEdit(QStringLiteral("0"));
    QLineEdit *e3 = new QLineEdit;
    e3->setPlaceholderText(QStringLiteral("请输入"));
    QTextEdit *te = new QTextEdit(QStringLiteral("这是一个文本编辑，占三行两列。"));
    grid->addWidget(l1, 0, 0);
    grid->addWidget(l2, 1, 0);
    grid->addWidget(l3, 2, 0);
    grid->addWidget(e1, 0, 1);
    grid->addWidget(e2, 1, 1);
    grid->addWidget(e3, 2, 1);
    grid->addWidget(te, 0, 2, 3, 2);
    makeDraggableRow(l1, e1, QStringLiteral("QLineEdit"));
    makeDraggableRow(l2, e2, QStringLiteral("QLineEdit"));
    makeDraggableRow(l3, e3, QStringLiteral("QLineEdit"));
    makeDraggable(te, QStringLiteral("QTextEdit"));

    // QFormLayout 布局
    QGroupBox *g3 = new QGroupBox(QStringLiteral("QFormLayout 布局"));
    g3->setCheckable(true);
    QFormLayout *form = new QFormLayout(g3);
    layout->addWidget(g3);
    QLabel *fl1 = new QLabel(QStringLiteral("请输入数据"));
    QLineEdit *fe1 = new QLineEdit;
    QLabel *fl2 = new QLabel(QStringLiteral("请选择数据"));
    QSpinBox *fs1 = new QSpinBox;
    QLabel *fl3 = new QLabel(QStringLiteral("请选择选项"));
    QComboBox *fc1 = new QComboBox;
    fc1->addItems(QStringList() << QStringLiteral("Item1") << QStringLiteral("Item2")
                                << QStringLiteral("Item3"));
    form->addRow(fl1, fe1);
    form->addRow(fl2, fs1);
    form->addRow(fl3, fc1);
    makeDraggableRow(fl1, fe1, QStringLiteral("QLineEdit"));
    makeDraggableRow(fl2, fs1, QStringLiteral("QSpinBox"));
    makeDraggableRow(fl3, fc1, QStringLiteral("QComboBox"));

    // Spliter MDI Dock
    QGroupBox *g4 = new QGroupBox(QStringLiteral("Spliter MDI Dock"));
    g4->setCheckable(true);
    QHBoxLayout *g4h = new QHBoxLayout(g4);
    layout->addWidget(g4);
    QSplitter *splitter1 = new QSplitter(Qt::Horizontal);
    QSplitter *splitter2 = new QSplitter(Qt::Vertical);
    splitter1->resize(420, 200);
    splitter2->resize(200, 200);
    QMdiArea *mdi = new QMdiArea;
    mdi->addSubWindow(new QTextEdit(QStringLiteral("MDI 子窗口 1")));
    mdi->addSubWindow(new QTextEdit(QStringLiteral("MDI 子窗口 2")));
    mdi->cascadeSubWindows();
    QWidget *w1 = new QWidget;
    QWidget *w2 = new QWidget;
    QDockWidget *d1 = new QDockWidget(QStringLiteral("dock1"), w1);
    QDockWidget *d2 = new QDockWidget(QStringLiteral("dock2"), w1);
    QDockWidget *d3 = new QDockWidget(QStringLiteral("dock3"), w2);
    d1->setFeatures(QDockWidget::AllDockWidgetFeatures | QDockWidget::DockWidgetVerticalTitleBar);
    d2->setFeatures(QDockWidget::DockWidgetVerticalTitleBar);
    w1->resize(120, 120);
    splitter2->addWidget(w1);
    splitter2->addWidget(w2);
    splitter1->addWidget(mdi);
    splitter1->addWidget(splitter2);
    g4h->addWidget(splitter1);
    makeDraggable(mdi, QStringLiteral("QMdiArea"));
    makeDraggable(d1, QStringLiteral("QDockWidget"));
    makeDraggable(d2, QStringLiteral("QDockWidget"));
    makeDraggable(d3, QStringLiteral("QDockWidget"));
    makeDraggable(splitter1, QStringLiteral("QSplitter"));

    layout->addStretch(1);
    return w;
}

QWidget *PreviewWidget::makeTabContainer()
{
    QWidget *w = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(w);
    layout->setSpacing(8);

    QGroupBox *g1 = new QGroupBox(QStringLiteral("这是一个 QGroupBox 控件"));
    QHBoxLayout *g1h = new QHBoxLayout(g1);
    g1h->addWidget(new QLabel(QStringLiteral("这是一个 QGroupBox 控件")));
    layout->addWidget(g1);
    makeDraggable(g1, QStringLiteral("QGroupBox"));

    QGroupBox *g2 = new QGroupBox(QStringLiteral("这是一个带选项的 QGroupBox 控件"));
    g2->setCheckable(true);
    QHBoxLayout *g2h = new QHBoxLayout(g2);
    g2h->addWidget(new QLabel(QStringLiteral("这是一个带选项的 QGroupBox 控件")));
    layout->addWidget(g2);
    makeDraggable(g2, QStringLiteral("QGroupBox"));

    // StackLayout 布局
    QGroupBox *g3 = new QGroupBox(QStringLiteral("StackLayout 布局"));
    g3->setCheckable(true);
    QHBoxLayout *g3h = new QHBoxLayout(g3);
    layout->addWidget(g3);
    QListWidget *list = new QListWidget;
    list->addItems(QStringList() << QStringLiteral("stack1") << QStringLiteral("stack2")
                                 << QStringLiteral("stack3"));
    QStackedWidget *stack = new QStackedWidget;
    g3h->addWidget(list);
    g3h->addWidget(stack);
    QObject::connect(list, &QListWidget::currentRowChanged, stack, &QStackedWidget::setCurrentIndex);
    for (int i = 1; i <= 3; ++i) {
        QGroupBox *page = new QGroupBox(QStringLiteral("stack%1").arg(i));
        QVBoxLayout *pl = new QVBoxLayout(page);
        pl->addWidget(new QLabel(QStringLiteral("stack%1").arg(i)));
        pl->addWidget(new QLineEdit);
        stack->addWidget(page);
    }
    makeDraggable(list, QStringLiteral("QListWidget"));
    makeDraggable(stack, QStringLiteral("QStackedWidget"));

    // ToolBox
    QGroupBox *g4 = new QGroupBox(QStringLiteral("ToolBox"));
    g4->setCheckable(true);
    QVBoxLayout *g4v = new QVBoxLayout(g4);
    layout->addWidget(g4);
    QToolBox *toolbox = new QToolBox;
    for (int i = 1; i <= 3; ++i) {
        QWidget *page = new QWidget;
        QVBoxLayout *pl = new QVBoxLayout(page);
        pl->addWidget(new QLabel(QStringLiteral("aaaaa")));
        pl->addWidget(new QLabel(QStringLiteral("aaaaa")));
        pl->addWidget(new QLabel(QStringLiteral("aaaaa")));
        pl->addStretch(1);
        toolbox->addItem(page, i == 3 ? QStringLiteral("tab3") : QStringLiteral("Tab%1").arg(i));
    }
    g4v->addWidget(toolbox);
    makeDraggable(toolbox, QStringLiteral("QToolBox"));

    // TabWidget
    QGroupBox *g5 = new QGroupBox(QStringLiteral("TabWidget"));
    g5->setCheckable(true);
    QVBoxLayout *g5v = new QVBoxLayout(g5);
    QHBoxLayout *g5r1 = new QHBoxLayout;
    QHBoxLayout *g5r2 = new QHBoxLayout;
    g5v->addLayout(g5r1);
    g5v->addLayout(g5r2);
    layout->addWidget(g5);
    QTabWidget *t1 = new QTabWidget;
    t1->addTab(new QWidget, QStringLiteral("tab1"));
    t1->addTab(new QWidget, QStringLiteral("tab2"));
    t1->setTabsClosable(true);
    t1->setMinimumHeight(140);
    QTabWidget *t2 = new QTabWidget;
    t2->addTab(new QWidget, QStringLiteral("tab1"));
    t2->addTab(new QWidget, QStringLiteral("tab2"));
    t2->setTabPosition(QTabWidget::South);
    t2->setTabShape(QTabWidget::Triangular);
    QTabWidget *t3 = new QTabWidget;
    t3->addTab(new QWidget, QStringLiteral("tab1"));
    t3->addTab(new QWidget, QStringLiteral("tab2"));
    t3->setTabPosition(QTabWidget::West);
    t3->setMinimumHeight(160);
    QTabWidget *t4 = new QTabWidget;
    t4->addTab(new QWidget, QStringLiteral("tab1"));
    t4->addTab(new QWidget, QStringLiteral("tab2"));
    t4->setTabPosition(QTabWidget::East);
    g5r1->addWidget(t1);
    g5r1->addWidget(t2);
    g5r2->addWidget(t3);
    g5r2->addWidget(t4);
    makeDraggable(t1, QStringLiteral("QTabWidget"));
    makeDraggable(t2, QStringLiteral("QTabWidget"));
    makeDraggable(t3, QStringLiteral("QTabWidget"));
    makeDraggable(t4, QStringLiteral("QTabWidget"));

    layout->addStretch(1);
    return w;
}

QWidget *PreviewWidget::makeTabAdvance()
{
    QWidget *w = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(w);
    layout->setSpacing(8);

    QHBoxLayout *lay1 = new QHBoxLayout;
    layout->addLayout(lay1);
    QListWidget *list1 = new QListWidget;
    list1->addItems(QStringList() << QStringLiteral("aaa") << QStringLiteral("bbb")
                                  << QStringLiteral("ccc"));
    QListWidget *list2 = new QListWidget;
    QListWidgetItem *i1 = new QListWidgetItem(
        QApplication::style()->standardIcon(QStyle::SP_DialogYesButton), QStringLiteral("blue"));
    QListWidgetItem *i2 = new QListWidgetItem(
        QApplication::style()->standardIcon(QStyle::SP_DialogNoButton), QStringLiteral("red"));
    QListWidgetItem *i3 = new QListWidgetItem(
        QApplication::style()->standardIcon(QStyle::SP_DialogApplyButton), QStringLiteral("green"));
    list2->addItem(i1);
    list2->addItem(i2);
    list2->addItem(i3);
    list2->setViewMode(QListWidget::IconMode);
    lay1->addWidget(list1);
    lay1->addWidget(list2);
    makeDraggable(list1, QStringLiteral("QListWidget"));
    makeDraggable(list2, QStringLiteral("QListWidget"), 1);

    QHBoxLayout *lay2 = new QHBoxLayout;
    layout->addLayout(lay2);
    QTableWidget *t1 = new QTableWidget(3, 3);
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c) {
            QTableWidgetItem *it = new QTableWidgetItem(QString::number((r + 1) * (c + 1)));
            it->setTextAlignment(Qt::AlignCenter);
            t1->setItem(r, c, it);
        }
    t1->setColumnWidth(0, 50);
    t1->setColumnWidth(1, 50);
    t1->setColumnWidth(2, 50);
    QTableWidget *t2 = new QTableWidget(2, 3);
    t2->setHorizontalHeaderLabels(QStringList() << QStringLiteral("Name")
                                                << QStringLiteral("Gender")
                                                << QStringLiteral("Age"));
    t2->setVerticalHeaderLabels(QStringList() << QStringLiteral("1st") << QStringLiteral("2rd"));
    t2->setItem(0, 0, new QTableWidgetItem(QStringLiteral("july")));
    QComboBox *cg = new QComboBox;
    cg->addItems(QStringList() << QStringLiteral("Male") << QStringLiteral("Famale"));
    t2->setCellWidget(0, 1, cg);
    t2->setItem(0, 2, new QTableWidgetItem(QStringLiteral("10")));
    t2->setItem(1, 0, new QTableWidgetItem(QStringLiteral("john")));
    QComboBox *cg2 = new QComboBox;
    cg2->addItems(QStringList() << QStringLiteral("Male") << QStringLiteral("Famale"));
    cg2->setEditable(true);
    t2->setCellWidget(1, 1, cg2);
    t2->setItem(1, 2, new QTableWidgetItem(QStringLiteral("11")));
    t2->resizeColumnsToContents();
    lay2->addWidget(t1);
    lay2->addWidget(t2);
    makeDraggable(t1, QStringLiteral("QTableWidget"));
    makeDraggable(t2, QStringLiteral("QTableWidget"));

    QHBoxLayout *lay3 = new QHBoxLayout;
    layout->addLayout(lay3);
    QTreeWidget *tree1 = new QTreeWidget;
    tree1->setColumnCount(2);
    tree1->setHeaderLabels(QStringList() << QStringLiteral("Key") << QStringLiteral("Value"));
    QTreeWidgetItem *root = new QTreeWidgetItem;
    root->setText(0, QStringLiteral("root"));
    root->setText(1, QStringLiteral("0"));
    root->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_DirIcon));
    tree1->addTopLevelItem(root);
    QTreeWidgetItem *node11 = new QTreeWidgetItem;
    node11->setText(0, QStringLiteral("child1"));
    QTreeWidgetItem *na = new QTreeWidgetItem;
    na->setText(0, QStringLiteral("red"));
    na->setBackground(1, QBrush(Qt::red));
    QTreeWidgetItem *nb = new QTreeWidgetItem;
    nb->setText(0, QStringLiteral("gray"));
    nb->setBackground(1, QBrush(Qt::gray));
    QTreeWidgetItem *nc = new QTreeWidgetItem;
    nc->setText(0, QStringLiteral("green"));
    nc->setBackground(1, QBrush(Qt::green));
    node11->addChildren(QList<QTreeWidgetItem *>() << na << nb << nc);
    QTreeWidgetItem *node12 = new QTreeWidgetItem;
    node12->setText(0, QStringLiteral("child2"));
    node12->setText(1, QStringLiteral("child2"));
    QTreeWidgetItem *node13 = new QTreeWidgetItem;
    node13->setText(0, QStringLiteral("child3"));
    node13->setText(1, QStringLiteral("child3"));
    root->addChild(node11);
    root->addChild(node12);
    root->addChild(node13);
    tree1->expandAll();
    QTreeView *tree2 = new QTreeView;
    QFileSystemModel *fsm = new QFileSystemModel(tree2);
    fsm->setRootPath(QDir::homePath());
    tree2->setModel(fsm);
    tree2->setRootIndex(fsm->index(QDir::homePath()));
    lay3->addWidget(tree1);
    lay3->addWidget(tree2);
    makeDraggable(tree1, QStringLiteral("QTreeWidget"));
    makeDraggable(tree2, QStringLiteral("QTreeView"), 1);

    layout->addStretch(1);
    return w;
}

QWidget *PreviewWidget::makeTabCustom()
{
    // 自定义（完整界面）：菜单栏 + 工具栏 + 状态栏 + 停靠面板 + 中央表单
    QMainWindow *mw = new QMainWindow;
    mw->setDockOptions(QMainWindow::AnimatedDocks);

    QMenu *fileMenu = mw->menuBar()->addMenu(QStringLiteral("文件"));
    fileMenu->addAction(QStringLiteral("新建"));
    fileMenu->addAction(QStringLiteral("打开"));
    fileMenu->addSeparator();
    fileMenu->addAction(QStringLiteral("退出"));
    QMenu *editMenu = mw->menuBar()->addMenu(QStringLiteral("编辑"));
    editMenu->addAction(QStringLiteral("剪切"));
    editMenu->addAction(QStringLiteral("复制"));
    editMenu->addAction(QStringLiteral("粘贴"));

    QToolBar *toolbar = mw->addToolBar(QStringLiteral("工具栏"));
    toolbar->setObjectName(QStringLiteral("customToolBar"));
    toolbar->addAction(QStringLiteral("新建"));
    toolbar->addAction(QStringLiteral("打开"));
    toolbar->addAction(QStringLiteral("保存"));

    mw->statusBar()->showMessage(QStringLiteral("定义完成后点击预览按钮。"));

    QDockWidget *dock = new QDockWidget(QStringLiteral("属性"), mw);
    dock->setObjectName(QStringLiteral("customDock"));
    QListWidget *dockList = new QListWidget;
    dockList->addItems(QStringList() << QStringLiteral("属性 A") << QStringLiteral("属性 B")
                                     << QStringLiteral("属性 C"));
    dock->setWidget(dockList);
    mw->addDockWidget(Qt::RightDockWidgetArea, dock);
    makeDraggable(dockList, QStringLiteral("QListWidget"));

    QWidget *central = new QWidget;
    QVBoxLayout *cl = new QVBoxLayout(central);
    QLabel *userLbl = new QLabel(QStringLiteral("用户名："));
    QLineEdit *userEd = new QLineEdit(QStringLiteral("admin"));
    QLabel *pwdLbl = new QLabel(QStringLiteral("密码："));
    QLineEdit *pwdEd = new QLineEdit(QStringLiteral("******"));
    pwdEd->setEchoMode(QLineEdit::Password);
    QFormLayout *form = new QFormLayout;
    form->addRow(userLbl, userEd);
    form->addRow(pwdLbl, pwdEd);
    cl->addLayout(form);
    makeDraggableRow(userLbl, userEd, QStringLiteral("QLineEdit"));
    makeDraggableRow(pwdLbl, pwdEd, QStringLiteral("QLineEdit"));

    QHBoxLayout *btnRow = new QHBoxLayout;
    QPushButton *okBtn = new QPushButton(QStringLiteral("确定"));
    QPushButton *cancelBtn = new QPushButton(QStringLiteral("取消"));
    btnRow->addWidget(okBtn);
    btnRow->addWidget(cancelBtn);
    btnRow->addStretch(1);
    cl->addLayout(btnRow);
    makeDraggable(okBtn, QStringLiteral("QPushButton"));
    makeDraggable(cancelBtn, QStringLiteral("QPushButton"));

    QCheckBox *remember = new QCheckBox(QStringLiteral("记住登录"));
    cl->addWidget(remember);
    makeDraggable(remember, QStringLiteral("QCheckBox"));

    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setRange(0, 100);
    slider->setValue(50);
    QProgressBar *progress = new QProgressBar;
    progress->setRange(0, 100);
    progress->setValue(50);
    QObject::connect(slider, &QSlider::valueChanged, progress, &QProgressBar::setValue);
    cl->addWidget(slider);
    cl->addWidget(progress);
    makeDraggable(slider, QStringLiteral("QSlider"));
    makeDraggable(progress, QStringLiteral("QProgressBar"));
    cl->addStretch(1);

    mw->setCentralWidget(central);

    // 整个主窗口可拖：从菜单栏/工具栏/状态栏/停靠面板 Ctrl+拖拽
    QList<WidgetDragFilter::DragSource> wholeSources;
    wholeSources << WidgetDragFilter::DragSource(QStringLiteral("QMainWindow"),
                                                 QVariant(0), mw);
    new WidgetDragFilter(mw->menuBar(), wholeSources, mw);
    new WidgetDragFilter(toolbar, wholeSources, mw);
    new WidgetDragFilter(mw->statusBar(), wholeSources, mw);
    new WidgetDragFilter(dock, wholeSources, mw);
    return mw;
}
