#include "resourcestable.h"

#include "pe.h"
#include <QTreeWidgetItem>
#include "TextItem.h"
#include <QDebug>

ResourcesTable::ResourcesTable(QTreeWidget *pTree , QTextEdit *pEdit)
    :m_pTree(pTree)
    ,m_pEdit(pEdit)
    ,m_nResTableOffset()
{

}


typedef struct ICON
{
	DWORD	dwId;
	DWORD	dwType;
	void*	pData;
	DWORD	dwSize;
}ICON;


void ResourcesTable::loadTable(const void *pFile , bool bIsFileAligin)
{
	if(pFile == nullptr)
		return;

	m_pFileData = (int)pFile;
	// 获取资源表
	PE  pe((void*)pFile);
	pe_ResDir *resDirFir = nullptr;
	pe_ResDir *resDirSec = nullptr;
	pe_ResDir *resDirThi = nullptr;
	pe_ResDirEntry* resDirEntryFir = nullptr;
	pe_ResDirEntry* resDirEntrySec = nullptr;
	pe_ResDirEntry* resDirEntryThi = nullptr;
	pe_ResDataEntry* resDataEntry = nullptr;

	resDirFir = (pe_ResDir*)((int)pFile + pe.RVAToOfs(pe.dataDirectory()[ 2 ].VirtualAddress));
	m_nResTableOffset = (int)resDirFir;
	AddDirectroy(resDirFir);

	resDirEntryFir = (pe_ResDirEntry*)(resDirFir + 1);
	// 找到图标资源和图标组资源
	// 遍历资源表
	for(int i = 0 ; i < resDirFir->NumberOfIdEntries + resDirFir->NumberOfNamedEntries; ++i , ++resDirEntryFir)
	{
		QTreeWidgetItem* FirEntry = AddDirectroyEntry(resDirEntryFir);
		if(resDirEntryFir->DataIsDirectory)
		{
			// 遍历第二层目录
			resDirSec = (pe_ResDir*)(resDirEntryFir->OffsetToDirectory + (int)resDirFir);
			AddDirectroy(resDirSec , FirEntry);

			resDirEntrySec = (pe_ResDirEntry*)(resDirSec + 1);
			for(int j = 0 ; j < resDirSec->NumberOfIdEntries + resDirSec->NumberOfNamedEntries;++j,++resDirEntrySec)
			{
				QTreeWidgetItem* SecEntry =  AddDirectroyEntry(resDirEntrySec ,1, FirEntry);
				// 进入第三层
				resDirThi = (pe_ResDir*)(resDirEntrySec->OffsetToDirectory + (int)resDirFir);
				AddDirectroy(resDirThi , SecEntry);
				resDirEntryThi = (pe_ResDirEntry*)(resDirThi + 1);
 				AddDirectroyEntry(resDirEntryThi , 2 , SecEntry);
		
 				// 得到数据目录入口
				resDataEntry = (pe_ResDataEntry*)(resDirEntryThi->OffsetToData + (int)resDirFir);
				AddDataDirectroyEntry(resDataEntry , SecEntry);
			}
		}
	}


}


QTreeWidgetItem* ResourcesTable::AddStruct(const QString& name , 
										   int addr ,
                                           int nSize , 
										   QColor& color , 
										   QTreeWidgetItem* parent)
{
	QTreeWidgetItem* item = new QTreeWidgetItem;
	if(parent)
		parent->addChild(item);
	else
		m_pTree->addTopLevelItem(item);

	TextItem* textItem = new TextItem(m_pEdit , addr - m_nResTableOffset , nSize , color);
	item->setData(0 , 20 , (int)textItem);
	item->setTextColor(0 , color);
	item->setText(0 , name);
	return item;
}



