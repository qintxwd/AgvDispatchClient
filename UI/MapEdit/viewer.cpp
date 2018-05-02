#include "viewer.h"

#include <QWheelEvent>
#include <QDebug>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QCoreApplication>
#include <QColorDialog>
#include "stationinputdialog.h"
#include "scene.h"
#include "global.h"

Viewer::Viewer(QWidget *parent) :  QGraphicsView(parent)
{
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setMouseTracking(true);	// mouse track on
    setViewportUpdateMode(FullViewportUpdate);
}

void Viewer::wheelEvent(QWheelEvent *event)
{
    static int kk = 0;
    double scaleFactor = 1.10;
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->delta() > 0)
        {
            if(kk<10){
                ++kk;
                scale(scaleFactor, scaleFactor);
            }
        }
        else
        {
            if(kk>-10){
                --kk;
                scale(1.0 / scaleFactor, 1.0 / scaleFactor);
            }
        }
        event->accept();
    } else {
        QGraphicsView::wheelEvent(event);
    }
}
