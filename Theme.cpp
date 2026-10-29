#include "Theme.h"
#include <QApplication>
#include <QFontDatabase>
#include <QPalette>
#include <QSettings>
#include <QStyleFactory>
#include <QStyleHints>

static QString current_theme = "system";
static ThemeColors colors;
static QString kana_family = "Klee One SemiBold";

//workbook paper and indigo ink
static const ThemeColors light_colors = {
    QColor("#F4F5F7"), QColor("#FFFFFF"), QColor("#1E2230"), QColor("#646A7C"),
    QColor("#D9DDE5"), QColor("#C3CDE0"), QColor("#2F4E8C"), QColor("#FFFFFF"),
    QColor("#2F7A45"), QColor("#B4323F")
};
//the same desk at night
static const ThemeColors dark_colors = {
    QColor("#161B26"), QColor("#1E2533"), QColor("#E4E7EE"), QColor("#8C93A6"),
    QColor("#2E3748"), QColor("#3A4763"), QColor("#8AA8E6"), QColor("#121722"),
    QColor("#7CC690"), QColor("#F08A95")
};

//a color between a ( t=0 ) and b ( t=1 )
static QColor mix(const QColor &a, const QColor &b, qreal t)
{
    return QColor::fromRgbF(a.redF()+(b.redF()-a.redF())*t,
                            a.greenF()+(b.greenF()-a.greenF())*t,
                            a.blueF()+(b.blueF()-a.blueF())*t);
}

QStringList availableThemes()
{
    return {"system", "light", "dark"};
}

QString initialTheme()
{
    QString code = QSettings().value("theme").toString();
    return availableThemes().contains(code) ? code : "system";
}

QString currentTheme()
{
    return current_theme;
}

const ThemeColors& themeColors()
{
    return colors;
}

static bool isDark()
{
    if (current_theme=="system")
        return QGuiApplication::styleHints()->colorScheme()==Qt::ColorScheme::Dark;
    return current_theme=="dark";
}

static QPalette makePalette(const ThemeColors &c)
{
    QPalette p;
    p.setColor(QPalette::Window, c.window);
    p.setColor(QPalette::WindowText, c.text);
    p.setColor(QPalette::Base, c.surface);
    p.setColor(QPalette::AlternateBase, c.window);
    p.setColor(QPalette::Text, c.text);
    p.setColor(QPalette::PlaceholderText, c.muted);
    p.setColor(QPalette::Button, c.surface);
    p.setColor(QPalette::ButtonText, c.text);
    p.setColor(QPalette::BrightText, c.wrong);
    p.setColor(QPalette::Highlight, c.accent);
    p.setColor(QPalette::HighlightedText, c.on_accent);
    p.setColor(QPalette::ToolTipBase, c.surface);
    p.setColor(QPalette::ToolTipText, c.text);
    p.setColor(QPalette::Link, c.accent);
    p.setColor(QPalette::Mid, c.line);
    p.setColor(QPalette::Dark, c.line);
    p.setColor(QPalette::Light, c.surface);
    p.setColor(QPalette::Midlight, c.surface);
    p.setColor(QPalette::Shadow, c.line);
    for (QPalette::ColorRole role : {QPalette::WindowText, QPalette::Text, QPalette::ButtonText})
        p.setColor(QPalette::Disabled, role, c.muted);
    return p;
}

