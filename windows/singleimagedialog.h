#ifndef SINGLEIMAGEDIALOG_H
#define SINGLEIMAGEDIALOG_H

#include <QDialog>
#include <QPixmap>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>

#include "cvqtinterface.h"
#include "custom_controller.h"

namespace Ui {
class SingleImageDialog;
}

/**
 * @brief The SingleImageDialog class
 */
class SingleImageDialog : public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief SingleImageDialog
     * @param src
     * @param parent
     */
    explicit SingleImageDialog(cv::Mat &src, QWidget *parent = nullptr);
    ~SingleImageDialog();

private:
    /**
     * @brief refreshImages refresh the images.
     */
    void refreshImages();

    /**
     * @brief check if we apply multiple times te filters.
     */
    void resetBeforeOperationCheck();


private slots:

    ///Resize principally the images
    void resizeEvent(QResizeEvent *event);

    ///Show the source image
    void on_cb_hidesrc_toggled(bool checked);

    ///Reset to the origin Image.
    void on_btn_Origin_clicked();

    ///Filters
    void on_btn_Gaussian_clicked();

    void on_btn_Sobel_clicked();

    void on_btn_Laplacian_clicked();

private:
    ///Image Source, need only for reset
    cv::Mat& src;

    ///Image Destination
    cv::Mat view;

    double time;


    Ui::SingleImageDialog *ui;
};

#endif // SINGLEIMAGEDIALOG_H
