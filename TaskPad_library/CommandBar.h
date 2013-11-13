/*
 * =====================================================================================
 *
 *       Filename:  CommandBar.h
 *
 *           Note:  CommandBar class handles all the issues of command input textbox, which
 * are used in MainWindow and QuickAddWindow
 *
 *        Version:  1.0
 *        Created:  10/21/13 16:13:09
 *
 *         Author:  XIE KAI (A0102016E), gigikie@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

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

class CommandBar: public QTextEdit{

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
	static const int TOO_LONG;
	static const int MAX_LENGTH;
	static const QStringList COMMAND_LIST;
	static const QStringList KEYWORD_LIST;
	static const QStringList KEYWORD_LIST_REMOVE;
	static const QStringList KEYWORD_LIST_ADD;
	static const QStringList KEYWORD_LIST_FIND;
	static const char* NEW_LINE;
	static const QString SPACE;
	static const QString INCLUDE_QUOTE_LEFT_PAIR;
	static const QString SINGLE_QUOTATION_MARK;
	static const QString QUOTE_LEFT;
	static const QString QUOTE_LEFT_PAIR;
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
	static const QString HOTKEY_TEMPLATE_BLANK;
	//INIT RELATED
	void initRegExp();
	void initWidgets();
	void initState();
	void initCompleter();
	void initModel();
	void initConnections();
	//AUTO COMPLETION RELATED
	void autoCompleteToggle(bool flag);
	bool isRHSEmpty();
	void performCompletion(const QString&);
	void handleTextTooLong();
	QString getWordUnderCursor();
	bool isLastCharLetter(QString str);
	bool hasKeywordNearby(QString keyword, QTextCursor::MoveOperation direction);
	bool hasQuoteLeft_RHS();
	bool hasQuoteLeft_LHS();
	bool hasKeywordAtStartOfWord(const char* keyword);
	bool hasSharpAtStartOfWord();
	bool hasPlusAtStartOfWord();
	bool hasMinusAtStartOfWord();
	void clearCharNearby(QTextCursor::MoveOperation direction);
	void clearCharRHS();
	void clearCharLHS();
	void insertCompletion(const QString &completion);
	void insertFromMimeData(const QMimeData * source);
	void makeTextPlain( const QMimeData * source );
	bool CommandBar::containsQuoteLeftPair(QString str);
	bool isWithinPairOfQuoteLeft();
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
	void handleHotKeyGoForwards();
	void handleHotKeyGoBackwards();
	void handleKeySpace(bool *isHandled);
	void handleKeyDelete(bool *isHandled);
	void handleKeyBackspace(bool *isHandled);
	void handleKeyLeftOrRight();
	void handleKeyUp();
	void handleKeyDown();
	void handleKeyDefault();
	void mousePressEvent(QMouseEvent *event);
	//FIELDS
	bool autoCompleteFlag;
	bool hotkeyTemplateMode;
	bool autoCompleteMode;
	QTextCursor lastTimeCursor;
	QStringListModel* model;
	QCompleter* completer;
	QRegExp hotkeyTemplate;
	QRegExp REGEXP_quoteLeftPairs;
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
