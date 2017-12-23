#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "executethread.h"
#include <QFileDialog>
#include <QMimeData>
#include <QUrl>
#include <QStandardPaths>
#include <QDateTime>
#include <QMessageBox>

#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 文件列表
    m_mFiles = new QStringListModel(this);
    ui->listFIles->setModel(m_mFiles);
    // 扩展名
    QStringList list;
    list << "" << "png" << "jpg" << "bmp" << "gif" << "svg";
    ui->comboExtension->addItems(list);
    // 倍率
    ui->boxRate_1_0->setChecked(true);
    ui->boxRate_1_5->setChecked(true);
    ui->boxRate_2_0->setChecked(true);
    ui->boxRate_3_0->setChecked(false);
    ui->boxRate_4_0->setChecked(false);
    ui->boxRate_5_0->setChecked(false);
    // 尺寸（1.0x）
    m_gSize = new QButtonGroup(this);
    m_gSize->addButton(ui->radioSize_18_18);
    m_gSize->addButton(ui->radioSize_20_20);
    m_gSize->addButton(ui->radioSize_30_30);
    m_gSize->addButton(ui->radioSize_40_40);
    ui->radioSize_20_20->setChecked(true);
    ui->editWidth->setText("20");
    ui->editHeight->setText("20");
    // 保持比例
    m_gMatainAspect = new QButtonGroup(this);
    m_gMatainAspect->addButton(ui->radioMatainAspectYes);
    m_gMatainAspect->addButton(ui->radioMatainAspectNo);
    ui->radioMatainAspectYes->setChecked(true);
    // statusBar 进度展示
    m_pStatLabel = new QLabel(this);
    m_pStatProgress = new QProgressBar(this);
    m_pStatLabel->setStyleSheet("color:#999999;");
    m_pStatLabel->setAlignment(Qt::AlignRight);
    m_pStatLabel->setText("欢迎使用！");
    m_pStatProgress->setTextVisible(true);
    m_pStatProgress->setRange(0,100);
    ui->statusBar->addPermanentWidget(m_pStatProgress,1);
    ui->statusBar->addPermanentWidget(m_pStatLabel,1);
    //    qDebug()<<QCoreApplication::applicationDirPath();
    //    qDebug()<<QCoreApplication::applicationFilePath();
    //    qDebug()<<QDir::currentPath();
    //    qDebug()<<QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    //    qDebug()<<QStandardPaths::standardLocations(QStandardPaths::DesktopLocation);
    //    qDebug()<<QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    m_sDefaultPath = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation)[0];
}

MainWindow::~MainWindow()
{
    if (m_mFiles) {
        delete m_mFiles;
        m_mFiles = nullptr;
    }
    if (m_gSize) {
        delete m_gSize;
        m_gSize = nullptr;
    }
    if (m_gMatainAspect) {
        delete m_gMatainAspect;
        m_gMatainAspect = nullptr;
    }
    if (m_pStatLabel) {
        delete m_pStatLabel;
        m_pStatLabel = nullptr;
    }
    if (m_pStatProgress) {
        delete m_pStatProgress;
        m_pStatProgress = nullptr;
    }
    delete ui;
}

void MainWindow::on_btnFiles_clicked()
{

    QStringList files = QFileDialog::getOpenFileNames(
                this,
                "选择一个或多个文件",
                m_sDefaultPath,
                "Images (*.png *.jpg *.bmp *.gif *.svg)");
    foreach (QString f, files) {
        m_sFiles.insert(f);
    }
    m_mFiles->setStringList(m_sFiles.toList());
}

void MainWindow::on_btnDelete_clicked()
{
    m_mFiles->removeRows(ui->listFIles->currentIndex().row(), 1);
    m_sFiles = QSet<QString>::fromList(m_mFiles->stringList());
}

void MainWindow::on_btnClear_clicked()
{
    m_sFiles.clear();
    m_mFiles->setStringList(m_sFiles.toList());
}

