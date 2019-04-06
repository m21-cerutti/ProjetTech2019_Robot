#include "singleimagedialog.h"
#include "ui_singleimagedialog.h"

using namespace cerutti;

SingleImageDialog::SingleImageDialog(cv::Mat &src, QWidget *parent) :
    QDialog(parent),
    src(src),
    ui(new Ui::SingleImageDialog)
{
    ui->setupUi(this);
    setWindowTitle("Filters");
    setWindowFlag(Qt::WindowMaximizeButtonHint);
    ui->BoxSrc->hide();

    src.copyTo(view);
    refreshImages();

}

SingleImageDialog::~SingleImageDialog()
{
    delete ui;
}

void SingleImageDialog::refreshImages()
{
    QImage img;
    CVQTInterface::toQImage(view, img);

    //Refresh source
    QImage img_src;
    CVQTInterface::toQImage(src, img_src);
    ui->lb_imgsrc->setPixmap(QPixmap::fromImage(img_src.scaled((int)(ui->BoxSrc->width()*0.95),
                                                               (int)(ui->BoxSrc->height()*0.95),
                                                               Qt::AspectRatioMode::KeepAspectRatio)));

    //Refresh destination
    ui->lb_imgdest->setPixmap(QPixmap::fromImage(img.scaled((int)(ui->BoxDst->width()*0.95),
                                                            (int)(ui->BoxDst->height()*0.95),
                                                            Qt::AspectRatioMode::KeepAspectRatio)));

    ui->lb_time->setText("Time(ms): "+ QString::number((time)));
}

void SingleImageDialog::resetBeforeOperationCheck()
{
    if (ui->cb_resetbefore->checkState() == Qt::Checked){
        src.copyTo(view);
    }
}

/***********************************************************************************/

void SingleImageDialog::resizeEvent(QResizeEvent *event)
{
    refreshImages();
}

void SingleImageDialog::on_cb_hidesrc_toggled(bool checked)
{
    if(!checked)
        ui->BoxSrc->show();
    else
        ui->BoxSrc->hide();
}

void SingleImageDialog::on_btn_Origin_clicked()
{
    //Convert
    src.copyTo(view);
    refreshImages();

}

void SingleImageDialog::on_btn_Gaussian_clicked()
{
    resetBeforeOperationCheck();

    time = Utilities::computeEfficiency(Filters::computeGaussianBlur, view, view);
    refreshImages();
}

void SingleImageDialog::on_btn_Sobel_clicked()
{
    resetBeforeOperationCheck();

    time = Utilities::computeEfficiency(Filters::computeGradient, view, view);
    refreshImages();
}

void SingleImageDialog::on_btn_Laplacian_clicked()
{
    resetBeforeOperationCheck();

    time = Utilities::computeEfficiency(Filters::computeLaplacian, view, view);
    refreshImages();
}

