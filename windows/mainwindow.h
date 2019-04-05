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

#include "windows/bmparamdialog.h"
#include "windows/cameraparamdialog.h"
#include "windows/sgbmparamdialog.h"
#include "windows/singleimagedialog.h"

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

private slots:

    void on_actionQuit_triggered();

    void on_actionDisparityBM_triggered();

    void on_actionDisparitySGBM_triggered();

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

    void on_actionOpen_filters_triggered();

private:

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
