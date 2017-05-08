#ifndef MYCALENDARWIDGET_H
#define MYCALENDARWIDGET_H

#include <QtWidgets>

#include "mylabel.h"

const QColor calendar_today    = Qt::red;
const QColor calendar_weekdays = Qt::black;
const QColor calendar_weekend  = Qt::darkGreen;

#define calendar_pixmap_1 ":/png/mark_1_1.png"
#define calendar_pixmap_2 ":/png/mark_1_2.png"
#define calendar_pixmap_3 ":/png/mark_1_3.png"
#define calendar_pixmap_4 ":/png/mark_1_4.png"
#define calendar_pixmap_5 ":/png/mark_1_5.png"

#define calendar_fixed_size 75

#define date_view_long "yyyy MMMM"

class CalendarWidget : public QWidget
{
    Q_OBJECT

public:
    CalendarWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

public:
    QColor weekdaysOrWeekendOrToday(QDate date);
    void createCalendar(QDate date, qint8 sender);
    QStringList createRule(QDate date, qint8 sender);

public:
    QGridLayout     *calendar_grid;
    QMap<MyLabel*,qint8> map_widget,
                         map_widget_delta;
    QDate current_date,
          selected_date;
    MyLabel *selected_label;
    QLabel *mounth_year;
    qint8 from_who;
    QString previous_pixmap_path;

public slots:
    void jumpToday();
    void jumpToDay(int direct);
    void jumpToYear(int direct);
    void datePressed(QDate date);
    void jumpToMounth(int direct);
    void onDateClick(MyLabel *label);
    void fullDate(QDate date, QString pixmap_path);
    void emptyDate(QDate date, QString pixmap_path);

protected:
    void keyPressEvent(QKeyEvent *event);

signals:
    void returnDatePressed(QDate date);
};

#endif // MYCALENDARWIDGET_H
