/*
 * =====================================================================================
 *
 *       Filename:  ListItemDelegate.cpp
 *
 *           Note:  ListItemDelegate class makes item displayed specially for different
 *	task priority and task state
 *
 *        Version:  1.0
 *        Created:  10/21/13 16:13:09
 *
 *         Author:  XIE KAI (A0102016E), gigikie@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#include "ListItemDelegate.h"

// @author A0102016E
const int ListItemDelegate::_1ST_COLUMN = 0;
const int ListItemDelegate::_2ND_COLUMN = 1;
const int ListItemDelegate::_3RD_COLUMN = 2;
const QColor ListItemDelegate::WHITE_COLOR = "#ffffff";
const QColor ListItemDelegate::RED_COLOR = "#FFB4B9";
const QColor ListItemDelegate::ORANGE_COLOR = "#FDEBA3";
const QString ListItemDelegate::FONT_SEGOE_UI = QStringLiteral("Segoe UI");

ListItemDelegate::ListItemDelegate( TP::PRIORITY priority, TP::TASK_STATE state, QObject *parent /*= 0*/ ) : QStyledItemDelegate(parent){
	_taskState = state;
	_taskPriority = priority;
}

//make the task align right, normally for the last column
void ListItemDelegate::alignRight( const QModelIndex &index, QPainter * painter, QStyleOptionViewItemV4 &opt ) const{
	const int DESIRED_FONT_SIZE = 15;
	const int X_PT1 = 18, Y_PT1 = 0, X_PT2 = -18, Y_PT2 = -3;
	
	QString displayText = index.data().toString();
	QTextOption alignment(Qt::AlignRight | Qt::AlignVCenter);
	QFont font;
	font.setFamily(FONT_SEGOE_UI);
	font.setPixelSize(DESIRED_FONT_SIZE);
	QRect drawingRegion = opt.rect;
	drawingRegion = drawingRegion.adjusted(X_PT1, Y_PT1, X_PT2, Y_PT2);
	painter->setFont(font);
	painter->setPen(WHITE_COLOR);
	painter->drawText(drawingRegion, displayText, alignment);
}

//paint a strike out, normally for the task name's column
void ListItemDelegate::paintStrikeOut( QPainter * painter, QStyleOptionViewItemV4 &opt ) const{
	const int X_PT1 = 5, Y_PT1 = 24, X_PT2 = -5, Y_PT2 = -24;

	QRect drawingRegion = opt.rect;
	drawingRegion.adjust(X_PT1, Y_PT1, X_PT2, Y_PT2);
	painter->setPen(WHITE_COLOR);
	painter->setBrush(QBrush(WHITE_COLOR));
	painter->drawRect(drawingRegion);
}

//paint a high priority bar at the left hand side corner, normally for the high priority task
void ListItemDelegate::paintHighPrioBar( QPainter * painter, QStyleOptionViewItemV4 &opt ) const{
	const int X_PT1 = -5, Y_PT1 = 0, X_PT2 = -52, Y_PT2 = -2;

	QRect drawingRegion = opt.rect;
	drawingRegion.adjust(X_PT1, Y_PT1, X_PT2, Y_PT2);
	painter->setPen(RED_COLOR);
	painter->setBrush(QBrush(RED_COLOR));
	painter->drawRect(drawingRegion);
}

//paint a "!!", normally for the overdue task
void ListItemDelegate::paintForOverdue( QPainter * painter ) const{
	const int DESIRED_FONT_SIZE = 18;
	const int X_PT1 = 0, Y_PT1 = 0, X_PT2 = 0, Y_PT2 = -2;
	QString displayTextForOVERDUE = "!!";
	
	QTextOption alignment(Qt::AlignLeft | Qt::AlignVCenter);
	QFont font;
	font.setFamily(FONT_SEGOE_UI);
	font.setPixelSize(DESIRED_FONT_SIZE);
	QRect drawingRegion = _opt.rect;
	drawingRegion = drawingRegion.adjusted(X_PT1, Y_PT1, X_PT2, Y_PT2);
	painter->setFont(font);
	painter->setPen(WHITE_COLOR);
	painter->drawText(drawingRegion, displayTextForOVERDUE, alignment);
}

//paint a medium priority bar at the left hand side corner, normally for the medium priority task
void ListItemDelegate::paintMediumPrioBar( QPainter * painter ) const{
	const int X_PT1 = -5, Y_PT1 = 0, X_PT2 = -52, Y_PT2 = -2;

	QRect drawingRegion = _opt.rect;
	drawingRegion.adjust(X_PT1, Y_PT1,X_PT2,Y_PT2);
	painter->setPen(ORANGE_COLOR);
	painter->setBrush(QBrush(ORANGE_COLOR));
	painter->drawRect(drawingRegion);
}

void ListItemDelegate::setupPainting( const QStyleOptionViewItem & option, const QModelIndex & index, QPainter * painter ) const{
	_opt = option;
	initStyleOption(&_opt, index);
	
	//make the dot-dot rectangle disappear, when select
	if (_opt.state & QStyle::State_HasFocus)
		_opt.state = _opt.state ^ QStyle::State_HasFocus;
	//paint css's style first
	QStyledItemDelegate::paint(painter, _opt, index);
}

//must have this function
//which will be called by MainWindow
void ListItemDelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const{
	setupPainting(option, index, painter);
	painter->save();
	if(index.column() == _1ST_COLUMN && _taskPriority == TP::HIGH)
		paintHighPrioBar(painter, _opt);
	else if(index.column() == _1ST_COLUMN && _taskPriority == TP::MEDIUM)
		paintMediumPrioBar(painter);
	else if(index.column() == _2ND_COLUMN && _taskState == TP::DONE)
		paintStrikeOut(painter, _opt);
	else if(index.column() == _2ND_COLUMN && _taskState == TP::OVERDUE)
		paintForOverdue(painter);
	else if(index.column() == _3RD_COLUMN)
		alignRight(index, painter, _opt);
	painter->restore();
}
