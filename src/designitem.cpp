#pragma execution_character_set("utf-8")

#include "designitem.h"

#include "designscene.h"

#include <QApplication>
#include <QBuffer>
#include <QCalendarWidget>
#include <QCheckBox>
#include <QColumnView>
#include <QComboBox>
#include <QCommandLinkButton>
#include <QDate>
#include <QDateTimeEdit>
#include <QDateEdit>
#include <QDial>
#include <QDialogButtonBox>
#include <QDockWidget>
#include <QDoubleSpinBox>
#include <QDir>
#include <QFileSystemModel>
#include <QFontComboBox>
#include <QFrame>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QGroupBox>
#include <QKeySequenceEdit>
#include <QLabel>
#include <QLCDNumber>
#include <QLineEdit>
#include <QListView>
#include <QListWidget>
#include <QMainWindow>
#include <QMdiArea>
#include <QMenu>
#include <QMenuBar>
#include <QPainter>
#include <QPlainTextEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QRadioButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QSlider>
#include <QSpinBox>
#include <QSplitter>
#include <QStackedWidget>
#include <QStatusBar>
#include <QStyle>
#include <QTabWidget>
#include <QTableView>
#include <QTableWidget>
#include <QTextBrowser>
#include <QTextEdit>
#include <QTime>
#include <QTimeEdit>
#include <QToolBox>
#include <QToolButton>
#include <QToolBar>
#include <QTreeView>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

