#include "Highlighter.h"

const char* Highlighter::COLOR_BLACK                 = "#232323";
const char* Highlighter::COLOR_BLUE                  = "#397CD4";
const char* Highlighter::COLOR_GRAY                  = "#787878";
const char* Highlighter::COLOR_PURPLE                = "#742894";
const char* Highlighter::REGEX_COMMAND               = "^(add|mod|del|find|undo|redo)";
const char* Highlighter::REGEX_KEYWORD               = " exact| name| by| due| from| to| location|"\
										 "place| at| with| ppl| note| priority| impt"\
	"| remind| rt| overdue| done| undone| deadline| timed| floating| #| -by| -due"\
	"| -from| -to| -with(all|[^a])| -ppl(all|[^a])| -remind(all|[^a])| -rt(all|[^a])|"\
	"-#| \\+with| \\+ppl| \\+remind| \\+rt| \\+#";
const char* Highlighter::REGEX_QUOTE_LEFT_PAIRS      = "`(.*)`";
const char* Highlighter::REGEX_HOTKEY_TEMPLATE_BLANK = "__[A-Z]+__";

Highlighter::Highlighter(QTextDocument *parent)
	:QSyntaxHighlighter(parent){
	CommandFormat.setFontWeight(QFont::Bold);
	CommandFormat.setForeground(QBrush(COLOR_BLACK));
	KeywordFormat.setFontWeight(QFont::Bold);
	KeywordFormat.setForeground(QBrush(COLOR_BLUE));
	PairOfQuoteLeft.setFontWeight(QFont::Bold);
	PairOfQuoteLeft.setForeground(QBrush(COLOR_GRAY));
	BlankFormat.setFontWeight(QFont::Bold);
	BlankFormat.setForeground(QBrush(COLOR_PURPLE));

	addRegex(COMMAND, REGEX_COMMAND);
	addRegex(KEYWORD, REGEX_KEYWORD);
	addRegex(PAIR_QUOTE_LEFT, REGEX_QUOTE_LEFT_PAIRS);
	addRegex(BLANK, REGEX_HOTKEY_TEMPLATE_BLANK);
}

void Highlighter::addRegex(HIGHLIGHT_TYPE type, const QString &pattern, bool minimal){
	QRegExp regex(pattern);
	regex.setPatternSyntax(QRegExp::RegExp2);
	regex.setMinimal(minimal);
	regexForType.push_back(regex);
}

void Highlighter::highlightPatterns(const QString &text){
	for(int i = 0; i < regexForType.size(); i++){//foreach regex
		const QRegExp &regex = regexForType[i];
		int index = regex.indexIn(text);
		while (index > -1) {//foreach regex's occurrence
			int length = regex.matchedLength();
			if (i == COMMAND)
				setFormat(index, length, CommandFormat);
			else if (i == KEYWORD)
				setFormat(index, length, KeywordFormat);
			else if (i == PAIR_QUOTE_LEFT)
				setFormat(index, length, PairOfQuoteLeft);
			else if (i == BLANK)
				setFormat(index, length, BlankFormat);
			index = regex.indexIn(text, index + length);
		}
	}
}
