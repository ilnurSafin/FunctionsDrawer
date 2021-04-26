#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <QPainter>
#include <QPixmap>
#include <QLabel>
#include "./Function/function.h"
class Graphic
{
public:
    Graphic(unsigned size, unsigned scale, QPen axis_pen, QPen grid_pen, QPen function_pen);
    ~Graphic();

    void clearBuffer();

    bool setSize(unsigned size);
    bool setScale(unsigned scale);

    void setGridIsDrawable(bool grid_drawable);
    bool isGridDrawable()const;

    void setPenOfAxis(const QPen & pen_of_axis);
    void setPenOfGrid(const QPen &pen_of_grid);
    void setPenOfPoints(const QPen &pen_of_points);

    //raw drawing
    void drawPoint(int x, int y);

    //drawing a function
    void setYFunction(Function fy);
    void setXFunction(Function fx);
    bool setRangeOfParameter(double begin, double end);
    bool setStepOfParameter(double step);
    void drawFunction();

    void bufferToLabel(QLabel* label);
private:
    QImage _buffer;
    unsigned _size;
    unsigned _scale;
    QPen _axis_pen;
    QPen _grid_pen;
    QPen _function_pen;
    bool _grid_drawable;
    Function _fy;
    Function _fx;
    double _begin_value_of_parameter;
    double _end_value_of_parameter;
    double _step_of_parameter;

};

#endif // GRAPHIC_H
