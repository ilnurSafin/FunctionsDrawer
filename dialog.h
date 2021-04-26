#ifndef DIALOG_H
#define DIALOG_H

#include <QPixmap>
#include <QGraphicsScene>
#include <QDialog>

#include "./Function/function.h"
#include "graphic.h"

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

    void clearGraphicsBuffers();

    void updateGraphics();

    void paintAxisAndCells(double scaleOfCells);
private slots:
    void on_sliderRatio_sliderMoved(int position);

    void on_sliderRatio_valueChanged(int value);

    void on_cartesian_le_textChanged(const QString &arg1);

    void on_pbCompute_clicked();

    void on_polar_le_textChanged(const QString &textOfFunction);

    void on_y_parametered_le_textChanged(const QString &textOfFunctionY);

    void on_x_parametered_le_textChanged(const QString &textOfFunctionX);

    void on_cb_show_grid_toggled(bool checked);

    void on_tabs_of_functions_currentChanged(int index);

private:
    Function fy;
    Function fx;
    Ui::Dialog *ui;
    int length;
    Graphic function_graphic;
};

#endif // DIALOG_H
