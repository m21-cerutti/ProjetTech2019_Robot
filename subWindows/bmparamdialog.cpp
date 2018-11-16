#include "bmparamdialog.h"
#include "ui_bmparamdialog.h"

BMParamDialog::BMParamDialog(QImage& src, QWidget *parent):
    QDialog(parent),
    img_src(src),
    img_dst(src),
    time(0),
    ui(new Ui::BMParamDialog)
{
    ImageAnalyser::toMatCV(src, mat_dst);
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
    ui->imgView->setPixmap(QPixmap::fromImage(img_dst.scaled(ui->boxImg->width()*0.9,
                                                             ui->boxImg->height()*0.9,
                                                             Qt::AspectRatioMode::KeepAspectRatio)));
    ui->labelTime->setText("Time(ms): "+ QString::number((time)));
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
    //bgm parameters
    int numDisparities = ui->numDisparities_slider->value();
    int blockSize = ui->blockSize_slider->value();

    cv::Ptr<cv::StereoBM> bmState = cv::StereoBM::create(numDisparities, blockSize);

    //Conversion and application of Disparity
    ImageAnalyser::toMatCV(img_src, mat_dst);

    mat_dst = ImageAnalyser::computeEfficiency(this->time, ImageAnalyser::computeBMDisparity, mat_dst, bmState);

    //View the result
    ImageAnalyser::toQImage(mat_dst, img_dst);

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
    img_dst = img_src;
    refreshImages();
}

cv::Mat BMParamDialog::getMatResult() const
{
    return mat_dst;
}

double BMParamDialog::getTimeResult() const
{
    return time;
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