QTreeWidgetItem* ResourcesTable::AddDirectroy(pe_ResDir* addr , QTreeWidgetItem* parent )
{
	QColor color(237,28,36);
	QTreeWidgetItem* master = AddStruct(QStringLiteral("资源目录") , 
										(int)addr , 
										sizeof(IMAGE_RESOURCE_DIRECTORY) , 
										color, 
										parent);

    int nAddr = (int)addr;
	QString value;
    // 插入字段
	value.sprintf("0x%08X" , addr->Characteristics);
    AddStruct("Characteristics",nAddr,sizeof(DWORD),color,master)->setText(1,value);
	value.sprintf("0x%08X" , addr->TimeDateStamp);
	AddStruct("TimeDateStamp" , nAddr + 4 , sizeof(DWORD) , color , master)->setText(1 , value);
	value.sprintf("0x%04X" , addr->MajorVersion);
	AddStruct("MajorVersion" , nAddr + 8 , sizeof(WORD) , color , master)->setText(1 , value);
	value.sprintf("0x%04X" , addr->MinorVersion);
	AddStruct("MinorVersion" , nAddr + 10 , sizeof(WORD) , color , master)->setText(1 , value);
	value.sprintf("0x%04X" , addr->NumberOfNamedEntries);
	AddStruct("NumberOfNamedEntries" , nAddr + 12 , sizeof(WORD) , color , master)->setText(1 , value);
	value.sprintf("0x%04X" , addr->NumberOfIdEntries);
	AddStruct("NumberOfIdEntries" , nAddr + 14 , sizeof(WORD) , color , master)->setText(1 , value);

	return master;
}

QTreeWidgetItem* ResourcesTable::AddDirectroyEntry(pe_ResDirEntry* addr , int nLevel ,QTreeWidgetItem* parent)
{
	QColor color;
	if(nLevel == 0)
		color = RGB(34,177,76);
	else if(nLevel == 1)
		color = RGB(63 , 72, 204);
	else if(nLevel == 2)
		color = RGB(185 , 122, 87);
	else
		color = RGB(181 , 230, 29);

	static QString type[] =
	{
		QStringLiteral("") ,				// 0
		QStringLiteral("光标") ,				// 1 
		QStringLiteral("位图") ,				// 2 RT_BITMAP
		QStringLiteral("图标") ,			    // 3 RT_ICON
		QStringLiteral("菜单") ,				// 4 RT_MENU
		QStringLiteral("对话框") ,			// 5 RT_DIALOG      
		QStringLiteral("字符串") ,			// 6 RT_STRING      
		QStringLiteral("字体目录") ,			// 7 RT_FONTDIR     
		QStringLiteral("字体") ,				// 8 RT_FONT        
		QStringLiteral("快捷键") ,			// 9 RT_ACCELERATOR 
		QStringLiteral("未格式化资源") ,		// 10 RT_RCDATA      
		QStringLiteral("消息表") ,			// 11 RT_MESSAGETABLE
		QStringLiteral("光标组") ,			// 12 
		QStringLiteral("") ,				// 13 
		QStringLiteral("图标组") ,			// 14 
		"" ,								// 15
		QStringLiteral("版本信息") ,			// 16
		QStringLiteral("对话框包含目录") ,	// 17 #define RT_DLGINCLUDE   MAKEINTRESOURCE(17)
		"" ,								// 18 #define RT_PLUGPLAY     MAKEINTRESOURCE(19)
		"" ,								// 19 #define RT_VXD          MAKEINTRESOURCE(20)
		"" ,								// 20 #define RT_ANICURSOR    MAKEINTRESOURCE(21)
		"" ,								// 21 #define RT_ANIICON      MAKEINTRESOURCE(22)
		"",									// 22 
		"html" ,						    // 23 #define RT_HTML         MAKEINTRESOURCE(23)
		QStringLiteral("清单文件")			// 24 RT_MANIFEST
	};

	QString Type ;
	QString Id;

	QTreeWidgetItem* master = AddStruct(QStringLiteral("资源目录入口") , (int)addr , sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY) , color , parent);
	if(addr->DataIsDirectory)
	{
		
		if(addr->NameIsString)
		{
			Type = (char*)((PIMAGE_RESOURCE_DIR_STRING_U)(addr->NameOffset + m_nResTableOffset))->NameString;
		}
		else if(nLevel == 0)
			Type = type[ addr->Id ];
		else
			Type.sprintf("%d" , addr->Id);

		master->setText(1 , Type);
	}
	else
	{
		if(addr->NameIsString)
		{
			Type = (char*)((PIMAGE_RESOURCE_DIR_STRING_U)(addr->NameOffset + m_nResTableOffset))->NameString;
		}
		else
			Type.sprintf("ID:%d" , addr->Id);
		master->setText(1 , Type);
	}
	QString	value;


	QTreeWidgetItem *Union1 = AddStruct("NameOrId" , (int)addr + 0 , sizeof(DWORD) , color , master);
	QTreeWidgetItem *Struct1 = AddStruct("NameStruct" , (int)addr + 0 , sizeof(DWORD) , color , Union1);

	value.sprintf("0x%08X" , addr->NameOffset);
	AddStruct("NameOffset" , (int)addr , sizeof(DWORD) , color , Struct1)->setText(1 , value);
	value.sprintf("0x%d" , addr->NameIsString);
	AddStruct("NameIsString" , (int)addr , 0 , color , Struct1)->setText(1 , value);

	value.sprintf("0x%08X" , addr->Name);
	AddStruct("Name" , (int)addr , sizeof(DWORD) , color , Union1)->setText(1 , value);
	value.sprintf("0x%04X" , addr->Id);
	AddStruct("Id" , (int)addr , sizeof(WORD) , color , Union1)->setText(1 , value);


	QTreeWidgetItem *Union2 = AddStruct("DirOrData" , (int)addr + 4 , sizeof(DWORD) , color , master);

	value.sprintf("0x%08X" , addr->OffsetToData);
	AddStruct("OffsetToData" , (int)addr + 4 , sizeof(DWORD) , color , Union2)->setText(1 , value);

	QTreeWidgetItem *Struct2 = AddStruct("DirectoryStruct" , (int)addr + 4 , sizeof(DWORD) , color , master);

	value.sprintf("0x%08X" , addr->OffsetToDirectory);
	AddStruct("OffsetToDirectory" , (int)addr + 4 , sizeof(DWORD) , color , Struct2)->setText(1 , value);
	value.sprintf("%d" , addr->DataIsDirectory);
	AddStruct("DataIsDirectory" , (int)addr + 4 , 0 , color , Struct2)->setText(1 , value);
	
	return master;
}

