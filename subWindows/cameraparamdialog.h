#ifndef CAMERAPARAMDIALOG_H
#define CAMERAPARAMDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QListWidgetItem>

#include "tools/projectutilities.h"
#include "tools/cvqtinterface.h"
#include "tools/imagefilter.h"
#include "tools/cameracalibration.h"

namespace Ui {
class CameraParamDialog;
}

class CameraParamDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CameraParamDialog(QWidget *parent = 0);

    ~CameraParamDialog();

private:

    void refreshImages();

    void refreshPrintMatrix();

private slots:

    void resizeEvent(QResizeEvent *event);

    void on_btn_openCamera_clicked();

    void on_btn_calibrate_clicked();

    void on_btn_cameramatrix_clicked();

    void on_btn_distcoeff_clicked();

    void on_btn_rvecs_clicked();

    void on_btn_tvecs_clicked();

    void on_btn_openImages_clicked();

    void on_btn_apply_clicked();

    void on_btn_reset_clicked();

    void on_list_imgcalib_currentRowChanged(int currentRow);

    void on_btn_clear_clicked();

    void on_btn_printimages_clicked();

private:

    QImage _img_selection;

    //Intrinsic camera parameters
    cv::Size _img_size;
    cv::Mat _camera_matrix;
    cv::Mat _dist_coeffs;
    std::vector<cv::Mat> _rvecs;
    std::vector<cv::Mat> _tvecs;

    int _current_img = -1;
    std::vector<cv::Mat> _vect_images;

    Ui::CameraParamDialog *ui;


};

#endif // CAMERAPARAMDIALOG_H
