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

class SingleImageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SingleImageDialog(cv::Mat &src, QWidget *parent = 0);
    ~SingleImageDialog();

private:
    /**
     * @brief refreshImages refresh the images.
     */
    void refreshImages();

    void resetBeforeOperationCheck();


private slots:

    void resizeEvent(QResizeEvent *event);

    void on_cb_hidesrc_toggled(bool checked);

    void on_btn_Origin_clicked();

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