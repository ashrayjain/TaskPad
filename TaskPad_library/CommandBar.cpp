#include <QShortcut>
#include <QMimeData>
#include "CommandBar.h"
#include "Highlighter.h"

const QStringList CommandBar::COMMAND_LIST = QStringList() \
	<< "add ``" << "mod" << "del" << "find" << "undo" <<     \
	"redo" << "sync";

const QStringList CommandBar::KEYWORD_LIST = QStringList() \
	<< "name ``" << "due ``" << "from ``" << "to ``" << "at ``" << "ppl ``" \
	<< "note ``" << "impt ``" << "rt ``" << "done" << "undone" <<  \
	"exact ``";

const QStringList CommandBar::KEYWORD_LIST_FIND = QStringList() \
	<< "name ``" << "from ``" << "to ``" << "at ``" << "ppl ``" \
	<< "note ``" << "impt ``" << "rt ``" << "done" << "undone" <<  \
	"deadline" << "timed" << "floating" << "exact name ``";

const QString CommandBar::SPACE = " ";
const QString CommandBar::INCLUDE_QUOTE_LEFT_PAIR = "(\\w+ ``)|( ``)|(``)";
const QString CommandBar::SINGLE_QUOTATION_MARK = "'";
const QString CommandBar::QUOTE_LEFT = "`";
const QString CommandBar::EMPTY = "";
const QString CommandBar::HOTKEY_TEMPLATE_ADD = "add `__NAME__` due `__DATE__` impt `__PRIORITY__` at `__WHERE__` ppl `__PARTICIPANTS__` #__TAGS__ rt `__REMINDTIME__` note `__NOTE__`";
const QString CommandBar::HOTKEY_TEMPLATE_ADD_TIMED = "add `__NAME__` from `__DATE__` to `__DATE__` impt `__PRIORITY__` at `__WHERE__` ppl `__PARTICIPANTS__` #__TAGS__ rt `__REMINDTIME__` note `__NOTE__`";
const QString CommandBar::HOTKEY_TEMPLATE_MOD_DONE = "mod __INDEX__ done";
const QString CommandBar::HOTKEY_TEMPLATE_MOD_BY_NAME = "mod `__NAME__` name `__MODIFIEDNAME__` from `__DATE__` to `__DATE__` impt `__PRIORITY__` at `__WHERE__` ppl `__PARTICIPANTS__` #__TAGS__ rt `__REMINDTIME__` note `__NOTE__` __DONE__";
const QString CommandBar::HOTKEY_TEMPLATE_MOD_BY_INDEX = "mod __INDEX__ name `__MODIFIEDNAME__` from `__DATE__` to `__DATE__` impt `__PRIORITY__` at `__WHERE__` ppl `__PARTICIPANTS__` #__TAGS__ rt `__REMINDTIME__` note `__NOTE__` __DONE__";
const QString CommandBar::HOTKEY_TEMPLATE_DEL_BY_NAME = "del `__NAME__`";
const QString CommandBar::HOTKEY_TEMPLATE_DEL_BY_INDEX = "del __INDEX__";
const QString CommandBar::HOTKEY_TEMPLATE_FIND = "find name `__NAME__` from `__DATE__` to `__DATE__` impt `__PRIORITY__` at `__WHERE__` ppl `__PARTICIPANTS__` note `__NOTE__`";
const QString CommandBar::HOTKEY_TEMPLATE_UNDO = "undo";
const QString CommandBar::HOTKEY_TEMPLATE_REDO = "redo";

CommandBar::CommandBar(QWidget *parent)
	:QTextEdit(parent), inputHistory_undo(), inputHistory_redo(),\
	hotkeyTemplate("__[A-Z]+__", Qt::CaseInsensitive),
	REGEXP_quoteLeft("`(.*)`")
{
	REGEXP_quoteLeft.setPatternSyntax(QRegExp::RegExp2);
	REGEXP_quoteLeft.setMinimal(true);
	initWidgets();
	initConnections();
	hotkeyTemplate.setPatternSyntax(QRegExp::RegExp2);
}

