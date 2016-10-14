#pragma once
#include <QTextEdit>
class TextItem
{
	QTextEdit* m_pTextEdit;
	int			m_nOffset;
	int			m_nSize;
	QColor		m_color;
public:
	TextItem(QTextEdit* pTextEdit,int nOffset , int nSize,const QColor& color=QColor(255,255,255));
	~TextItem();

	void setColor(const QColor& color);
	void select();
};

