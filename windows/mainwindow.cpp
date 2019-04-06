#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow() :
    QMainWindow(nullptr),
    ui(new Ui::MainWindow)
{
    //Setup and initiate elements in windows
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    //Desallocate widgets in windows
    delete ui;

    //Use to delete windows create by cv
    destroyAllWindows();
}

void MainWindow::pickImages(std::vector<Mat>& vect_images)
{
    QStringList images = QFileDialog::getOpenFileNames(this, "Get one image stereo or left and right", "~/", tr("Image Files (*.GIF *.png *.jpg *.bmp *.jpeg)"), nullptr, QFileDialog::DontUseNativeDialog);

    for(QString filename : images)
    {
        Mat tmp = imread(filename.toStdString());
        vect_images.push_back(tmp);
    }
}

/***********************************************************************************/

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionDisparityBM_triggered()
{
    std::vector<Mat> vect_images;
    pickImages(vect_images);

    if(vect_images.size() ==0 ) return;

    Mat left, right;
    if(vect_images.size() == 1)
    {
        Filters::separateImage(vect_images.at(0), left, right);
    }
    else
    {
        left = vect_images.at(0);
        right = vect_images.at(1);
    }

    BMParamDialog dial(left, right);
    dial.exec();

}

void MainWindow::on_actionDisparitySGBM_triggered()
{
    std::vector<Mat> vect_images;
    pickImages(vect_images);

    if(vect_images.size() ==0 ) return;

    Mat left, right;
    if(vect_images.size() == 1)
    {
        Filters::separateImage(vect_images.at(0), left, right);
    }
    else
    {
        left = vect_images.at(0);
        right = vect_images.at(1);
    }

    SGBMParamDialog dial(left, right);
    dial.exec();
}

void MainWindow::on_actionCalibration_triggered()
{
    CameraParamDialog dial;
    dial.exec();
}


void MainWindow::on_actionCalibrationStereo_triggered()
{
    std::vector<Mat> vect_images_l, vect_images_r;

    // open set
    if(CVQTInterface::getSetImagesStereo(vect_images_l, vect_images_r ))
    {
        Calibration::StereoCamera calib;
        calib.calibrate(vect_images_l, vect_images_r);
    }
}

void MainWindow::on_actionCalibrationStereoVideo_triggered()
{
    QStringList filepaths = QFileDialog::getOpenFileNames(this, "Open video stero left-right", "~/", tr("Video Files (*.mp4)"), nullptr, QFileDialog::DontUseNativeDialog);

    // open image
    if(filepaths.size() == 2)
    {
        std::vector<Mat> vect_images_l, vect_images_r ;
        CVQTInterface::stereoVideoExtraction(filepaths.at(0).toStdString(), filepaths.at(1).toStdString(), 100, -1, vect_images_l, vect_images_r, true);
        Calibration::StereoCamera calib;
        calib.calibrate(vect_images_l, vect_images_r);
    }
}


void MainWindow::on_actionExtractImages_triggered()
{
    QStringList filepaths = QFileDialog::getOpenFileNames(this, "Open video stero left-right", "~/", tr("Video Files (*.mp4)"), nullptr, QFileDialog::DontUseNativeDialog);

    // open image
    if(filepaths.size() == 2)
    {
        std::vector<Mat> vect_images_left, vect_images_right;
        CVQTInterface::stereoVideoExtraction(filepaths.at(0).toStdString(), filepaths.at(1).toStdString(), DEFAULT_BEGIN_FRAME_EXTRACT, DEFAULT_NB_FRAME_EXTRACT,
                                             vect_images_left, vect_images_right, true);
        CVQTInterface::saveSetImagesStereo("./setTest", vect_images_left, vect_images_right);
    }
}


