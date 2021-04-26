#include <cmath>
#include <iostream>
#include <QtCore>
#include <QDebug>
#include <QPainter>
#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    fy("0","x"),fx("x","x"),
    ui(new Ui::Dialog),length(50),function_graphic(101,10,QPen(Qt::darkGray,2),QPen(Qt::gray),QPen(Qt::darkMagenta,2))
{
    ui->setupUi(this);

    ui->pbCompute->setEnabled(false);

    ui->cartesian_le->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9.+\\-*/%^() ]{1,}"),this));


    function_graphic.setSize(ui->slider_graphic_size->value());
    function_graphic.setScale(ui->sliderRatio->value());
    function_graphic.setGridIsDrawable(ui->cb_show_grid->isChecked());
    function_graphic.clearBuffer();
    function_graphic.bufferToLabel(ui->function_graphic_label);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::reCallingAndPaint()
{
    int size = ui->slider_graphic_size->value();
    int scale = ui->sliderRatio->value();

    function_graphic.setSize(size);
    function_graphic.setScale(scale);
    function_graphic.clearBuffer();

    int pixelsPerOneX = scale;
    double numberOfXsPerOnePixel = 1.0/pixelsPerOneX;

    function_graphic.setStepOfParameter(numberOfXsPerOnePixel);

    switch(ui->tabs_of_functions->currentIndex())
    {
    case 0:
        {
            const std::string textOfFunction = ui->cartesian_le->text().toStdString();
            fy.setNameOfVariable("x");
            fx.setNameOfVariable("x");

            if(Function::textOfFunctionIsOkWithNullingCounterOfTempVariables(textOfFunction,fy.variableName()) && textOfFunction != fy.textOfFunction() )
                fy = textOfFunction;
            fx = "x";

            function_graphic.setXFunction(fx);
            function_graphic.setYFunction(fy);

            function_graphic.setRangeOfParameter(-size*scale/2,size*scale/2);


        break;
        }
    case 1:
        {
            const std::string textOfFunction = ui->polar_le->text().toStdString();
            const std::string textOfFunctionX = "("+textOfFunction+")*cos(angle)";
            const std::string textOfFunctionY = "("+textOfFunction+")*sin(angle)";
            fy.setNameOfVariable("angle");
            fx.setNameOfVariable("angle");
            if( Function::textOfFunctionIsOkWithNullingCounterOfTempVariables(textOfFunctionX,"angle"))
                fx = textOfFunctionX;
            if(Function::textOfFunctionIsOkWithNullingCounterOfTempVariables(textOfFunctionY,"angle"))
                fy = textOfFunctionY;
            function_graphic.setXFunction(fx);
            function_graphic.setYFunction(fy);
            function_graphic.setRangeOfParameter(ui->spinb_FROM_of_polar->value(),ui->spinb_TO_of_polar->value());
        break;
        }
    case 2:
    {
        const std::string textOfFunctionX = ui->x_parametered_le->text().toStdString();
        const std::string textOfFunctionY = ui->y_parametered_le->text().toStdString();
        fy.setNameOfVariable("t");
        fx.setNameOfVariable("t");
        if( Function::textOfFunctionIsOkWithNullingCounterOfTempVariables(textOfFunctionX,"t"))
            fx = textOfFunctionX;
        if(Function::textOfFunctionIsOkWithNullingCounterOfTempVariables(textOfFunctionY,"t"))
            fy = textOfFunctionY;
        function_graphic.setXFunction(fx);
        function_graphic.setYFunction(fy);
        function_graphic.setRangeOfParameter(ui->spinb_FROM_of_parametered->value(),ui->spinb_TO_of_parametered->value());
     break;
    }
    }
    function_graphic.drawFunction();
    function_graphic.bufferToLabel(ui->function_graphic_label);
}

void Dialog::paintAxisAndCells(double scaleOfCells)
{
}

void Dialog::on_sliderRatio_sliderMoved(int position)
{
}

void Dialog::on_sliderRatio_valueChanged(int value)
{
}

void Dialog::on_cartesian_le_textChanged(const QString &textOfFunction)
{
    if( Function::textOfFunctionIsOkWithNullingCounterOfTempVariables(textOfFunction.toStdString(),"x") )
        ui->pbCompute->setEnabled(true);
    else
        ui->pbCompute->setEnabled(false);
}

void Dialog::on_pbCompute_clicked()
{
    reCallingAndPaint();
}

void Dialog::on_polar_le_textChanged(const QString &textOfFunction)
{
    const std::string textOfFunctionX = "("+textOfFunction.toStdString()+")*cos(angle)";
    const std::string textOfFunctionY = "("+textOfFunction.toStdString()+")*sin(angle)";
    if( Function::textOfFunctionIsOkWithNullingCounterOfTempVariables(textOfFunctionX,"angle") &&  Function::textOfFunctionIsOkWithNullingCounterOfTempVariables(textOfFunctionY,"angle"))
        ui->pbCompute->setEnabled(true);
    else
        ui->pbCompute->setEnabled(false);
}

void Dialog::on_y_parametered_le_textChanged(const QString &textOfFunctionY)
{
    if( Function::textOfFunctionIsOkWithNullingCounterOfTempVariables(textOfFunctionY.toStdString(),"t") && Function::textOfFunctionIsOkWithNullingCounterOfTempVariables(ui->x_parametered_le->text().toStdString(),"t"))
        ui->pbCompute->setEnabled(true);
    else
        ui->pbCompute->setEnabled(false);
}

void Dialog::on_x_parametered_le_textChanged(const QString &textOfFunctionX)
{
    if( Function::textOfFunctionIsOkWithNullingCounterOfTempVariables(textOfFunctionX.toStdString(),"t") && Function::textOfFunctionIsOkWithNullingCounterOfTempVariables(ui->y_parametered_le->text().toStdString(),"t"))
        ui->pbCompute->setEnabled(true);
    else
        ui->pbCompute->setEnabled(false);
}

void Dialog::on_cb_show_grid_toggled(bool checked)
{
    function_graphic.setGridIsDrawable(checked);
}

void Dialog::on_tabs_of_functions_currentChanged(int index)
{
    switch (index) {
    case 0:
    on_cartesian_le_textChanged(ui->cartesian_le->text());
        break;
    case 1:
    on_polar_le_textChanged(ui->polar_le->text());
        break;
    case 2:
    on_y_parametered_le_textChanged(ui->y_parametered_le->text());
        break;
    default:
        break;
    }
}