void CommandBar::initWidgets()
{
	autoCompleteToggle(true);
	hotkeyTemplateMode = false;
	initModel();
	initCompleter();
	(void) new Highlighter(document());
}

void CommandBar::initModel(){
	model = new QStringListModel(this);
	produceCommandModel();
}

void CommandBar::initCompleter()
{
	completer = new QCompleter(this);
	completer->setWidget(this);
	completer->setCompletionMode(QCompleter::InlineCompletion);
	completer->setModel(model);
	completer->setModelSorting(QCompleter::UnsortedModel);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	completer->setWrapAround(true);
}

void CommandBar::initConnections()
{
	connect(this, SIGNAL(textChanged()), this, SLOT(performCompletion()));
	newDeadlineTask =  new QShortcut(QKeySequence(tr("Ctrl+N", "New Deadline Task")), 
		this, SLOT(createTemplateAdd()));
	newTimedTask = new QShortcut(QKeySequence(tr("Ctrl+Shift+N", "New Timed Task")), 
		this, SLOT(createTemplateAddTimed()));
	modifyDone = new QShortcut(QKeySequence(tr("Ctrl+M", "Modify Task done")), 
		this, SLOT(createTemplateModDone()));
	modifyByName = new QShortcut(QKeySequence(tr("Ctrl+Alt+Shift+M", "Modify Task by Name")), 
		this, SLOT(createTemplateModByName()));
	modifyByIndex = new QShortcut(QKeySequence(tr("Ctrl+Shift+M", "Modify Task by Index")), 
		this, SLOT(createTemplateModByIndex()));
	delByName = new QShortcut(QKeySequence(tr("Ctrl+Shift+D", "Delete Task by Name")), 
		this, SLOT(createTemplateDelByName()));
	delByIndex = new QShortcut(QKeySequence(tr("Ctrl+D", "Delete Task by Index")), 
		this, SLOT(createTemplateDelByIndex()));
	find = new QShortcut(QKeySequence(tr("Ctrl+F", "Find Task")), 
		this, SLOT(createTemplateFind()));
	undo = new QShortcut(QKeySequence(tr("Ctrl+U", "Undo")), 
		this, SLOT(createTemplateUndo()));
	redo = new QShortcut(QKeySequence(tr("Ctrl+R", "Redo")), 
		this, SLOT(createTemplateRedo()));
	(void) new QShortcut(QKeySequence(tr("Shift+Tab", "HotKey Template: Go Backwards")), this, SLOT(hkTemplateGoBackwards()));
}

QString CommandBar::getCurrentLine()
{
	QRegExp regex("__[A-Z]+__");//magic string...

	QString result = toPlainText();
	result.replace(regex, "");
	return result;
}

void CommandBar::pushCurrentLine()
{
	QString currentInput = toPlainText();
	if(currentInput != EMPTY)
	{
		while(!inputHistory_redo.isEmpty())
		{
			QString input = inputHistory_redo.pop();
			inputHistory_undo.push(input);
		}

		inputHistory_undo.push(currentInput);
		clear();
	}
}

void CommandBar::autoCompleteToggle(bool flag)
{
	autoCompleteFlag = flag;
}

void CommandBar::performCompletion()
{
	produceModel();

	if(autoCompleteFlag)
	{
		QString completionPrefix = getWordUnderCursor();
		if (!completionPrefix.isEmpty() &&
			isLastCharLetter(completionPrefix))
		{
			performCompletion(completionPrefix);
		}
	}
}

void CommandBar::performCompletion(const QString &completionPrefix)
{
	if (completionPrefix != completer->completionPrefix())
	{
		completer->setCompletionPrefix(completionPrefix);
	}
	if (completer->completionCount() > 0)
	{
		insertCompletion(completer->currentCompletion());
	}
}

