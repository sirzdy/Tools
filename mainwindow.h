#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QSet>
#include <QButtonGroup>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QLabel>
#include <QProgressBar>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showInfo();

private slots:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    void on_btnFiles_clicked();

    void on_btnDelete_clicked();

    void on_commandLinkButton_clicked();

    void on_radioSize_18_18_clicked(bool checked);

    void on_radioSize_20_20_clicked(bool checked);

    void on_radioSize_30_30_clicked(bool checked);

    void on_radioSize_40_40_clicked(bool checked);

    void on_btnClear_clicked();

    void on_btnPath_clicked();

    void onFinish(QString cmd,int ret);
private:
    Ui::MainWindow *ui;
    QStringListModel *m_mFiles;
    QStringList m_pError;
    QButtonGroup *m_gSize;
    QButtonGroup *m_gMatainAspect;
    QSet<QString> m_sFiles;
    QString m_sPath;
    QString m_sDefaultPath;
    QLabel *m_pStatLabel;
    QProgressBar *m_pStatProgress;

    int m_iAll;//任务总数
    int m_iSuc;//当前成功的数量
    int m_iCur;//当前执行完数量
};

#endif // MAINWINDOW_H
