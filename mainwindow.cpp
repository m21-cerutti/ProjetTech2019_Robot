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
    QString filename = QFileDialog::getOpenFileName(this, "Open Image", "~/", tr("Image Files (*.GIF *.png *.jpg *.bmp *.jpeg)"),0, QFileDialog::DontUseNativeDialog);

    // open image
    if(image_src.load(filename)){

        //Create default matrice
        on_btnOrigin_clicked();

        //Refresh window
        refreshImages();
    }

}

void MainWindow::refreshImages()
{
    if (!image_src.isNull())
    {
        //Refresh source
        ui->imageLabelSrc->setPixmap(QPixmap::fromImage(image_src.scaled(ui->boxSrc->width()*0.9,
                                                                         ui->boxSrc->height()*0.9,
                                                                         Qt::AspectRatioMode::KeepAspectRatio)));

        //Refresh destination
        QImage img;
        ImageAnalyser::toQImage(image_mat, img);
        ui->imageLabelDst->setPixmap(QPixmap::fromImage(img.scaled(ui->boxDest->width()*0.9,
                                                                   ui->boxDest->height()*0.9,
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
    if (image_src.isNull())
    {
        return;
    }

    //Convert
    ImageAnalyser::toMatCV(image_src, image_mat);

    refreshImages();
}

void MainWindow::on_btnGauss_clicked()
{
    resetBeforeOperationCheck();
    if (image_src.isNull())
    {
        return;
    }

    double time;
    image_mat = ImageAnalyser::computeEfficiency(time, ImageAnalyser::computeGaussianBlur, image_mat);

    refreshImages();
    showEfficiency("GaussianBlur", time);
}

void MainWindow::on_btnSobel_clicked()
{
    resetBeforeOperationCheck();
    if (image_src.isNull())
    {
        return;
    }

    double time;
    image_mat = ImageAnalyser::computeEfficiency(time, ImageAnalyser::computeSobel, image_mat);

    refreshImages();
    showEfficiency("Sobel", time);
}


void MainWindow::on_btnLaplacian_clicked()
{
    resetBeforeOperationCheck();
    if (image_src.isNull())
    {
        return;
    }

    double time;
    image_mat = ImageAnalyser::computeEfficiency(time, ImageAnalyser::computeLaplacian, image_mat);

    refreshImages();
    showEfficiency("Laplacian", time);
}

void MainWindow::on_btnSGBMDisparity_clicked()
{
    resetBeforeOperationCheck();
    if (image_src.isNull())
    {
        return;
    }
    QImage img;
    ImageAnalyser::toQImage(image_mat, img);
    SGBMParamDialog dial(img);
    if(dial.exec() != QDialog::Rejected)
    {
        image_mat = dial.getMatResult().clone();
        double time = dial.getTimeResult();
        refreshImages();
        showEfficiency("SGBMDisparity", time);
    }
}

void MainWindow::on_btnBMDisparity_clicked()
{
    resetBeforeOperationCheck();
    if (image_src.isNull())
    {
        return;
    }
    QImage img;
    ImageAnalyser::toQImage(image_mat, img);
    BMParamDialog dial(img);
    if(dial.exec() != QDialog::Rejected)
    {
        image_mat = dial.getMatResult().clone();
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