void MainWindow::on_actionDepthBM_triggered()
{
    using namespace cv;

    Calibration::StereoCamera calib;
    std::vector<Mat> vect_images_l, vect_images_r;

    // open set
    if(CVQTInterface::getSetImagesStereo(vect_images_l, vect_images_r ))
    {
        Ptr<StereoBM> bmState;

        Mat undist_left, undist_right, disparity, depth_map;

        //UNDISTORD
        undistort(vect_images_l.at(0), undist_left, calib.camera_matrix_l, calib.dist_coeffs_l);
        undistort(vect_images_r.at(0), undist_right, calib.camera_matrix_r, calib.dist_coeffs_r);

        BMParamDialog dial(undist_left, undist_right);
        if(dial.exec() != QDialog::Rejected)
        {
            bmState = dial.getBMState();

            for(int i =1; i<vect_images_l.size() && i<vect_images_r.size(); i++)
            {
                //UNDISTORD
                undistort(vect_images_l.at(i), undist_left, calib.camera_matrix_l, calib.dist_coeffs_l);
                undistort(vect_images_r.at(i), undist_right, calib.camera_matrix_r, calib.dist_coeffs_r);

                StereoMap::computeBMDisparity(undist_left, undist_right, disparity, bmState);
                StereoMap::computeDepthMap(disparity, calib.Q, depth_map, THRESHOLD_MIN, THRESHOLD_MAX);

                //threshold(depth_map, depth_map, -150, -20, CV_THRESH_TRUNC);

                double min;
                double max;
                minMaxIdx(depth_map, &min, &max);
                Mat adjMap;
                // expand your range to 0..255. Similar to histEq();
                float scale = 255 / (max-min);
                depth_map.convertTo(adjMap,CV_8UC1, scale, -min*scale);

                // this is great. It converts your grayscale image into a tone-mapped one,
                // much more pleasing for the eye
                // function is found in contrib module, so include contrib.hpp
                // and link accordingly
                Mat falseColorsMap;
                applyColorMap(adjMap, falseColorsMap, COLORMAP_HOT);
                Utilities::showMatrice("false"+std::to_string(i), falseColorsMap);
            }
        }
    }
}

void MainWindow::on_actionDepthSGBM_triggered()
{
    using namespace cv;

    Calibration::StereoCamera calib;
    std::vector<Mat> vect_images_l, vect_images_r;

    // open set
    if(CVQTInterface::getSetImagesStereo(vect_images_l, vect_images_r ))
    {

        Mat depth_map, disparity;
        Ptr<StereoSGBM> sgbmState;

        Mat undist_left, undist_right;
        //UNDISTORD
        undistort(vect_images_l.at(0), undist_left, calib.camera_matrix_l, calib.dist_coeffs_l);
        undistort(vect_images_r.at(0), undist_right, calib.camera_matrix_r, calib.dist_coeffs_r);

        SGBMParamDialog dial(undist_left, undist_right);
        if(dial.exec() != QDialog::Rejected)
        {
            sgbmState = dial.getSGBMState();
            for(int i =1; i<vect_images_l.size() && i<vect_images_r.size(); i++)
            {
                //UNDISTORD
                undistort(vect_images_l.at(i), undist_left, calib.camera_matrix_l, calib.dist_coeffs_l);
                undistort(vect_images_r.at(i), undist_right, calib.camera_matrix_r, calib.dist_coeffs_r);

                StereoMap::computeSGBMDisparity(undist_left, undist_right, disparity, sgbmState);

                StereoMap::computeDepthMap(disparity, calib.Q, depth_map, THRESHOLD_MIN, THRESHOLD_MAX);

                double min;
                double max;
                minMaxIdx(depth_map, &min, &max);
                Mat adjMap;
                // expand your range to 0..255. Similar to histEq();
                float scale = 255 / (max-min);
                depth_map.convertTo(adjMap,CV_8UC1, scale, -min*scale);

                // this is great. It converts your grayscale image into a tone-mapped one,
                // much more pleasing for the eye
                // function is found in contrib module, so include contrib.hpp
                // and link accordingly
                Mat falseColorsMap;
                applyColorMap(adjMap, falseColorsMap, COLORMAP_HOT);
                Utilities::showMatrice("false"+std::to_string(i), falseColorsMap);
            }
        }
    }
}

void MainWindow::on_actionTest_controller_triggered()
{
    std::vector<Mat> vect_images_l, vect_images_r;

    // open set
    if(CVQTInterface::getSetImagesStereo(vect_images_l, vect_images_r ))
    {
        cerutti::CustomController cont = cerutti::CustomController();
        cont.load();
        float vx, vy, omega;
        for(int i =0; i< vect_images_l.size(); i++)
        {
            cont.process(vect_images_l.at(i), vect_images_r.at(i), &vx, &vy, &omega);
            Utilities::messageDebug("End frame " +std::to_string(i), false);
            Utilities::messageDebug(std::to_string(vx)+";"+std::to_string(vy)+";"+std::to_string(omega), false);
        }
    }
}

void MainWindow::on_actionOpen_filters_triggered()
{
    std::vector<Mat> vect_images;
    pickImages(vect_images);
    if(vect_images.size() == 1)
    {
        SingleImageDialog dial(vect_images.at(0));
        dial.exec();
    }
}

void MainWindow::on_actionOpen_simulation_unity_triggered()
{
    ServerMaster server;
    server.StartServer();
}
