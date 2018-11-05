#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QPixmap>
#include <QLabel>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QDebug>

#include "tools/imageanalyser.h"
#include "subWindows/sgbmparamdialog.h"
#include "subWindows/bmparamdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();

private:

    void chooseImage();

    void refreshImages();

    void resetBeforeOperationCheck();

    void showEfficiency(QString nameFunct, double time_ms);

private slots:

    ///Resize principally the images
    void resizeEvent(QResizeEvent *event);

    void on_actionAbout_triggered();

    void on_actionOpen_triggered();

    void on_actionQuit_triggered();

    void on_btnOrigin_clicked();

    void on_btnGauss_clicked();

    void on_btnSobel_clicked();

    void on_btnLaplacian_clicked();

    void on_cbDestination_stateChanged(int arg1);

    void on_btnSGBMDisparity_clicked();

    void on_btnBMDisparity_clicked();

    void on_actionOrigin_triggered();

    void on_actionAdvanced_triggered();

    void on_actionSimple_triggered();

    void on_actionLaplacian_triggered();

    void on_actionGaussian_triggered();

    void on_actionSobel_triggered();

private:

    double efficiencyFunction;

    QImage image_src;

    cv::Mat image_mat;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
