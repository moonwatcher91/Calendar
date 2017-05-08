#include "mycalendarwidget.h"

CalendarWidget::CalendarWidget(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent,f)
{
    current_date = QDate::currentDate();
    calendar_grid = new QGridLayout;

    mounth_year = new QLabel();
    QFont font(application_font,normal);
    mounth_year->setFont(font);

    for (qint8 i=1;i<=42;i++)
    {
        MyLabel *label = new MyLabel();
        label->setObjectName(QString::number(i));
        label->setScaledContents(true);
        label->setFixedSize(calendar_fixed_size,calendar_fixed_size);
        connect(label,SIGNAL(returnLabel(MyLabel*)),this,SLOT(onDateClick(MyLabel*)));
        map_widget.insert(label,i);
    }

    connect(this,SIGNAL(returnDatePressed(QDate)),this,SLOT(datePressed(QDate)));

    QVBoxLayout *v_lay = new QVBoxLayout;

    v_lay->addWidget(mounth_year);
    v_lay->addLayout(calendar_grid);

    setLayout(v_lay);

    createCalendar(current_date, -1);
    datePressed(current_date);
}

void CalendarWidget::createCalendar(QDate date, qint8 sender)
{
    from_who=sender;
    map_widget_delta.clear();

    QStringList rule = createRule(date,sender);

    selected_date = date;

    date.setDate(date.year(),date.month(),1);

    qint8 key = 1,
          next_day = 1,
          delta_key = 42,
          today = selected_date.day(),
          day_week = date.dayOfWeek(),
          max_days = date.daysInMonth(),
          max_days_previous = date.addMonths(-1).daysInMonth();

    int list_i = rule.size();

    for (qint8 row=0;row<6;row++)
    {
        for (qint8 col=0;col<7;col++)
        {
            MyLabel *label;
            QColor local_color = calendar_weekdays;

            QString local_pixmap,
                    day;

            if (key>max_days || day_week>1)
            {
                label = map_widget.key(delta_key);

                local_pixmap = calendar_pixmap_5;

                qint8 day_i;

                if (day_week>1)
                {
                    day_i = max_days_previous-day_week+2;
                    map_widget_delta.insert(label,day_i-max_days_previous);
                    day_week--;
                }
                else
                {
                    day_i = next_day;
                    map_widget_delta.insert(label,max_days+day_i);
                    next_day++;
                }

                day = QString::number(day_i);

                delta_key--;
            }
            else
            {
                label = map_widget.key(key);

                day = QString::number(key);

                local_pixmap = calendar_pixmap_4;

                for (int k=0;k<list_i;k++)
                {
                    if (rule.at(k) == day)
                    {
                        local_pixmap = calendar_pixmap_3;
                        rule.removeAt(k);
                        list_i--;
                        break;
                    }
                }

                local_color = weekdaysOrWeekendOrToday(date);

                if (key == today)
                {
                    selected_label = label;
                }

                date = date.addDays(1);

                key++;
            }

            label->setEnabled(true);
            label->setMyPixmap(local_pixmap,day,local_color);

            calendar_grid->addWidget(label,row,col,Qt::AlignHCenter);
        }
    }
}

QStringList CalendarWidget::createRule(QDate date,qint8 sender)
{
    switch(sender)
    {
//    case 1:
//    {
//        QDir dir(QDir::currentPath()+diary_path+"/"+date.toString(path_mounth_year));
//        return dir.entryList(QStringList(), QDir::Files, QDir::Name);
//    }break;

//    case 2:
//    {
//        QDir dir(QDir::currentPath()+note_path+"/"+date.toString(path_mounth_year));
//        return dir.entryList(QStringList(), QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
//    }break;

//    case 3:
//    {
//        QDir dir(QDir::currentPath()+time_work_path+"/"+date.toString(path_year)+"/"+date.toString(path_mounth));
//        return dir.entryList(QStringList(), QDir::Files, QDir::Time);
//    }break;

    default: QStringList();
    }

    return QStringList();
}

QColor CalendarWidget::weekdaysOrWeekendOrToday(QDate date)
{
    if (date == current_date)
    {
        return calendar_today;
    }

    if (date.dayOfWeek()>5)
    {
        return calendar_weekend;
    }

    return calendar_weekdays;
}

void CalendarWidget::fullDate(QDate date, QString pixmap_path)
{
    previous_pixmap_path = calendar_pixmap_3;
    map_widget.key(date.day())->setMyPixmap(pixmap_path,QString::number(date.day()),weekdaysOrWeekendOrToday(date));
}

void CalendarWidget::emptyDate(QDate date, QString pixmap_path)
{
    previous_pixmap_path = calendar_pixmap_4;
    map_widget.key(date.day())->setMyPixmap(pixmap_path,QString::number(date.day()),weekdaysOrWeekendOrToday(date));
}

