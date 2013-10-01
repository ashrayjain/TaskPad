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
	void createNewTaskTemplate();

protected slots:
	void performCompletion();
	void hkTemplateGoBackwards();

private:
	static const QStringList COMMAND_LIST;
	static const QStringList KEYWORD_LIST;
	static const QString SPACE;
	static const QString SINGLE_QUOTATION_MARK;
	static const QString EMPTY;
	//HOTKEY TEMPLATE RELATED
	static const QString HOTKEY_TEMPLATE_NEW;
	//INIT RELATED
	void initWidgets();
	void initCompleter();
	void initModel();
	void initConnections();
	//AUTO COMPLETION RELATED
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
	//MODEL RELATED
	void produceModel();
	void produceCommandModel();
	void produceKeywordModel();
	bool containsCommand();
	//KEY PRESS RELATED
	void keyPressEvent(QKeyEvent*event);
	bool handleKeyPress(QKeyEvent*event);
	void handleKeyEscape(bool *isHandled);
	void handleKeyTab(bool *isHandled);
	void handleKeySpace(bool *isHandled);
	void handleKeyDeleteAndBackspace();
	void handleKeyUp();
	void handleKeyDown();
	void handleKeyDefault();
	//FIELDS
	bool autoCompleteFlag;
	bool hotkeyTemplateMode;
	QTextCursor lastTimeCursor;
	QStringListModel* model;
	QCompleter* completer;
	QRegExp hotkeyTemplate;
	QStack<QString> inputHistory_undo;
	QStack<QString> inputHistory_redo;
};
#endif