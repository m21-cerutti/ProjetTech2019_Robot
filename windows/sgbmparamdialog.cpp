#include "windows/sgbmparamdialog.h"
#include "ui_sgbmparamdialog.h"

using namespace cerutti;

SGBMParamDialog::SGBMParamDialog(const Mat &src_left, const Mat &src_right, QWidget *parent):
    QDialog(parent),
    src_left(src_left),
    src_right(src_right),
    time(0),
    ui(new Ui::SGBMParamDialog)
{
    ui->setupUi(this);
    setWindowTitle("Parameters of SGBM Disparity Map");
    setWindowFlag(Qt::WindowMaximizeButtonHint);

    if(StereoMap::loadSGBMParameters(DEFAULT_SGBM_FILE, sgbm_state))
    {
        ui->minDisparity_slider->setValue(sgbm_state->getMinDisparity());
        ui->numDisparities_slider->setValue(sgbm_state->getNumDisparities());
        ui->blockSize_slider->setValue(sgbm_state->getBlockSize());
        ui->P1_slider->setValue(sgbm_state->getP1());
        ui->P2_slider->setValue(sgbm_state->getP2());
        ui->disp12_slider->setValue(sgbm_state->getDisp12MaxDiff());
        ui->preFilterCap_slider->setValue(sgbm_state->getPreFilterCap());
        ui->uniquenessRatio_slider->setValue(sgbm_state->getUniquenessRatio());
        ui->speckleWindowsSize_slider->setValue(sgbm_state->getSpeckleWindowSize());
        ui->speckleRange_slider->setValue(sgbm_state->getSpeckleRange());
        if( sgbm_state->getMode() == cv::StereoSGBM::MODE_HH)
        {
            ui->FullDP_cb->setCheckState(Qt::Checked);
        }
        else
        {
            ui->FullDP_cb->setCheckState(Qt::Unchecked);
        }
    }
    else
    {
        sgbm_state = getSGBMState();
    }


    hconcat(src_left, src_right, view);
    refreshImages();
}

SGBMParamDialog::~SGBMParamDialog()
{
    delete ui;
}

void SGBMParamDialog::refreshImages()
{
    QImage img;
    CVQTInterface::toQImage(view, img);

    //refresh
    ui->imgView->setPixmap(QPixmap::fromImage(img.scaled((int)ui->boxImg->width()*0.9,
                                                         (int)ui->boxImg->height()*0.9,
                                                         Qt::AspectRatioMode::KeepAspectRatio)));
    ui->labelTime->setText("Time(ms): "+ QString::number((time)));
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
    sgbm_state = getSGBMState();
    time = Utilities::computeEfficiency(StereoMap::computeSGBMDisparity, src_left, src_right, view, sgbm_state);
}



void SGBMParamDialog::on_buttonBox_accepted()
{
    sgbm_state = getSGBMState();
    StereoMap::saveSGBMParameters(DEFAULT_SGBM_FILE, sgbm_state);
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
    hconcat(src_left, src_right, view);
    refreshImages();
}

cv::Mat SGBMParamDialog::getMatResult() const
{
    return view;
}

double SGBMParamDialog::getTimeResult() const
{
    return time;
}

cv::Ptr<cv::StereoSGBM> SGBMParamDialog::getSGBMState()
{
    //sgbgm parameters
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


    sgbm_state = cv::StereoSGBM::create(minDisparity,
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
    return sgbm_state;
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
