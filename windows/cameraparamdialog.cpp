#include "windows/cameraparamdialog.h"
#include "ui_cameraparamdialog.h"

using namespace cerutti;

CameraParamDialog::CameraParamDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraParamDialog)
{
    ui->setupUi(this);
    setWindowTitle("Camera calibration");

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
        ui->imgview_left->setPixmap(QPixmap::fromImage(_img_selection.scaled((int)(ui->imgview_left->width()*0.9),
                                                                        (int)(ui->imgview_left->height()*0.9),
                                                                        Qt::AspectRatioMode::KeepAspectRatio)));

        ui->imgview_left->setPixmap(QPixmap::fromImage(_img_selection.scaled((int)(ui->imgview_left->width()*0.9),
                                                                        (int)(ui->imgview_left->height()*0.9),
                                                                        Qt::AspectRatioMode::KeepAspectRatio)));
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
    text += QString::fromStdString(Utilities::matToString<double>(_camera_matrix));
    text += QString::fromStdString(Utilities::matToString<double>(_dist_coeffs));

    for(cv::Mat mat : _rvecs)
    {
        text += QString::fromStdString(Utilities::matToString<double>(mat) +"\n*********\n");
    }

    for(cv::Mat mat : _tvecs)
    {
        text+= QString::fromStdString(Utilities::matToString<double>(mat) +"\n*********\n");
    }

     ui->txt_infobox->setText(text);
}

void CameraParamDialog::on_btn_openCamera_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this, "Open camera calibration file", "~/", tr("XML Files (*.xml)"), nullptr, QFileDialog::DontUseNativeDialog);

    // open image
    if(!filepath.isEmpty())
    {
        //Files::loadIntrinsicCamera(filepath.toStdString(), _img_size, _camera_matrix, _dist_coeffs, _rvecs, _tvecs);
        refreshPrintMatrix();
        ui->label_pathfile->setText(filepath);
    }
}

void CameraParamDialog::on_btn_calibrate_clicked()
{

    QString filepath = QFileDialog::getSaveFileName(this, "Open camera calibration file", "~/", tr("XML Files (*.xml)"), nullptr, QFileDialog::DontUseNativeDialog);

    if(!filepath.isEmpty() && !_vect_images.empty())
    {

        //Calibration::chessBoardCalibration(_vect_images, filepath.toStdString(), _camera_matrix, _dist_coeffs, _rvecs, _tvecs);

        _img_size = _vect_images[0].size();
        ui->label_pathfile->setText(filepath);
        refreshPrintMatrix();
    }
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
        Calibration::StereoCamera calib = Calibration::StereoCamera("stereo_calibration.xml");
        //Calib::applyUndistorded(tmp, tmp, _camera_matrix, _dist_coeffs);
        //CVQTInterface::toQImage(tmp, _img_selection);
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

