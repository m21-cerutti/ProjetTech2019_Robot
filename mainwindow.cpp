#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow() :
    QMainWindow(nullptr),
    ui(new Ui::MainWindow)
{
    //Setup and initiate elements in windows
    ui->setupUi(this);
    ui->boxSrc->hide();
}

MainWindow::~MainWindow()
{
    //Desallocate widgets in windows
    delete ui;

    //Use to delete windows create by cv
    cv::destroyAllWindows();
}

void MainWindow::chooseImage(){

    //get filename of image
    QString filename = QFileDialog::getOpenFileName(this, "Open Image", "~/", tr("Image Files (*.GIF *.png *.jpg *.bmp *.jpeg)"), nullptr, QFileDialog::DontUseNativeDialog);

    // open image
    if(_image_src.load(filename)){

        //Create default matrice
        on_btnOrigin_clicked();

        //Refresh window
        refreshImages();
    }

}

void MainWindow::refreshImages()
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

void MainWindow::resetBeforeOperationCheck()
{
    if (ui->cbResetBefore->checkState() == Qt::Checked){
        on_btnOrigin_clicked();
    }
}

void MainWindow::showEfficiency(QString nameFunct, double time_ms)
{
    ui->statusbar->showMessage(nameFunct+" time(ms): "+QString::number(time_ms), 0);
}

/***********************************************************************************/

void MainWindow::resizeEvent(QResizeEvent *event)
{
    refreshImages();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "About", "Project by CERUTTI Marc"\
                                      "\nInterface de tests pour les fonctions d'analyse d'images.");
}

void MainWindow::on_actionOpen_triggered()
{
    chooseImage();
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_cbDestination_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked){
        ui->boxSrc->show();
    }
    else{
        ui->boxSrc->hide();
    }
}

void MainWindow::on_btnOrigin_clicked()
{
    if (_image_src.isNull())
    {
        return;
    }

    //Convert
    CVQTInterface::toMatCV(_image_src, _image_mat);

    refreshImages();
}

void MainWindow::on_btnGauss_clicked()
{
    resetBeforeOperationCheck();
    if (_image_src.isNull())
    {
        return;
    }

    double time;
    time = ProjectDebuger::computeEfficiency(ImageAnalyser::computeGaussianBlur, _image_mat, _image_mat);

    refreshImages();
    showEfficiency("GaussianBlur", time);
}

void MainWindow::on_btnSobel_clicked()
{
    resetBeforeOperationCheck();
    if (_image_src.isNull())
    {
        return;
    }

    double time;
    time = ProjectDebuger::computeEfficiency(ImageAnalyser::computeGradient, _image_mat, _image_mat);

    refreshImages();
    showEfficiency("Sobel", time);
}


void MainWindow::on_btnLaplacian_clicked()
{
    resetBeforeOperationCheck();
    if (_image_src.isNull())
    {
        return;
    }

    double time;
    time = ProjectDebuger::computeEfficiency(ImageAnalyser::computeLaplacian, _image_mat, _image_mat);

    refreshImages();
    showEfficiency("Laplacian", time);
}

void MainWindow::on_btnSGBMDisparity_clicked()
{
    resetBeforeOperationCheck();
    if (_image_src.isNull())
    {
        return;
    }
    QImage img;
    CVQTInterface::toQImage(_image_mat, img);
    SGBMParamDialog dial(img);
    if(dial.exec() != QDialog::Rejected)
    {
        _image_mat = dial.getMatResult().clone();
        double time = dial.getTimeResult();
        refreshImages();
        showEfficiency("SGBMDisparity", time);
    }
}

void MainWindow::on_btnBMDisparity_clicked()
{
    resetBeforeOperationCheck();
    if (_image_src.isNull())
    {
        return;
    }
    QImage img;
    CVQTInterface::toQImage(_image_mat, img);
    BMParamDialog dial(img);
    if(dial.exec() != QDialog::Rejected)
    {
        _image_mat = dial.getMatResult().clone();
        double time = dial.getTimeResult();
        refreshImages();
        showEfficiency("BMDisparity", time);
    }
}

void MainWindow::on_actionLaplacian_triggered()
{
    on_btnLaplacian_clicked();
}

void MainWindow::on_actionGaussian_triggered()
{
    on_btnGauss_clicked();
}

void MainWindow::on_actionSobel_triggered()
{
    on_btnSobel_clicked();
}

void MainWindow::on_actionSimple_triggered()
{
    on_btnBMDisparity_clicked();
}

void MainWindow::on_actionAdvanced_triggered()
{
    on_btnSGBMDisparity_clicked();
}

void MainWindow::on_actionOrigin_triggered()
{
    on_btnOrigin_clicked();
}

void MainWindow::on_actionApplyFromFile_triggered()
{
    resetBeforeOperationCheck();
    if (_image_src.isNull())
    {
        return;
    }

    QString filename = QFileDialog::getOpenFileName(this, "Open camera calibration file", "~/", tr("XML Files (*.xml)"), nullptr, QFileDialog::DontUseNativeDialog);

    // open image
    if(!filename.isEmpty())
    {
        CameraCalibration::applyUndistordedFromFile(filename.toStdString(), _image_mat, _image_mat);
        //Refresh window
        refreshImages();
    }
}

void MainWindow::on_actionCalibrate_triggered()
{
    resetBeforeOperationCheck();

    QStringList filenames = QFileDialog::getOpenFileNames(this, "Open Folder Image", "~/", tr("Image Files (*.GIF *.png *.jpg *.bmp *.jpeg)"), nullptr, QFileDialog::DontUseNativeDialog);

    std::vector<cv::Mat> vect_images;
    for(QString filename : filenames)
    {
        QImage img;
        if(img.load(filename)){

           cv::Mat tmp;
           CVQTInterface::toMatCV(img, tmp);
           vect_images.push_back(tmp);
        }
    }

    CameraCalibration::calibrateFromImages(vect_images, CameraCalibration::DEFAULT_CAMERA_PATH());
    //double time;
    //time = ProjectDebuger::computeEfficiency();

    //Refresh window
    refreshImages();
    showEfficiency("Calibration multiple", 0);
}
