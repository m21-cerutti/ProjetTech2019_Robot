#include "bmparamdialog.h"
#include "ui_bmparamdialog.h"

BMParamDialog::BMParamDialog(QImage& src, QWidget *parent):
    QDialog(parent),
    _img_src(src),
    _img_dst(src),
    _time(0),
    ui(new Ui::BMParamDialog)
{
    CVQTInterface::toMatCV(src, _mat_dst);
    ui->setupUi(this);
    setWindowTitle("Parameters of BM Disparity Map");
    refreshImages();
}

BMParamDialog::~BMParamDialog()
{
    delete ui;
}

void BMParamDialog::refreshImages()
{
    //refresh
    ui->imgView->setPixmap(QPixmap::fromImage(_img_dst.scaled(ui->boxImg->width()*0.9,
                                                              ui->boxImg->height()*0.9,
                                                              Qt::AspectRatioMode::KeepAspectRatio)));
    ui->labelTime->setText("Time(ms): "+ QString::number((_time)));
}

void BMParamDialog::refreshModifs()
{
    //check realtime
    if (ui->cbRealTime->checkState() == Qt::Checked){
        applyDisparity();
    }
    refreshImages();
}

void BMParamDialog::applyDisparity()
{
    cv::Ptr<cv::StereoBM> bmState = getBMState();

    //Conversion and application of Disparity
    CVQTInterface::toMatCV(_img_src, _mat_dst);

    this->_time = ProjectUtilities::computeEfficiency(StereoMap::computeBMDisparity, _mat_dst, _mat_dst, bmState);

    //View the result
    CVQTInterface::toQImage(_mat_dst, _img_dst);

}

void BMParamDialog::resizeEvent(QResizeEvent *event)
{
    refreshImages();
}

void BMParamDialog::on_btnShow_clicked()
{
    //Apply disparity even if not in real time
    applyDisparity();
    refreshImages();
}

void BMParamDialog::on_btnReset_clicked()
{
    _img_dst = _img_src;
    refreshImages();
}

cv::Mat BMParamDialog::getMatResult() const
{
    return _mat_dst;
}

double BMParamDialog::getTimeResult() const
{
    return _time;
}

cv::Ptr<cv::StereoBM> BMParamDialog::getBMState() const
{
    //bgm parameters
    int num_disparities = ui->numDisparities_slider->value();
    int block_size = ui->blockSize_slider->value();



    cv::Ptr<cv::StereoBM> bm_state = cv::StereoBM::create(num_disparities, block_size);
    /*
    bm_state->setPreFilterCap();
    bm_state->setPreFilterSize();
    bm_state->setPreFilterType();
    bm_state->setROI1();
    bm_state->setROI2();
    bm_state->setSmallerBlockSize();
    bm_state->setTextureThreshold();
    bm_state->setUniquenessRatio();
    bm_state->setBlockSize();
    bm_state->setDisp12MaxDiff();
    bm_state->setMinDisparity();
    bm_state->setNumDisparities();
    bm_state->setSpeckleRange();
    bm_state->setSpeckleWindowSize();
    */

    return bm_state;
}

void BMParamDialog::on_numDisparities_slider_valueChanged(int value)
{
    // need to be divisible by 16
    value -= (value % 16);

    ui->numDisparities_slider->setValue(value);
    refreshModifs();
}

void BMParamDialog::on_blockSize_slider_valueChanged(int value)
{
    // need to be not divisible by 2
    if ((value % 2) != 1) {
        value -= 1;
    }
    ui->blockSize_slider->setValue(value);
    refreshModifs();
}

void BMParamDialog::on_cbRealTime_toggled(bool checked)
{
    refreshModifs();
}
