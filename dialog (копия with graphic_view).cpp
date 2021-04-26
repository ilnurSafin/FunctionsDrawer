#include <cmath>
#include <iostream>
#include <QtCore>
#include <QDebug>

#include "dialog.h"
#include "ui_dialog.h"
#include "function.h"

Dialog::Dialog(QWidget *parent) :
    f("0","x"),
    QDialog(parent),
    ui(new Ui::Dialog),length(50)
{
    ui->setupUi(this);

    sceneOfF = new QGraphicsScene(-length,-length,length*2,length*2,this);
    ui->sourceView->setScene(sceneOfF);

    sceneOfPruductionOfF = new QGraphicsScene(-length,-length,length*2,length*2,this);
    ui->distView->setScene(sceneOfPruductionOfF);

    integratedScene = new QGraphicsScene(-length,-length,length*2,length*2,this);
    ui->sourceAndDistView->setScene(integratedScene);

    ui->sliderRatio->setTracking(false);
    ui->pbCompute->setEnabled(false);

    ui->leOfFunction->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9.+\\-*/%^() ]{1,}"),this));
    paintAxisAndCells(ui->sliderRatio->value());

    ui->label_8->setPixmap(QPixmap("/home/ilnur/Pictures/graybutton.png"));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::reCallingAndPaint()
{
    std::string textOfFunction = ui->leOfFunction->text().toStdString();
    if(Function::textOfFunctionIsOkWithNullingCounterOfTempVariables(textOfFunction,f.variableName()) && textOfFunction != f.textOfFunction() )
        f = ui->leOfFunction->text().toStdString();

    sceneOfF->clear();
    sceneOfPruductionOfF->clear();
    integratedScene->clear();

    int pixelsPerOneX = ui->sliderRatio->value();
    paintAxisAndCells(pixelsPerOneX);

    double numberOfXsPerOnePixel = 1.0/pixelsPerOneX;
    double oldY=f(-length-numberOfXsPerOnePixel);

    for(double x=-length; x<=length; x+=numberOfXsPerOnePixel)
    {
        double newY = f(x);

        sceneOfF->addRect(x     * pixelsPerOneX, -newY * pixelsPerOneX,
                                                                       1,1,QPen(Qt::darkGreen),QBrush(Qt::darkGreen));

        sceneOfPruductionOfF->addRect(x * pixelsPerOneX,-(newY-oldY)/numberOfXsPerOnePixel * pixelsPerOneX,
                                                                       1,1,QPen(Qt::darkMagenta),QBrush(Qt::darkMagenta));

        integratedScene->addRect(x     * pixelsPerOneX, -newY * pixelsPerOneX,
                                                                       1,1,QPen(Qt::darkGreen),QBrush(Qt::darkGreen));

        integratedScene->addRect(x * pixelsPerOneX,-(newY-oldY)/numberOfXsPerOnePixel * pixelsPerOneX,
                                                                       1,1,QPen(Qt::darkMagenta),QBrush(Qt::darkMagenta));
        /*sceneOfPruductionOfF->addRect(x * pixelsPerOneX,-(newY-oldY)/numberOfXsPerOnePixel * pixelsPerOneX,
                                                                       1,1,QPen(Qt::darkMagenta),QBrush(Qt::darkMagenta));
        integratedScene->addRect(x * pixelsPerOneX,-(newY-oldY)/numberOfXsPerOnePixel * pixelsPerOneX,
                                                                       1,1,QPen(Qt::darkMagenta),QBrush(Qt::darkMagenta));

        for(double y=oldY;newY>oldY ? y<=newY:y>=newY newY>oldY ? y+=numberOfXsPerOnePixel:y-=numberOfXsPerOnePixel)
        {
            sceneOfF->addRect(x     * pixelsPerOneX, -y * pixelsPerOneX,
                     1,1,QPen(Qt::darkGreen),QBrush(Qt::darkGreen));
            integratedScene->addRect(x     * pixelsPerOneX, -y * pixelsPerOneX,
                                                                           1,1,QPen(Qt::darkGreen),QBrush(Qt::darkGreen));

        }*/
        oldY=newY;
    }
}

