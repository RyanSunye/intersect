#include "widget.h"
#include "ui_widget.h"
#include <QPaintEvent>
#include <QtGui>
#include <fstream>
#include <iostream>
#include <sstream>

//void add_diagram(char T, int x1, int y1, int x2, int y2);
//void sub_diagram(char T, int a, int b, int c, int d);

//void lineIntersectLine_ui(const Line& l1, const Line& l2);
//void lineIntersectCircle_ui(const Line& L, const Circle& C);
//void circleIntersectCircle_ui(const Circle& c1, const Circle& c2);
//void calPoints();

//set<pair<double, double>> uiPoints;
//vector<Line> lineVector;
//vector<Circle> circleVector;
Dll dll;

void handle_error(const string &msg) {
    cout << msg;
    exit(1);
}

string DoubleToString(double value, unsigned int precision=6) {
    ostringstream out;
    if (precision > 0) out.precision(precision);
    out << value;
    return out.str();
}


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi((QWidget *)this);
    margin_size = 35;

    paint_org = QPointF(margin_size + 20, margin_size - 20);
    paint_size_first = QSize(this->width() - margin_size * 2, this->height() - margin_size * 2);
    paint_size_old = paint_size_first;
    paint_size_new = paint_size_first;
    paint_center_old = QPointF(paint_size_first.width() / 2, paint_size_first.height() / 2);
    paint_center_new = paint_center_old;

    offset_x = 0;
    offset_y = paint_size_new.height();

    rect_center = QPointF(paint_size_new.width() / 2, paint_size_new.height() / 2);
    rect_topl = QPointF(rect_center.rx() - (paint_size_new.height() / 2 - 15), rect_center.ry() + (paint_size_new.height() / 2 - 15));
    rect_bottomr = QPointF(rect_center.rx() + (paint_size_new.height() / 2 - 15), rect_center.ry() - (paint_size_new.height() / 2 - 15));

    scale_value = 1.05;

    mousepress_org = QPointF(0, 0);

    axis_x_old = QPointF(rect_center.rx(), 0);
    axis_y_old = QPointF(0, rect_center.ry());

    axis_scale = 20.0;

    offsetv_x = rect_center.rx();
    offsetv_y = rect_bottomr.ry();
    pixel_per_mm = (double)(rect_bottomr.rx() - rect_topl.rx()) / 600;

    mouse_current_pos = QPointF(rect_center.rx(), rect_center.ry());
    this->setMouseTracking(true);

    this->resize(1024, 768);

    setAcceptDrops(true);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::drawLine(double x1, double y1, double x2, double y2, QPainter* painter) {
//    cout<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<endl;
    painter->drawLine(ValuePtoObjectP(QPointF(x1, y1)), ValuePtoObjectP(QPointF(x2, y2)));
}

void Widget::drawCircle(double x, double y,double r, QPainter* painter){
//    cout<<x<<" "<<y<<" "<<r<<endl;
    painter->drawEllipse(ValuePtoObjectP(QPointF(x, y)), r * pixel_per_mm, r * pixel_per_mm);
}

void Widget::drawPoint(double x, double y, QPainter* painter){
//    cout<<x<<" "<<y<<endl;
    painter->drawPoint(ValuePtoObjectP(QPointF(x,y)));
    painter->drawEllipse(ValuePtoObjectP(QPointF(x, y)), 3, 3);
//    painter->drawEllipse(ValuePtoObjectP(QPointF(x, y)), 5, 5);
}

