#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class Highlighter: public QSyntaxHighlighter
{
	Q_OBJECT

public:
	enum HIGHLIGHT_TYPE { COMMAND, KEYWORD, BLANK };

	Highlighter(QTextDocument *parent)
	:QSyntaxHighlighter(parent)
	{
		CommandFormat.setFontWeight(QFont::Bold);
		CommandFormat.setForeground(QBrush("#232323"));
		KeywordFormat.setFontWeight(QFont::Bold);
		KeywordFormat.setForeground(QBrush("#397CD4"));
		BlankFormat.setFontWeight(QFont::Bold);
		BlankFormat.setForeground(QBrush("#742894"));
		addRegex(COMMAND, "^(add|mod|del|find|undo|redo|sync)");
		addRegex(KEYWORD, " name| due| from| to| at| ppl| note| impt"\
						"| rt| done| undone| deadline| timed| floating| #");
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
		regexForType.insert(type, regex);
	}

	void highlightPatterns(const QString &text)
	{
		QHashIterator<HIGHLIGHT_TYPE, QRegExp> i(regexForType);
		while (i.hasNext()) {
			i.next();
			HIGHLIGHT_TYPE type = i.key();
			const QRegExp &regex = i.value();
			int index = regex.indexIn(text);
			while (index > -1) {
				int length = regex.matchedLength();
				if (type == COMMAND)
					setFormat(index, length, CommandFormat);
				else if (type == KEYWORD)
					setFormat(index, length, KeywordFormat);
				else if (type == BLANK)
					setFormat(index, length, BlankFormat);
				index = regex.indexIn(text, index + length);
			}
		}
	}

	QTextCharFormat CommandFormat;
	QTextCharFormat KeywordFormat;
	QTextCharFormat BlankFormat;
	QMultiHash< HIGHLIGHT_TYPE, QRegExp > regexForType;
};