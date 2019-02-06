#ifndef BMPARAMDIALOG_H
#define BMPARAMDIALOG_H

#include <QDialog>
#include "tools/stereoanalyser.h"
#include "tools/cvqtinterface.h"

namespace Ui {
class BMParamDialog;
}

/**
 * @brief The BMParamDialog class is a QT dialog in order to see and try parameters for disparity map with BM algorithm.
 */
class BMParamDialog : public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief Constructor
     * @param src QImage to transform in disparity map using BM algorithm.
     * @param parent optionnal parameter for a parent widget.
     */
    explicit BMParamDialog(QImage &src, QWidget *parent = nullptr);
    ~BMParamDialog();

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
     * @brief getBMState Fonction using ui for parameters
     * @return BM parameters for a future reuse.
     */
    cv::Ptr<cv::StereoBM>  getBMState() const;

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

    ///Resize principally the images
    void resizeEvent(QResizeEvent *event);

    ///Show the result of disparity with parameter
    void on_btnShow_clicked();

    ///Reset to the origin Image.
    void on_btnReset_clicked();

    ///Sliders
    void on_numDisparities_slider_valueChanged(int value);

    void on_blockSize_slider_valueChanged(int value);

    ///For aplying disparity in realtime.
    void on_cbRealTime_toggled(bool checked);

private:
    ///Image Source, need only for reset
    QImage& _img_src;

    ///Image Destination
    QImage _img_dst;
    cv::Mat _mat_dst;

    double _time;

    Ui::BMParamDialog *ui;
};

#endif // BMPARAMDIALOG_H
