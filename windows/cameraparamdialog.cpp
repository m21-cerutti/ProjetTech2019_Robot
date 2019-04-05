#include "windows/cameraparamdialog.h"
#include "ui_cameraparamdialog.h"

using namespace cerutti;

CameraParamDialog::CameraParamDialog(QWidget *parent) :
    QDialog(parent),
    calib(Calibration::StereoCamera(DEFAULT_FILE_STEREO)),
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
    if (!_img_selection_left.isNull())
    {
        ui->imgview_left->setPixmap(QPixmap::fromImage(_img_selection_left.scaled((int)(ui->imgview_left->width()*0.9),
                                                                                  (int)(ui->imgview_left->height()*0.9),
                                                                                  Qt::AspectRatioMode::KeepAspectRatio)));
    }
    if (!_img_selection_right.isNull())
    {

        ui->imgview_right->setPixmap(QPixmap::fromImage(_img_selection_right.scaled((int)(ui->imgview_right->width()*0.9),
                                                                                    (int)(ui->imgview_right->height()*0.9),
                                                                                    Qt::AspectRatioMode::KeepAspectRatio)));
    }
}

void CameraParamDialog::on_btn_save_clicked()
{
    QFile file(ui->label_pathfile->text());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        Utilities::messageDebug("Can't open file calib for write.");
        return;
    }

    QTextStream out(&file);
    out << ui->txt_infobox->toPlainText();
}

void CameraParamDialog::refreshPrintMatrix()
{
    QFile file(ui->label_pathfile->text());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Utilities::messageDebug("Can't open file calib for read.");
        return;
    }

    QTextStream in(&file);
    QString text = in.readAll();

    ui->txt_infobox->setText(text);
}

void CameraParamDialog::on_btn_openCamera_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this, "Open camera calibration file", "~/", tr("XML Files (*.xml)"), nullptr, QFileDialog::DontUseNativeDialog);

    // open image
    if(!filepath.isEmpty())
    {
        calib.load(filepath.toStdString());
        ui->label_pathfile->setText(filepath);
        refreshPrintMatrix();
    }
}

void CameraParamDialog::on_btn_calibrate_clicked()
{
    calib.calibrate(_vect_images_left, _vect_images_right);
    calib.save();
    refreshPrintMatrix();
}

void CameraParamDialog::on_btn_openImages_clicked()
{
    QStringList filepaths = QFileDialog::getOpenFileNames(this, "Open images left", "~/", tr("Image Files (*.GIF *.png *.jpg *.bmp *.jpeg)"), nullptr, QFileDialog::DontUseNativeDialog);

    for(QString filepath : filepaths)
    {
        QImage img;
        if(img.load(filepath)){

            QListWidgetItem* item = new QListWidgetItem(ui->list_imgcalib_left);
            ui->list_imgcalib_left->addItem(item);
            QLabel* label = new QLabel(QFileInfo(filepath).fileName(), ui->list_imgcalib_left);
            item->setSizeHint(label->minimumSizeHint());
            ui->list_imgcalib_left->setItemWidget(item, label);

            cv::Mat tmp;
            CVQTInterface::toMatCV(img, tmp);
            _vect_images_left.push_back(tmp);
        }
    }

    filepaths = QFileDialog::getOpenFileNames(this, "Open images right", "~/", tr("Image Files (*.GIF *.png *.jpg *.bmp *.jpeg)"), nullptr, QFileDialog::DontUseNativeDialog);

    for(QString filepath : filepaths)
    {
        QImage img;
        if(img.load(filepath)){

            QListWidgetItem* item = new QListWidgetItem(ui->list_imgcalib_right);
            ui->list_imgcalib_right->addItem(item);
            QLabel* label = new QLabel(QFileInfo(filepath).fileName(), ui->list_imgcalib_right);
            item->setSizeHint(label->minimumSizeHint());
            ui->list_imgcalib_right->setItemWidget(item, label);

            cv::Mat tmp;
            CVQTInterface::toMatCV(img, tmp);
            _vect_images_right.push_back(tmp);
        }
    }
}

void CameraParamDialog::on_btn_apply_clicked()
{
    if((_current_img_left != -1) && (_current_img_right!= -1))
    {
        cv::Mat tmp_l = _vect_images_left.at(_current_img_left).clone();
        cv::Mat tmp_r = _vect_images_right.at(_current_img_right).clone();
        calib.undistord(tmp_l, tmp_r);
        CVQTInterface::toQImage(tmp_l, _img_selection_left);
        CVQTInterface::toQImage(tmp_r, _img_selection_right);
        refreshImages();
    }

}

void CameraParamDialog::on_btn_reset_clicked()
{
    if(_current_img_left != -1)
    {
        cv::Mat tmp = _vect_images_left.at(_current_img_left);
        CVQTInterface::toQImage(tmp, _img_selection_left);

    }
    if(_current_img_right != -1)
    {
        cv::Mat tmp = _vect_images_right.at(_current_img_right);
        CVQTInterface::toQImage(tmp, _img_selection_right);
    }
    refreshImages();
}

void CameraParamDialog::on_list_imgcalib_left_currentRowChanged(int currentRow)
{
    if(currentRow >= _vect_images_left.size() || currentRow < 0 )
    {
        return;
    }
    _current_img_left = currentRow;
    cv::Mat tmp = _vect_images_left.at(_current_img_left);
    CVQTInterface::toQImage(tmp, _img_selection_left);
    refreshImages();
}


void CameraParamDialog::on_list_imgcalib_right_currentRowChanged(int currentRow)
{
    if(currentRow >= _vect_images_right.size() || currentRow < 0 )
    {
        return;
    }
    _current_img_right = currentRow;
    cv::Mat tmp = _vect_images_right.at(_current_img_right);
    CVQTInterface::toQImage(tmp, _img_selection_right);
    refreshImages();
}

void CameraParamDialog::on_btn_clear_clicked()
{
    _current_img_left = -1;
    _vect_images_left.clear();
    ui->list_imgcalib_left->clear();
    _img_selection_left.fill(0);

    _current_img_right = -1;
    _vect_images_right.clear();
    ui->list_imgcalib_right->clear();
    _img_selection_right.fill(0);

    refreshImages();
}

