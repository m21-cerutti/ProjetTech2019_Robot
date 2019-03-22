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
        on_actionOrigin_triggered();

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
        on_actionOrigin_triggered();
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

void MainWindow::on_actionLaplacian_triggered()
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

void MainWindow::on_actionGaussian_triggered()
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

void MainWindow::on_actionSobel_triggered()
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

void MainWindow::on_actionDisparityBM_triggered()
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

void MainWindow::on_actionDisparitySGBM_triggered()
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

void MainWindow::on_actionOrigin_triggered()
{
    if (_image_src.isNull())
    {
        return;
    }

    //Convert
    CVQTInterface::toMatCV(_image_src, _image_mat);

    refreshImages();
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
        Calibration::applyUndistorded(_image_mat, _image_mat, filename.toStdString());
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

    Calibration::chessBoardCalibration(vect_images, "quick_calibration.xml");
}

void MainWindow::on_actionCalibration_triggered()
{
    CameraParamDialog dial;
    dial.exec();
}


void MainWindow::on_actionCalibrationStereo_triggered()
{
    QString folder_set = QFileDialog::getExistingDirectory(this, "Open set folder", QString());

    // open image
    if(!folder_set.isEmpty())
    {
        std::vector<cv::Mat> vect_images_l, vect_images_r;

        CVQTInterface::getSetImagesStereo(folder_set, vect_images_l, vect_images_r );
        Calibration::stereoCalibration("stereo_calibration.xml", vect_images_l, vect_images_r, "l_calibration.xml", "r_calibration.xml");
    }
}

void MainWindow::on_actionCalibrationStereoVideo_triggered()
{
    QStringList filepaths = QFileDialog::getOpenFileNames(this, "Open video stero left-right", "~/", tr("Video Files (*.mp4)"), nullptr, QFileDialog::DontUseNativeDialog);

    // open image
    if(filepaths.size() == 2)
    {
        std::vector<cv::Mat> vect_images_l, vect_images_r ;
        CVQTInterface::stereoVideoExtraction(filepaths.at(0).toStdString(), filepaths.at(1).toStdString(), 100, -1, vect_images_r, vect_images_l, true);

        Calibration::stereoCalibration("stereo_calibration.xml", vect_images_l, vect_images_r, "l_calibration.xml", "r_calibration.xml");
    }
}


void MainWindow::on_actionExtractImages_triggered()
{
    QStringList filepaths = QFileDialog::getOpenFileNames(this, "Open video stero left-right", "~/", tr("Video Files (*.mp4)"), nullptr, QFileDialog::DontUseNativeDialog);

    // open image
    if(filepaths.size() == 2)
    {
        std::vector<cv::Mat> vect_images_left, vect_images_right;
        CVQTInterface::stereoVideoExtraction(filepaths.at(0).toStdString(), filepaths.at(1).toStdString(), 100, 25, vect_images_left, vect_images_right, true);
        Files::saveSetImages( "./setTest/stereo", vect_images_left, vect_images_right);
    }
}


