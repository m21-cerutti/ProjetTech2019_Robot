#ifndef SGBMPARAMDIALOG_H
#define SGBMPARAMDIALOG_H

#include <QDialog>

#include "cvqtinterface.h"
#include "custom_controller.h"

namespace Ui {
class SGBMParamDialog;
}

/**
 * @brief The SGBMParamDialog class is a QT dialog in order to see and try parameters for disparity map with SGBM algorithm.
 */
class SGBMParamDialog : public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief Constructor
     * @param src QImage to transform in disparity map using SGBM algorithm.
     * @param parent optionnal parameter for a parent widget.
     */
    explicit SGBMParamDialog(QImage &src, QWidget *parent = nullptr);
    ~SGBMParamDialog();

    /**
     * @brief getMatResult
     * @return the result matrix after transformation.
     */
    cv::Mat getMatResult() const;

    /**
     * @brief getTimeResult
     * @return  the approximative time of execute the algorithm.
     */
    double getTimeResult() const;

    /**
     * @brief getSGBMState Fonction using ui for parameters
     * @return BM parameters for a future reuse.
     */
    cv::Ptr<cv::StereoSGBM> getSGBMState();

private:

    /**
     * @brief refreshImages refresh the images.
     */
    void refreshImages();

    /**
     * @brief applyDisparity apply to the UI
     */
    void applyDisparity();

public slots:

    /**
     * @brief Refresh and check for disparity, use refreshImages.
     * Call this when parameters value changed.
     */
    void refreshModifs();

private slots:

    ///Show the result of disparity with parameter
    void on_btnShow_clicked();

    ///Reset to the origin Image.
    void on_btnReset_clicked();

    ///Sliders and check box parameters
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

    ///For aplying disparity in realtime.
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
