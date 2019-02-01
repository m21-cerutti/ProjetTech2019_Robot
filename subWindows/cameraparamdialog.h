#ifndef CAMERAPARAMDIALOG_H
#define CAMERAPARAMDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QListWidgetItem>

#include "tools/projectdebuger.h"
#include "tools/cvqtinterface.h"
#include "tools/imageanalyser.h"
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

private:
    std::string _path_camera = DEFAULT_FILE_CAMERA_PATH;

    QImage _img_selection;

    //Intrinsic camera parameters
    cv::Mat _camera_matrix;
    cv::Mat _dist_coeffs;
    std::vector<cv::Mat> _rvecs;
    std::vector<cv::Mat> _tvecs;

    int _current_img;
    std::vector<cv::Mat> _vect_images;

    Ui::CameraParamDialog *ui;


};

#endif // CAMERAPARAMDIALOG_H
