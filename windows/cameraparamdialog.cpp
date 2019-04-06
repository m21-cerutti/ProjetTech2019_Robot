#include "windows/cameraparamdialog.h"
#include "ui_cameraparamdialog.h"

using namespace cerutti;

CameraParamDialog::CameraParamDialog(QWidget *parent) :
    QDialog(parent),
    calib(Calibration::StereoCamera()),
    ui(new Ui::CameraParamDialog)
{
    ui->setupUi(this);
    setWindowTitle("Camera calibration");
    setWindowFlag(Qt::WindowMaximizeButtonHint);
    refreshPrintMatrix();
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
    if (!img_selection_left.isNull())
    {
        ui->imgview_left->setPixmap(QPixmap::fromImage(img_selection_left.scaled((int)(ui->imgview_left->width()*0.9),
                                                                                 (int)(ui->imgview_left->height()*0.9),
                                                                                 Qt::AspectRatioMode::KeepAspectRatio)));
    }
    if (!img_selection_right.isNull())
    {

        ui->imgview_right->setPixmap(QPixmap::fromImage(img_selection_right.scaled((int)(ui->imgview_right->width()*0.9),
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
    Utilities::messageDebug("Save text edit.", false);
}

void CameraParamDialog::refreshPrintMatrix()
{
    QFile file(QString::fromStdString(calib.file_path));
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
    calib.calibrate(vect_images_left, vect_images_right);
    refreshPrintMatrix();
}

void CameraParamDialog::on_btn_openImages_clicked()
{
    // open set
    if(!CVQTInterface::getSetImagesStereo(vect_images_left, vect_images_right ))
    {
        return;
    }

    auto it_left = vect_images_left.begin();
    auto it_right = vect_images_right.begin();
    int i = 0;
    for(;it_left< vect_images_left.end() && it_right<vect_images_right.end() ; it_left++, it_right++ )
    {
        if(!it_left->empty() && !it_right->empty()){

            QListWidgetItem* item = new QListWidgetItem(ui->list_imgcalib);
            ui->list_imgcalib->addItem(item);

            QString path_name = QString::number(i);
            QLabel* label = new QLabel(path_name, ui->list_imgcalib);
            item->setSizeHint(label->minimumSizeHint());
            ui->list_imgcalib->setItemWidget(item, label);
            i++;
        }
    }
}

void CameraParamDialog::on_btn_apply_clicked()
{
    if((current_img != -1) && (current_img!= -1))
    {
        cv::Mat tmp_l = vect_images_left.at(current_img).clone();
        cv::Mat tmp_r = vect_images_right.at(current_img).clone();

        undistort(tmp_l, tmp_l, calib.camera_matrix_l, calib.dist_coeffs_l);
        undistort(tmp_r, tmp_r, calib.camera_matrix_r, calib.dist_coeffs_r);

        CVQTInterface::toQImage(tmp_l, img_selection_left);
        CVQTInterface::toQImage(tmp_r, img_selection_right);
        refreshImages();
    }

}

void CameraParamDialog::on_btn_reset_clicked()
{
    if(current_img != -1)
    {
        cv::Mat tmp = vect_images_left.at(current_img);
        CVQTInterface::toQImage(tmp, img_selection_left);

        cv::Mat tmp2 = vect_images_right.at(current_img);
        CVQTInterface::toQImage(tmp2, img_selection_right);
    }
    refreshImages();
}

void CameraParamDialog::on_list_imgcalib_currentRowChanged(int currentRow)
{
    if(currentRow >= vect_images_left.size() || currentRow < 0 )
    {
        return;
    }
    current_img = currentRow;
    cv::Mat tmp = vect_images_left.at(current_img);
    CVQTInterface::toQImage(tmp, img_selection_left);
    cv::Mat tmp2 = vect_images_right.at(current_img);
    CVQTInterface::toQImage(tmp2, img_selection_right);
    refreshImages();
}

void CameraParamDialog::on_btn_clear_clicked()
{
    current_img = -1;
    vect_images_left.clear();
    img_selection_left.fill(0);
    vect_images_right.clear();
    img_selection_right.fill(0);

    ui->list_imgcalib->clear();

    refreshImages();
}


void CameraParamDialog::on_btn_remove_clicked()
{
    if(current_img != -1)
    {
        auto it = vect_images_left.begin() + current_img;
        vect_images_left.erase(it);
        img_selection_left.fill(0);

        it = vect_images_right.begin() + current_img;
        vect_images_right.erase(it);
        img_selection_right.fill(0);

        QListWidgetItem *item = ui->list_imgcalib->takeItem(current_img);
        delete item;

        current_img = -1;
    }
    refreshImages();
}

void CameraParamDialog::on_btn_saveset_clicked()
{
    CVQTInterface::saveSetImagesStereo("./Sets/calibration", vect_images_left, vect_images_right);
}