void CalendarWidget::onDateClick(MyLabel* label)
{
    if (selected_label!=label)
    {
        if (map_widget.value(label)<=selected_date.daysInMonth())
        {
            selected_label->setMyPixmap(previous_pixmap_path,QString::number(selected_date.day()),weekdaysOrWeekendOrToday(selected_date));
            selected_label = label;
            selected_date.setDate(selected_date.year(),selected_date.month(),map_widget.value(label));
            emit returnDatePressed(selected_date);
        }
        else
        {
            selected_date = selected_date.addDays(map_widget_delta.value(label)-map_widget.value(selected_label));
            createCalendar(selected_date, from_who);
            emit returnDatePressed(selected_date);
        }
    }
}

void CalendarWidget::jumpToDay(int direct)
{
    qint8 jump = map_widget.value(selected_label)+direct;

    if (jump>0 && jump<=selected_date.daysInMonth())
    {
        onDateClick(map_widget.key(jump));
    }
    else
    {
        createCalendar(selected_date.addDays(direct), from_who);
        emit returnDatePressed(selected_date);
    }
}

void CalendarWidget::jumpToMounth(int direct)
{
    selected_date = selected_date.addMonths(direct);
    createCalendar(selected_date, from_who);
    emit returnDatePressed(selected_date);
}

void CalendarWidget::jumpToYear(int direct)
{
    selected_date = selected_date.addYears(direct);
    createCalendar(selected_date, from_who);
    emit returnDatePressed(selected_date);
}

void CalendarWidget::jumpToday()
{
    if (selected_date!=current_date)
    {
        createCalendar(current_date, from_who);
        emit returnDatePressed(current_date);
    }
    else
    {
        onDateClick(map_widget.key(current_date.day()));
    }
}

void CalendarWidget::datePressed(QDate date)
{
    //you can create rule and color your date, but now all days are empty
    emptyDate(date, calendar_pixmap_2);
    mounth_year->setText(date.toString(date_view_long));
}

void CalendarWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::NoModifier)
    {
        switch(event->key())
        {
        case Qt::Key_Up:    jumpToDay(-7); break;
        case Qt::Key_Down:  jumpToDay(7); break;
        case Qt::Key_Left:  jumpToDay(-1); break;
        case Qt::Key_Right: jumpToDay(1); break;
        default: QWidget::keyPressEvent(event);
        }
    }
    else
    {
        if (event->modifiers() == Qt::ControlModifier)
        {
            switch(event->key())
            {
            case Qt::Key_Up: jumpToMounth(-1); break;
            case Qt::Key_Left: jumpToMounth(-1); break;
            case Qt::Key_Down: jumpToMounth(1); break;
            case Qt::Key_Right: jumpToMounth(1); break;
            case Qt::Key_T: jumpToday(); break;
            default: QWidget::keyPressEvent(event);
            }
        }
        else
        {
            if (event->modifiers() == (Qt::ShiftModifier))
            {
                switch(event->key())
                {
                case Qt::Key_Up: jumpToYear(-1); break;
                case Qt::Key_Left: jumpToYear(-1); break;
                case Qt::Key_Down: jumpToYear(1); break;
                case Qt::Key_Right: jumpToYear(1); break;
                default: QWidget::keyPressEvent(event);
                }
            }
            else
            {
                if (event->modifiers() == (Qt::AltModifier))
                {
                    switch(event->key())
                    {
                    case Qt::Key_Up: jumpToYear(-5); break;
                    case Qt::Key_Left: jumpToYear(-5); break;
                    case Qt::Key_Down: jumpToYear(5); break;
                    case Qt::Key_Right: jumpToYear(5); break;
                    default: QWidget::keyPressEvent(event);
                    }
                }
                else
                {
                    if (event->modifiers() == (Qt::ControlModifier | Qt::AltModifier))
                    {
                        switch(event->key())
                        {
                        case Qt::Key_Up: jumpToYear(-10); break;
                        case Qt::Key_Left: jumpToYear(-10); break;
                        case Qt::Key_Down: jumpToYear(10); break;
                        case Qt::Key_Right: jumpToYear(10); break;
                        default: QWidget::keyPressEvent(event);
                        }
                    }
                    else
                    {
                        if (event->modifiers() == (Qt::ShiftModifier | Qt::AltModifier))
                        {
                            switch(event->key())
                            {
                            case Qt::Key_Up: jumpToYear(-100); break;
                            case Qt::Key_Left: jumpToYear(-100); break;
                            case Qt::Key_Down: jumpToYear(100); break;
                            case Qt::Key_Right: jumpToYear(100); break;
                            default: QWidget::keyPressEvent(event);
                            }
                        }
                        else
                        {
                            QWidget::keyPressEvent(event);
                        }
                    }
                }
            }
        }
    }
}
