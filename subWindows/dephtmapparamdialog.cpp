#include "dephtmapparamdialog.h"
#include "ui_dephtmapparamdialog.h"

DephtMapParamDialog::DephtMapParamDialog(QImage& src, QWidget *parent) :
    QDialog(parent),
    img_src(src),
    img_dst(src),
    ui(new Ui::DephtMapParamDialog)


{
    ui->setupUi(this);
    setWindowTitle("Parameters of Depth Map");
    refreshImages();

}

DephtMapParamDialog::~DephtMapParamDialog()
{
    delete ui;
}

void DephtMapParamDialog::refreshImages()
{
    //refresh
    ui->imgView->setPixmap(QPixmap::fromImage(img_dst.scaled(ui->boxImg->width()*0.9,
                                                             ui->boxImg->height()*0.9,
                                                             Qt::AspectRatioMode::KeepAspectRatio)));
}

void DephtMapParamDialog::refreshModifs()
{
    //check realtime
    if (ui->cbRealTime->checkState() == Qt::Checked){
        applyDisparity();
    }
    refreshImages();
}

void DephtMapParamDialog::applyDisparity()
{
    //bgm parameters
    cv::StereoSGBM bmState;

    bmState.preFilterCap = ui->preFilterCap_slider->value();
    bmState.fullDP = (ui->FullDP_cb->checkState() == Qt::Checked);
    bmState.P1 = ui->P1_slider->value();
    bmState.P2 = ui->P2_slider->value();
    bmState.minDisparity = ui->minDisparity_slider->value();
    bmState.numberOfDisparities = ui->numDisparities_slider->value();
    bmState.uniquenessRatio = ui->uniquenessRatio_slider->value();
    bmState.speckleWindowSize = ui->speckleWindowsSize_slider->value();
    bmState.speckleRange = ui->speckleRange_slider->value();
    bmState.disp12MaxDiff = ui->disp12_slider->value();
    bmState.SADWindowSize = ui->SADwindowSize_slider->value();

    //Conversion and application of Disparity
    mat_dst = ImageAnalyser::toMatCV(img_src);
    mat_dst = ImageAnalyser::computeAdvancedDepthMap(mat_dst, bmState);

    //View of the result
    img_dst = ImageAnalyser::toQImage(mat_dst);

}

void DephtMapParamDialog::resizeEvent(QResizeEvent *event)
{
    refreshModifs();
}

void DephtMapParamDialog::on_btnShow_clicked()
{
    //Apply disparity even if not in real time
    applyDisparity();
    refreshImages();
}

void DephtMapParamDialog::on_btnReset_clicked()
{
    img_dst = img_src;
    refreshImages();
}

cv::Mat DephtMapParamDialog::getMatResult() const
{
    return mat_dst;
}

void DephtMapParamDialog::on_minDisparity_slider_valueChanged(int value)
{
    refreshModifs();
}

void DephtMapParamDialog::on_numDisparities_slider_valueChanged(int value)
{
    // need to be divisible by 16
    if ((value % 16) != 0) {
        value -= (value % 16);
    }
    ui->numDisparities_slider->setValue(value);
    refreshModifs();
}

void DephtMapParamDialog::on_SADwindowSize_slider_valueChanged(int value)
{
    refreshModifs();
}

void DephtMapParamDialog::on_P1_slider_valueChanged(int value)
{
    if(ui->P2_slider->value() < value)
        ui->P2_slider->setValue(value+1);
    refreshModifs();
}


void DephtMapParamDialog::on_P2_slider_valueChanged(int value)
{
    if(ui->P1_slider->value() >= value)
        ui->P2_slider->setValue(ui->P1_slider->value()+1);
    refreshModifs();
}

void DephtMapParamDialog::on_disp12_slider_valueChanged(int value)
{
    refreshModifs();
}

void DephtMapParamDialog::on_preFilterCap_slider_valueChanged(int value)
{
    refreshModifs();
}

void DephtMapParamDialog::on_uniquenessRatio_slider_valueChanged(int value)
{
    refreshModifs();
}

void DephtMapParamDialog::on_speckleWindowsSize_slider_valueChanged(int value)
{
    refreshModifs();
}

void DephtMapParamDialog::on_speckleRange_slider_valueChanged(int value)
{
    refreshModifs();
}

void DephtMapParamDialog::on_FullDP_cb_toggled(bool checked)
{
    refreshModifs();
}

void DephtMapParamDialog::on_cbRealTime_toggled(bool checked)
{
    refreshModifs();
}