static QString makeStyleSheet(const ThemeColors &c)
{
    QString qss = R"(
QMenuBar { background: @window; color: @text; padding: 2px 6px; font-size: 10pt; }
QMenuBar::item { padding: 4px 10px; border-radius: 6px; }
QMenuBar::item:selected { background: @hover; }
QMenu { font-size: 10pt; background: @surface; color: @text; border: 1px solid @line; border-radius: 8px; padding: 4px; }
QMenu::item { padding: 6px 24px 6px 28px; border-radius: 6px; }
QMenu::item:selected { background: @accent_soft; }
QToolBar { background: @window; border: none; padding: 8px 12px 0 12px; spacing: 10px; }
QToolTip { background: @surface; color: @text; border: 1px solid @line; padding: 4px 6px; }

QPushButton { background: @surface; color: @text; border: 1px solid @line; border-radius: 8px;
              padding: 8px 16px; }
QPushButton:hover { border-color: @accent; }
QPushButton:focus { border-color: @accent; }
QPushButton:pressed { background: @accent_soft; }
QPushButton:checked { background: @accent; color: @on_accent; border-color: @accent; }
QPushButton:disabled { color: @muted; border-color: @line; }
QPushButton[role="primary"] { background: @accent; color: @on_accent; border-color: @accent; font-weight: 600; }
QPushButton[role="primary"]:hover, QPushButton[role="primary"]:focus { background: @accent_hover; border-color: @accent_hover; }
QPushButton[role="ghost"] { background: transparent; border-color: transparent; }
QPushButton[role="ghost"]:hover, QPushButton[role="ghost"]:focus { background: @hover; }
QPushButton[role="mode"] { border-radius: 12px; padding: 0; }
QPushButton[role="mode"]:hover, QPushButton[role="mode"]:focus { border-color: @accent; background: @accent_soft; }
QPushButton[role="choice"] { border-radius: 12px; padding: 14px; }

QLineEdit { background: @surface; color: @text; border: 1px solid @line; border-radius: 10px;
            padding: 10px 14px; selection-background-color: @accent; selection-color: @on_accent; }
QLineEdit:focus { border-color: @accent; }

QFrame#card { background: @surface; border: 1px solid @line; border-radius: 14px; padding: 1px; }
QFrame#card[selected="true"] { border: 2px solid @accent; padding: 0; }
QFrame#panel { background: @surface; border: 1px solid @line; border-radius: 14px; }

QLabel { background: transparent; }
QLabel[role="muted"] { color: @muted; }
QLabel[role="urgent"] { color: @wrong; font-weight: 600; }
QLabel[role="title"] { font-size: 22pt; font-weight: 600; }
QLabel[role="heading"] { font-size: 13pt; font-weight: 600; }
QLabel[role="big"] { font-size: 48pt; font-weight: 600; }
)";
    QMap<QString,QColor> tokens{
        {"@accent_soft", mix(c.surface, c.accent, 0.12)},
        {"@accent_hover", mix(c.accent, c.text, 0.18)},
        {"@hover", mix(c.window, c.text, 0.07)},
        {"@on_accent", c.on_accent},
        {"@accent", c.accent},
        {"@window", c.window},
        {"@surface", c.surface},
        {"@text", c.text},
        {"@muted", c.muted},
        {"@line", c.line},
        {"@wrong", c.wrong},
    };
    //longest tokens first, so that @accent does not eat @accent_soft
    for (const QString &token : {"@accent_soft","@accent_hover","@hover","@on_accent","@accent",
                                 "@window","@surface","@text","@muted","@line","@wrong"})
        qss.replace(token, tokens[token].name());
    return qss;
}

void applyTheme(QString code)
{
    if (!availableThemes().contains(code))
        code = "system";
    current_theme = code;
    QSettings().setValue("theme", code);

    static bool first = true;
    if (first)
    {
        first = false;
        //Fusion looks the same everywhere and follows the palette
        QApplication::setStyle(QStyleFactory::create("Fusion"));
        //"system" follows the OS while the app is running
        QObject::connect(QGuiApplication::styleHints(), &QStyleHints::colorSchemeChanged, qApp, []{
            if (current_theme=="system")
                applyTheme("system");
        });
    }

    colors = isDark() ? dark_colors : light_colors;
    QApplication::setPalette(makePalette(colors));
    qApp->setStyleSheet(makeStyleSheet(colors));
}

void loadFonts()
{
    QFontDatabase::addApplicationFont(":/fonts/ressources/fonts/Onest-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/ressources/fonts/Onest-SemiBold.ttf");
    int id = QFontDatabase::addApplicationFont(":/fonts/ressources/fonts/KleeOne-SemiBold-kana.ttf");
    QStringList families = QFontDatabase::applicationFontFamilies(id);
    if (!families.isEmpty())
        kana_family = families.first();

    QFont font("Onest");
    font.setPointSize(11);
    QApplication::setFont(font);
}

QFont kanaFont(int pixel_size)
{
    QFont font(kana_family);
    font.setPixelSize(qMax(1,pixel_size));
    return font;
}
