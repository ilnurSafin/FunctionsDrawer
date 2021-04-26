#ifndef DIALOG_H
#define DIALOG_H

#include <QGraphicsScene>
#include <QDialog>

#include "function.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    void reCallingAndPaint();
    void paintAxisAndCells(double scaleOfCells);
private slots:
    void on_sliderRatio_sliderMoved(int position);

    void on_sliderRatio_valueChanged(int value);

    void on_leOfFunction_textChanged(const QString &arg1);

    void on_pbCompute_clicked();

private:
    Function f;
    Ui::Dialog *ui;
    QGraphicsScene* sceneOfF;
    QGraphicsScene* sceneOfPruductionOfF;
    QGraphicsScene* integratedScene;
    int length;
};

#endif // DIALOG_H
