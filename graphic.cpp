#include <iostream>
#include "graphic.h"

Graphic::Graphic(unsigned size, unsigned scale,QPen axis_pen,QPen grid_pen,QPen function_pen)
    : _buffer(),_fx("x"),_fy("0")
{
    setSize(size);
    setScale(scale);
    setPenOfAxis(axis_pen);
    setPenOfGrid(grid_pen);
    setPenOfPoints(function_pen);
}

Graphic::~Graphic()
{}


bool Graphic::setSize(unsigned size)
{
   if(size<10)
        return false;
    else
   {
        _size=size;
        if(_size%2==0)
            ++_size;
        return true;
   }
}

bool Graphic::setScale(unsigned scale)
{
    if(scale==0 /*|| scale*_size>2000*/)
        return false;
    else
    {
        _scale=scale;
        return true;
    }
}

void Graphic::setGridIsDrawable(bool grid_drawable)
{
    _grid_drawable=grid_drawable;
}

bool Graphic::isGridDrawable()const
{
    return _grid_drawable;
}

void Graphic::setPenOfAxis(const QPen &pen_of_axis)
{
    _axis_pen = pen_of_axis;
}

void Graphic::setPenOfGrid(const QPen &pen_of_grid)
{
    _grid_pen = pen_of_grid;
}

void Graphic::setPenOfPoints(const QPen &pen_of_points)
{
    _function_pen = pen_of_points;
}


void Graphic::setYFunction(Function fy)
{
    _fy = fy;
}

void Graphic::setXFunction(Function fx)
{
    _fx = fx;
}

bool Graphic::setRangeOfParameter(double begin, double end)
{
    if(begin > end)
        return false;
    else
    {
        _begin_value_of_parameter = begin;
        _end_value_of_parameter = end;
        return true;
    }
}

bool Graphic::setStepOfParameter(double step)
{
    if( step == 0)
        return false;
    else
    {
        _step_of_parameter = step;
        return true;
    }
}


void Graphic::clearBuffer()
{
    unsigned size_of_side = _size*_scale;
    _buffer = QImage(size_of_side,size_of_side,QImage::Format_RGB32);
    _buffer.fill(Qt::white);

    QPainter p(&_buffer);
    if(_grid_drawable)
    {
        p.setPen(_grid_pen);
        for(unsigned i=0; i<=_size; ++i)
        {
            //verical lines
            p.drawLine(i*_scale+_scale/2.f,0,
                       i*_scale+_scale/2.f,size_of_side);
            //horizontal lines
            p.drawLine(0,i*_scale+_scale/2.f,
                       size_of_side,i*_scale+_scale/2.f);
        }
    }

    p.setPen(_axis_pen);
    //y axis
    p.drawLine(size_of_side/2+1,0,
               size_of_side/2+1,size_of_side);
    //x axis
    p.drawLine(0,size_of_side/2+1,
               size_of_side,size_of_side/2+1);
}

void Graphic::drawPoint(int x, int y)
{
    QPainter p(&_buffer);
    p.setPen(_function_pen);
    int half_of_side = _buffer.height()/2;
    if(y<half_of_side && y>-half_of_side && x<half_of_side && x>-half_of_side)
        p.drawPoint(x,y);
}
void Graphic::drawFunction()
{
    QPainter p(&_buffer);
    p.setPen(_function_pen);

    int half_of_side = _buffer.height()/2;
    QPoint last_point(_fx(_begin_value_of_parameter)*_scale+half_of_side,-_fy(_begin_value_of_parameter)*_scale+half_of_side);
    QPoint current_point;
    for(double parameter=_begin_value_of_parameter+_step_of_parameter; parameter <= _end_value_of_parameter; parameter+=_step_of_parameter, last_point=current_point)
    {
        current_point = QPoint(_fx(parameter)*_scale+half_of_side,-_fy(parameter)*_scale+half_of_side);

        //if(y<half_of_side && y>-half_of_side && x<half_of_side && x>-half_of_side)
            //p.drawPoint(x+half_of_side,-y+half_of_side);
            p.drawLine(last_point,current_point);
    }
}

void Graphic::bufferToLabel(QLabel* label)
{
    label->setPixmap(QPixmap::fromImage(_buffer));
}