void Widget::paintEvent(QPaintEvent *event)
{
    paint_size_new = QSize(this->width() - margin_size * 2, this->height() - margin_size * 2);

    QImage image = QImage(paint_size_new, QImage::Format_RGB32);
    QColor backColor = qRgb(255, 255, 255);
    image.fill(backColor);

    if (paint_size_new != paint_size_old)
    {
        offset_x = 0;
        offset_y = paint_size_new.height();

        paint_center_new = QPointF(paint_size_new.width() / 2, paint_size_new.height() / 2);
        rect_center = paint_center_new - paint_center_old + rect_center;
        rect_topl = paint_center_new - paint_center_old + rect_topl;
        rect_bottomr = paint_center_new - paint_center_old + rect_bottomr;

        paint_size_old = paint_size_new;
        paint_center_old = paint_center_new;
    }

    offsetv_x = rect_center.rx();
    offsetv_y = rect_bottomr.ry();
    pixel_per_mm = (double)(rect_bottomr.rx() - rect_topl.rx()) / 600;

    QPainter painter2(&image);
    QRectF rec(DisplayPtoObjectP(rect_topl), DisplayPtoObjectP(rect_bottomr));

//    cout<<"repaint! "<<circles.size()<<" "<<lines.size()<<endl;
    for(auto &it:dll.circleVector) {
        drawCircle(it.c.x, it.c.y, it.r, &painter2);
    }
    for (auto &it:dll.lineVector) {
        Point pp(it.p.x+it.v.x, it.p.y+it.v.y);
        drawLine(it.p.x, it.p.y, pp.x, pp.y, &painter2);
    }
    for (auto &it:dll.uiPoints) {
        drawPoint(it.first, it.second, &painter2);
    }
    ui->textBrowser->clear();
    ui->textBrowser->append(QString::number(dll.uiPoints.size()));

    // DrawLine
//    painter2.drawLine(ValuePtoObjectP(QPointF(0, 0)), ValuePtoObjectP(QPointF(100, 100)));
//    painter2.drawEllipse(ValuePtoObjectP(QPointF(0, 0)), 100 * pixel_per_mm, 100 * pixel_per_mm);
//    painter2.drawLine(DisplayPtoObjectP(rect_topl),DisplayPtoObjectP(rect_bottomr));
//    painter2.drawText(DisplayPtoObjectP(rect_center), "(" + QString::number((mouse_current_pos).rx(), 'f', 1) + ", " + QString::number((mouse_current_pos).ry(), 'f', 1) + ")");
//    painter2.drawRect(rec);
//    painter2.drawPoint(DisplayPtoObjectP(rect_center));
    painter2.drawText(QPointF(mouse_current_pos.rx(), mouse_current_pos.ry()), "(" + QString::number(ObjectPtoValueP(mouse_current_pos).rx(), 'f', 1) + ", " + QString::number(ObjectPtoValueP(mouse_current_pos).ry(), 'f', 1) + ")");
//    painter2.drawPoint(ValuePtoObjectP(QPointF(-150.0, 150.0)));            //(-150, 150)

    QPainter painter(this);
     //painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawLine(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(0, 0))), ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(image.width(), 0))));
    painter.drawLine(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(-0.01, 0))), ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(-0.01, image.height()))));

    painter.drawLine(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(0, 0))), ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(0, -10))));
    painter.drawLine(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(0, 0))), ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(-10, 0))));
    painter.drawLine(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(image.width(), 0))), ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(image.width(), -10))));
    painter.drawLine(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(0, image.height()))), ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(-10, image.height()))));
    painter.drawText(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(0, -20))), QString::number((double)(0 - offsetv_x) / pixel_per_mm, 'f', 1));
    painter.drawText(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(-30, 0))), QString::number((double)(0 - offsetv_y) / pixel_per_mm, 'f', 1));
    painter.drawText(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(image.width(), -20))), QString::number((double)(image.width() - offsetv_x) / pixel_per_mm, 'f', 1));
    painter.drawText(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(-30, image.height()))), QString::number((double)(image.height() - offsetv_y) / pixel_per_mm, 'f', 1));
    painter.drawText(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(image.width() / 2, -35))), QString(tr("X")));

    painter.rotate(-90);
    painter.drawText(QPointF(- (image.height() / 2 + 20), 15), QString(tr("Y")));
    painter.rotate(90);

    if (rect_center.rx() > 0 && rect_center.rx() < image.width())
    {
        painter.drawLine(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(rect_center.rx(), 0))), ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(rect_center.rx(), -10))));
        painter.drawText(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(rect_center.rx(), -20))), QString::number((double)(rect_center.rx() - offsetv_x) / pixel_per_mm, 'f', 1));
    }

    if (rect_center.ry() > 0 && rect_center.ry() < image.height())
    {
        painter.drawLine(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(0, rect_center.ry()))), ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(-10, rect_center.ry()))));
        painter.drawText(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(-30, rect_center.ry()))), QString::number((double)(rect_center.ry() - offsetv_y) / pixel_per_mm, 'f', 1));
    }

    if (axis_scale >= 35.0)
    {
        axis_scale = axis_scale / 2;
    }
    else if (axis_scale <= 15.0)
    {
        axis_scale = axis_scale * 2;
    }

    axis_x_old = QPointF(rect_center.rx(),0);
    int i = 0;
    while (axis_x_old.rx() > axis_scale)
    {
        axis_x_old.rx() = axis_x_old.rx() - axis_scale;
        i++;
        if (axis_x_old.rx() > 0 && axis_x_old.rx() < image.width())
        {
            if (i % 5 == 0)
            {
                painter.drawLine(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(axis_x_old.rx(), 0))), ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(axis_x_old.rx(), -10))));
                painter.drawText(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(axis_x_old.rx(), -20))), QString::number((double)(axis_x_old.rx() - offsetv_x) / pixel_per_mm, 'f', 1));
            }
            else
            {
                painter.drawLine(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(axis_x_old.rx(), 0))), ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(axis_x_old.rx(), -5))));
            }
        }
    }
    axis_x_old = QPointF(rect_center.rx(), 0);
    i = 0;
    while ((image.width() - axis_x_old.rx()) > axis_scale)
    {
        axis_x_old.rx() = axis_x_old.rx() + axis_scale;
        i++;
        if (axis_x_old.rx() > 0 && axis_x_old.rx() < image.width())
        {
            if (i % 5 == 0)
            {
                painter.drawLine(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(axis_x_old.rx(), 0))), ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(axis_x_old.rx(), -10))));
                painter.drawText(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(axis_x_old.rx(), -20))), QString::number((double)(axis_x_old.rx() - offsetv_x) / pixel_per_mm, 'f', 1));
            }
            else
            {
                painter.drawLine(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(axis_x_old.rx(), 0))), ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(axis_x_old.rx(), -5))));
            }
        }
    }

    axis_y_old = QPointF(0, rect_center.ry());
    i = 0;
    while (axis_y_old.ry() > axis_scale)
    {
        axis_y_old.ry() = axis_y_old.ry() - axis_scale;
        i++;
        if (axis_y_old.ry() > 0 && axis_y_old.ry() < image.height())
        {
            if (i % 5 == 0)
            {
                painter.drawLine(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(0, axis_y_old.ry()))), ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(-10, axis_y_old.ry()))));
                painter.drawText(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(-30, axis_y_old.ry()))), QString::number((double)(axis_y_old.ry() - offsetv_y) / pixel_per_mm, 'f', 1));
            }
            else
            {
                painter.drawLine(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(0, axis_y_old.ry()))), ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(-5, axis_y_old.ry()))));
            }
        }
    }
    axis_y_old = QPointF(0, rect_center.ry());
    i = 0;
    while ((image.height() - axis_y_old.ry()) > axis_scale)
    {
        axis_y_old.ry() = axis_y_old.ry() + axis_scale;
        i++;
        if (axis_y_old.ry() > 0 && axis_y_old.ry() < image.height())
        {
            if (i % 5 == 0)
            {
                painter.drawLine(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(0, axis_y_old.ry()))), ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(-10, axis_y_old.ry()))));
                painter.drawText(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(-30, axis_y_old.ry()))), QString::number((double)(axis_y_old.ry() - offsetv_y) / pixel_per_mm, 'f', 1));
            }
            else
            {
                painter.drawLine(ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(0, axis_y_old.ry()))), ObjectPtoWidgetP(DisplayPtoObjectP(QPointF(-5, axis_y_old.ry()))));
            }
        }
    }

    painter.drawImage(paint_org, image);
}



