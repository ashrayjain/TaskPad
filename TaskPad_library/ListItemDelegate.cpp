#include "ListItemDelegate.h"

const int ListItemDelegate::_1ST_COLUMN = 0;
const int ListItemDelegate::_2ND_COLUMN = 1;
const int ListItemDelegate::_3RD_COLUMN = 2;
const QColor ListItemDelegate::WHITE_COLOR = "#ffffff";
const QColor ListItemDelegate::RED_COLOR = "#FFB4B9";
const QString ListItemDelegate::FONT_SEGOE_UI = QStringLiteral("Segoe UI");

ListItemDelegate::ListItemDelegate( TP::PRIORITY priority, TP::TASK_STATE state, QObject *parent /*= 0*/ ) : QStyledItemDelegate(parent){
	_taskState = state;
	_taskPriority = priority;
}

void ListItemDelegate::alignRight( const QModelIndex &index, QPainter * painter, QStyleOptionViewItemV4 &opt ) const{
	QString displayText = index.data().toString();
	QTextOption alignment(Qt::AlignRight | Qt::AlignVCenter);
	QFont font;
	font.setFamily(FONT_SEGOE_UI);
	font.setPixelSize(16);
	QRect rect = opt.rect;
	rect = rect.adjusted(18,0,-18,-3);
	painter->setFont(font);
	painter->setPen(WHITE_COLOR);
	painter->drawText(rect, displayText, alignment);
}

void ListItemDelegate::paintStrikeOut( QPainter * painter, QStyleOptionViewItemV4 &opt ) const{
	QRect rect = opt.rect;
	rect.adjust(5,24,-5,-24);
	painter->setPen(WHITE_COLOR);
	painter->setBrush(QBrush(WHITE_COLOR));
	painter->drawRect(rect);
}

void ListItemDelegate::paintHighPrioBar( QPainter * painter, QStyleOptionViewItemV4 &opt ) const{
	QRect rect = opt.rect;
	rect.adjust(-5,0,-52,-2);
	painter->setPen(RED_COLOR);
	painter->setBrush(QBrush(RED_COLOR));
	painter->drawRect(rect);
}

void ListItemDelegate::setupPainting( const QStyleOptionViewItem & option, const QModelIndex & index, QPainter * painter ) const{
	_opt = option;
	initStyleOption(&_opt, index);
	QStyledItemDelegate::paint(painter, option, index);
}

void ListItemDelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const{
	setupPainting(option, index, painter);
	painter->save();
	if(index.column() == _1ST_COLUMN && _taskPriority == TP::HIGH){
		paintHighPrioBar(painter, _opt);
	}
	else if(index.column() == _2ND_COLUMN && _taskState == TP::DONE){
		paintStrikeOut(painter, _opt);
	}
	else if(index.column() == _3RD_COLUMN){
		alignRight(index, painter, _opt);
	}
	painter->restore();
}