namespace {

QWidget *createWidgetForType(const QString &type, const QVariant &variant)
{
    QWidget *w = 0;
    if (type == QLatin1String("QPushButton"))
    {
        QPushButton *b = new QPushButton(QStringLiteral("按钮"));
        b->setMinimumSize(80, 28);
        w = b;
    }
    else if (type == QLatin1String("QToolButton"))
    {
        QToolButton *b = new QToolButton;
        b->setText(QStringLiteral("工具按钮"));
        b->setMinimumSize(70, 28);
        w = b;
    }
    else if (type == QLatin1String("QCommandLinkButton"))
    {
        QCommandLinkButton *b = new QCommandLinkButton(QStringLiteral("命令链接"));
        b->setDescription(QStringLiteral("命令链接说明文字"));
        b->setMinimumSize(170, 50);
        w = b;
    }
    else if (type == QLatin1String("QDialogButtonBox"))
    {
        QDialogButtonBox *b = new QDialogButtonBox;
        b->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        b->setMinimumSize(160, 34);
        w = b;
    }
    else if (type == QLatin1String("QLabel"))
    {
        QLabel *l = new QLabel(QStringLiteral("标签"));
        if (variant.toInt() == 1)
        {
            l->setPixmap(QApplication::style()->standardIcon(QStyle::SP_ComputerIcon)
                             .pixmap(48, 48));
            l->setAlignment(Qt::AlignCenter);
            l->setMinimumSize(64, 56);
        }
        else
        {
            l->setMinimumSize(50, 20);
        }
        w = l;
    }
    else if (type == QLatin1String("QLineEdit"))
    {
        QLineEdit *e = new QLineEdit;
        e->setPlaceholderText(QStringLiteral("请输入..."));
        e->setMinimumSize(100, 26);
        w = e;
    }
    else if (type == QLatin1String("QTextEdit"))
    {
        QTextEdit *e = new QTextEdit;
        e->setPlainText(QStringLiteral("文本编辑"));
        e->setMinimumSize(120, 80);
        w = e;
    }
    else if (type == QLatin1String("QPlainTextEdit"))
    {
        QPlainTextEdit *e = new QPlainTextEdit;
        e->setPlainText(QStringLiteral("纯文本编辑"));
        e->setMinimumSize(120, 80);
        w = e;
    }
    else if (type == QLatin1String("QTextBrowser"))
    {
        QTextBrowser *e = new QTextBrowser;
        e->setPlainText(QStringLiteral("文本浏览器"));
        e->setMinimumSize(120, 80);
        w = e;
    }
    else if (type == QLatin1String("QComboBox"))
    {
        QComboBox *c = new QComboBox;
        c->addItems(QStringList() << QStringLiteral("选项 1") << QStringLiteral("选项 2")
                                   << QStringLiteral("选项 3"));
        c->setMinimumSize(90, 26);
        w = c;
    }
    else if (type == QLatin1String("QFontComboBox"))
    {
        QFontComboBox *c = new QFontComboBox;
        c->setMinimumSize(120, 26);
        w = c;
    }
    else if (type == QLatin1String("QCheckBox"))
    {
        QCheckBox *c = new QCheckBox(QStringLiteral("复选框"));
        c->setMinimumSize(80, 22);
        w = c;
    }
    else if (type == QLatin1String("QRadioButton"))
    {
        QRadioButton *r = new QRadioButton(QStringLiteral("单选按钮"));
        r->setMinimumSize(80, 22);
        w = r;
    }
    else if (type == QLatin1String("QSpinBox"))
    {
        QSpinBox *s = new QSpinBox;
        s->setRange(0, 100);
        s->setValue(50);
        s->setMinimumSize(80, 26);
        w = s;
    }
    else if (type == QLatin1String("QDoubleSpinBox"))
    {
        QDoubleSpinBox *s = new QDoubleSpinBox;
        s->setRange(0.0, 100.0);
        s->setValue(50.0);
        s->setMinimumSize(90, 26);
        w = s;
    }
    else if (type == QLatin1String("QKeySequenceEdit"))
    {
        QKeySequenceEdit *e = new QKeySequenceEdit;
        e->setKeySequence(QKeySequence(QStringLiteral("Ctrl+S")));
        e->setMinimumSize(110, 26);
        w = e;
    }
    else if (type == QLatin1String("QTimeEdit"))
    {
        QTimeEdit *t = new QTimeEdit(QTime::currentTime());
        t->setMinimumSize(90, 26);
        w = t;
    }
    else if (type == QLatin1String("QDateEdit"))
    {
        QDateEdit *d = new QDateEdit(QDate::currentDate());
        d->setCalendarPopup(true);
        d->setMinimumSize(100, 26);
        w = d;
    }
    else if (type == QLatin1String("QDateTimeEdit"))
    {
        QDateTimeEdit *e = new QDateTimeEdit(QDateTime::currentDateTime());
        e->setCalendarPopup(true);
        e->setMinimumSize(110, 26);
        w = e;
    }
    else if (type == QLatin1String("QSlider"))
    {
        QSlider *s = new QSlider(Qt::Horizontal);
        s->setRange(0, 100);
        s->setValue(50);
        s->setMinimumSize(120, 24);
        w = s;
    }
    else if (type == QLatin1String("QScrollBar"))
    {
        QScrollBar *s = new QScrollBar(Qt::Horizontal);
        s->setRange(0, 100);
        s->setValue(40);
        s->setMinimumSize(120, 20);
        w = s;
    }
    else if (type == QLatin1String("QDial"))
    {
        QDial *d = new QDial;
        d->setRange(0, 100);
        d->setValue(50);
        d->setMinimumSize(80, 80);
        w = d;
    }
    else if (type == QLatin1String("QProgressBar"))
    {
        QProgressBar *p = new QProgressBar;
        p->setRange(0, 100);
        p->setValue(60);
        p->setMinimumSize(120, 24);
        w = p;
    }
    else if (type == QLatin1String("QCalendarWidget"))
    {
        QCalendarWidget *c = new QCalendarWidget;
        c->setMinimumSize(240, 180);
        w = c;
    }
    else if (type == QLatin1String("QLCDNumber"))
    {
        QLCDNumber *l = new QLCDNumber;
        l->display(123);
        l->setMinimumSize(100, 40);
        w = l;
    }
    else if (type == QLatin1String("QGraphicsView"))
    {
        QGraphicsView *v = new QGraphicsView;
        v->setMinimumSize(140, 100);
        w = v;
    }
    else if (type == QLatin1String("QListWidget"))
    {
        QListWidget *l = new QListWidget;
        l->addItems(QStringList() << QStringLiteral("项目 1") << QStringLiteral("项目 2")
                                  << QStringLiteral("项目 3"));
        if (variant.toInt() == 1)
        {
            // 图标模式列表
            l->setViewMode(QListView::IconMode);
            l->setIconSize(QSize(32, 32));
            l->setSpacing(6);
            for (int i = 0; i < l->count(); ++i)
            {
                l->item(i)->setIcon(QApplication::style()->standardIcon(
                    i == 0 ? QStyle::SP_FileIcon
                           : (i == 1 ? QStyle::SP_DirIcon : QStyle::SP_ComputerIcon)));
            }
            l->setMinimumSize(110, 90);
        }
        else
        {
            l->setMinimumSize(100, 90);
        }
        w = l;
    }
    else if (type == QLatin1String("QListView"))
    {
        QListView *v = new QListView;
        v->setMinimumSize(100, 90);
        w = v;
    }
    else if (type == QLatin1String("QTableWidget"))
    {
        QTableWidget *t = new QTableWidget(3, 3);
        t->setHorizontalHeaderLabels(QStringList() << QStringLiteral("列1") << QStringLiteral("列2")
                                                   << QStringLiteral("列3"));
        t->setMinimumSize(140, 100);
        w = t;
    }
    else if (type == QLatin1String("QTableView"))
    {
        QTableView *v = new QTableView;
        v->setMinimumSize(140, 100);
        w = v;
    }
    else if (type == QLatin1String("QColumnView"))
    {
        QColumnView *v = new QColumnView;
        v->setMinimumSize(140, 100);
        w = v;
    }
    else if (type == QLatin1String("QTreeWidget"))
    {
        QTreeWidget *t = new QTreeWidget;
        t->setColumnCount(1);
        t->setHeaderLabel(QStringLiteral("树"));
        QTreeWidgetItem *item = new QTreeWidgetItem(t, QStringList(QStringLiteral("根节点")));
        new QTreeWidgetItem(item, QStringList(QStringLiteral("子节点")));
        t->expandAll();
        t->setMinimumSize(120, 100);
        w = t;
    }
    else if (type == QLatin1String("QTreeView"))
    {
        QTreeView *v = new QTreeView;
        if (variant.toInt() == 1)
        {
            // 文件系统树
            QFileSystemModel *fsm = new QFileSystemModel(v);
            fsm->setRootPath(QDir::homePath());
            v->setModel(fsm);
            v->setRootIndex(fsm->index(QDir::homePath()));
            v->setMinimumSize(160, 110);
        }
        else
        {
            v->setMinimumSize(120, 100);
        }
        w = v;
    }
    else if (type == QLatin1String("QGroupBox"))
    {
        QGroupBox *g = new QGroupBox(QStringLiteral("分组框"));
        g->setCheckable(true);
        g->setMinimumSize(160, 110);
        w = g;
    }
    else if (type == QLatin1String("QTabWidget"))
    {
        QTabWidget *t = new QTabWidget;
        t->addTab(new QWidget, QStringLiteral("页签 1"));
        t->addTab(new QWidget, QStringLiteral("页签 2"));
        t->setMinimumSize(200, 120);
        w = t;
    }
    else if (type == QLatin1String("QToolBox"))
    {
        QToolBox *t = new QToolBox;
        t->addItem(new QWidget, QStringLiteral("页面 1"));
        t->addItem(new QWidget, QStringLiteral("页面 2"));
        t->setMinimumSize(140, 110);
        w = t;
    }
    else if (type == QLatin1String("QStackedWidget"))
    {
        QStackedWidget *s = new QStackedWidget;
        QLabel *p1 = new QLabel(QStringLiteral("页面 1"));
        p1->setAlignment(Qt::AlignCenter);
        QLabel *p2 = new QLabel(QStringLiteral("页面 2"));
        p2->setAlignment(Qt::AlignCenter);
        s->addWidget(p1);
        s->addWidget(p2);
        s->setMinimumSize(140, 90);
        w = s;
    }
    else if (type == QLatin1String("QScrollArea"))
    {
        QScrollArea *s = new QScrollArea;
        s->setWidgetResizable(true);
        QLabel *content = new QLabel(QStringLiteral("滚动内容"));
        content->setAlignment(Qt::AlignCenter);
        content->setMinimumSize(200, 120);
        s->setWidget(content);
        s->setMinimumSize(140, 100);
        w = s;
    }
    else if (type == QLatin1String("QFrame"))
    {
        QFrame *f = new QFrame;
        const int shape = variant.toInt();
        if (shape == 4)
            f->setFrameShape(QFrame::HLine);
        else if (shape == 5)
            f->setFrameShape(QFrame::VLine);
        else
            f->setFrameShape(QFrame::StyledPanel);
        f->setFrameShadow(QFrame::Sunken);
        f->setMinimumSize(shape == 5 ? 4 : 120, shape == 5 ? 80 : 4);
        w = f;
    }
    else if (type == QLatin1String("QWidget"))
    {
        QWidget *c = new QWidget;
        c->setMinimumSize(120, 80);
        w = c;
    }
    else if (type == QLatin1String("QMdiArea"))
    {
        QMdiArea *m = new QMdiArea;
        m->addSubWindow(new QTextEdit(QStringLiteral("MDI 子窗口")));
        m->setMinimumSize(180, 120);
        w = m;
    }
    else if (type == QLatin1String("QDockWidget"))
    {
        QDockWidget *d = new QDockWidget(QStringLiteral("停靠窗口"));
        d->setWidget(new QLabel(QStringLiteral("停靠内容")));
        d->setMinimumSize(160, 100);
        w = d;
    }
    else if (type == QLatin1String("QSplitter"))
    {
        QSplitter *s = new QSplitter(Qt::Horizontal);
        s->addWidget(new QLabel(QStringLiteral("左")));
        s->addWidget(new QLabel(QStringLiteral("右")));
        s->setMinimumSize(150, 90);
        w = s;
    }
    else if (type == QLatin1String("QMainWindow"))
    {
        // 完整主窗口（与预览“完整界面”页一致）：菜单栏+工具栏+状态栏+中央表单
        QMainWindow *mw = new QMainWindow;
        mw->setMinimumSize(420, 260);
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
        toolbar->setObjectName(QStringLiteral("toolBar"));
        toolbar->addAction(QStringLiteral("新建"));
        toolbar->addAction(QStringLiteral("打开"));
        toolbar->addAction(QStringLiteral("保存"));
        mw->statusBar()->showMessage(QStringLiteral("就绪"));
        QWidget *central = new QWidget;
        QVBoxLayout *cl = new QVBoxLayout(central);
        QFormLayout *form = new QFormLayout;
        form->addRow(QStringLiteral("用户名："), new QLineEdit(QStringLiteral("admin")));
        QLineEdit *pwd = new QLineEdit(QStringLiteral("******"));
        pwd->setEchoMode(QLineEdit::Password);
        form->addRow(QStringLiteral("密码："), pwd);
        cl->addLayout(form);
        QHBoxLayout *btnRow = new QHBoxLayout;
        btnRow->addWidget(new QPushButton(QStringLiteral("确定")));
        btnRow->addWidget(new QPushButton(QStringLiteral("取消")));
        btnRow->addStretch(1);
        cl->addLayout(btnRow);
        QProgressBar *pb = new QProgressBar;
        pb->setRange(0, 100);
        pb->setValue(50);
        cl->addWidget(pb);
        cl->addStretch(1);
        mw->setCentralWidget(central);
        w = mw;
    }
    return w;
}

const int HandleSize = 7;

} // namespace

