#include <QStyledItemDelegate>
#include <QPainter>

#ifndef HIGH_PRIORITY_DELEGATE_H
#define HIGH_PRIORITY_DELEGATE_H

class HighPriorityDelegate: public QStyledItemDelegate
{
	Q_OBJECT

public:
	HighPriorityDelegate(bool isDone, QObject *parent = 0) : QStyledItemDelegate(parent)
	{
		_isDone = isDone;
	}

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
	{
		QStyleOptionViewItemV4 opt = option;
		initStyleOption(&opt, index);

		QStyledItemDelegate::paint(painter, option, index);
		painter->save();

		if(index.column() == 0){
			painter->setPen("#FFB4B9");
			painter->setBrush(QBrush("#FFB4B9"));
			QRect rect = opt.rect;
			rect.adjust(-5,0,-52,-2);
			//painter->drawRect(15,0,3,46);
			painter->drawRect(rect);
		}
		else if(index.column() == 1 && _isDone){
			painter->setPen("#ffffff");
			painter->setBrush(QBrush("#ffffff"));
			QRect rect = opt.rect;
			rect.adjust(5,24,-5,-24);
			painter->drawRect(rect);
		}
		else if(index.column() == 2){
			QString displayText = index.data().toString();
			QTextOption alignment(Qt::AlignRight | Qt::AlignVCenter);
			QPen penHText(QColor("#fff"));
			painter->setPen(penHText);
			QFont font1;
			font1.setFamily(QStringLiteral("Segoe UI"));
			font1.setPixelSize(16);
			painter->setFont(font1);
			QRect rect = opt.rect;
			rect = rect.adjusted(18,0,-18,-3);
			painter->drawText(rect, displayText, alignment);
		}

		painter->restore();
	}

private:
	bool _isDone;
};
#endif