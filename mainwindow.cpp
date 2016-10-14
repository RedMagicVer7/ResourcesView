#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>
#include <QDebug>
#include "resourcestable.h"
#include "pe.h"
#include "TextItem.h"
#include <QTreeWidgetItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    ui->textEdit->setAcceptDrops(false);
	ui->treeWidget->setColumnWidth(0 , 240);
	this->setWindowTitle("资源查看工具");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    qDebug()<< urls.first().toLocalFile();
	

	QFile file(urls.first().toLocalFile());
	file.open(QIODevice::ReadOnly);
	if (file.isOpen())
	{
		qint64 size = file.size();
		ui->treeWidget->clear();
		ui->textEdit->clear();
		QByteArray fileData = file.readAll();

		char* pFileData = fileData.data();
		PE pe(pFileData);

		pe_DataDirectroy* pDataDir = pe.optionHeader()->DataDirectory;

		putFileDataToTextEdit(pe.RVAToOfs(pDataDir[ 2 ].VirtualAddress) + pFileData ,
							  pDataDir[ 2 ].Size);

		ResourcesTable resTableView(ui->treeWidget , ui->textEdit);
		resTableView.loadTable(pFileData);
	}
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("text/uri-list"))
    {
        //允许放下文件
        event->acceptProposedAction();
    }
}


void MainWindow::putFileDataToTextEdit(const char* pFileData , int nSize)
{
	int buffSize = nSize * 3 + (nSize / 16) * 2 + 3;
	char*	buff = new char[ buffSize ];
	char* temp = buff;
	memset(buff , 0 , buffSize);

	while(nSize>=0)
	{
		for(int i = 0 ; i < 16 && nSize >= 0; ++i , --nSize , buff += 3 , ++pFileData)
		{
			sprintf(buff , "%02X " , *(unsigned char*)pFileData);
		}
		*(buff-1) = '\r';
		*(buff++) = '\n';
	}
	*buff = 0;
	ui->textEdit->setText(temp);
	delete[] temp;
}



void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
	TextItem *textItem = (TextItem*)item->data(0 , 20).toInt();
    if(textItem != nullptr)
        textItem->select();
	ui->textEdit->setFocus();
}
