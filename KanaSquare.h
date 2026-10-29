#ifndef KANASQUARE_H
#define KANASQUARE_H
#include <QWidget>

//a kana drawn in a square of a handwriting workbook, with its dashed center guides
class KanaSquare : public QWidget
{
    Q_OBJECT
public:
    explicit KanaSquare(QWidget *parent=nullptr);
    void setText(const QString &text);
    QString text();
    //the square fits in the widget, the kana fills about 2/3 of it
    QSize sizeHint() const;
    QSize minimumSizeHint() const;
protected:
    void paintEvent(QPaintEvent *event);
private:
    QString kana;
};

#endif // KANASQUARE_H