void Dialog::paintAxisAndCells(double scaleOfCells)
{
    //y axis
    sceneOfF->addLine(0,-length * scaleOfCells,
                      0, length * scaleOfCells, QPen(Qt::darkGray,2));
    //x axis
    sceneOfF->addLine(-length * scaleOfCells,0,
                       length * scaleOfCells,0, QPen(Qt::darkGray,2));

    //y axis
    sceneOfPruductionOfF->addLine(0,-length * scaleOfCells,
                                  0, length * scaleOfCells, QPen(Qt::darkGray,2));
    //x axis
    sceneOfPruductionOfF->addLine(-length * scaleOfCells,0,
                                   length * scaleOfCells,0, QPen(Qt::darkGray,2));

    //y axis
    integratedScene->addLine(0,-length * scaleOfCells,
                             0, length * scaleOfCells, QPen(Qt::darkGray,2));
    //x axis
    integratedScene->addLine(-length * scaleOfCells,0,
                              length * scaleOfCells,0, QPen(Qt::darkGray,2));

    for(int i=1; i<=length; i++)
    //cells
    {
        //horizontal lines
        //  down
        sceneOfF->addLine(-length * scaleOfCells, i*scaleOfCells ,
                           length * scaleOfCells, i*scaleOfCells, QPen(Qt::gray));
        //  up
        sceneOfF->addLine(-length * scaleOfCells, -i*scaleOfCells ,
                           length * scaleOfCells, -i*scaleOfCells, QPen(Qt::gray));

        //vertical lines
        //rigth
        sceneOfF->addLine(i*scaleOfCells,-length * scaleOfCells,
                          i*scaleOfCells, length * scaleOfCells, QPen(Qt::gray));
        //left
        sceneOfF->addLine(-i*scaleOfCells,-length * scaleOfCells,
                          -i*scaleOfCells, length * scaleOfCells, QPen(Qt::gray));
        //----------------------------------------------------------------------------
        //horizontal lines
        //  down
        sceneOfPruductionOfF->addLine(-length * scaleOfCells, i*scaleOfCells ,
                                       length * scaleOfCells, i*scaleOfCells, QPen(Qt::gray));
        //  up
        sceneOfPruductionOfF->addLine(-length * scaleOfCells, -i*scaleOfCells ,
                                       length * scaleOfCells, -i*scaleOfCells, QPen(Qt::gray));

        //vertical lines
        //rigth
        sceneOfPruductionOfF->addLine(i*scaleOfCells,-length * scaleOfCells,
                                      i*scaleOfCells, length * scaleOfCells, QPen(Qt::gray));
        //left
        sceneOfPruductionOfF->addLine(-i*scaleOfCells,-length * scaleOfCells,
                                      -i*scaleOfCells, length * scaleOfCells, QPen(Qt::gray));
        //----------------------------------------------------------------------------
        //horizontal lines
        //  down
        integratedScene->addLine(-length * scaleOfCells, i*scaleOfCells ,
                                  length * scaleOfCells, i*scaleOfCells, QPen(Qt::gray));
        //  up
        integratedScene->addLine(-length * scaleOfCells, -i*scaleOfCells ,
                                  length * scaleOfCells, -i*scaleOfCells, QPen(Qt::gray));

        //vertical lines
        //rigth
        integratedScene->addLine(i*scaleOfCells,-length * scaleOfCells,
                                 i*scaleOfCells, length * scaleOfCells, QPen(Qt::gray));
        //left
        integratedScene->addLine(-i*scaleOfCells,-length * scaleOfCells,
                                 -i*scaleOfCells, length * scaleOfCells, QPen(Qt::gray));
    }
}

void Dialog::on_sliderRatio_sliderMoved(int position)
{
    //reCallingAndPaint();
}

void Dialog::on_sliderRatio_valueChanged(int value)
{
    sceneOfF->setSceneRect(-length * value,-length * value,length * 2 * value,length * 2 * value);
    sceneOfPruductionOfF->setSceneRect(-length * value,-length * value,length * 2 * value,length * 2 * value);
    integratedScene->setSceneRect(-length * value,-length * value,length * 2 * value,length * 2 * value);
    reCallingAndPaint();
}

void Dialog::on_leOfFunction_textChanged(const QString &textOfFunction)
{
    if( Function::textOfFunctionIsOkWithNullingCounterOfTempVariables(textOfFunction.toStdString(),f.variableName()) )
        ui->pbCompute->setEnabled(true);
    else
        ui->pbCompute->setEnabled(false);
}

void Dialog::on_pbCompute_clicked()
{
    reCallingAndPaint();
}