QTreeWidgetItem* ResourcesTable::AddDataDirectroyEntry(pe_ResDataEntry* addr , QTreeWidgetItem* parent)
{
	QColor color(128 , 128, 0);
    QTreeWidgetItem* master =  AddStruct(QStringLiteral("数据目录入口") , 
										 (int)addr , 
										 sizeof(IMAGE_RESOURCE_DATA_ENTRY) , 
										 color , 
										 parent);

	// 找到资源段的RVA
	PE pe((char*)m_pFileData);
	int nResOfs = pe.RVAToOfs(pe.optionHeader()->DataDirectory[ 2 ].VirtualAddress);

	int nOfsToData = pe.RVAToOfs(addr->OffsetToData) - nResOfs ;

	QString	value;

	value.sprintf("0x%08X" , addr->OffsetToData);
    AddStruct("OffsetToData" , (int)nOfsToData+m_nResTableOffset, addr->Size,color,master)->setText(1,value);
	value.sprintf("0x%08X" , addr->Size);
	AddStruct("Size" , (int)addr + 4 , sizeof(DWORD) , color , master)->setText(1 , value);
	value.sprintf("0x%08X" , addr->CodePage);
	AddStruct("CodePage" , (int)addr + 8 , sizeof(DWORD) , color , master)->setText(1 , value);
	value.sprintf("0x%08X" , addr->Reserved);
	AddStruct("Reserved" , (int)addr + 12 , sizeof(DWORD) , color , master)->setText(1 , value);
    return master;
}































