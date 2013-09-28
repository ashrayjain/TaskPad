#ifndef _CMD_BAR_H
#define _CMD_BAR_H
#include <QtWidgets/QTextEdit>
#include <QCompleter>
#include <QStringListModel>
#include <QShortcut>
#include <QtAlgorithms>
#include <QKeyEvent>

class CommandBar: public QTextEdit
{
	Q_OBJECT

public:
	CommandBar(QWidget *parent)
		:QTextEdit(parent)
	{
		//set up
		setObjectName(QStringLiteral("CommandBar"));
		setGeometry(QRect(9, 485, 600, 35));
		QFont font;
		font.setFamily(QStringLiteral("Segoe UI Light"));
		setFont(font);
		setStyleSheet(QLatin1String("QTextEdit#CommandBar {\n"
			"	font-family: \"Segoe UI Light\";\n"
			"	font-size: 18px;\n"
			"	background-color: #fff;\n"
			"	border: 1px solid #9ac3fa;\n"
			"	border-color: #9ac3fa;\n"
			"	padding-top: 4px;\n"
			"	padding-bottom: 4px;\n"
			"	padding-left: 5px;\n"
			"	padding-right: 5px;\n"
			"	margin-left: 57px;\n"
			"	max-height: 33px;\n"
			"}\n"
			"\n"
			"QTextEdit#CommandBar:hover {\n"
			"	border-color: #565656;\n"
			"}"));
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setLineWrapMode(QTextEdit::NoWrap);
		///////////////////
		//model & completer set up
		causedByBackspace = false;
		completedAndSelected = false;
		model = new QStringListModel(this);
		completer = new QCompleter(this);
		completer->setWidget(this);
		completer->setCompletionMode(QCompleter::InlineCompletion);
		completer->setModel(model);
		completer->setModelSorting(QCompleter::UnsortedModel);
		completer->setCaseSensitivity(Qt::CaseInsensitive);
		completer->setWrapAround(true);
		////////////////////
		populateModel();
		//connect(completer, SIGNAL(activated(const QString&)), this, SLOT(insertCompletion(const QString&)));
		connect(this, SIGNAL(textChanged()), this, SLOT(performCompletion()));
		//(void) new QShortcut(QKeySequence(tr("Ctrl+M", "Complete")), this, SLOT(performCompletion()));
	}

	private slots:
		void performCompletion()
		{
			if(completedAndSelected == false && causedByBackspace == false)
			{
				QTextCursor cursor = textCursor();
				cursor.select(QTextCursor::WordUnderCursor);
				const QString completionPrefix = cursor.selectedText();
				if (!completionPrefix.isEmpty() &&
					completionPrefix.at(completionPrefix.length() - 1)
					.isLetter())
					performCompletion(completionPrefix);
			}
		}
		private slots:
			void insertCompletion(const QString &completion, bool singleWord)
			{
				QTextCursor cursor = textCursor();
				int numberOfCharsToComplete = completion.length() -completer->completionPrefix().length();
				if(numberOfCharsToComplete == 0)
					return;
				int insertionPosition = cursor.position();
				completedAndSelected = true;
				cursor.insertText(completion.right(numberOfCharsToComplete));
				cursor.setPosition(insertionPosition);
				cursor.movePosition(QTextCursor::EndOfWord,
					QTextCursor::KeepAnchor);

				setTextCursor(cursor);
			}

private:
	void performCompletion(const QString &completionPrefix)
	{
		if (completionPrefix != completer->completionPrefix()) {
			completer->setCompletionPrefix(completionPrefix);
		}
		if (completer->completionCount() == 1)
			insertCompletion(completer->currentCompletion(), true);
	}

	void populateModel()
	{
		QStringList strings;
		strings.push_back("add");
		strings.push_back("mod");
		strings.push_back("del");
		strings.push_back("find");
		strings.push_back("undo");
		strings.push_back("redo");
		strings.push_back("sync");
		model->setStringList(strings);
	}
	bool caseInsensitiveLessThan(const QString &a, const QString &b)
	{
		return a.compare(b, Qt::CaseInsensitive) < 0;
	}
	void keyPressEvent(QKeyEvent*event)
	{
		if (event->key() == Qt::Key_Backspace ||
			event->key() == Qt::Key_Delete)
			causedByBackspace = true;
		else
			causedByBackspace = false;
		if (completedAndSelected && handledCompletedAndSelected(event))
			return;
		QTextEdit::keyPressEvent(event);
	}
	bool handledCompletedAndSelected(QKeyEvent*event)
	{
		completedAndSelected = false;
		QTextCursor cursor = textCursor();
		switch (event->key()) {
		case Qt::Key_Enter: // Fallthrough
		case Qt::Key_Return: 
			causedByBackspace = false;
			cursor.clearSelection(); break;
		case Qt::Key_Tab:
		case Qt::Key_Space:
			cursor.clearSelection();
			cursor.insertText(" ");
			causedByBackspace = false;
			break;
		case Qt::Key_Delete:
		case Qt::Key_Backspace:
		case Qt::Key_Escape:
			cursor.removeSelectedText();
			causedByBackspace = true;
			break;
		default: 
			causedByBackspace = false;
			return false;
		}
		setTextCursor(cursor);
		event->accept();
		return true;
	}
	void mousePressEvent(QMouseEvent*event)
	{
		if (completedAndSelected) {
			completedAndSelected = false;
			QTextCursor cursor = textCursor();
			cursor.removeSelectedText();
			setTextCursor(cursor);
		}
		QTextEdit::mousePressEvent(event);
	}
	bool causedByBackspace;
	bool completedAndSelected;
	QCompleter* completer;
	QStringListModel* model;
};
#endif