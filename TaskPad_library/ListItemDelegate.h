/*
 * =====================================================================================
 *
 *       Filename:  ListItemDelegate.h
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

#include <QStyledItemDelegate>
#include <QPainter>
#include "Enum.h"

#ifndef HIGH_PRIORITY_DELEGATE_H
#define HIGH_PRIORITY_DELEGATE_H

class ListItemDelegate: public QStyledItemDelegate{

	Q_OBJECT

public:
	ListItemDelegate(TP::PRIORITY priority, TP::TASK_STATE state, QObject *parent = 0);
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
	static const int _1ST_COLUMN;
	static const int _2ND_COLUMN;
	static const int _3RD_COLUMN;
	static const QColor WHITE_COLOR;
	static const QColor RED_COLOR;
	static const QColor ORANGE_COLOR;
	static const QString FONT_SEGOE_UI;

	void setupPainting( const QStyleOptionViewItem & option, const QModelIndex & index, QPainter * painter ) const;
	void paintHighPrioBar( QPainter * painter, QStyleOptionViewItemV4 &opt ) const;
	void paintMediumPrioBar( QPainter * painter ) const;
	void paintStrikeOut( QPainter * painter, QStyleOptionViewItemV4 &opt ) const;
	void paintForOverdue( QPainter * painter ) const;
	void alignRight( const QModelIndex &index, QPainter * painter, QStyleOptionViewItemV4 &opt ) const;

	mutable QStyleOptionViewItemV4 _opt;
	TP::TASK_STATE _taskState;
	TP::PRIORITY _taskPriority;
	
};
#endif