DesignItem::DesignItem(const QString &typeName, const QVariant &variant, QGraphicsItem *parent)
    : QGraphicsProxyWidget(parent)
    , m_typeName(typeName)
    , m_variant(variant)
    , m_activeHandle(NoHandle)
    , m_resizing(false)
{
    QWidget *w = createWidgetForType(typeName, variant);
    setWidget(w);
    setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges | ItemSendsScenePositionChanges);
    setAcceptHoverEvents(true);
    setZValue(1);
    setMinimumSize(w->minimumSize());
    updateToolTip();
}

QString DesignItem::typeName() const
{
    return m_typeName;
}

QVariant DesignItem::variant() const
{
    return m_variant;
}

QString DesignItem::objectName() const
{
    return widget() ? widget()->objectName() : QString();
}

void DesignItem::setObjectName(const QString &name)
{
    if (widget())
        widget()->setObjectName(name);
    updateToolTip();
}

QRectF DesignItem::itemGeometry() const
{
    return QRectF(pos(), boundingRect().size());
}

void DesignItem::setItemGeometry(const QRectF &rect)
{
    const QRectF r = rect.normalized();
    setPos(r.topLeft());
    if (widget())
    {
        widget()->resize(r.size().toSize());
    }
    if (designScene())
        emit designScene()->itemPropertyChanged(this);
}

