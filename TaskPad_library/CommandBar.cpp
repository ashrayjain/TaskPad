#include <QShortcut>
#include "CommandBar.h"
#include "Highlighter.h"

const QStringList CommandBar::COMMAND_LIST = QStringList() \
	<< "add" << "mod" << "del" << "find" << "undo" <<     \
	"redo" << "sync";

const QStringList CommandBar::KEYWORD_LIST = QStringList() \
	<< "name" << "due" << "from" << "to" << "at" << "ppl" \
	<< "note" << "impt" << "rt" << "done" << "undone" <<  \
	"deadline" << "timed" << "floating" << "exact";

const QString CommandBar::SPACE = " ";
const QString CommandBar::SINGLE_QUOTATION_MARK = "'";
const QString CommandBar::QUOTE_LEFT = "`";
const QString CommandBar::EMPTY = "";
const QString CommandBar::HOTKEY_TEMPLATE_NEW = "add '__NAME__' due '__DATE__' at '__WHERE__' note '__NOTE__'";

CommandBar::CommandBar(QWidget *parent)
	:QTextEdit(parent), inputHistory_undo(), inputHistory_redo(),\
	hotkeyTemplate("__[A-Z]+__", Qt::CaseInsensitive)
{
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
	(void) new QShortcut(QKeySequence(tr("Shift+Tab", "HotKey Template: Go Backwards")), this, SLOT(hkTemplateGoBackwards()));
	(void) new QShortcut(QKeySequence(tr("Alt+'", "Insert Single Quotation Mark")), this, SLOT(insertSingleQuotationMark()));
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
		cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
		setTextCursor(cursor);
	}
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
	bool result;

	QTextCursor cursor = textCursor();
	cursor.movePosition(direction, QTextCursor::KeepAnchor);
	QString str = cursor.selectedText();

	result = str == keyword;
	return result;
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
	if(containsCommand())
	{
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

void CommandBar::produceKeywordModel()
{
	model->setStringList(KEYWORD_LIST);
}

bool CommandBar::containsCommand()
{
	bool result = false;
	QString currentLine = textCursor().block().text();

	for(int i = 0; i < COMMAND_LIST.length(); i++)
	{
		if(currentLine.startsWith(COMMAND_LIST.at(i)))
		{
			result = true;
			break;
		}
	}
	return result;
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
	if(hotkeyTemplateMode)
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

void CommandBar::insertSingleQuotationMark(){
	insertPlainText(SINGLE_QUOTATION_MARK);
}

void CommandBar::handleKeyTab(bool *isHandled)
{
	//TODO: tab control need to be consistent
	QTextCursor cursor = textCursor();
	if(hotkeyTemplateMode)
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
		TEXT_EDIT_BEGIN
		cursor.clearSelection();
		cursor.insertText(SPACE);
		TEXT_EDIT_END
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
		TEXT_EDIT_BEGIN
		cursor.clearSelection();
		cursor.insertText(SPACE);
		TEXT_EDIT_END
		*isHandled = true;
		setTextCursor(cursor);
	}
}

void CommandBar::handleKeyDelete(bool *isHandled)
{
	TURN_OFF_AC
	if(hasQuoteLeft_RHS() && !hasQuoteLeft_LHS()){
		moveCursor(QTextCursor::Right);
		*isHandled = true;
	}
	else if(hasQuoteLeft_RHS() && hasQuoteLeft_LHS()){
		clearCharLHS();
	}
}

void CommandBar::handleKeyBackspace(bool *isHandled)
{
	TURN_OFF_AC
	if(!hasQuoteLeft_RHS() && hasQuoteLeft_LHS()){
		moveCursor(QTextCursor::Left);
		*isHandled = true;
	}
	else if(hasQuoteLeft_RHS() && hasQuoteLeft_LHS()){
		clearCharRHS();
	}
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

void CommandBar::createNewTaskTemplate()
{
	pushCurrentLine();
	TEXT_EDIT_BEGIN
	insertPlainText(HOTKEY_TEMPLATE_NEW);
	TEXT_EDIT_END
	
	moveCursor(QTextCursor::Start);

	lastTimeCursor = document()->find(hotkeyTemplate, textCursor());
	setTextCursor(lastTimeCursor);

	hotkeyTemplateMode = true;//TODO: need dynamically change this mode
}