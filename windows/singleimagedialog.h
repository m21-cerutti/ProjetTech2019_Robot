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
    explicit SingleImageDialog(QWidget *parent = 0);
    ~SingleImageDialog();

private slots:
    void on_cb_hidesrc_toggled(bool checked);

    void on_cb_resetbefore_toggled(bool checked);

    void on_btn_Origin_clicked();

    void on_btn_Gaussian_clicked();

    void on_btn_Sobel_clicked();

    void on_btn_Laplacian_clicked();

    void on_btn_Open_clicked();

private:
    double _efficiency_time;
    QImage _image_src;
    cv::Mat _image_mat;

    Ui::SingleImageDialog *ui;
};

#endif // SINGLEIMAGEDIALOG_H