QMap<QString, QVariant> collectWidgetProperties(QWidget *widget)
{
    QMap<QString, QVariant> props;
    if (!widget)
        return props;

    props.insert(QStringLiteral("enabled"), widget->isEnabled());
    props.insert(QStringLiteral("visible"), widget->isVisible());
    props.insert(QStringLiteral("toolTip"), widget->toolTip());
    props.insert(QStringLiteral("width"), qMax(widget->width(), widget->minimumWidth()));
    props.insert(QStringLiteral("height"), qMax(widget->height(), widget->minimumHeight()));
    if (!widget->styleSheet().isEmpty())
        props.insert(QStringLiteral("styleSheet"), widget->styleSheet());

    if (QAbstractButton *btn = qobject_cast<QAbstractButton *>(widget)) {
        props.insert(QStringLiteral("text"), btn->text());
        props.insert(QStringLiteral("checkable"), btn->isCheckable());
        props.insert(QStringLiteral("checked"), btn->isChecked());
        if (!btn->icon().isNull())
        {
            QBuffer buffer;
            buffer.open(QIODevice::WriteOnly);
            btn->icon().pixmap(32, 32).toImage().save(&buffer, "PNG");
            props.insert(QStringLiteral("icon"),
                         QString::fromLatin1(buffer.data().toBase64()));
        }
    }
    else if (QLabel *lbl = qobject_cast<QLabel *>(widget))
    {
        props.insert(QStringLiteral("text"), lbl->text());
    }
    else if (QLineEdit *le = qobject_cast<QLineEdit *>(widget))
    {
        props.insert(QStringLiteral("text"), le->text());
        props.insert(QStringLiteral("placeholderText"), le->placeholderText());
        props.insert(QStringLiteral("echoMode"), (int)le->echoMode());
        props.insert(QStringLiteral("readOnly"), le->isReadOnly());
        props.insert(QStringLiteral("inputMask"), le->inputMask());
    }
    else if (QTextEdit *te = qobject_cast<QTextEdit *>(widget))
    {
        props.insert(QStringLiteral("plainText"), te->toPlainText());
        props.insert(QStringLiteral("readOnly"), te->isReadOnly());
    }
    else if (QPlainTextEdit *pte = qobject_cast<QPlainTextEdit *>(widget))
    {
        props.insert(QStringLiteral("plainText"), pte->toPlainText());
        props.insert(QStringLiteral("readOnly"), pte->isReadOnly());
    }
    else if (QTextBrowser *tb = qobject_cast<QTextBrowser *>(widget))
    {
        props.insert(QStringLiteral("plainText"), tb->toPlainText());
    }
    else if (QComboBox *cb = qobject_cast<QComboBox *>(widget))
    {
        QStringList items;
        for (int i = 0; i < cb->count(); ++i)
            items << cb->itemText(i);
        props.insert(QStringLiteral("items"), items);
        props.insert(QStringLiteral("currentIndex"), cb->currentIndex());
        props.insert(QStringLiteral("editable"), cb->isEditable());
    }
    else if (QAbstractSpinBox *sb = qobject_cast<QAbstractSpinBox *>(widget))
    {
        if (QSpinBox *s = qobject_cast<QSpinBox *>(sb))
        {
            props.insert(QStringLiteral("minimum"), s->minimum());
            props.insert(QStringLiteral("maximum"), s->maximum());
            props.insert(QStringLiteral("value"), s->value());
        }
        else if (QDoubleSpinBox *s = qobject_cast<QDoubleSpinBox *>(sb))
        {
            props.insert(QStringLiteral("minimum"), s->minimum());
            props.insert(QStringLiteral("maximum"), s->maximum());
            props.insert(QStringLiteral("decimals"), s->decimals());
            props.insert(QStringLiteral("value"), s->value());
        }
    }
    else if (QAbstractSlider *sl = qobject_cast<QAbstractSlider *>(widget))
    {
        props.insert(QStringLiteral("minimum"), sl->minimum());
        props.insert(QStringLiteral("maximum"), sl->maximum());
        props.insert(QStringLiteral("value"), sl->value());
        props.insert(QStringLiteral("orientation"), sl->orientation() == Qt::Horizontal ? 1 : 2);
    }
    else if (QProgressBar *pb = qobject_cast<QProgressBar *>(widget))
    {
        props.insert(QStringLiteral("minimum"), pb->minimum());
        props.insert(QStringLiteral("maximum"), pb->maximum());
        props.insert(QStringLiteral("value"), pb->value());
    }
    else if (QGroupBox *gb = qobject_cast<QGroupBox *>(widget))
    {
        props.insert(QStringLiteral("title"), gb->title());
        props.insert(QStringLiteral("checkable"), gb->isCheckable());
        props.insert(QStringLiteral("checked"), gb->isChecked());
    }
    else if (QListWidget *lw = qobject_cast<QListWidget *>(widget))
    {
        QStringList items;
        for (int i = 0; i < lw->count(); ++i)
            items << lw->item(i)->text();
        props.insert(QStringLiteral("items"), items);
    }
    else if (QTableWidget *tw = qobject_cast<QTableWidget *>(widget))
    {
        props.insert(QStringLiteral("rowCount"), tw->rowCount());
        props.insert(QStringLiteral("columnCount"), tw->columnCount());
    }
    else if (QFrame *f = qobject_cast<QFrame *>(widget))
    {
        if (f->frameShape() == QFrame::HLine)
            props.insert(QStringLiteral("frameShape"), QStringLiteral("HLine"));
        else if (f->frameShape() == QFrame::VLine)
            props.insert(QStringLiteral("frameShape"), QStringLiteral("VLine"));
    }
    return props;
}

