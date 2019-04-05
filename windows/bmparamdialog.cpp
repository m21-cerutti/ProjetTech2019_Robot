#include "windows/bmparamdialog.h"
#include "ui_bmparamdialog.h"

using namespace cerutti;


BMParamDialog::BMParamDialog(const Mat &src_left, const Mat &src_right, QWidget *parent):
    QDialog(parent),
    src_left(src_left),
    src_right(src_right),
    _time(0),
    ui(new Ui::BMParamDialog)
{
    src_left.copyTo(dst_left);
    src_left.copyTo(dst_right);
    ui->setupUi(this);
    setWindowTitle("Parameters of BM Disparity Map");

    hconcat(src_left, src_right, view);

    refreshImages();
}

BMParamDialog::~BMParamDialog()
{
    delete ui;
}

void BMParamDialog::refreshImages()
{

    QImage img;
    CVQTInterface::toQImage(view, img);

    //refresh
    ui->imgView->setPixmap(QPixmap::fromImage(img.scaled(ui->boxImg->width()*0.9,
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
    Mat left, right;

    cv::Ptr<cv::StereoBM> bmState = getBMState();

    //Conversion and application of Disparity
    CVQTInterface::toMatCV(_img_src, _mat_dst);
    Filters::separateImage(_mat_dst, left, right);
    this->_time = Utilities::computeEfficiency(StereoMap::computeBMDisparity, src_left, src_right, disparity, bmState);

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


    int preFilterCap = ui->preFilterCap_slider->value();
    int PreFilterSize = ui->preFilterSize_slider->value();
    int PreFilterType = ui->preFilterType_slider->value();
    int ROI1_x = ui->ROI1_x_slider->value();
    int ROI1_y = ui->ROI1_y_slider->value();
    int ROI1_w = ui->ROI1_w_slider->value();
    int ROI1_h = ui->ROI1_h_slider->value();
    int ROI2_x = ui->ROI2_x_slider->value();
    int ROI2_y = ui->ROI2_y_slider->value();
    int ROI2_w = ui->ROI2_w_slider->value();
    int ROI2_h = ui->ROI2_h_slider->value();
    int SmallerBlockSize = ui->smallerBlockSize_slider->value();
    int TextureThreshold = ui->textureThreshold_slider->value();
    int uniquenessRatio = ui->uniquenessRatio_slider->value();
    int blockSize = ui->blockSize_slider->value();
    int disp12MaxDiff = ui->disp12_slider->value();
    int minDisparity = ui->minDisparity_slider->value();
    int numDisparities = ui->numDisparities_slider->value();
    int speckleRange = ui->speckleRange_slider->value();
    int speckleWindowSize = ui->speckleWindowsSize_slider->value();

    cv::Ptr<cv::StereoBM> bm_state = cv::StereoBM::create();

    bm_state->setPreFilterCap(preFilterCap);
    bm_state->setPreFilterSize(PreFilterSize);
    bm_state->setPreFilterType(PreFilterType);
    bm_state->setROI1(cv::Rect(ROI1_x,ROI1_y,ROI1_w,ROI1_h));
    bm_state->setROI2(cv::Rect(ROI2_x,ROI2_y,ROI2_w,ROI2_h));
    bm_state->setSmallerBlockSize(SmallerBlockSize);
    bm_state->setTextureThreshold(TextureThreshold);
    bm_state->setUniquenessRatio(uniquenessRatio);
    bm_state->setBlockSize(blockSize);
    bm_state->setDisp12MaxDiff(disp12MaxDiff);
    bm_state->setMinDisparity(minDisparity);
    bm_state->setNumDisparities(numDisparities);
    bm_state->setSpeckleRange(speckleRange);
    bm_state->setSpeckleWindowSize(speckleWindowSize);


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

void BMParamDialog::on_minDisparity_slider_valueChanged(int value)
{
    refreshModifs();
}

void BMParamDialog::on_disp12_slider_valueChanged(int value)
{
    refreshModifs();
}

void BMParamDialog::on_preFilterCap_slider_valueChanged(int value)
{
    refreshModifs();
}

void BMParamDialog::on_uniquenessRatio_slider_valueChanged(int value)
{
    refreshModifs();
}

void BMParamDialog::on_speckleWindowsSize_slider_valueChanged(int value)
{
    refreshModifs();
}

void BMParamDialog::on_speckleRange_slider_valueChanged(int value)
{
    refreshModifs();
}

void BMParamDialog::on_preFilterSize_slider_valueChanged(int value)
{
    if ((value % 2) != 1) {
        value -= 1;
    }
    ui->preFilterSize_slider->setValue(value);
    refreshModifs();
}

void BMParamDialog::on_preFilterType_slider_valueChanged(int value)
{
    if(value == CV_STEREO_BM_NORMALIZED_RESPONSE)
    {
        ui->preFilterType_slider->setValue(CV_STEREO_BM_NORMALIZED_RESPONSE);
    }
    else if (value == CV_STEREO_BM_XSOBEL)
    {
        ui->preFilterType_slider->setValue(CV_STEREO_BM_XSOBEL);
    }
    refreshModifs();
}

void BMParamDialog::on_ROI1_x_slider_valueChanged(int value)
{
    refreshModifs();
}

void BMParamDialog::on_ROI1_y_slider_valueChanged(int value)
{
    refreshModifs();
}

void BMParamDialog::on_ROI1_w_slider_valueChanged(int value)
{
    refreshModifs();
}

void BMParamDialog::on_ROI1_h_slider_valueChanged(int value)
{
    refreshModifs();
}

void BMParamDialog::on_ROI2_x_slider_valueChanged(int value)
{
    refreshModifs();
}

void BMParamDialog::on_ROI2_y_slider_valueChanged(int value)
{
    refreshModifs();
}

void BMParamDialog::on_ROI2_w_slider_valueChanged(int value)
{
    refreshModifs();
}

void BMParamDialog::on_ROI2_h_slider_valueChanged(int value)
{
    refreshModifs();
}

void BMParamDialog::on_smallerBlockSize_slider_valueChanged(int value)
{
    refreshModifs();
}

void BMParamDialog::on_textureThreshold_slider_valueChanged(int value)
{
    refreshModifs();
}


void BMParamDialog::on_cbRealTime_toggled(bool checked)
{
    refreshModifs();
}
