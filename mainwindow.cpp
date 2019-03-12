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

        //Create default matrix
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
    time = ProjectUtilities::computeEfficiency(ImageFilter::computeGaussianBlur, _image_mat, _image_mat);

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
    time = ProjectUtilities::computeEfficiency(ImageFilter::computeGradient, _image_mat, _image_mat);

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
    time = ProjectUtilities::computeEfficiency(ImageFilter::computeLaplacian, _image_mat, _image_mat);

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

void MainWindow::on_actionDisparityBM_triggered()
{
    on_btnBMDisparity_clicked();
}

void MainWindow::on_actionDisparitySGBM_triggered()
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
        CameraCalibration::applyUndistorded(_image_mat, _image_mat, filename.toStdString());
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

    CameraCalibration::chessBoardCalibration(vect_images, "quick_calibration.xml");
}

void MainWindow::on_actionCalibration_triggered()
{
    CameraParamDialog dial;
    dial.exec();
}

void MainWindow::on_actionOpen_video_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open video file", "~/", tr("Video Files (*.mp4)"), nullptr, QFileDialog::DontUseNativeDialog);

    // open image
    if(!filename.isEmpty())
    {
        VideoExtractor::filterVideo(filename.toStdString(), ImageFilter::computeGradient);
    }
}

void MainWindow::on_actionChessboard_debug_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open video file", "~/", tr("Video Files (*.mp4)"), nullptr, QFileDialog::DontUseNativeDialog);

    // open image
    if(!filename.isEmpty())
    {
        VideoExtractor::videoChessDebug(filename.toStdString());
    }
}


void MainWindow::on_actionCalibrationStereo_triggered()
{
    QString folder_set = QFileDialog::getExistingDirectory(this, "Open set folder", QString());

    // open image
    if(!folder_set.isEmpty())
    {
        std::vector<cv::Mat> vect_images_l, vect_images_r;

        CVQTInterface::getSetImagesStereo(folder_set, vect_images_l, vect_images_r );
        CameraCalibration::stereoCalibration("stereo_calibration.xml", vect_images_l, vect_images_r, "l_calibration.xml", "r_calibration.xml");
    }
}

void MainWindow::on_actionCalibrationStereoVideo_triggered()
{
    QStringList filepaths = QFileDialog::getOpenFileNames(this, "Open video stero left-right", "~/", tr("Video Files (*.mp4)"), nullptr, QFileDialog::DontUseNativeDialog);

    // open image
    if(filepaths.size() == 2)
    {
        std::vector<cv::Mat> vect_images_l, vect_images_r ;
        VideoExtractor::stereoVideoExtraction(filepaths.at(0).toStdString(), filepaths.at(1).toStdString(), 100, -1, vect_images_r, vect_images_l, true);

        CameraCalibration::stereoCalibration("stereo_calibration.xml", vect_images_l, vect_images_r, "l_calibration.xml", "r_calibration.xml");
    }
}


void MainWindow::on_actionExtractImages_triggered()
{
    QStringList filepaths = QFileDialog::getOpenFileNames(this, "Open video stero left-right", "~/", tr("Video Files (*.mp4)"), nullptr, QFileDialog::DontUseNativeDialog);

    // open image
    if(filepaths.size() == 2)
    {
        std::vector<cv::Mat> vect_images_left, vect_images_right;
        VideoExtractor::stereoVideoExtraction(filepaths.at(0).toStdString(), filepaths.at(1).toStdString(), 100, 25, vect_images_left, vect_images_right, true);
        ProjectFiles::saveSetImages( "./setTest/stereo", vect_images_left, vect_images_right);
    }
}


void MainWindow::on_actionDepthBM_triggered()
{
    QString folder_set = QFileDialog::getExistingDirectory(this, "Open set folder", QString());

    // open image
    if(!folder_set.isEmpty())
    {
        std::vector<cv::Mat> vect_images_l, vect_images_r;

        CVQTInterface::getSetImagesStereo(folder_set, vect_images_l, vect_images_r );

        cv::Mat Q, reproj, disparity;
        cv::Ptr<cv::StereoBM> bmState;

        QImage disp_q;
        cv::hconcat(vect_images_l.at(0), vect_images_r.at(0), disparity);
        CVQTInterface::toQImage(disparity, disp_q);
        BMParamDialog dial(disp_q);
        if(dial.exec() != QDialog::Rejected)
        {
            bmState = dial.getBMState();
            ProjectFiles::getMatrixCalibrationFileStorage("stereo_calibration.xml", "Q", Q);

            cv::Mat tmp;
            Q.convertTo(Q, CV_32F);
            qDebug()<<"Q\n"<< (Q.type()==CV_32F) << ProjectUtilities::matToString<double>(Q).c_str();

            for(int i =1; i<vect_images_r.size() && i<vect_images_l.size(); i++)
            {
                StereoMap::computeBMDisparityStereo(vect_images_l.at(i), vect_images_r.at(i), disparity, bmState);

                StereoMap::computeDepthMap(disparity, Q, reproj);


                qDebug() << i << ">>>>>>>>>>>>>\n";
                qDebug() << QString(ProjectUtilities::matToString<float>(reproj).c_str());
                qDebug() << "End "<< i<<">>>>>>>>>>>>>\n";

                /*
                double min;
                double max;

                cv::minMaxIdx(reproj, &min, &max);
                cv::Mat adjMap;
                // expand your range to 0..255. Similar to histEq();
                float scale = 255 / (max-min);
                reproj.convertTo(adjMap, CV_8UC1, scale, -min*scale);

                // this is great. It converts your grayscale image into a tone-mapped one,
                // much more pleasing for the eye
                // function is found in contrib module, so include contrib.hpp
                // and link accordingly
                cv::Mat falseColorsMap;
                applyColorMap(adjMap, falseColorsMap, cv::COLORMAP_AUTUMN);
                               //ProjectUtilities::showMatrice("disparity_"+std::to_string(i), disparity);
                ProjectUtilities::showMatrice(std::to_string(i), falseColorsMap);
                */
                ProjectUtilities::showMatrice(std::to_string(i), reproj);

            }
        }
    }
}

