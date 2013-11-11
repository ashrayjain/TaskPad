/*
 * =====================================================================================
 *
 *       Filename:  CommandBar.cpp
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

#include "CommandBar.h"
#include <QShortcut>
#include <QMimeData>
#include "Highlighter.h"

// @author A0102016E
const int CommandBar::TOO_LONG		= 777;
const int CommandBar::MAX_LENGTH	= 777;
const QStringList CommandBar::COMMAND_LIST = QStringList() \
	<< "add ``" << "mod" << "del" << "find" << "undo" <<     \
	"redo";
const QStringList CommandBar::KEYWORD_LIST = QStringList() \
	<< "name ``" << "by ``" << "due ``" << "from ``" << "to ``" << "location ``" << "place ``" << "at ``" << \
	"with ``" << "ppl ``" << "note ``" << "priority ``" << "impt ``" << "rt ``" << "remind ``" << "done" << \
	"undone" <<  "exact ``";
const QStringList CommandBar::KEYWORD_LIST_REMOVE = QStringList() \
	<< "by" << "due" << "from" << "to" << "with ``" << "ppl ``" << "pplall" << "rt ``" << "remind ``" \
	<< "rtall";
const QStringList CommandBar::KEYWORD_LIST_ADD = QStringList() \
	<< "with ``" << "ppl ``" << "rt ``" << "remind ``";
const QStringList CommandBar::KEYWORD_LIST_FIND = QStringList() \
	<< "name ``" << "from ``" << "to ``" << "location ``" << "place ``" << "at ``" << "with ``" << "ppl ``" \
	<< "note ``" << "priority ``" << "impt ``" << "rt ``" << "remind ``" << "overdue" << "done" << "undone" <<  \
	"deadline" << "timed" << "floating" << "exact name ``";
const char*	  CommandBar::NEW_LINE                     = "\n";
const QString CommandBar::SPACE                        = " ";
const QString CommandBar::INCLUDE_QUOTE_LEFT_PAIR      = "(\\w+ ``)|( ``)|(``)";
const QString CommandBar::SINGLE_QUOTATION_MARK        = "'";
const QString CommandBar::QUOTE_LEFT                   = "`";
const QString CommandBar::QUOTE_LEFT_PAIR              = "`(.*)`";
const QString CommandBar::EMPTY                        = "";
const QString CommandBar::HOTKEY_TEMPLATE_ADD          = "add `__NAME__` due `__DATE__` impt `__PRIORITY__` "\
	"at `__WHERE__` ppl `__PARTICIPANTS__` #__TAGS__ rt `__REMINDTIME__` note `__NOTE__`";
const QString CommandBar::HOTKEY_TEMPLATE_ADD_TIMED    = "add `__NAME__` from `__DATE__` to `__DATE__` impt `"\
	"__PRIORITY__` at `__WHERE__` ppl `__PARTICIPANTS__` #__TAGS__ rt `__REMINDTIME__` note `__NOTE__`";
const QString CommandBar::HOTKEY_TEMPLATE_MOD_DONE     = "mod __INDEX__ done";
const QString CommandBar::HOTKEY_TEMPLATE_MOD_BY_NAME  = "mod `__NAME__` name `__MODIFIEDNAME__` from `__DATE"\
	"__` to `__DATE__` impt `__PRIORITY__` at `__WHERE__` ppl `__PARTICIPANTS__` #__TAGS__ rt `__REMINDTIME__"\
	"` note `__NOTE__` __DONE__";
const QString CommandBar::HOTKEY_TEMPLATE_MOD_BY_INDEX = "mod __INDEX__ name `__MODIFIEDNAME__` from `__DATE_"\
	"_` to `__DATE__` impt `__PRIORITY__` at `__WHERE__` ppl `__PARTICIPANTS__` #__TAGS__ rt `__REMINDTIME__`"\
	" note `__NOTE__` __DONE__";
const QString CommandBar::HOTKEY_TEMPLATE_DEL_BY_NAME  = "del `__NAME__`";
const QString CommandBar::HOTKEY_TEMPLATE_DEL_BY_INDEX = "del __INDEX__";
const QString CommandBar::HOTKEY_TEMPLATE_FIND         = "find name `__NAME__` from `__DATE__` to `__DATE__` "\
	"impt `__PRIORITY__` at `__WHERE__` ppl `__PARTICIPANTS__` note `__NOTE__`";
const QString CommandBar::HOTKEY_TEMPLATE_UNDO         = "undo";
const QString CommandBar::HOTKEY_TEMPLATE_REDO         = "redo";
const QString CommandBar::HOTKEY_TEMPLATE_BLANK        = "__[A-Z]+__";

CommandBar::CommandBar(QWidget *parent)
:QTextEdit(parent), inputHistory_undo(), inputHistory_redo(){
	initRegExp();
	initWidgets();
	initConnections();
}

void CommandBar::initRegExp(){
	hotkeyTemplate.setPattern(HOTKEY_TEMPLATE_BLANK);
	hotkeyTemplate.setPatternSyntax(QRegExp::RegExp2);
	REGEXP_quoteLeftPairs.setPattern(QUOTE_LEFT_PAIR);
	REGEXP_quoteLeftPairs.setPatternSyntax(QRegExp::RegExp2);
	REGEXP_quoteLeftPairs.setMinimal(true);
}

void CommandBar::initWidgets(){
	initState();
	initModel();
	initCompleter();
}

//Highlighter will be deleted by its parent (class CommandBar)
//No memory leak here, it's just a Qt's feature.
//DO NOT DELETE IT! otherwise you delete it twice and abort.
void CommandBar::initState(){
	autoCompleteToggle(true);
	hotkeyTemplateMode = false;
	autoCompleteMode = false;
	(void) new Highlighter(document());
}

//Model is used by Completer to produce words to complete what users type
void CommandBar::initModel(){
	model = new QStringListModel(this);
	produceCommandModel();
}

void CommandBar::initCompleter(){
	completer = new QCompleter(this);
	completer->setWidget(this);
	completer->setCompletionMode(QCompleter::InlineCompletion);
	completer->setModel(model);
	completer->setModelSorting(QCompleter::UnsortedModel);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	completer->setWrapAround(true);
}

//all QShortcut will be deleted by their parent (class CommandBar) automatically
//it's a Qt feature. 
//DO NOT DELETE THEM! otherwise you delete them twice and abort.
void CommandBar::initConnections(){
	const char* NEW_DEADLINE_TASK    = "Ctrl+N";
	const char* NEW_TIMED_TASK       = "Ctrl+Shift+N";
	const char* MOD_TASK_DONE        = "Ctrl+M";
	const char* MOD_TASK_BY_NAME     = "Ctrl+Alt+Shift+M";
	const char* MOD_TASK_BY_INDEX    = "Ctrl+Shift+M";
	const char* DEL_TASK_BY_NAME     = "Ctrl+Shift+D";
	const char* DEL_TASK_BY_INDEX    = "Ctrl+D";
	const char* FIND_TASK            = "Ctrl+F";
	const char* UNDO                 = "Ctrl+U";
	const char* REDO                 = "Ctrl+R";
	const char* JUMP_BACK_PREV_BLANK = "Shift+Tab";

	connect(this, SIGNAL(textChanged()), this, SLOT(performCompletion()));
	newDeadlineTask = new QShortcut(QKeySequence(NEW_DEADLINE_TASK), this, SLOT(createTemplateAdd()));
	newTimedTask    = new QShortcut(QKeySequence(NEW_TIMED_TASK), this, SLOT(createTemplateAddTimed()));
	modifyDone      = new QShortcut(QKeySequence(MOD_TASK_DONE), this, SLOT(createTemplateModDone()));
	modifyByName    = new QShortcut(QKeySequence(MOD_TASK_BY_NAME), this, SLOT(createTemplateModByName()));
	modifyByIndex   = new QShortcut(QKeySequence(MOD_TASK_BY_INDEX), this, SLOT(createTemplateModByIndex()));
	delByName       = new QShortcut(QKeySequence(DEL_TASK_BY_NAME), this, SLOT(createTemplateDelByName()));
	delByIndex      = new QShortcut(QKeySequence(DEL_TASK_BY_INDEX), this, SLOT(createTemplateDelByIndex()));
	find            = new QShortcut(QKeySequence(FIND_TASK), this, SLOT(createTemplateFind()));
	undo            = new QShortcut(QKeySequence(UNDO), this, SLOT(createTemplateUndo()));
	redo            = new QShortcut(QKeySequence(REDO), this, SLOT(createTemplateRedo()));
	(void)            new QShortcut(QKeySequence(JUMP_BACK_PREV_BLANK), this, SLOT(hkTemplateGoBackwards()));
}

QString CommandBar::getCurrentLine(){
	QRegExp regex(HOTKEY_TEMPLATE_BLANK);
	QString result = toPlainText();
	result.replace(regex, EMPTY);
	return result;
}

//push current line into input history
//then user can use up/down arrow to read thru them
void CommandBar::pushCurrentLine(){
	QString currentInput = toPlainText();
	if(currentInput != EMPTY){
		while(!inputHistory_redo.isEmpty()){
			QString input = inputHistory_redo.pop();
			inputHistory_undo.push(input);
		}
		inputHistory_undo.push(currentInput);
		clear();
	}
}

//it's used by MACRO:
//#define TEXT_EDIT_BEGIN	autoCompleteToggle(false);
//#define TEXT_EDIT_END		autoCompleteToggle(true);
//#define TURN_OFF_AC		autoCompleteToggle(false);
//#define TURN_ON_AC		autoCompleteToggle(true);
//make the coding more readable :p
void CommandBar::autoCompleteToggle(bool flag){
	autoCompleteFlag = flag;
}

//when textChanged, this function will be called by textChanged signal
void CommandBar::performCompletion(){
	produceModel();
	if(getCurrentLine().length() > TOO_LONG){
		handleTextTooLong();
	}
	else if(autoCompleteFlag && isRHSEmpty()){
			QString completionPrefix = getWordUnderCursor();
			if (!completionPrefix.isEmpty() &&
				isLastCharLetter(completionPrefix)){
					performCompletion(completionPrefix);
			}
	}
}

bool CommandBar::isRHSEmpty(){
	bool isRHSaSpace = hasKeywordNearby(SPACE, QTextCursor::Right);
	bool isAtTheEndOfLine = textCursor().position() == getCurrentLine().length();
	return isRHSaSpace || isAtTheEndOfLine;
}

void CommandBar::produceModel(){
	//produce empty model if within `` or after #..
	if(isWithinPairOfQuoteLeft() || hasSharpAtStartOfWord()){
		model->setStringList(QStringList());
	}//produce keyword model for -xxx keyword
	else if(!isWithinPairOfQuoteLeft() && hasMinusAtStartOfWord()){
		model->setStringList(KEYWORD_LIST_REMOVE);
	}//produce keyword model for +xxx keyword
	else if(!isWithinPairOfQuoteLeft() && hasPlusAtStartOfWord()){
		model->setStringList(KEYWORD_LIST_ADD);
	}
	else if(containsCommand()){
		//special keyword model for command Find
		if(isCommandFind())
			produceKeywordModel_forFind();
		else//normal keyword model for all kinds of command
			produceKeywordModel();
	}
	else{//produce command model
		produceCommandModel();
	}
}

void CommandBar::produceCommandModel(){
	model->setStringList(COMMAND_LIST);
}

void CommandBar::produceKeywordModel(){
	model->setStringList(KEYWORD_LIST);
}

void CommandBar::produceKeywordModel_forFind(){
	model->setStringList(KEYWORD_LIST_FIND);
}

bool CommandBar::isLastCharLetter(QString str){
	const int END_INDEX = str.length() - 1;
	bool result = str.at(END_INDEX).isLetter();
	return result;
}

//just remove the too-long text, and move the cursor to the end of line
void CommandBar::handleTextTooLong(){
	QString currContent = getCurrentLine();
	QTextCursor cursor = textCursor();
	cursor.movePosition(QTextCursor::EndOfLine);
	int pos = cursor.position();
	currContent.remove(MAX_LENGTH, pos);
	setText(currContent);
	setTextCursor(cursor);
}

void CommandBar::performCompletion(const QString &completionPrefix){
	const int NOT_FOUND = 0;
	if (completionPrefix != completer->completionPrefix()){
		completer->setCompletionPrefix(completionPrefix);
	}
	if (completer->completionCount() > NOT_FOUND){
		insertCompletion(completer->currentCompletion());
	}
}

void CommandBar::insertCompletion(const QString &completion){
	const int NOT_FOUND = 0;
	QTextCursor cursor = textCursor();
	int numberOfCharsToComplete = completion.length() - completer->completionPrefix().length();
	if(numberOfCharsToComplete > NOT_FOUND){
		//start auto-complete
		int insertionPosition = cursor.position();
		//select the input text, override later
		cursor.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
		//in case of auto-complete infinite loop
		TEXT_EDIT_BEGIN
		cursor.insertHtml( completion );
		TEXT_EDIT_END
		//back to previous cursor position
		cursor.setPosition(insertionPosition);
		//move cursor to end of completion
		cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, numberOfCharsToComplete);
		setTextCursor(cursor);
		autoCompleteMode = true;
	}
}

//make copy or paste data from Web plain
void CommandBar::insertFromMimeData(const QMimeData *source){
	if(source->hasText()){
		makeTextPlain(source);
	}
}

void CommandBar::makeTextPlain( const QMimeData * source ){
	QString strToInsert = source->text();
	strToInsert.replace(QRegExp(NEW_LINE), EMPTY);
	insertPlainText(strToInsert);
}

bool CommandBar::containsQuoteLeftPair(QString str){
	const int NOT_FOUND = -1;
	QRegExp regex(INCLUDE_QUOTE_LEFT_PAIR);
	int index = regex.indexIn(str);
	return index != NOT_FOUND;
}

bool CommandBar::isWithinPairOfQuoteLeft(){
	bool result = false;
	QVector<QPair<int, int> > quoteLeftPositions = getQuoteLeftPositions();
	for(int i = 0; i < quoteLeftPositions.size(); i++){
		int leftQuoteLeftPos = quoteLeftPositions[i].first;
		int rightQuoteLeftPos = quoteLeftPositions[i].second;
		int currentPos = textCursor().position();
		if(leftQuoteLeftPos < currentPos && currentPos <= rightQuoteLeftPos){
			result = true;
			break;
		}
	}
	return result;
}

//produce pairs of Quote Lefts:
//Assumption made: if the number of QL is odd, the last QL is neglected
QVector<QPair<int, int> > CommandBar::getQuoteLeftPositions(){
	const int NOT_FOUND = -1;
	QString	currentLine = getCurrentLine();
	int index = REGEXP_quoteLeftPairs.indexIn(currentLine);
	QVector<QPair<int, int> > result;

	while (index > NOT_FOUND) {
		int length = REGEXP_quoteLeftPairs.matchedLength();
		int first_pos = index;
		int second_pos = index + length - 1;
		int next = index + length;
		result.push_back(QPair<int, int>(first_pos, second_pos));
		index = REGEXP_quoteLeftPairs.indexIn(currentLine, next);
	}

	return result;
}

//handling of odd number of QL will use this function
bool CommandBar::isEvenQuoteLefts(){
	const int NOT_FOUND = -1;
	const int EVEN = 2;
	int count = 0;
	int pos = 0;
	QRegExp REGEXP_ql(QUOTE_LEFT);
	QString currentLine = getCurrentLine();
	
	while ((pos = REGEXP_ql.indexIn(currentLine, pos)) != NOT_FOUND) {
		++count;
		pos += REGEXP_ql.matchedLength();
	}
	QVector<QPair<int, int> > quoteLeftPositions = getQuoteLeftPositions();
	return count == quoteLeftPositions.size() * EVEN;
}

bool CommandBar::isHotkeyTemplateMode(){
	bool result = true;
	QTextCursor cursor = textCursor();

	cursor.movePosition(QTextCursor::Start);
	QTextCursor result_cursor = document()->find(hotkeyTemplate, cursor);
	if(result_cursor.isNull()){
		result = false;
	}
	return result;
}

QString CommandBar::getWordUnderCursor(){
	QTextCursor cursor = textCursor();
	cursor.select(QTextCursor::WordUnderCursor);
	QString completionPrefix = cursor.selectedText();

	return completionPrefix;
}

//check whether has a certain keyword next to the cursor
bool CommandBar::hasKeywordNearby(QString keyword, QTextCursor::MoveOperation direction){
	QTextCursor cursor = textCursor();
	cursor.movePosition(direction, QTextCursor::KeepAnchor);
	QString str = cursor.selectedText();

	return str == keyword;
}

bool CommandBar::hasQuoteLeft_RHS(){
	return hasKeywordNearby(QUOTE_LEFT, QTextCursor::Right);
}

bool CommandBar::hasQuoteLeft_LHS(){
	return hasKeywordNearby(QUOTE_LEFT, QTextCursor::Left);
}

bool CommandBar::hasKeywordAtStartOfWord(const char* keyword){
	QTextCursor cursor = textCursor();
	cursor.movePosition(QTextCursor::StartOfWord);
	cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
	QString str = cursor.selectedText();

	return str == keyword;
}

bool CommandBar::hasSharpAtStartOfWord(){
	const char* SHARP = "#";
	return hasKeywordAtStartOfWord(SHARP);
}

bool CommandBar::hasPlusAtStartOfWord(){
	const char* PLUS = "+";
	return hasKeywordAtStartOfWord(PLUS);
}

bool CommandBar::hasMinusAtStartOfWord(){
	const char* MINUS = "-";
	return hasKeywordAtStartOfWord(MINUS);
}

//clear a character next to the cursor
void CommandBar::clearCharNearby(QTextCursor::MoveOperation direction){
	QTextCursor cursor = textCursor();
	cursor.movePosition(direction, QTextCursor::KeepAnchor);
	cursor.removeSelectedText();
	setTextCursor(cursor);
}

void CommandBar::clearCharRHS(){
	clearCharNearby(QTextCursor::Right);
}

void CommandBar::clearCharLHS(){
	clearCharNearby(QTextCursor::Left);
}

bool CommandBar::isCommandFind(){
	const char* CMD_FIND = "find ";
	QString currentLine = getCurrentLine();
	return currentLine.startsWith(CMD_FIND, Qt::CaseInsensitive);
}

bool CommandBar::containsCommand(){
	const QString CMD_PATTERN = "^(add|mod|del|find|undo|redo) ";
	const int UNFOUND = -1;
	QString currentLine = textCursor().block().text();
	QRegExp regex(CMD_PATTERN);
	int index = regex.indexIn(currentLine);

	return index != UNFOUND;
}

//******************************
//ENTRANCE of KEY PRESS HANDLING
//******************************
void CommandBar::keyPressEvent(QKeyEvent*event){
	bool isHandled = handleKeyPress(event);
	if(!isHandled){
		QTextEdit::keyPressEvent(event);
	}
}

bool CommandBar::handleKeyPress(QKeyEvent*event){
	bool isHandled = false;
	switch(event->key()){
	case Qt::Key_QuoteLeft:
		if(event->modifiers() != Qt::ShiftModifier)
			handleKeyQuoteLeft(&isHandled);
		break;
	case Qt::Key_Escape:
			handleKeyEscape(&isHandled);
		break;
	case Qt::Key_Tab:
			handleKeyTab(&isHandled);
		break;
	case Qt::Key_Space:
			handleKeySpace(&isHandled);
		break;
	case Qt::Key_Delete:
			handleKeyDelete(&isHandled);
		break;
	case Qt::Key_Backspace:
			handleKeyBackspace(&isHandled);
		break;
	case Qt::Key_Left:
	case Qt::Key_Right:
		if(event->modifiers() == Qt::ShiftModifier)
			handleKeyLeftOrRight();
		break;
	case Qt::Key_Up:
			handleKeyUp();
		break;
	case Qt::Key_Down:
			handleKeyDown();
		break;
	default:
			handleKeyDefault();
		break;
	}
	return isHandled;
}

void CommandBar::handleKeyQuoteLeft(bool *isHandled){
	TURN_OFF_AC
	QTextCursor cursor = textCursor();
	//in auto-completion, press QL will move the cursor between ``, if 
	//the model has `` pair
	if(autoCompleteMode && containsQuoteLeftPair(cursor.selectedText())){
		cursor.clearSelection();
		cursor.movePosition(QTextCursor::Left);
		autoCompleteMode = false;
	}//if the model does not have `` pair, insert `` at the back of it
	else if(autoCompleteMode && cursor.selectedText() != EMPTY){
		cursor.clearSelection();
		cursor.insertText(SPACE + QUOTE_LEFT + QUOTE_LEFT);
		cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor);
		autoCompleteMode = false;
	}//normal processing
	else if(!autoCompleteMode && cursor.selectedText() != EMPTY){
		cursor.removeSelectedText();
		cursor.insertText(QUOTE_LEFT + QUOTE_LEFT);
		cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor);
	}//if within ``, and has a ` next to the cursor (RHS), then go across it
	else if(isWithinPairOfQuoteLeft() && hasQuoteLeft_RHS()){
		cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor);
		cursor.insertText(SPACE);
	}//if not within ``, and has even number of QL, insert ``
	else if(!isWithinPairOfQuoteLeft() && isEvenQuoteLefts()){
		cursor.insertText(QUOTE_LEFT + QUOTE_LEFT);
		cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor);
	}
	else{//other case, just insert `
		cursor.insertText(QUOTE_LEFT);
	}
	setTextCursor(cursor);
	*isHandled = true;
}

void CommandBar::handleKeyEscape(bool *isHandled){
	QTextCursor cursor = textCursor();
	TEXT_EDIT_BEGIN
	cursor.removeSelectedText();
	TEXT_EDIT_END
	setTextCursor(cursor);
	*isHandled = true;
}

//used by shift+tab
void CommandBar::hkTemplateGoBackwards(){
	QTextCursor cursor = textCursor();
	if(isHotkeyTemplateMode()){
		handleHotKeyGoBackwards();
	}
}

void CommandBar::handleHotKeyGoBackwards(){
	lastTimeCursor = document()->find(hotkeyTemplate, textCursor(), QTextDocument::FindBackward);
	if(lastTimeCursor.isNull()){
		moveCursor(QTextCursor::End);
		lastTimeCursor = document()->find(hotkeyTemplate, textCursor(), QTextDocument::FindBackward);
		if(lastTimeCursor.isNull()){
			hotkeyTemplateMode = false;
		}
	}
	if(!(lastTimeCursor.isNull())){
		int length = lastTimeCursor.selectedText().length();
		lastTimeCursor.clearSelection();
		lastTimeCursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, length);
		setTextCursor(lastTimeCursor);
	}
}

void CommandBar::handleHotKeyGoForwards(){
	lastTimeCursor = document()->find(hotkeyTemplate, textCursor());
	if(lastTimeCursor.isNull()){
		moveCursor(QTextCursor::Start);
		lastTimeCursor = document()->find(hotkeyTemplate, textCursor());
		if(lastTimeCursor.isNull()){
			hotkeyTemplateMode = false;
		}
	}
	if(!(lastTimeCursor.isNull())){
		setTextCursor(lastTimeCursor);
		int length = lastTimeCursor.selectedText().length();
		lastTimeCursor.clearSelection();
		lastTimeCursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, length);
		setTextCursor(lastTimeCursor);
	}
}

void CommandBar::handleKeyTab(bool *isHandled){
	TURN_OFF_AC
	QTextCursor cursor = textCursor();
	if(isHotkeyTemplateMode()){
		handleHotKeyGoForwards();
		*isHandled = true;
	}
	else{
		//auto complete the keyword model in auto-complete mode
		if(autoCompleteMode && containsQuoteLeftPair(cursor.selectedText())){
			cursor.clearSelection();
			cursor.movePosition(QTextCursor::Left);
			autoCompleteMode = false;
		}//normal processing
		else if(!autoCompleteMode && cursor.selectedText() != EMPTY){
			cursor.removeSelectedText();
			cursor.insertText(SPACE);
		}//move across the quote left `
		else if(hasQuoteLeft_RHS()){
			cursor.movePosition(QTextCursor::Right);
			cursor.insertText(SPACE);
		}//just insert a space
		else{
			cursor.clearSelection();
			cursor.insertText(SPACE);
		}
		*isHandled = true;
		setTextCursor(cursor);
	}
}

void CommandBar::handleKeySpace(bool *isHandled){
	TURN_OFF_AC
	QTextCursor cursor = textCursor();
	if(cursor.hasSelection() && autoCompleteMode){
		//auto complete the keyword model in auto-complete mode, if it has ``
		if(containsQuoteLeftPair(cursor.selectedText())){
			cursor.clearSelection();
			cursor.movePosition(QTextCursor::Left);
		}//otherwise just insert a space
		else{
			cursor.clearSelection();
			cursor.insertText(SPACE);
		}
		autoCompleteMode = false;
		*isHandled = true;
		setTextCursor(cursor);
	}
}

void CommandBar::handleKeyDelete(bool *isHandled){
	TURN_OFF_AC
	//if cursor is within ``, like `<cursor>`, delete both ``
	if(isWithinPairOfQuoteLeft() && hasQuoteLeft_RHS() && hasQuoteLeft_LHS())
		clearCharLHS();
}

void CommandBar::handleKeyBackspace(bool *isHandled){
	TURN_OFF_AC
	//if cursor is within ``, like `<cursor>`, delete both ``
	if(isWithinPairOfQuoteLeft() && hasQuoteLeft_RHS() && hasQuoteLeft_LHS())
		clearCharRHS();
}

void CommandBar::handleKeyLeftOrRight(){
	autoCompleteMode = false;
}

//handle user input history
//refer to pushCurrentLine
void CommandBar::handleKeyUp(){
	if(!inputHistory_undo.isEmpty()){
		QString lastInput = inputHistory_undo.pop();
		inputHistory_redo.push(lastInput);
		clear();
		TEXT_EDIT_BEGIN
		insertHtml(inputHistory_redo.top());
		TEXT_EDIT_END
	}
}

//handle user input history
//refer to pushCurrentLine
void CommandBar::handleKeyDown(){
	if(!inputHistory_redo.isEmpty()){
		QString prevInput = inputHistory_redo.pop();
		inputHistory_undo.push(prevInput);
		clear();
		if(!inputHistory_redo.isEmpty()){
			TEXT_EDIT_BEGIN
			insertHtml(inputHistory_redo.top());
			TEXT_EDIT_END
		}
		else{
			TEXT_EDIT_BEGIN
			insertHtml(EMPTY);
			TEXT_EDIT_END
		}
	}
}

void CommandBar::handleKeyDefault(){
	TURN_ON_AC
}

void CommandBar::mousePressEvent(QMouseEvent *event){
	autoCompleteMode = false;
	QTextEdit::mousePressEvent(event);
}

//create a hotkey template according to templateStr
//hotkey template is a quite useful feature that inserts the templateStr for user,
//then the user can use tab/shift-tab to go thru any blanks and start typing, without
//need to type all the sentence
//btw, blank is defined as "__[A-Z]+__"
void CommandBar::createTemplate(QString templateStr){
	pushCurrentLine();
	TEXT_EDIT_BEGIN
	insertPlainText(templateStr);
	TEXT_EDIT_END
	moveCursor(QTextCursor::Start);
	lastTimeCursor = document()->find(hotkeyTemplate, textCursor());
	if(!lastTimeCursor.isNull())
		setTextCursor(lastTimeCursor);
	hotkeyTemplateMode = true;
}

void CommandBar::createTemplateAdd(){
	createTemplate(HOTKEY_TEMPLATE_ADD);
}

void CommandBar::createTemplateAddTimed(){
	createTemplate(HOTKEY_TEMPLATE_ADD_TIMED);
}

void CommandBar::createTemplateModDone(){
	createTemplate(HOTKEY_TEMPLATE_MOD_DONE);
}

void CommandBar::createTemplateModByName(){
	createTemplate(HOTKEY_TEMPLATE_MOD_BY_NAME);
}

void CommandBar::createTemplateModByIndex(){
	createTemplate(HOTKEY_TEMPLATE_MOD_BY_INDEX);
}

void CommandBar::createTemplateDelByName(){
	createTemplate(HOTKEY_TEMPLATE_DEL_BY_NAME);
}

void CommandBar::createTemplateDelByIndex(){
	createTemplate(HOTKEY_TEMPLATE_DEL_BY_INDEX);
}

void CommandBar::createTemplateFind(){
	createTemplate(HOTKEY_TEMPLATE_FIND);
}

void CommandBar::createTemplateUndo(){
	createTemplate(HOTKEY_TEMPLATE_UNDO);
}

void CommandBar::createTemplateRedo(){
	createTemplate(HOTKEY_TEMPLATE_REDO);
}

//in quick add window, only template add is allowed
void CommandBar::setQuickAddMode(){
	modifyDone->setEnabled(false);
	modifyByName->setEnabled(false);
	modifyByIndex->setEnabled(false);
	delByName->setEnabled(false);
	delByIndex->setEnabled(false);
	find->setEnabled(false);
	undo->setEnabled(false);
	redo->setEnabled(false);
}
