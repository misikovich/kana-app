#ifndef THEME_H
#define THEME_H
#include <QString>
#include <QStringList>
#include <QColor>
#include <QFont>

//colors of the current theme, for the widgets that paint themselves
struct ThemeColors
{
    QColor window;    //page background
    QColor surface;   //cards, buttons, inputs
    QColor text;
    QColor muted;     //secondary text
    QColor line;      //borders
    QColor guide;     //dashed guides of the kana squares
    QColor accent;
    QColor on_accent; //text on the accent color
    QColor correct;
    QColor wrong;
};

//"system" follows the light/dark setting of the OS
QStringList availableThemes();
QString initialTheme();
//sets the style, palette and style sheet of the application, saves the choice
void applyTheme(QString code);
QString currentTheme();
const ThemeColors& themeColors();

//loads the bundled fonts, to call once before creating the widgets
void loadFonts();
//font of the kana ( Klee One, a schoolbook handwriting style ), size in pixels
QFont kanaFont(int pixel_size);

#endif // THEME_H