void Widget::wheelEvent(QWheelEvent *event)
{
     QPointF mousepos = WidgetPtoObjectP(event->pos());
     if (event->delta() > 0)
     {
        rect_center = scaleIn(rect_center, ObjectPtoDisplayP(mousepos), scale_value);
        rect_topl = scaleIn(rect_topl, ObjectPtoDisplayP(mousepos), scale_value);
        rect_bottomr = scaleIn(rect_bottomr, ObjectPtoDisplayP(mousepos), scale_value);
        axis_scale = axis_scale * scale_value;
     }
     else
     {
        rect_center = scaleOut(rect_center, ObjectPtoDisplayP(mousepos), scale_value);
        rect_topl = scaleOut(rect_topl, ObjectPtoDisplayP(mousepos), scale_value);
        rect_bottomr = scaleOut(rect_bottomr, ObjectPtoDisplayP(mousepos), scale_value);
        axis_scale = axis_scale / scale_value;
      }

       repaint();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    mouse_current_pos = WidgetPtoObjectP(event->pos());

    if (event->buttons() & Qt::LeftButton)
    {
        QPointF mousepos_move = ObjectPtoDisplayP(WidgetPtoObjectP(event->pos()));

        rect_center = mousepos_move - mousepress_org + rect_center;
        rect_topl = mousepos_move - mousepress_org + rect_topl;
        rect_bottomr = mousepos_move - mousepress_org + rect_bottomr;

        mousepress_org = mousepos_move;
    }

    repaint();
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mousepress_org = ObjectPtoDisplayP(WidgetPtoObjectP(event->pos()));
    }
}

