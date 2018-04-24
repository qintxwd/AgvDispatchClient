#include "widgetwaiting.h"
#include <QtWidgets>
WidgetWaiting::WidgetWaiting(QWidget *parent) : QWidget(parent)
{
    QLabel *l = new QLabel("loading please wait...");
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(l);
    setLayout(hlayout);
}
