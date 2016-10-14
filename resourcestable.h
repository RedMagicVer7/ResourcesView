#ifndef RESOURCESTABLE_H
#define RESOURCESTABLE_H
#include <QColor>
#include "pe.h"

class QTreeWidget;
class QTreeWidgetItem;
class QTextEdit;
class ResourcesTable
{
    QTreeWidget*    m_pTree;
    QTextEdit*      m_pEdit;
    
	int             m_nResTableOffset;
	int				m_pFileData;
public:
    ResourcesTable(QTreeWidget* pTree,QTextEdit* pEdit);

	QTreeWidgetItem* AddStruct(const QString& name , 
							   int offset , 
                               int nSize ,
                               QColor& color ,
                               QTreeWidgetItem* parent = nullptr);

    QTreeWidgetItem* AddDirectroy(pe_ResDir* addr , QTreeWidgetItem* parent = nullptr);
    QTreeWidgetItem* AddDirectroyEntry(pe_ResDirEntry* addr , int nLevel = 0,QTreeWidgetItem* parent = nullptr );
    QTreeWidgetItem* AddDataDirectroyEntry(pe_ResDataEntry* addr , QTreeWidgetItem* parent = nullptr );


    void loadTable(const void *pFile , bool bIsFileAligin = true);


};

#endif // RESOURCESTABLE_H
