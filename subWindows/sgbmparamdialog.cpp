#include "sgbmparamdialog.h"
#include "ui_sgbmparamdialog.h"

SGBMParamDialog::SGBMParamDialog(QImage& src, QWidget *parent) :
    QDialog(parent),
    _img_src(src),
    _img_dst(src),
    _time(0),
    ui(new Ui::SGBMParamDialog)
{
    CVQTInterface::toMatCV(src, _mat_dst);
    ui->setupUi(this);
    setWindowTitle("Parameters of SGBM Disparity Map");
    refreshImages();
}

SGBMParamDialog::~SGBMParamDialog()
{
    delete ui;
}

void SGBMParamDialog::refreshImages()
{
    //refresh
    ui->imgView->setPixmap(QPixmap::fromImage(_img_dst.scaled(ui->boxImg->width()*0.9,
                                                              ui->boxImg->height()*0.9,
                                                              Qt::AspectRatioMode::KeepAspectRatio)));
    ui->labelTime->setText("Time(ms): "+ QString::number((_time)));
}

void SGBMParamDialog::refreshModifs()
{
    //check realtime
    if (ui->cbRealTime->checkState() == Qt::Checked){
        applyDisparity();
    }
    refreshImages();
}

void SGBMParamDialog::applyDisparity()
{

    cv::Ptr<cv::StereoSGBM> sgbmState = getSGBMState();


    //Conversion and application of Disparity
    CVQTInterface::toMatCV(_img_src, _mat_dst);
    _time = ProjectUtilities::computeEfficiency(StereoMap::computeSGBMDisparity, _mat_dst, _mat_dst, sgbmState);

    //View the result
    CVQTInterface::toQImage(_mat_dst, _img_dst);
}

void SGBMParamDialog::resizeEvent(QResizeEvent *event)
{
    refreshImages();
}

void SGBMParamDialog::on_btnShow_clicked()
{
    //Apply disparity even if not in real time
    applyDisparity();
    refreshImages();
}

void SGBMParamDialog::on_btnReset_clicked()
{
    _img_dst = _img_src;
    refreshImages();
}

cv::Mat SGBMParamDialog::getMatResult() const
{
    return _mat_dst;
}

double SGBMParamDialog::getTimeResult() const
{
    return _time;
}

cv::Ptr<cv::StereoSGBM> SGBMParamDialog::getSGBMState()
{
    //bgm parameters
    int minDisparity = ui->minDisparity_slider->value();;
    int numDisparities = ui->numDisparities_slider->value();
    int blockSize = ui->blockSize_slider->value();
    int P1 =  ui->P1_slider->value();
    int P2 = ui->P2_slider->value();
    int disp12MaxDiff = ui->disp12_slider->value();
    int preFilterCap = ui->preFilterCap_slider->value();
    int uniquenessRatio = ui->uniquenessRatio_slider->value();
    int speckleWindowSize = ui->speckleWindowsSize_slider->value();
    int speckleRange = ui->speckleRange_slider->value();

    int mode;
    if(ui->FullDP_cb->checkState() == Qt::Checked)
    {
        mode = cv::StereoSGBM::MODE_HH;
    }
    else
    {
        mode = cv::StereoSGBM::MODE_SGBM;
    }


    cv::Ptr<cv::StereoSGBM> sgbmState =
            cv::StereoSGBM::create(minDisparity,
                                   numDisparities,
                                   blockSize,
                                   P1,
                                   P2,
                                   disp12MaxDiff,
                                   preFilterCap,
                                   uniquenessRatio,
                                   speckleWindowSize,
                                   speckleRange,
                                   mode);
    return sgbmState;
}

void SGBMParamDialog::on_minDisparity_slider_valueChanged(int value)
{
    refreshModifs();
}

void SGBMParamDialog::on_numDisparities_slider_valueChanged(int value)
{
    // need to be divisible by 16
    value -= (value % 16);

    ui->numDisparities_slider->setValue(value);
    refreshModifs();
}

void SGBMParamDialog::on_blockSize_slider_valueChanged(int value)
{
    // need to be not divisible by 2
    if ((value % 2) != 1) {
        value -= 1;
    }
    ui->blockSize_slider->setValue(value);
    refreshModifs();
}

void SGBMParamDialog::on_P1_slider_valueChanged(int value)
{
    if(ui->P2_slider->value() < value)
        ui->P2_slider->setValue(value+1);
    refreshModifs();
}


void SGBMParamDialog::on_P2_slider_valueChanged(int value)
{
    if(ui->P1_slider->value() >= value)
        ui->P2_slider->setValue(ui->P1_slider->value()+1);
    refreshModifs();
}

void SGBMParamDialog::on_disp12_slider_valueChanged(int value)
{
    refreshModifs();
}

void SGBMParamDialog::on_preFilterCap_slider_valueChanged(int value)
{
    refreshModifs();
}

void SGBMParamDialog::on_uniquenessRatio_slider_valueChanged(int value)
{
    refreshModifs();
}

void SGBMParamDialog::on_speckleWindowsSize_slider_valueChanged(int value)
{
    refreshModifs();
}

void SGBMParamDialog::on_speckleRange_slider_valueChanged(int value)
{
    refreshModifs();
}

void SGBMParamDialog::on_FullDP_cb_toggled(bool checked)
{
    refreshModifs();
}

void SGBMParamDialog::on_cbRealTime_toggled(bool checked)
{
    refreshModifs();
}
