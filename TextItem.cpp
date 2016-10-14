#include "TextItem.h"
#include <QColor>


TextItem::TextItem(QTextEdit* pTextEdit , int nOffset , int nSize , const QColor& color/*=QColor(255,255,255)*/)
	:m_pTextEdit(pTextEdit)
	, m_nOffset(nOffset)
	, m_nSize(nSize)
	, m_color(color)
{
	select();
}


TextItem::~TextItem()
{
}

void TextItem::setColor(const QColor& color)
{
	m_color = color;
}

void TextItem::select()
{
	QTextCursor cur = m_pTextEdit->textCursor();
	cur.setPosition(m_nOffset * 3, QTextCursor::MoveAnchor);
	cur.movePosition(QTextCursor::Right , QTextCursor::KeepAnchor , m_nSize*3-1);

	QTextCharFormat tcf;
	tcf.setForeground(m_color);
	cur.setCharFormat(tcf);

	m_pTextEdit->setTextCursor(cur);
}

