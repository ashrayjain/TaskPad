#ifndef _CMD_BAR_H
#define _CMD_BAR_H
#include <QtWidgets/QTextEdit>
#include <QCompleter>
#include <QStringListModel>
#include <QKeyEvent>
#include <QTextBlock>
#include <QStack>

#define TEXT_EDIT_BEGIN	autoCompleteToggle(false);
#define TEXT_EDIT_END	autoCompleteToggle(true);
#define TURN_OFF_AC		autoCompleteToggle(false);
#define TURN_ON_AC		autoCompleteToggle(true);

class CommandBar: public QTextEdit
{
	Q_OBJECT

public:
	CommandBar(QWidget *parent);
	
	QString getCurrentLine();
	void pushCurrentLine();

	protected slots:
		void performCompletion();

private:
	static const QStringList COMMAND_LIST;
	static const QStringList KEYWORD_LIST;
	static const QString SPACE;
	static const QString SINGLE_QUOTATION_MARK;
	static const QString EMPTY;

	void initWidgets();
	void initCompleter();
	void initModel();
	void initConnections();

	void autoCompleteToggle(bool flag);

	void performCompletion(const QString&);
	QString getWordUnderCursor();
	bool isLastCharLetter(QString str);
	bool hasSingleQuotationMark(QTextCursor::MoveOperation);
	bool hasSingleQuotationMark_LHS();
	bool hasSingleQuotationMark_RHS();
	void clearSingleQuotationMark_RHS();
	void insertSingleQuotationMark_RHS();
	void insertSpace();
	void insertCompletion(const QString &completion);

	void produceModel();
	void produceCommandModel();
	void produceKeywordModel();
	bool containsCommand();

	void keyPressEvent(QKeyEvent*event);

	bool handleKeyPress(QKeyEvent*event);
	void handleKeyEscape(bool *isHandled);
	void handleKeySpaceAndTab(bool *isHandled);
	void handleKeyDeleteAndBackspace();
	void CommandBar::handleKeyUp();
	void CommandBar::handleKeyDown();
	void handleKeyDefault();

	void mousePressEvent(QMouseEvent*event);

	bool autoCompleteFlag;
	QStringListModel* model;
	QCompleter* completer;
	QStack<QString> inputHistory_undo;
	QStack<QString> inputHistory_redo;
};
#endif