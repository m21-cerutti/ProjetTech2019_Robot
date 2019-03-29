#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define IMAGE_DEBUG

#include <QApplication>
#include <QMainWindow>
#include <QPixmap>
#include <QLabel>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QDebug>

#include "custom_controller.h"

#define THRESHOLD_MIN 0.0
#define THRESHOLD_MAX 200.0

using namespace cerutti;

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

    void on_cbDestination_stateChanged(int arg1);

    void on_actionOrigin_triggered();

    void on_actionDisparityBM_triggered();

    void on_actionDisparitySGBM_triggered();

    void on_actionLaplacian_triggered();

    void on_actionGaussian_triggered();

    void on_actionSobel_triggered();

    void on_actionApplyFromFile_triggered();

    void on_actionCalibrate_triggered();

    void on_actionCalibration_triggered();

    void on_actionCalibrationStereo_triggered();

    void on_actionCalibrationStereoVideo_triggered();

    void on_actionExtractImages_triggered();

    void on_actionDepthBM_triggered();

    void on_actionDepthSGBM_triggered();

    void on_actionDepthBMVideo_triggered();

    void on_actionDepthSGBMVideo_triggered();


    void on_actionTest_controller_triggered();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_btn_stereo_clicked();

    void on_btn_simulation_clicked();

    void on_btn_singleImage_clicked();

private:

    double _efficiency_time;

    QImage _image_src;

    cv::Mat _image_mat;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