QMap<QString, QVariant> DesignItem::exportProperties() const
{
    QMap<QString, QVariant> props = collectWidgetProperties(widget());
    props.insert(QStringLiteral("objectName"), objectName());
    props.insert(QStringLiteral("geometry"), itemGeometry());
    return props;
}

void DesignItem::applyProperty(const QString &key, const QVariant &value)
{
    if (!widget())
        return;

    if (key == QLatin1String("objectName"))
        setObjectName(value.toString());
    else if (key == QLatin1String("icon"))
    {
        const QByteArray data = QByteArray::fromBase64(value.toString().toLatin1());
        QPixmap loaded;
        loaded.loadFromData(data, "PNG");
        if (QAbstractButton *btn = qobject_cast<QAbstractButton *>(widget()))
            btn->setIcon(QIcon(loaded));
        else if (QLabel *lbl = qobject_cast<QLabel *>(widget()))
            lbl->setPixmap(loaded);
    }
    else if (key == QLatin1String("enabled"))
        widget()->setEnabled(value.toBool());
    else if (key == QLatin1String("visible"))
        widget()->setVisible(value.toBool());
    else if (key == QLatin1String("toolTip"))
        widget()->setToolTip(value.toString());
    else if (key == QLatin1String("styleSheet"))
    {
        m_localStyleSheet = value.toString();
        const QString cur = designScene() ? designScene()->currentStyleSheet() : QString();
        setDesignStyleSheet(cur);
    }
    else if (key == QLatin1String("geometry"))
    {
        setItemGeometry(value.toRectF());
    }
    else if (key == QLatin1String("width") || key == QLatin1String("height"))
    {
        QRectF g = itemGeometry();
        if (key == QLatin1String("width"))
            g.setWidth(qMax(value.toInt(), (int)widget()->minimumWidth()));
        else
            g.setHeight(qMax(value.toInt(), (int)widget()->minimumHeight()));
        setItemGeometry(g);
    }
    else if (QAbstractButton *btn = qobject_cast<QAbstractButton *>(widget()))
    {
        if (key == QLatin1String("text"))
            btn->setText(value.toString());
        else if (key == QLatin1String("checkable"))
            btn->setCheckable(value.toBool());
        else if (key == QLatin1String("checked"))
            btn->setChecked(value.toBool());
    }
    else if (QLabel *lbl = qobject_cast<QLabel *>(widget()))
    {
        if (key == QLatin1String("text"))
            lbl->setText(value.toString());
    }
    else if (QLineEdit *le = qobject_cast<QLineEdit *>(widget()))
    {
        if (key == QLatin1String("text"))
            le->setText(value.toString());
        else if (key == QLatin1String("placeholderText"))
            le->setPlaceholderText(value.toString());
        else if (key == QLatin1String("echoMode"))
            le->setEchoMode((QLineEdit::EchoMode)value.toInt());
        else if (key == QLatin1String("readOnly"))
            le->setReadOnly(value.toBool());
        else if (key == QLatin1String("inputMask"))
            le->setInputMask(value.toString());
    }
    else if (QTextEdit *te = qobject_cast<QTextEdit *>(widget()))
    {
        if (key == QLatin1String("plainText"))
            te->setPlainText(value.toString());
        else if (key == QLatin1String("readOnly"))
            te->setReadOnly(value.toBool());
    }
    else if (QPlainTextEdit *pte = qobject_cast<QPlainTextEdit *>(widget()))
    {
        if (key == QLatin1String("plainText"))
            pte->setPlainText(value.toString());
        else if (key == QLatin1String("readOnly"))
            pte->setReadOnly(value.toBool());
    }
    else if (QTextBrowser *tb = qobject_cast<QTextBrowser *>(widget()))
    {
        if (key == QLatin1String("plainText"))
            tb->setPlainText(value.toString());
    }
    else if (QComboBox *cb = qobject_cast<QComboBox *>(widget()))
    {
        if (key == QLatin1String("items"))
        {
            cb->clear();
            cb->addItems(value.toStringList());
        }
        else if (key == QLatin1String("currentIndex"))
            cb->setCurrentIndex(value.toInt());
        else if (key == QLatin1String("editable"))
            cb->setEditable(value.toBool());
    }
    else if (QAbstractSpinBox *sb = qobject_cast<QAbstractSpinBox *>(widget()))
    {
        if (QSpinBox *s = qobject_cast<QSpinBox *>(sb))
        {
            if (key == QLatin1String("minimum"))
                s->setMinimum(value.toInt());
            else if (key == QLatin1String("maximum"))
                s->setMaximum(value.toInt());
            else if (key == QLatin1String("value"))
                s->setValue(value.toInt());
        }
        else if (QDoubleSpinBox *s = qobject_cast<QDoubleSpinBox *>(sb))
        {
            if (key == QLatin1String("minimum"))
                s->setMinimum(value.toDouble());
            else if (key == QLatin1String("maximum"))
                s->setMaximum(value.toDouble());
            else if (key == QLatin1String("decimals"))
                s->setDecimals(value.toInt());
            else if (key == QLatin1String("value"))
                s->setValue(value.toDouble());
        }
    }
    else if (QAbstractSlider *sl = qobject_cast<QAbstractSlider *>(widget()))
    {
        if (key == QLatin1String("minimum"))
            sl->setMinimum(value.toInt());
        else if (key == QLatin1String("maximum"))
            sl->setMaximum(value.toInt());
        else if (key == QLatin1String("value"))
            sl->setValue(value.toInt());
        else if (key == QLatin1String("orientation"))
            sl->setOrientation(value.toInt() == 1 ? Qt::Horizontal : Qt::Vertical);
    }
    else if (QProgressBar *pb = qobject_cast<QProgressBar *>(widget()))
    {
        if (key == QLatin1String("minimum"))
            pb->setMinimum(value.toInt());
        else if (key == QLatin1String("maximum"))
            pb->setMaximum(value.toInt());
        else if (key == QLatin1String("value"))
            pb->setValue(value.toInt());
    }
    else if (QGroupBox *gb = qobject_cast<QGroupBox *>(widget()))
    {
        if (key == QLatin1String("title"))
            gb->setTitle(value.toString());
        else if (key == QLatin1String("checkable"))
            gb->setCheckable(value.toBool());
        else if (key == QLatin1String("checked"))
            gb->setChecked(value.toBool());
    }
    else if (QTableWidget *tw = qobject_cast<QTableWidget *>(widget()))
    {
        if (key == QLatin1String("rowCount"))
            tw->setRowCount(value.toInt());
        else if (key == QLatin1String("columnCount"))
            tw->setColumnCount(value.toInt());
    }

    if (designScene())
        emit designScene()->itemPropertyChanged(this);
}