void CommandBar::insertCompletion(const QString &completion)
{
	QTextCursor cursor = textCursor();
	int numberOfCharsToComplete = completion.length() -completer->completionPrefix().length();
	if(numberOfCharsToComplete > 0)// if == 0, nothing to complete
	{
		int insertionPosition = cursor.position();
		cursor.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);

		TEXT_EDIT_BEGIN
		cursor.insertHtml( completion );
		TEXT_EDIT_END

		cursor.setPosition(insertionPosition);//back to prev. cursor position
		cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, numberOfCharsToComplete);
		setTextCursor(cursor);
	}
}

void CommandBar::insertFromMimeData(const QMimeData *source){
	if(source->hasText()){
		QString strToInsert = source->text();
		insertPlainText(strToInsert);
	}
}

bool CommandBar::containsQuoteLeftPair(QString str){
	QRegExp regex(INCLUDE_QUOTE_LEFT_PAIR);
	int index = regex.indexIn(str);
	return index != -1;//todo: magic number
}

bool CommandBar::isWithinPairOfQuoteLeft(){
	bool result = false;
	QVector<QPair<int, int> > quoteLeftPositions = getQuoteLeftPositions();
	for(int i = 0; i < quoteLeftPositions.size(); i++){
		int leftPos = quoteLeftPositions[i].first;
		int rightPos = quoteLeftPositions[i].second;
		int currentPos = textCursor().position();
		if(leftPos < currentPos && currentPos < rightPos){
			result = true;
			break;
		}
	}
	return result;
}

