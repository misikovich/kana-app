#include "KanaSquare.h"
#include <QPainter>
#include "Theme.h"

KanaSquare::KanaSquare(QWidget *parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

void KanaSquare::setText(const QString &text)
{
    kana=text;
    update();
}

QString KanaSquare::text()
{
    return kana;
}

QSize KanaSquare::sizeHint() const
{
    return QSize(200,200);
}

QSize KanaSquare::minimumSizeHint() const
{
    return QSize(40,40);
}

void KanaSquare::paintEvent(QPaintEvent *)
{
    const ThemeColors &c = themeColors();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int side = qMin(width(),height())-2;
    QRectF square((width()-side)/2.0,(height()-side)/2.0,side,side);
    //thin lines on small squares, a bit thicker on the big one
    qreal pen_width = side>160 ? 1.5 : 1.0;

    //dashed center guides, like in a workbook
    QPen guide_pen(c.guide,pen_width,Qt::CustomDashLine);
    guide_pen.setDashPattern({4,4});
    painter.setPen(guide_pen);
    painter.drawLine(QPointF(square.center().x(),square.top()),QPointF(square.center().x(),square.bottom()));
    painter.drawLine(QPointF(square.left(),square.center().y()),QPointF(square.right(),square.center().y()));

    painter.setPen(QPen(c.guide,pen_width));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(square.adjusted(pen_width/2,pen_width/2,-pen_width/2,-pen_width/2),3,3);

    painter.setPen(c.text);
    painter.setFont(kanaFont(qRound(side*0.68)));
    painter.drawText(square,Qt::AlignCenter,kana);
}
