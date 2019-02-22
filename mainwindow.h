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

#include "tools/projectutilities.h"
#include "tools/cvqtinterface.h"
#include "tools/imagefilter.h"
#include "tools/cameracalibration.h"
#include "tools/videoextractor.h"
#include "subWindows/sgbmparamdialog.h"
#include "subWindows/bmparamdialog.h"
#include "subWindows/cameraparamdialog.h"

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

    void on_actionDisparityBM_triggered();

    void on_actionDisparitySGBM_triggered();

    void on_actionLaplacian_triggered();

    void on_actionGaussian_triggered();

    void on_actionSobel_triggered();

    void on_actionApplyFromFile_triggered();

    void on_actionCalibrate_triggered();

    void on_actionCalibration_triggered();

    void on_actionOpen_video_triggered();

    void on_actionChessboard_debug_triggered();

    void on_actionStereoCalibration_triggered();

    void on_actionDepthMap_triggered();

    void on_actionExtractImages_triggered();

    void on_actionBM_triggered();

    void on_actionBM_2_triggered();

    void on_actionSGBM_triggered();

    void on_actionCalibrationStereo_triggered();

private:

    double _efficiency_time;

    QImage _image_src;

    cv::Mat _image_mat;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
