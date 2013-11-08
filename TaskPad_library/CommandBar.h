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
	void setQuickAddMode();

protected slots:
	void createTemplateAdd();
	void createTemplateAddTimed();
	void createTemplateModDone();
	void createTemplateModByName();
	void createTemplateModByIndex();
	void createTemplateDelByName();
	void createTemplateDelByIndex();
	void createTemplateFind();
	void createTemplateUndo();
	void createTemplateRedo();
	void performCompletion();
	void hkTemplateGoBackwards();

private:
	static const QStringList COMMAND_LIST;
	static const QStringList KEYWORD_LIST;
	static const QStringList KEYWORD_LIST_REMOVE;
	static const QStringList KEYWORD_LIST_ADD;
	static const QStringList KEYWORD_LIST_FIND;
	static const QString SPACE;
	static const QString INCLUDE_QUOTE_LEFT_PAIR;
	static const QString SINGLE_QUOTATION_MARK;
	static const QString QUOTE_LEFT;
	static const QString EMPTY;
	//HOTKEY TEMPLATE RELATED
	static const QString HOTKEY_TEMPLATE_ADD;
	static const QString HOTKEY_TEMPLATE_ADD_TIMED;
	static const QString HOTKEY_TEMPLATE_MOD_DONE;
	static const QString HOTKEY_TEMPLATE_MOD_BY_NAME;
	static const QString HOTKEY_TEMPLATE_MOD_BY_INDEX;
	static const QString HOTKEY_TEMPLATE_DEL_BY_NAME;
	static const QString HOTKEY_TEMPLATE_DEL_BY_INDEX;
	static const QString HOTKEY_TEMPLATE_FIND;
	static const QString HOTKEY_TEMPLATE_UNDO;
	static const QString HOTKEY_TEMPLATE_REDO;
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
	bool hasKeywordNearby(QString keyword, QTextCursor::MoveOperation direction);
	bool hasSingleQuotationMark_LHS();
	bool hasSingleQuotationMark_RHS();
	bool hasQuoteLeft_RHS();
	bool hasQuoteLeft_LHS();
	bool hasSpace_LHS();
	bool hasSpace_RHS();
	bool hasSharp_LHS();
	bool hasAdd_LHS();
	bool hasMinus_LHS();
	void clearCharNearby(QTextCursor::MoveOperation direction);
	void clearCharRHS();
	void clearCharLHS();
	void insertCompletion(const QString &completion);
	void insertFromMimeData(const QMimeData * source);
	bool CommandBar::containsQuoteLeftPair(QString str);
	bool isWithinPairOfQuoteLeft();
	int getLastQuoteLeftPos();
	QVector<QPair<int, int> > getQuoteLeftPositions();
	bool isEvenQuoteLefts();
	bool isHotkeyTemplateMode();
	void createTemplate(QString templateStr);
	//MODEL RELATED
	void produceModel();
	void produceCommandModel();
	void produceKeywordModel();
	void produceKeywordModel_forFind();
	bool isCommandFind();
	bool containsCommand();
	//KEY PRESS RELATED
	void keyPressEvent(QKeyEvent*event);
	bool handleKeyPress(QKeyEvent*event);
	void handleKeyQuoteLeft(bool *isHandled);
	void handleKeyEscape(bool *isHandled);
	void handleKeyTab(bool *isHandled);
	void handleKeySpace(bool *isHandled);
	void handleKeyDelete(bool *isHandled);
	void handleKeyBackspace(bool *isHandled);
	void handleKeyUp();
	void handleKeyDown();
	void handleKeyDefault();
	//FIELDS
	bool autoCompleteFlag;
	bool hotkeyTemplateMode;
	QTextCursor lastTimeCursor;
	QStringListModel* model;
	QCompleter* completer;
	QRegExp hotkeyTemplate;//TODO: rename it
	QRegExp REGEXP_quoteLeft;
	QStack<QString> inputHistory_undo;
	QStack<QString> inputHistory_redo;
	//Short cut
	QShortcut* newDeadlineTask;
	QShortcut* newTimedTask;
	QShortcut* modifyDone;
	QShortcut* modifyByName;
	QShortcut* modifyByIndex;
	QShortcut* delByName;
	QShortcut* delByIndex;
	QShortcut* find;
	QShortcut* undo;
	QShortcut* redo;
};
#endif