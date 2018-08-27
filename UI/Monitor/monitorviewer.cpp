
#include "monitorviewer.h"

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
#include "monitorscene.h"
#include "global.h"

MonitorViewer::MonitorViewer(QWidget *parent) :  QGraphicsView(parent)
{
    setRenderHint(QPainter::Antialiasing, false);
    setDragMode(QGraphicsView::RubberBandDrag);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMouseTracking(true);
    setFrameStyle(QFrame::NoFrame);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setInteractive(false);
}

void MonitorViewer::toggleSelectMode()
{
    setDragMode( QGraphicsView::RubberBandDrag);
    setInteractive(true);
}

void MonitorViewer::toggleDragMode()
{
    setDragMode(QGraphicsView::ScrollHandDrag);
    setInteractive(false);
}

void MonitorViewer::wheelEvent(QWheelEvent *event)
{
    static int kk = 0;
    double scaleFactor = 1.10;
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->delta() > 0)
        {
            if(kk<20){
                ++kk;
                scale(scaleFactor, scaleFactor);
            }
        }
        else
        {
            if(kk>-20){
                --kk;
                scale(1.0 / scaleFactor, 1.0 / scaleFactor);
            }
        }
        viewport()->update();
        event->accept();
    } else {
        QGraphicsView::wheelEvent(event);
    }
}
