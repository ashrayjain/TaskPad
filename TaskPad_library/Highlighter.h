#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class Highlighter: public QSyntaxHighlighter
{
	Q_OBJECT

public:
	enum HIGHLIGHT_TYPE { COMMAND = 0, KEYWORD = 1, PAIR_QUOTE_LEFT = 2, BLANK = 3 };

	Highlighter(QTextDocument *parent)
	:QSyntaxHighlighter(parent)
	{
		CommandFormat.setFontWeight(QFont::Bold);
		CommandFormat.setForeground(QBrush("#232323"));
		KeywordFormat.setFontWeight(QFont::Bold);
		KeywordFormat.setForeground(QBrush("#397CD4"));
		PairOfQuoteLeft.setFontWeight(QFont::Bold);
		PairOfQuoteLeft.setForeground(QBrush("#787878"));
		BlankFormat.setFontWeight(QFont::Bold);
		BlankFormat.setForeground(QBrush("#742894"));

		addRegex(COMMAND, "^(add|mod|del|find|undo|redo|sync)");
		addRegex(KEYWORD, " exact| name| by| due| from| to| location| place| at| with| ppl| note| priority| impt"\
						"| remind| rt| done| undone| deadline| timed| floating| #| -by| -due"\
						"| -from| -to| -with(all|[^a])| -ppl(all|[^a])| -remind(all|[^a])| -rt(all|[^a])| -#| \\+with| \\+ppl| \\+remind| \\+rt| \\+#");
		addRegex(PAIR_QUOTE_LEFT, "`(.*)`");
		addRegex(BLANK, "__[A-Z]+__");
	}

	void highlightBlock(const QString &text)
	{
		highlightPatterns(text);
	}

private:
	void addRegex(HIGHLIGHT_TYPE type, const QString &pattern, bool minimal = true)
	{
		QRegExp regex(pattern);
		regex.setPatternSyntax(QRegExp::RegExp2);
		regex.setMinimal(minimal);
		regexForType.push_back(regex);
	}

	void highlightPatterns(const QString &text)
	{
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

	QTextCharFormat CommandFormat;
	QTextCharFormat KeywordFormat;
	QTextCharFormat PairOfQuoteLeft;
	QTextCharFormat BlankFormat;
	QVector<QRegExp> regexForType;
};