QPointF Widget::ObjectPtoDisplayP(QPointF objPoint)
{
    return QPointF(objPoint.rx() - offset_x, -objPoint.ry() + offset_y);
}

QPointF Widget::DisplayPtoObjectP(QPointF disPoint)
{
    return QPointF(disPoint.rx() + offset_x, -disPoint.ry() + offset_y);
}

QPointF Widget::scaleIn(QPointF pos_before, QPointF scale_center, double scale_value)
{
    QPointF temp;
    temp.rx() = (double)(pos_before.rx() - scale_center.rx()) * scale_value + scale_center.rx();
    temp.ry() = (double)(pos_before.ry() - scale_center.ry()) * scale_value + scale_center.ry();
    return temp;
}

QPointF Widget::scaleOut(QPointF pos_before, QPointF scale_center, double scale_value)
{
    QPointF temp;
    temp.rx() = (double)(pos_before.rx() - scale_center.rx()) / scale_value + scale_center.rx();
    temp.ry() = (double)(pos_before.ry() - scale_center.ry()) / scale_value + scale_center.ry();
    return temp;
}

QPointF Widget::WidgetPtoObjectP(QPointF wigPoint)
{
    return QPointF(wigPoint - paint_org);
}

QPointF Widget::ObjectPtoWidgetP(QPointF objPoint)
{
    return QPointF(objPoint + paint_org);
}

QPointF Widget::ValuePtoObjectP(QPointF valPoint)
{
    return DisplayPtoObjectP(QPointF(valPoint.rx() * pixel_per_mm + offsetv_x, valPoint.ry() * pixel_per_mm + offsetv_y));
}

QPointF Widget::ObjectPtoValueP(QPointF objPoint)
{
    return QPointF((double)(ObjectPtoDisplayP(objPoint).rx() - offsetv_x) / pixel_per_mm, (double)(ObjectPtoDisplayP(objPoint).ry() - offsetv_y) / pixel_per_mm);
}



bool Widget::IsValidDragFile(QDropEvent *e)
{
    QString text = e->mimeData()->text();
    bool bre = false;
    do
    {
        // 存在多个文件时，只判断第一个
        QStringList files = text.split("\n");
        if(files.size() < 1)
        {
            break;
        }

        int pos = files[0].indexOf("file:///");
        if(0 != pos)
        {
            break;
        }

        //去掉拖拽头标 file:///
        QString filename = files[0].mid(8);

        // 接受目录
        QDir dir(filename);
        if (dir.exists())
        {
            bre = true;
            break;
        }

        if(QFile::exists(filename))
        {
            bre = true;
            break;
        }

    }while(false);

    return bre;
}