QVector<QPair<int, int> > CommandBar::getQuoteLeftPositions(){
	QVector<QPair<int, int> > result;
	QString currentLine = getCurrentLine();
	int index = REGEXP_quoteLeft.indexIn(currentLine);
	while (index > -1) {//TODO: magic number
		int length = REGEXP_quoteLeft.matchedLength();
		result.push_back(QPair<int, int>(index, index + length - 1));
		index = REGEXP_quoteLeft.indexIn(currentLine, index + length);//find next
	}
	return result;
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

bool CommandBar::isLastCharLetter(QString str)
{
	const int END_INDEX = str.length() - 1;

	bool result = str.at(END_INDEX).isLetter();
	return result;
}

QString CommandBar::getWordUnderCursor()
{
	QTextCursor cursor = textCursor();
	cursor.select(QTextCursor::WordUnderCursor);

	QString completionPrefix = cursor.selectedText();
	return completionPrefix;
}

bool CommandBar::hasKeywordNearby(QString keyword, QTextCursor::MoveOperation direction)
{
	QTextCursor cursor = textCursor();
	cursor.movePosition(direction, QTextCursor::KeepAnchor);
	QString str = cursor.selectedText();

	return str == keyword;
}

bool CommandBar::hasSingleQuotationMark_RHS()
{
	return hasKeywordNearby(SINGLE_QUOTATION_MARK, QTextCursor::Right);
}

bool CommandBar::hasSingleQuotationMark_LHS()
{
	return hasKeywordNearby(SINGLE_QUOTATION_MARK, QTextCursor::Left);
}

bool CommandBar::hasQuoteLeft_RHS()
{
	return hasKeywordNearby(QUOTE_LEFT, QTextCursor::Right);
}

bool CommandBar::hasQuoteLeft_LHS()
{
	return hasKeywordNearby(QUOTE_LEFT, QTextCursor::Left);
}

bool CommandBar::hasSpace_RHS()
{
	return hasKeywordNearby(SPACE, QTextCursor::Right);
}

bool CommandBar::hasSharp_LHS(){
	QTextCursor cursor = textCursor();
	cursor.movePosition(QTextCursor::StartOfWord);
	cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
	QString str = cursor.selectedText();

	return str == "#";
}

bool CommandBar::hasSpace_LHS()
{
	return hasKeywordNearby(SPACE, QTextCursor::Left);
}

void CommandBar::clearCharNearby(QTextCursor::MoveOperation direction){
	QTextCursor cursor = textCursor();
	cursor.movePosition(direction, QTextCursor::KeepAnchor);

	TEXT_EDIT_BEGIN
	cursor.removeSelectedText();
	TEXT_EDIT_END

	setTextCursor(cursor);
}

void CommandBar::clearCharRHS()
{
	clearCharNearby(QTextCursor::Right);
}

void CommandBar::clearCharLHS()
{
	clearCharNearby(QTextCursor::Left);
}

void CommandBar::produceModel()
{
	if(isWithinPairOfQuoteLeft() || hasSharp_LHS()){
		model->setStringList(QStringList());
	}
	else if(containsCommand())
	{
		if(isCommandFind())
			produceKeywordModel_forFind();
		else
			produceKeywordModel();
	}
	else
	{
		produceCommandModel();
	}
}

void CommandBar::produceCommandModel()
{
	model->setStringList(COMMAND_LIST);
}

void CommandBar::produceKeywordModel()//common
{
	model->setStringList(KEYWORD_LIST);
}

void CommandBar::produceKeywordModel_forFind()
{
	model->setStringList(KEYWORD_LIST_FIND);
}

bool CommandBar::isCommandFind(){
	QString currentLine = getCurrentLine();
	return currentLine.startsWith("find ", Qt::CaseInsensitive);
}

bool CommandBar::containsCommand()
{
	const QString CMD_PATTERN = "^(add|mod|del|find|undo|redo|sync) ";
	const int UNFOUND = -1;
	QString currentLine = textCursor().block().text();
	QRegExp regex(CMD_PATTERN);
	int index = regex.indexIn(currentLine);

	return index != UNFOUND;
}

void CommandBar::keyPressEvent(QKeyEvent*event)
{
	bool isHandled = handleKeyPress(event);
	if(!isHandled)
	{
		QTextEdit::keyPressEvent(event);
	}
}

bool CommandBar::handleKeyPress(QKeyEvent*event)
{
	bool isHandled = false;
	switch(event->key())
	{
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
	case Qt::Key_Delete:	// Fallthrough
			handleKeyDelete(&isHandled);
		break;
	case Qt::Key_Backspace:
			handleKeyBackspace(&isHandled);
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

void CommandBar::handleKeyQuoteLeft(bool *isHandled)
{
	QTextCursor cursor = textCursor();
	if((hasSpace_LHS() && cursor.atEnd()) || (hasSpace_LHS() && hasSpace_RHS())){
		cursor.insertText(QUOTE_LEFT + QUOTE_LEFT);
		cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor);
	}
	else if(hasQuoteLeft_RHS()){
		cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor);
		cursor.insertText(SPACE);
	}
	else if(containsQuoteLeftPair(cursor.selectedText())){
		TEXT_EDIT_BEGIN
		cursor.clearSelection();
		cursor.movePosition(QTextCursor::Left);
		TEXT_EDIT_END
	}
	else{
		cursor.clearSelection();
		cursor.insertText(SPACE + QUOTE_LEFT + QUOTE_LEFT);
		cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor);
	}
	setTextCursor(cursor);
	*isHandled = true;
}

void CommandBar::handleKeyEscape(bool *isHandled)
{
	QTextCursor cursor = textCursor();
	TEXT_EDIT_BEGIN
	cursor.removeSelectedText();
	TEXT_EDIT_END
	setTextCursor(cursor);
	*isHandled = true;
}

