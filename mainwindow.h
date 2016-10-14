#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QTreeWidgetItem;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	void putFileDataToTextEdit(const char* pFileData , int nSize);

    void dropEvent(QDropEvent *);
    void dragEnterEvent(QDragEnterEvent *);
private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