void DesignItem::setDesignStyleSheet(const QString &qss)
{
    if (!widget())
        return;
    QString full = qss;
    if (!m_localStyleSheet.isEmpty())
        full = full.isEmpty() ? m_localStyleSheet : full + QLatin1Char('\n') + m_localStyleSheet;
    widget()->setStyleSheet(full);
}

void DesignItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsProxyWidget::paint(painter, option, widget);

    if (!isSelected())
        return;

    painter->save();
    painter->setPen(QPen(QColor(0, 120, 215), 1, Qt::DashLine));
    painter->setBrush(Qt::NoBrush);
    const QRectF r = boundingRect().adjusted(1.0, 1.0, -1.0, -1.0);
    painter->drawRect(r);

    painter->setPen(QPen(QColor(255, 255, 255), 1));
    painter->setBrush(QColor(0, 120, 215));
    const qreal h = HandleSize;
    const QPointF pts[8] =
    {
        r.topLeft(), QPointF(r.center().x(), r.top()), r.topRight(),
        QPointF(r.right(), r.center().y()), r.bottomRight(),
        QPointF(r.center().x(), r.bottom()), r.bottomLeft(),
        QPointF(r.left(), r.center().y())
    };
    for (int i = 0; i < 8; ++i)
        painter->drawRect(QRectF(pts[i].x() - h / 2, pts[i].y() - h / 2, h, h));
    painter->restore();
}

