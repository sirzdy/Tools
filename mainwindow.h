#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QSet>
#include <QButtonGroup>
#include <QDragEnterEvent>
#include <QDropEvent>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

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

private:
    Ui::MainWindow *ui;
    QStringListModel *m_mFiles;
    QButtonGroup *m_gSize;
    QButtonGroup *m_gMatainAspect;
    QSet<QString> m_sFiles;
    QString m_sPath;
    QString m_sDefaultPath;
};

#endif // MAINWINDOW_H