void MainWindow::on_actionDepthBM_triggered()
{
    using namespace cv;

    QString folder_set = QFileDialog::getExistingDirectory(this, "Open set folder", QString());

    // open image
    if(!folder_set.isEmpty())
    {
        std::vector<cv::Mat> vect_images_l, vect_images_r;

        CVQTInterface::getSetImagesStereo(folder_set, vect_images_l, vect_images_r );

        cv::Mat Q, depth_map, disparity;
        cv::Ptr<cv::StereoBM> bmState;

        /*
        initUndistortRectifyMap(left_cam_matrix, left_dist_coeffs, R1, P1, frame_size,CV_32FC1, left_undist_rect_map_x, left_undist_rect_map_y);
        initUndistortRectifyMap(right_cam_matrix, right_dist_coeffs, R2, P2, frame_size, CV_32FC1, right_undist_rect_map_x, right_undist_rect_map_y);
        cv::remap(left_frame, left_undist_rect, left_undist_rect_map_x, left_undist_rect_map_y, CV_INTER_CUBIC, BORDER_CONSTANT, 0);
        cv::remap(right_frame, right_undist_rect, right_undist_rect_map_x, right_undist_rect_map_y, CV_INTER_CUBIC, BORDER_CONSTANT, 0);
        */

        QImage disp_q;
        cv::hconcat(vect_images_l.at(0), vect_images_r.at(0), disparity);
        CVQTInterface::toQImage(disparity, disp_q);
        BMParamDialog dial(disp_q);
        if(dial.exec() != QDialog::Rejected)
        {
            bmState = dial.getBMState();
            Files::getMatrixCalibrationFileStorage("stereo_calibration.xml", "Q", Q);

            for(int i =1; i<vect_images_r.size() && i<vect_images_l.size(); i++)
            {
                StereoMap::computeBMDisparityStereo(vect_images_l.at(i), vect_images_r.at(i), disparity, bmState);

                StereoMap::computeDepthMap(disparity, Q, depth_map);

                cv::threshold(depth_map, depth_map, -200, 0, CV_THRESH_TRUNC);

                double min;
                double max;
                cv::minMaxIdx(depth_map, &min, &max);
                cv::Mat adjMap;
                // expand your range to 0..255. Similar to histEq();
                float scale = 255 / (max-min);
                depth_map.convertTo(adjMap,CV_8UC1, scale, -min*scale);

                // this is great. It converts your grayscale image into a tone-mapped one,
                // much more pleasing for the eye
                // function is found in contrib module, so include contrib.hpp
                // and link accordingly
                cv::Mat falseColorsMap;
                applyColorMap(adjMap, falseColorsMap, cv::COLORMAP_HOT);
                Utilities::showMatrice("false"+std::to_string(i), falseColorsMap);
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

        cv::Mat Q, depth_map, disparity;
        cv::Ptr<cv::StereoSGBM> sgbmState;

        QImage disp_q;
        cv::hconcat(vect_images_l.at(0), vect_images_r.at(0), disparity);
        CVQTInterface::toQImage(disparity, disp_q);
        SGBMParamDialog dial(disp_q);
        if(dial.exec() != QDialog::Rejected)
        {
            sgbmState = dial.getSGBMState();
            Files::getMatrixCalibrationFileStorage("stereo_calibration.xml", "Q", Q);
            for(int i =1; i<vect_images_r.size() && i<vect_images_l.size(); i++)
            {
                StereoMap::computeSGBMDisparityStereo(vect_images_l.at(i), vect_images_r.at(i), disparity, sgbmState);
                StereoMap::computeDepthMap(disparity, Q, depth_map);

                cv::threshold(depth_map, depth_map, -200, 0, CV_THRESH_TRUNC);

                double min;
                double max;
                cv::minMaxIdx(depth_map, &min, &max);
                cv::Mat adjMap;
                // expand your range to 0..255. Similar to histEq();
                float scale = 255 / (max-min);
                depth_map.convertTo(adjMap,CV_8UC1, scale, -min*scale);

                // this is great. It converts your grayscale image into a tone-mapped one,
                // much more pleasing for the eye
                // function is found in contrib module, so include contrib.hpp
                // and link accordingly
                cv::Mat falseColorsMap;
                applyColorMap(adjMap, falseColorsMap, cv::COLORMAP_HOT);
                Utilities::showMatrice("false"+std::to_string(i), falseColorsMap);
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
        CVQTInterface::stereoVideoExtraction(filepaths.at(0).toStdString(), filepaths.at(1).toStdString(), 100, 20, vect_images_r, vect_images_l, false);

        cv::Mat Q, depth_map, disparity;
        cv::Ptr<cv::StereoBM> bmState;

        QImage disp_q;
        cv::hconcat(vect_images_l.at(0), vect_images_r.at(0), disparity);
        CVQTInterface::toQImage(disparity, disp_q);
        BMParamDialog dial(disp_q);
        if(dial.exec() != QDialog::Rejected)
        {
            bmState = dial.getBMState();
            Files::getMatrixCalibrationFileStorage("stereo_calibration.xml", "Q", Q);
            for(int i =1; i<vect_images_r.size() && i<vect_images_l.size(); i++)
            {
                StereoMap::computeBMDisparityStereo(vect_images_l.at(i), vect_images_r.at(i), disparity, bmState);
                StereoMap::computeDepthMap(disparity, Q, depth_map);



                Utilities::showMatrice(std::to_string(i), depth_map);
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
        CVQTInterface::stereoVideoExtraction(filepaths.at(0).toStdString(), filepaths.at(1).toStdString(), 100, 20, vect_images_r, vect_images_l, false);

        cv::Mat Q, depth_map, disparity;
        cv::Ptr<cv::StereoSGBM> sgbmState;


        QImage disp_q;
        cv::hconcat(vect_images_r.at(0), vect_images_l.at(0), disparity);
        CVQTInterface::toQImage(disparity, disp_q);
        SGBMParamDialog dial(disp_q);
        if(dial.exec() != QDialog::Rejected)
        {
            sgbmState = dial.getSGBMState();

            Files::getMatrixCalibrationFileStorage("stereo_calibration.xml", "Q", Q);

            for(int i =1; i<vect_images_r.size() && i<vect_images_l.size(); i++)
            {
                StereoMap::computeSGBMDisparityStereo(vect_images_l.at(i), vect_images_r.at(i), disparity, sgbmState);
                StereoMap::computeDepthMap( disparity, Q, depth_map);
            }
        }
    }
}