DesignItem::Handle DesignItem::handleAt(const QPointF &scenePos) const
{
    if (!isSelected())
        return NoHandle;
    const QRectF r = boundingRect().adjusted(1.0, 1.0, -1.0, -1.0);
    const qreal h = HandleSize;
    const QPointF p = mapFromScene(scenePos);
    const QPointF pts[8] =
    {
        r.topLeft(), QPointF(r.center().x(), r.top()), r.topRight(),
        QPointF(r.right(), r.center().y()), r.bottomRight(),
        QPointF(r.center().x(), r.bottom()), r.bottomLeft(),
        QPointF(r.left(), r.center().y())
    };
    const Handle handles[8] = { TopLeft, Top, TopRight, Right, BottomRight, Bottom, BottomLeft, Left };
    for (int i = 0; i < 8; ++i)
    {
        if (QRectF(pts[i].x() - h / 2, pts[i].y() - h / 2, h, h).contains(p))
            return handles[i];
    }
    return NoHandle;
}

QRectF DesignItem::resizeRect(const QRectF &original, Handle handle, const QPointF &delta) const
{
    QRectF r = original;
    switch (handle)
    {
    case TopLeft: r.setTopLeft(original.topLeft() + delta); break;
    case Top: r.setTop(original.top() + delta.y()); break;
    case TopRight: r.setTopRight(original.topRight() + delta); break;
    case Right: r.setRight(original.right() + delta.x()); break;
    case BottomRight: r.setBottomRight(original.bottomRight() + delta); break;
    case Bottom: r.setBottom(original.bottom() + delta.y()); break;
    case BottomLeft: r.setBottomLeft(original.bottomLeft() + delta); break;
    case Left: r.setLeft(original.left() + delta.x()); break;
    default: break;
    }
    const QSizeF minSize = widget() ? widget()->minimumSize() : QSizeF(20, 20);
    if (r.width() < minSize.width())
        r.setWidth(minSize.width());
    if (r.height() < minSize.height())
        r.setHeight(minSize.height());
    return r;
}

