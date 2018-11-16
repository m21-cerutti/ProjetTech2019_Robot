#ifndef BMPARAMDIALOG_H
#define BMPARAMDIALOG_H

#include <QDialog>
#include "tools/imageanalyser.h"

namespace Ui {
class BMParamDialog;
}

class BMParamDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BMParamDialog(QImage &src, QWidget *parent = 0);
    ~BMParamDialog();

    cv::Mat getMatResult() const;

    double getTimeResult() const;

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

    void on_numDisparities_slider_valueChanged(int value);

    void on_blockSize_slider_valueChanged(int value);

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