//TODO: dynamically check hotkeyTemplateMode
//TODO: combind this with go forwards
void CommandBar::hkTemplateGoBackwards(){
	QTextCursor cursor = textCursor();
	if(isHotkeyTemplateMode())
	{
		lastTimeCursor = document()->find(hotkeyTemplate, lastTimeCursor, QTextDocument::FindBackward);
		if(lastTimeCursor.isNull())
		{
			moveCursor(QTextCursor::End);
			lastTimeCursor = document()->find(hotkeyTemplate, textCursor(), QTextDocument::FindBackward);
			if(lastTimeCursor.isNull())
			{
				hotkeyTemplateMode = false;
			}
		}
		if(!(lastTimeCursor.isNull()))
		{
			setTextCursor(lastTimeCursor);
		}
	}
}

void CommandBar::handleKeyTab(bool *isHandled)
{
	//TODO: tab control need to be consistent
	QTextCursor cursor = textCursor();
	if(isHotkeyTemplateMode())//TODO: field hotkeyTemplateMode seems to be non-sense
	{
		lastTimeCursor = document()->find(hotkeyTemplate, lastTimeCursor);
		if(lastTimeCursor.isNull())
		{
			lastTimeCursor.movePosition(QTextCursor::Start);
			lastTimeCursor = document()->find(hotkeyTemplate, lastTimeCursor);
			if(lastTimeCursor.isNull())
			{
				hotkeyTemplateMode = false;
			}
		}
		if(!(lastTimeCursor.isNull()))
		{
			setTextCursor(lastTimeCursor);
		}
		*isHandled = true;
	}
	else
	{
		if(containsQuoteLeftPair(cursor.selectedText())){
			TEXT_EDIT_BEGIN
			cursor.clearSelection();
			cursor.movePosition(QTextCursor::Left);
			TEXT_EDIT_END
		}
		else if(hasQuoteLeft_RHS()){
			TEXT_EDIT_BEGIN
			cursor.movePosition(QTextCursor::Right);
			cursor.insertText(SPACE);
			TEXT_EDIT_END
		}
		else{
			TEXT_EDIT_BEGIN
			cursor.clearSelection();
			cursor.insertText(SPACE);
			TEXT_EDIT_END
		}
		*isHandled = true;
		setTextCursor(cursor);
	}
}

void CommandBar::handleKeySpace(bool *isHandled)
{
	//TODO: tab control need to be consistent
	QTextCursor cursor = textCursor();
	if(cursor.hasSelection())
	{
		if(containsQuoteLeftPair(cursor.selectedText())){
			TEXT_EDIT_BEGIN
			cursor.clearSelection();
			cursor.movePosition(QTextCursor::Left);
			TEXT_EDIT_END
		}
		else{
			TEXT_EDIT_BEGIN
			cursor.clearSelection();
			cursor.insertText(SPACE);
			TEXT_EDIT_END
		}
		*isHandled = true;
		setTextCursor(cursor);
	}
}

void CommandBar::handleKeyDelete(bool *isHandled)
{
	TURN_OFF_AC
}

void CommandBar::handleKeyBackspace(bool *isHandled)
{
	TURN_OFF_AC
}

void CommandBar::handleKeyUp()
{
	if(!inputHistory_undo.isEmpty())
	{
		QString lastInput = inputHistory_undo.pop();
		inputHistory_redo.push(lastInput);

		clear();
		TEXT_EDIT_BEGIN
		insertHtml(inputHistory_redo.top());
		TEXT_EDIT_END
	}
}

void CommandBar::handleKeyDown()
{
	if(!inputHistory_redo.isEmpty())
	{
		QString prevInput = inputHistory_redo.pop();
		inputHistory_undo.push(prevInput);

		clear();
		if(!inputHistory_redo.isEmpty())
		{
			TEXT_EDIT_BEGIN
			insertHtml(inputHistory_redo.top());
			TEXT_EDIT_END
		}
		else
		{
			TEXT_EDIT_BEGIN
			insertHtml(EMPTY);
			TEXT_EDIT_END
		}
	}
}

void CommandBar::handleKeyDefault()
{
	TURN_ON_AC
}

void CommandBar::createTemplate(QString templateStr)
{
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