void MainWindow::on_commandLinkButton_clicked()
{
    // 前缀
    QString prefix = ui->editPrefix->text();
    // 后缀
    QString suffix = ui->editSuffix->text();
    // 扩展名
    QString extension = ui->comboExtension->currentText();

    // 倍率数组
    QList<float> listRates;
    if (ui->boxRate_1_0->isChecked()) {
        listRates.append(1.0);
    }
    if (ui->boxRate_1_5->isChecked()) {
        listRates.append(1.5);
    }
    if (ui->boxRate_2_0->isChecked()) {
        listRates.append(2.0);
    }
    if (ui->boxRate_3_0->isChecked()) {
        listRates.append(3.0);
    }
    if (ui->boxRate_4_0->isChecked()) {
        listRates.append(4.0);
    }
    if (ui->boxRate_5_0->isChecked()) {
        listRates.append(5.0);
    }

    // 尺寸 1.0x
    QString width = ui->editWidth->text();
    QString height = ui->editHeight->text();

    // 保持比例
    QString maintainAspect;
    if (ui->radioMatainAspectYes->isChecked()) {
        maintainAspect = "";
    } else {
        maintainAspect = "!";
    }

    if(m_sFiles.size() == 0 || listRates.size() == 0)
        return;

    m_iAll = m_sFiles.size() * listRates.size();
    m_iCur = 0;
    m_iSuc = 0;
    //重置显示信息
    m_pStatProgress->setValue(0);
    m_pStatLabel->setText("共："+QString("%1").arg(m_iAll));
    m_pStatLabel->repaint();
    // 清空错误信息
    m_pError.clear();

    // 禁用执行按钮
    ui->commandLinkButton->setEnabled(false);
    // 执行
    foreach (QString f, m_sFiles) {
        // 存储路径
        QString path = m_sPath.isEmpty() ? f.left(f.lastIndexOf("/")) : m_sPath;
        // 文件名（含扩展名）
        QString fullname = f.right(f.size() - f.lastIndexOf("/") - 1);
        // 文件名（不含扩展名）
        QString name = fullname.left(fullname.lastIndexOf("."));
        // 原扩展名
        QString ext = fullname.right(fullname.size() - fullname.lastIndexOf(".") - 1);
        foreach (float i, listRates) {
            bool ok1, ok2;
            float fw = width.toFloat(&ok1);
            float fh = height.toFloat(&ok2);
            if (ok1 && ok2) {
                QString w = QString("%1").arg(fw * i);
                QString h = QString("%1").arg(fh * i);
                QString rate;
                if (i == 1.0) {
                    rate = "@1.0x";
                }
                if (i == 1.5) {
                    rate = "@1.5x";
                }
                if (i == 2.0) {
                    rate = "@2.0x";
                }
                if (i == 3.0) {
                    rate = "@3.0x";
                }
                if (i == 4.0) {
                    rate = "@4.0x";
                }
                if (i == 5.0) {
                    rate = "@5.0x";
                }
                //注意：如果传入的宽高是小数，会四舍五入
                QString cmd = "/usr/local/bin/convert -resize " + w + "x" + h + maintainAspect + " \"" + f + "\" \"" + path + "/" + prefix + name + suffix + rate + "." + (extension.isEmpty() ? ext : extension) + "\"";
                ExecuteThread *mThread = new ExecuteThread(cmd);
                connect(mThread,SIGNAL(finish(QString,int)),this,SLOT(onFinish(QString,int)));
                mThread->start();
            }
        }
    }

}


void MainWindow::on_radioSize_18_18_clicked(bool checked)
{
    if (checked) {
        ui->editWidth->setText("18");
        ui->editHeight->setText("18");
        ui->editWidth->repaint();
        ui->editHeight->repaint();
    }
}

void MainWindow::on_radioSize_20_20_clicked(bool checked)
{
    if (checked) {
        ui->editWidth->setText("20");
        ui->editHeight->setText("20");
        ui->editWidth->repaint();
        ui->editHeight->repaint();
    }
}

void MainWindow::on_radioSize_30_30_clicked(bool checked)
{
    if (checked) {
        ui->editWidth->setText("30");
        ui->editHeight->setText("30");
        ui->editWidth->repaint();
        ui->editHeight->repaint();
    }
}

void MainWindow::on_radioSize_40_40_clicked(bool checked)
{
    if (checked) {
        ui->editWidth->setText("40");
        ui->editHeight->setText("40");
        ui->editWidth->repaint();
        ui->editHeight->repaint();
    }
}

void MainWindow::on_btnPath_clicked()
{
    m_sPath = QFileDialog::getExistingDirectory(
                this,
                "选择存储路径",
                m_sDefaultPath);
    ui->editPath->setText(m_sPath);
    ui->editPath->repaint();
}

//当用户拖动文件到窗口部件上时候，就会触发dragEnterEvent事件
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    //如果为文件，则支持拖放
    //    qDebug() << event->mimeData()->formats();
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

//当用户放下这个文件后，就会触发dropEvent事件
void MainWindow::dropEvent(QDropEvent *event)
{
    //注意：这里如果有多文件存在，意思是用户一下子拖动了多个文件，而不是拖动一个目录
    //如果想读取整个目录，则在不同的操作平台下，自己编写函数实现读取整个目录文件名
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        return;

    //往文本框中追加文件名
    foreach (QUrl url, urls) {
        QString file_name = url.toLocalFile();
        if (file_name.endsWith(".png")
                || file_name.endsWith(".jpg")
                || file_name.endsWith(".bmp")
                || file_name.endsWith(".gif")
                || file_name.endsWith(".svg")) {
            m_sFiles.insert(file_name);
            m_mFiles->setStringList(m_sFiles.toList());
        }
    }
}

void MainWindow::onFinish(QString cmd, int ret)
{
    m_iCur++;
    if(ret==0){
        m_iSuc++;
    }else{
        //errors<<cmd;
        m_pError<<cmd+" "+QString::number(ret);
    }
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss ddd");
    qDebug()<<str<<ret<<cmd<<m_iCur*100/m_iAll<<"%";

    m_pStatProgress->setValue(m_iCur*100/m_iAll);
    m_pStatProgress->repaint();
    m_pStatLabel->setText("共："+QString("%1").arg(m_iAll)+"  成功："+QString("%1").arg(m_iSuc)+"  失败："+QString("%1").arg(m_iCur-m_iSuc));
    m_pStatLabel->repaint();
    if(m_iCur == m_iAll){
        ui->commandLinkButton->setEnabled(true);
        if(m_pError.size()>0)
            showInfo();
    }
}

void MainWindow::showInfo()
{
    QMessageBox msgBox;
    msgBox.setInformativeText(m_pError.join("\n\n"));
    msgBox.setText("失败项");
    msgBox.exec();
}
