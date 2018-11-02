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
#include "subWindows/dephtmapparamdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();

    ///Verify the matrice with OpenCV window
    static void showMatrice(cv::Mat mat);

private:

    void chooseImage();

    void refreshImages();

private slots:

    ///Resize principally the images
    void resizeEvent(QResizeEvent *event);

    void on_actionAbout_triggered();

    void on_actionOpen_triggered();

    void on_actionQuit_triggered();

    void on_btnOrigin_clicked();

    void on_btnShowMatrice_clicked();

    void on_btnLaplacian_clicked();

    void on_cbDestination_stateChanged(int arg1);

    void on_btnTest_clicked();

    void on_btnDisparity_clicked();

    void on_btnDisparitySimple_clicked();

    void on_actionOrigin_triggered();

    void on_actionShow_mat_triggered();

    void on_actionAdvanced_triggered();

    void on_actionSimple_triggered();

    void on_actionLaplacian_triggered();

private:

    QImage image_src;

    cv::Mat image_mat;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
