#ifndef SGBMPARAMDIALOG_H
#define SGBMPARAMDIALOG_H

#include <QDialog>
#include "tools/stereoanalyser.h"
#include "tools/cvqtinterface.h"


namespace Ui {
class SGBMParamDialog;
}

class SGBMParamDialog : public QDialog
{
    Q_OBJECT

public:

    explicit SGBMParamDialog(QImage &src, QWidget *parent = nullptr);

    ~SGBMParamDialog();

    cv::Mat getMatResult() const;

    double getTimeResult() const;

    cv::Ptr<cv::StereoSGBM> getSGBMState();

private:

    ///Refresh standart, do not apply disparity
    void refreshImages();

    ///Fonction using ui for parameters
    void applyDisparity();

public slots:

    ///Refresh and check for disparity, use refreshImages.
    /// Call this when parameters value changed.
    void refreshModifs();

private slots:

    ///Resize principally the images
    void resizeEvent(QResizeEvent *event);

    ///Show the result of disparity with parameter
    void on_btnShow_clicked();

    ///Reset to the origin Image.
    void on_btnReset_clicked();

    void on_minDisparity_slider_valueChanged(int value);

    void on_numDisparities_slider_valueChanged(int value);

    void on_blockSize_slider_valueChanged(int value);

    void on_P2_slider_valueChanged(int value);

    void on_P1_slider_valueChanged(int value);

    void on_disp12_slider_valueChanged(int value);

    void on_preFilterCap_slider_valueChanged(int value);

    void on_uniquenessRatio_slider_valueChanged(int value);

    void on_speckleWindowsSize_slider_valueChanged(int value);

    void on_speckleRange_slider_valueChanged(int value);

    void on_FullDP_cb_toggled(bool checked);

    void on_cbRealTime_toggled(bool checked);

private:
    ///Image Source, need only for reset
    QImage& _img_src;

    ///Image Destination
    QImage _img_dst;
    cv::Mat _mat_dst;

    double _time;

    Ui::SGBMParamDialog *ui;

};

#endif // SGBMPARAMDIALOG_H
