#include "subWindows/cameraparamdialog.h"
#include "ui_cameraparamdialog.h"

CameraParamDialog::CameraParamDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraParamDialog)
{
    ui->setupUi(this);
    setWindowTitle("Camera calibration");

    ui->print_images->hide();
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

void CameraParamDialog::refreshPrintMatrix()
{
    QString text;
    text += "width: ";
    text += QString::number(_img_size.width);
    text +="\nheight: ";
    text +=QString::number(_img_size.height);
    text += "\n";
    ui->print_images->setText(text);
    ui->print_cameramatrix->setText(QString::fromStdString(ProjectUtilities::matToString<double>(_camera_matrix)));
    ui->print_distcoeffs->setText(QString::fromStdString(ProjectUtilities::matToString<double>(_dist_coeffs)));
    std::string rvecs_print;
    for(cv::Mat mat : _rvecs)
    {
        rvecs_print+= ProjectUtilities::matToString<double>(mat) +"\n*********\n";
    }
    ui->print_rvecs->setText(QString::fromStdString(rvecs_print));

    std::string tvecs_print;
    for(cv::Mat mat : _tvecs)
    {
        tvecs_print+= ProjectUtilities::matToString<double>(mat) +"\n*********\n";
    }
    ui->print_tvecs->setText(QString::fromStdString(tvecs_print));
}

void CameraParamDialog::on_btn_openCamera_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this, "Open camera calibration file", "~/", tr("XML Files (*.xml)"), nullptr, QFileDialog::DontUseNativeDialog);

    // open image
    if(!filepath.isEmpty())
    {
        ProjectFiles::loadIntrinsicCamera(filepath.toStdString(), _img_size, _camera_matrix, _dist_coeffs, _rvecs, _tvecs);
        refreshPrintMatrix();
    }

    ui->print_images->show();
    ui->print_cameramatrix->show();
    ui->print_distcoeffs->show();
    ui->print_rvecs->show();
    ui->print_tvecs->show();
}

void CameraParamDialog::on_btn_calibrate_clicked()
{

    QString filepath = QFileDialog::getSaveFileName(this, "Open camera calibration file", "~/", tr("XML Files (*.xml)"), nullptr, QFileDialog::DontUseNativeDialog);

    if(!filepath.isEmpty() && !_vect_images.empty())
    {
        if(ui->cb_charuco->checkState() == Qt::Checked)
        {
            CameraCalibration::charucoCalibration(_vect_images, filepath.toStdString(), _camera_matrix, _dist_coeffs, _rvecs, _tvecs);
        }
        else
        {
            CameraCalibration::chessBoardCalibration(_vect_images, filepath.toStdString(), _camera_matrix, _dist_coeffs, _rvecs, _tvecs);
        }
        _img_size = _vect_images[0].size();
        ui->label_pathfile->setText(filepath);
        refreshPrintMatrix();
    }
}

void CameraParamDialog::on_btn_printimages_clicked()
{
    bool is_visible = ui->print_images->isVisible();
    ui->print_images->setVisible(!is_visible);
}

void CameraParamDialog::on_btn_cameramatrix_clicked()
{
    bool is_visible = ui->print_cameramatrix->isVisible();
    ui->print_cameramatrix->setVisible(!is_visible);
}

void CameraParamDialog::on_btn_distcoeff_clicked()
{
    bool is_visible = ui->print_distcoeffs->isVisible();
    ui->print_distcoeffs->setVisible(!is_visible);
}

void CameraParamDialog::on_btn_rvecs_clicked()
{
    bool is_visible = ui->print_rvecs->isVisible();
    ui->print_rvecs->setVisible(!is_visible);
}

void CameraParamDialog::on_btn_tvecs_clicked()
{
    bool is_visible = ui->print_tvecs->isVisible();
    ui->print_tvecs->setVisible(!is_visible);
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
            QLabel* label = new QLabel(QFileInfo(filepath).fileName(), ui->list_imgcalib);
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
    if(_current_img != -1)
    {
        cv::Mat tmp = _vect_images.at(_current_img).clone();
        CameraCalibration::applyUndistorded(tmp, tmp, _camera_matrix, _dist_coeffs);
        CVQTInterface::toQImage(tmp, _img_selection);
        refreshImages();
    }

}

void CameraParamDialog::on_btn_reset_clicked()
{
    if(_current_img != -1)
    {
        cv::Mat tmp = _vect_images.at(_current_img);
        CVQTInterface::toQImage(tmp, _img_selection);
        refreshImages();
    }
}

void CameraParamDialog::on_list_imgcalib_currentRowChanged(int currentRow)
{
    QImage img;
    if(currentRow >= _vect_images.size() || currentRow < 0 )
    {
        return;
    }
    _current_img = currentRow;
    cv::Mat tmp = _vect_images.at(_current_img);
    CVQTInterface::toQImage(tmp, _img_selection);
    refreshImages();

}

void CameraParamDialog::on_btn_clear_clicked()
{
    _current_img = -1;
    _vect_images.clear();
    ui->list_imgcalib->clear();
    _img_selection.fill(0);
    refreshImages();
}

