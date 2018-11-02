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

void MainWindow::showMatrice(cv::Mat mat)
{
    if(mat.empty())
    {
        return;
    }

    namedWindow("Matrice", cv::WINDOW_NORMAL);
    cv::imshow("Matrice", mat);
}

void MainWindow::chooseImage(){

    //get filename of image
    QString filename = QFileDialog::getOpenFileName(this, "Open Image", "~/", tr("Image Files (*.png *.jpg *.bmp *.jpeg)"),0, QFileDialog::DontUseNativeDialog);

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
        QImage img = ImageAnalyser::toQImage(image_mat);
        ui->imageLabelDst->setPixmap(QPixmap::fromImage(img.scaled(ui->boxDest->width()*0.9,
                                                                   ui->boxDest->height()*0.9,
                                                                   Qt::AspectRatioMode::KeepAspectRatio)));
    }
}

/***********************************************************************************/

void MainWindow::resizeEvent(QResizeEvent *event)
{
    refreshImages();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "About", "Project by : <ul><li>BANDET Alexis</li><li>CERUTTI Marc</li><li>VISTE Isak</li>");
}

void MainWindow::on_actionOpen_triggered()
{
    chooseImage();
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_btnOrigin_clicked()
{
    if (image_src.isNull())
    {
        return;
    }

    //Try for Convertion in mat
    image_mat = ImageAnalyser::toMatCV(image_src);

    refreshImages();
}

void MainWindow::on_btnShowMatrice_clicked()
{
    showMatrice(image_mat);
}


void MainWindow::on_btnLaplacian_clicked()
{
    if (image_src.isNull())
    {
        return;
    }
    else if (image_mat.empty())
    {
        image_mat = ImageAnalyser::toMatCV(image_src);
    }

    //Laplacian
    image_mat = ImageAnalyser::computeLaplacian(image_mat);

    refreshImages();

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

void MainWindow::on_btnTest_clicked()
{
    cv::Mat matL;
    cv::Mat matR;

    ImageAnalyser::separateImage(image_mat, matL, matR);

    MainWindow::showMatrice(matR);
}

void MainWindow::on_btnDisparity_clicked()
{
    if (image_src.isNull())
    {
        return;
    }
    DephtMapParamDialog dial(image_src);
    dial.exec();
    image_mat = dial.getMatResult().clone();
    refreshImages();
}

void MainWindow::on_btnDisparitySimple_clicked()
{
    if (image_src.isNull())
    {
        return;
    }
    image_mat = ImageAnalyser::computeDepthMap(image_mat);
    refreshImages();
}

void MainWindow::on_actionLaplacian_triggered()
{
    on_btnLaplacian_clicked();
}

void MainWindow::on_actionSimple_triggered()
{
    on_btnDisparitySimple_clicked();
}

void MainWindow::on_actionAdvanced_triggered()
{
    on_btnDisparity_clicked();
}

void MainWindow::on_actionShow_mat_triggered()
{
    on_btnShowMatrice_clicked();
}

void MainWindow::on_actionOrigin_triggered()
{
    on_btnOrigin_clicked();
}
