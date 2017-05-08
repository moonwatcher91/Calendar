#include "mylabel.h"

MyLabel::MyLabel(QWidget *parent)
    : QLabel(parent)
{
}

MyLabel::MyLabel(QPixmap pixmap)
{
    MyLabel();
    setPixmap(pixmap);
}

MyLabel::MyLabel(QString file_path)
{
    MyLabel(QPixmap(file_path));
}

MyLabel::MyLabel(const QString &text, QWidget *parent, Qt::WindowFlags f) :
    QLabel(text, parent, f)
{
    MyLabel();
    setText(text);
}

void MyLabel::setMyPixmap(QString pixmap_path, QString text, QColor color)
{
    QFont font(application_font,calendar_size_font);
    QPixmap pixmap(pixmap_path);
    QPainter painter(&pixmap);
    painter.setFont(font);
    painter.setPen(color);
    painter.drawText(QRect(0,0,pixmap.width(),pixmap.height()), Qt::AlignCenter, text);
    setPixmap(pixmap);
}

void MyLabel::mousePressEvent(QMouseEvent *mouse_event)
{
    if (mouse_event->button() == Qt::LeftButton)
    {
        emit MousePressed();
        emit returnLabel(this);
    }
}
