#include "cameraparamdialog.h"
#include "ui_cameraparamdialog.h"

CameraParamDialog::CameraParamDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraParamDialog)
{
    ui->setupUi(this);
    setWindowTitle("Camera calibration");

    ui->print_cameramatrix->hide();
    ui->print_distcoeffs->hide();
    ui->print_rvecs->hide();
    ui->print_tvecs->hide();

}

CameraParamDialog::~CameraParamDialog()
{
    delete ui;
}

void CameraParamDialog::resizeEvent(QResizeEvent *event)
{
    refreshImages();
}

void CameraParamDialog::refreshImages()
{
    if (!_img_selection.isNull())
    {
        ui->imgview->setPixmap(QPixmap::fromImage(_img_selection.scaled((int)(ui->imgview->width()*0.9),                                                                     (int)(ui->imgview->height()*0.9),                                                                     Qt::AspectRatioMode::KeepAspectRatio)));
    }

}

void CameraParamDialog::on_btn_openCamera_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this, "Open camera calibration file", "~/", tr("XML Files (*.xml)"), nullptr, QFileDialog::DontUseNativeDialog);

    // open image
    if(!filepath.isEmpty())
    {
        CameraCalibration::loadCameraParemeters(filepath.toStdString(), _camera_matrix, _dist_coeffs, _rvecs, _tvecs);

        ui->print_cameramatrix->setText(QString::fromStdString(ProjectDebuger::matToString<double>(_camera_matrix)));

    }

        ui->print_cameramatrix->show();
        ui->print_distcoeffs->show();
        ui->print_rvecs->show();
        ui->print_tvecs->show();
}




void CameraParamDialog::on_btn_calibrate_clicked()
{
    /*
    if(ui->cb_charuco->checkState() == Qt::Checked)
    {
        CameraCalibration::calibrateFromImages(vect_images, CameraCalibration::DEFAULT_CAMERA_PATH(), CameraCalibration::MODE_CALIBRATION::Charuco);
    }
    else
    {
        CameraCalibration::calibrateFromImages(vect_images, CameraCalibration::DEFAULT_CAMERA_PATH(), CameraCalibration::MODE_CALIBRATION::Chessboard);
    }
    */
}


void CameraParamDialog::on_btn_cameramatrix_clicked()
{

}

void CameraParamDialog::on_btn_distcoeff_clicked()
{

}

void CameraParamDialog::on_btn_rvecs_clicked()
{

}

void CameraParamDialog::on_btn_tvecs_clicked()
{

}

void CameraParamDialog::on_btn_openImages_clicked()
{
    QStringList filepaths = QFileDialog::getOpenFileNames(this, "Open Folder Image", "~/", tr("Image Files (*.GIF *.png *.jpg *.bmp *.jpeg)"), nullptr, QFileDialog::DontUseNativeDialog);


    for(QString filepath : filepaths)
    {
        QImage img;
        if(img.load(filepath)){

            QListWidgetItem* item = new QListWidgetItem(ui->list_imgcalib);
            ui->list_imgcalib->addItem(item);
            QLabel* label = new QLabel(QFileInfo(filepath).fileName());
            item->setSizeHint(label->minimumSizeHint());
            ui->list_imgcalib->setItemWidget(item, label);

            cv::Mat tmp;
            CVQTInterface::toMatCV(img, tmp);
            _vect_images.push_back(tmp);
        }
    }
}

void CameraParamDialog::on_btn_apply_clicked()
{

}

void CameraParamDialog::on_btn_reset_clicked()
{
    refreshImages();
}

void CameraParamDialog::on_list_imgcalib_currentRowChanged(int currentRow)
{
    QImage img;
    _current_img = currentRow;
    cv::Mat tmp = _vect_images.at(_current_img);
    if (!tmp.empty())
        CVQTInterface::toQImage(tmp, _img_selection);
    on_btn_reset_clicked();


}