void MainWindow::on_actionDepthSGBM_triggered()
{
    QString folder_set = QFileDialog::getExistingDirectory(this, "Open set folder", QString());

    // open image
    if(!folder_set.isEmpty())
    {
        std::vector<cv::Mat> vect_images_l, vect_images_r;

        CVQTInterface::getSetImagesStereo(folder_set, vect_images_l, vect_images_r );

        cv::Mat Q, reproj, disparity;
        cv::Ptr<cv::StereoSGBM> sgbmState;

        QImage disp_q;
        cv::hconcat(vect_images_l.at(0), vect_images_r.at(0), disparity);
        CVQTInterface::toQImage(disparity, disp_q);
        SGBMParamDialog dial(disp_q);
        if(dial.exec() != QDialog::Rejected)
        {
            sgbmState = dial.getSGBMState();
            ProjectFiles::getMatrixCalibrationFileStorage("stereo_calibration.xml", "Q", Q);
            for(int i =1; i<vect_images_r.size() && i<vect_images_l.size(); i++)
            {
                StereoMap::computeSGBMDisparityStereo(vect_images_l.at(i), vect_images_r.at(i), disparity, sgbmState);
                ProjectUtilities::showMatrice("disparity_"+std::to_string(i), disparity);
                StereoMap::computeDepthMap(disparity, Q, reproj);
                ProjectUtilities::showMatrice(std::to_string(i), reproj);


                qDebug() << i << ">>>>>>>>>>>>>\n";
                qDebug() << QString(ProjectUtilities::matToString<float>(reproj).c_str());
                qDebug() << "End "<< i<<">>>>>>>>>>>>>\n";
            }
        }
    }
}


void MainWindow::on_actionDepthBMVideo_triggered()
{
    QStringList filepaths = QFileDialog::getOpenFileNames(this, "Open video stero left-right", "~/", tr("Video Files (*.mp4)"), nullptr, QFileDialog::DontUseNativeDialog);

    // open image
    if(filepaths.size() == 2)
    {
        std::vector<cv::Mat> vect_images_l, vect_images_r;
        VideoExtractor::stereoVideoExtraction(filepaths.at(0).toStdString(), filepaths.at(1).toStdString(), 100, 20, vect_images_r, vect_images_l, false);

        cv::Mat Q, reproj, disparity;
        cv::Ptr<cv::StereoBM> bmState;

        QImage disp_q;
        cv::hconcat(vect_images_l.at(0), vect_images_r.at(0), disparity);
        CVQTInterface::toQImage(disparity, disp_q);
        BMParamDialog dial(disp_q);
        if(dial.exec() != QDialog::Rejected)
        {
            bmState = dial.getBMState();
            ProjectFiles::getMatrixCalibrationFileStorage("stereo_calibration.xml", "Q", Q);
            for(int i =1; i<vect_images_r.size() && i<vect_images_l.size(); i++)
            {
                StereoMap::computeBMDisparityStereo(vect_images_l.at(i), vect_images_r.at(i), disparity, bmState);
                ProjectUtilities::showMatrice("disparity_"+std::to_string(i), disparity);
                StereoMap::computeDepthMap(disparity, Q, reproj);
                ProjectUtilities::showMatrice(std::to_string(i), reproj);

                qDebug() << i << ">>>>>>>>>>>>>\n";
                qDebug() << QString(ProjectUtilities::matToString<float>(reproj).c_str());
                qDebug() << "End "<< i<<">>>>>>>>>>>>>\n";
            }
        }
    }
}

void MainWindow::on_actionDepthSGBMVideo_triggered()
{
    QStringList filepaths = QFileDialog::getOpenFileNames(this, "Open video stero left-right", "~/", tr("Video Files (*.mp4)"), nullptr, QFileDialog::DontUseNativeDialog);

    // open image
    if(filepaths.size() == 2)
    {
        std::vector<cv::Mat> vect_images_r, vect_images_l;
        VideoExtractor::stereoVideoExtraction(filepaths.at(0).toStdString(), filepaths.at(1).toStdString(), 100, 20, vect_images_r, vect_images_l, false);

        cv::Mat Q, reproj, disparity;
        cv::Ptr<cv::StereoSGBM> sgbmState;


        QImage disp_q;
        cv::hconcat(vect_images_r.at(0), vect_images_l.at(0), disparity);
        CVQTInterface::toQImage(disparity, disp_q);
        SGBMParamDialog dial(disp_q);
        if(dial.exec() != QDialog::Rejected)
        {
            sgbmState = dial.getSGBMState();

            ProjectFiles::getMatrixCalibrationFileStorage("stereo_calibration.xml", "Q", Q);

            for(int i =1; i<vect_images_r.size() && i<vect_images_l.size(); i++)
            {
                StereoMap::computeSGBMDisparityStereo(vect_images_l.at(i), vect_images_r.at(i), disparity, sgbmState);
                ProjectUtilities::showMatrice("disparity_"+std::to_string(i), disparity);
                StereoMap::computeDepthMap( disparity, Q, reproj);
                ProjectUtilities::showMatrice(std::to_string(i), reproj);

                qDebug() << i << ">>>>>>>>>>>>>\n";
                qDebug() << QString(ProjectUtilities::matToString<float>(reproj).c_str());
                qDebug() << "End "<< i<<">>>>>>>>>>>>>\n";
            }
        }
    }
}