void DesignItem::updateCursor(Handle handle)
{
    switch (handle)
    {
    case TopLeft:
    case BottomRight:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case TopRight:
    case BottomLeft:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case Top:
    case Bottom:
        setCursor(Qt::SizeVerCursor);
        break;
    case Left:
    case Right:
        setCursor(Qt::SizeHorCursor);
        break;
    default:
        unsetCursor();
        break;
    }
}

void DesignItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    updateCursor(handleAt(event->scenePos()));
    QGraphicsProxyWidget::hoverMoveEvent(event);
}

void DesignItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_activeHandle = handleAt(event->scenePos());
    if (m_activeHandle != NoHandle)
    {
        setSelected(true);
        m_resizing = true;
        m_pressGeometry = itemGeometry();
        m_pressPos = event->scenePos();
        event->accept();
        return;
    }
    m_resizing = false;
    QGraphicsProxyWidget::mousePressEvent(event);
}

void DesignItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_resizing && m_activeHandle != NoHandle)
    {
        const QPointF delta = event->scenePos() - m_pressPos;
        setItemGeometry(resizeRect(m_pressGeometry, m_activeHandle, delta));
        event->accept();
        return;
    }
    QGraphicsProxyWidget::mouseMoveEvent(event);
}

void DesignItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_resizing)
    {
        m_resizing = false;
        m_activeHandle = NoHandle;
        if (designScene())
            emit designScene()->itemPropertyChanged(this);
        event->accept();
        return;
    }
    QGraphicsProxyWidget::mouseReleaseEvent(event);
}

QVariant DesignItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    const QVariant result = QGraphicsProxyWidget::itemChange(change, value);
    if (change == ItemPositionHasChanged && scene())
    {
        DesignScene *ds = qobject_cast<DesignScene *>(scene());
        if (ds)
            emit ds->itemPropertyChanged(this);
    }
    return result;
}

void DesignItem::ensureUniqueObjectName()
{
    // handled by DesignScene::nextObjectName
}

DesignScene *DesignItem::designScene() const
{
    return qobject_cast<DesignScene *>(scene());
}

void DesignItem::updateToolTip()
{
    if (widget())
        widget()->setToolTip(m_typeName);
}
