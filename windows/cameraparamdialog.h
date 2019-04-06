#ifndef CAMERAPARAMDIALOG_H
#define CAMERAPARAMDIALOG_H

#include <QDialog>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QListWidgetItem>

#include "cvqtinterface.h"
#include "custom_controller.h"

using namespace cerutti;

namespace Ui {
class CameraParamDialog;
}

/**
 * @brief The CameraParamDialog class is a QT dialog in order to see and try camera calibration.
 * Also permit a better visualisation of matrix and easy qualitative test on sets of images.
 */
class CameraParamDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param parent optionnal parameter for a parent widget.
     */
    explicit CameraParamDialog(QWidget *parent = 0);
    ~CameraParamDialog();

private:

    /**
     * @brief refreshImages refresh the images.
     */
    void refreshImages();

    /**
     * @brief refreshPrintMatrix refresh the matrix.
     */
    void refreshPrintMatrix();

private slots:

    ///Resize principally the images
    void resizeEvent(QResizeEvent *event);

    ///Interactions user
    void on_btn_openCamera_clicked();

    void on_btn_calibrate_clicked();

    void on_btn_openImages_clicked();

    void on_btn_apply_clicked();

    void on_btn_reset_clicked();

    void on_list_imgcalib_currentRowChanged(int currentRow);

    void on_btn_clear_clicked();

    void on_btn_save_clicked();

    void on_btn_remove_clicked();

    void on_btn_saveset_clicked();

private:

    Calibration::StereoCamera calib;

    ///The selected image for preview
    QImage img_selection_left;
    QImage img_selection_right;

    ///Set management of images
    int current_img= -1;
    std::vector<cv::Mat> vect_images_left = std::vector<cv::Mat>();
    std::vector<cv::Mat> vect_images_right = std::vector<cv::Mat>();

    Ui::CameraParamDialog *ui;

};

#endif // CAMERAPARAMDIALOG_H

