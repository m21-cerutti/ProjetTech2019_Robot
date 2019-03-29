#include "singleimagedialog.h"
#include "ui_singleimagedialog.h"

SingleImageDialog::SingleImageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SingleImageDialog)
{
    ui->setupUi(this);

    //Setup and initiate elements in windows
    ui->setupUi(this);
    ui->boxSrc->hide();
}

SingleImageDialog::~SingleImageDialog()
{
    delete ui;
}

void SingleImageDialog::refreshImages()
{
    if (!_image_src.isNull())
    {
        //Refresh source
        ui->imageLabelSrc->setPixmap(QPixmap::fromImage(_image_src.scaled((int)(ui->boxSrc->width()*0.9),
                                                                          (int)(ui->boxSrc->height()*0.9),
                                                                          Qt::AspectRatioMode::KeepAspectRatio)));

        //Refresh destination
        QImage img;
        CVQTInterface::toQImage(_image_mat, img);
        ui->imageLabelDst->setPixmap(QPixmap::fromImage(img.scaled((int)(ui->boxDest->width()*0.9),
                                                                   (int)(ui->boxDest->height()*0.9),
                                                                   Qt::AspectRatioMode::KeepAspectRatio)));
    }
}

void SingleImageDialog::resetBeforeOperationCheck()
{
    if (ui->cbResetBefore->checkState() == Qt::Checked){
        on_actionOrigin_triggered();
    }
}

void SingleImageDialog::showEfficiency(QString nameFunct, double time_ms)
{
    ui->statusbar->showMessage(nameFunct+" time(ms): "+QString::number(time_ms), 0);
}

/***********************************************************************************/

void SingleImageDialog::resizeEvent(QResizeEvent *event)
{
    refreshImages();
}

void SingleImageDialog::on_btn_Open_clicked()
{
    //get filename of image
    QString filename = QFileDialog::getOpenFileName(this, "Open Image", "~/", tr("Image Files (*.GIF *.png *.jpg *.bmp *.jpeg)"), nullptr, QFileDialog::DontUseNativeDialog);

    // open image
    if(_image_src.load(filename)){

        //Create default matrix
        on_actionOrigin_triggered();

        //Refresh window
        refreshImages();
    }
}

void SingleImageDialog::on_cb_hidesrc_toggled(bool checked)
{
    /*
    if (arg1 == Qt::Checked){
        ui->boxSrc->show();
    }
    else{
        ui->boxSrc->hide();
    }
    */
}

void SingleImageDialog::on_btn_Origin_clicked()
{
    if (_image_src.isNull())
    {
        return;
    }

    //Convert
    CVQTInterface::toMatCV(_image_src, _image_mat);

    refreshImages();

}

void SingleImageDialog::on_btn_Gaussian_clicked()
{
    resetBeforeOperationCheck();
    if (_image_src.isNull())
    {
        return;
    }

    double time;
    time = Utilities::computeEfficiency(Filters::computeGaussianBlur, _image_mat, _image_mat);

    refreshImages();
    showEfficiency("GaussianBlur", time);
}

void SingleImageDialog::on_btn_Sobel_clicked()
{
    resetBeforeOperationCheck();
    if (_image_src.isNull())
    {
        return;
    }

    double time;
    time = Utilities::computeEfficiency(Filters::computeGradient, _image_mat, _image_mat);

    refreshImages();
    showEfficiency("Sobel", time);
}

void SingleImageDialog::on_btn_Laplacian_clicked()
{
    resetBeforeOperationCheck();
    if (_image_src.isNull())
    {
        return;
    }

    double time;
    time = Utilities::computeEfficiency(Filters::computeLaplacian, _image_mat, _image_mat);

    refreshImages();
    showEfficiency("Laplacian", time);
}

