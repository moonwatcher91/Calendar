#ifndef MYLABLE_H
#define MYLABLE_H

#include <QtWidgets>

#define application_font  "Arial"

#define normal             20
#define calendar_size_font 75

class MyLabel : public QLabel
{
    Q_OBJECT

public:
    MyLabel(QWidget *parent = 0);
    MyLabel(QPixmap pixmap);
    MyLabel(QString file_path);
    MyLabel(const QString &text, QWidget *parent=0, Qt::WindowFlags f=0);

public slots:
    void setMyPixmap(QString pixmap_path, QString text, QColor color);

protected:
    virtual void mousePressEvent(QMouseEvent *mouse_event);

signals:
    void MousePressed();
    void returnLabel(MyLabel *label);
};

#endif // MYLABLE_H