void Widget::AcceptFolder(QString folder)
{
    QDir dir(folder);

    //文件
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::NoSymLinks);

    for(int i = 0; i < file_list.size(); i++)
    {
        QString fileName = file_list[i].filePath();
        AcceptFile(fileName);
    }

    //目录
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for(int i = 0; i < folder_list.size(); i++)
    {
        AcceptFolder(folder_list[i].absoluteFilePath());
    }
}

void Widget::AcceptFile(QString pathfile)
{
    ifstream file;
    cout<<"reading " << pathfile.toStdString()<<endl;
    file.open(pathfile.toStdString());

    if(!file) handle_error("cannot open file: "+pathfile.toStdString());
    char s;
    int num, x0, y0, x1, y1;

    try{
        file>>num;
    } catch(exception()) {
        handle_error("why not input a N?");
    }

    for (int i = 0; i < num; i++) {
        if (file >> s) {
            if (s == 'L' || s == 'R' || s == 'S') {
                if (file >> x0 >> y0 >> x1 >> y1) dll.add_diagram(s, x0, y0, x1, y1);
            } else if (s == 'C') {
                if (file >> x0 >> y0 >> x1) dll.add_diagram(s, x0, y0, x1, 0);
            } else {
                handle_error("line " + DoubleToString(i + 1) + " format error");
            }
            dll.calPoints();
        } else {
                handle_error("need more lines");
        }
    }
}

void Widget::dragEnterEvent(QDragEnterEvent *event)
{
    if(IsValidDragFile(event))
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void Widget::dragMoveEvent(QDragMoveEvent *event)
{
    if(IsValidDragFile(event))
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void Widget::dropEvent(QDropEvent *event)
{
    QStringList files = event->mimeData()->text().split("\n");
    for(int i = 0; i < files.size(); i++)
    {
        QString fileName = files[i].mid(8); //  去掉头file:///

        // 接受目录
        QDir dir(fileName);
        if (dir.exists())
        {
            AcceptFolder(fileName);
            continue;
        }

        if(QFile::exists(fileName))
        {
            AcceptFile(fileName);
            continue;
        }
    }
}

void Widget::on_add_diagram_clicked()
{
    stringstream streambuf(ui->input->text().toStdString());
    char s;
    int x0, y0, x1, y1;
    if (streambuf >> s) {
        if (s == 'L' || s == 'R' || s == 'S') {
            if (streambuf >> x0 >> y0 >> x1 >> y1) {
                dll.add_diagram(s, x0, y0, x1, y1);
                dll.calPoints();
                return;
            }
        } else if (s == 'C') {
            if (streambuf >> x0 >> y0 >> x1) {
                dll.add_diagram(s, x0, y0, x1, 0);
                dll.calPoints();
                return;
            }
        }
    }
    handle_error("input format error");
}

void Widget::on_sub_diagram_clicked()
{
    stringstream streambuf(ui->input->text().toStdString());
    char s;
    int x0, y0, x1, y1;
    if (streambuf >> s) {
        if (s == 'L' || s == 'R' || s == 'S') {
            if (streambuf >> x0 >> y0 >> x1 >> y1) {
                try{
                    dll.sub_diagram(s, x0, y0, x1, y1);
                    dll.calPoints();
                } catch(exception()) {
                    handle_error("no such diagram!");
                }

                return;
            }
        } else if (s == 'C') {
            if (streambuf >> x0 >> y0 >> x1) {
                try{
                    dll.sub_diagram(s, x0, y0, x1, 0);
                    dll.calPoints();
                } catch(exception()) {
                    handle_error("no such diagram!");
                }
                return;
            }
        }
    }
    handle_error("input format error");
}
