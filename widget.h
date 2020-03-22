#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<qpainter.h>
#include "dll.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    QPointF ObjectPtoDisplayP(QPointF objPoint);
    QPointF DisplayPtoObjectP(QPointF disPoint);
    QPointF WidgetPtoObjectP(QPointF wigPoint);
    QPointF ObjectPtoWidgetP(QPointF objPoint);
    QPointF ValuePtoObjectP(QPointF valPoint);
    QPointF ObjectPtoValueP(QPointF objPoint);
    QPointF scaleIn(QPointF pos_before, QPointF scale_center, double scale_value);
    QPointF scaleOut(QPointF pos_before, QPointF scale_center, double scale_value);

    void drawLine(double x1, double y1, double x2, double y2, QPainter* painter);
    void drawCircle(double x, double y, double r,QPainter* painter);
    void drawPoint(double x, double y, QPainter* painter);

protected:
    void paintEvent(QPaintEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    ///判断是否为有效的文件
    virtual bool IsValidDragFile(QDropEvent *e);

    ///接受目录
    /// @note 遍例目录，调用AcceptFile
    virtual void AcceptFolder(QString folder);

    ///接受文件
    virtual void AcceptFile(QString pathfile);

private slots:
    void on_add_diagram_clicked();

    void on_sub_diagram_clicked();

private:
    Ui::Widget *ui;

    int margin_size;

    QPointF paint_org;              //widget坐标系
    QSize paint_size_first;
    QSize paint_size_old;
    QSize paint_size_new;
    QPointF paint_center_old;       //display坐标系
    QPointF paint_center_new;       //display坐标系

    double offset_x;
    double offset_y;

    QPointF rect_center;           //display坐标系
    QPointF rect_topl;             //display坐标系
    QPointF rect_bottomr;          //display坐标系

    double scale_value;

    QPointF mousepress_org;        //display坐标系

    QPointF axis_x_old;            //display坐标系
    QPointF axis_y_old;            //display坐标系
    double axis_scale;

    double offsetv_x;
    double offsetv_y;
    double pixel_per_mm;

    QPointF mouse_current_pos;      //object坐标系

    virtual void dragEnterEvent(QDragEnterEvent *e) override;
    virtual void dragMoveEvent(QDragMoveEvent *e) override;
    virtual void dropEvent(QDropEvent *e) override;
};

#endif // WIDGET_H
