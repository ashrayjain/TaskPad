#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class Highlighter: public QSyntaxHighlighter
{
	Q_OBJECT

public:
	enum HIGHLIGHT_TYPE { COMMAND = 0, KEYWORD = 1, PAIR_QUOTE_LEFT = 2, BLANK = 3 };

	Highlighter(QTextDocument *parent);
	void highlightBlock(const QString &text){  highlightPatterns(text);   }

private:
	static const char* COLOR_BLACK;
	static const char* COLOR_BLUE;
	static const char* COLOR_GRAY;
	static const char* COLOR_PURPLE;
	static const char* REGEX_COMMAND;
	static const char* REGEX_KEYWORD;
	static const char* REGEX_QUOTE_LEFT_PAIRS;
	static const char* REGEX_HOTKEY_TEMPLATE_BLANK;

	void addRegex(HIGHLIGHT_TYPE type, const QString &pattern, bool minimal = true);
	void highlightPatterns(const QString &text);

	QTextCharFormat CommandFormat;
	QTextCharFormat KeywordFormat;
	QTextCharFormat PairOfQuoteLeft;
	QTextCharFormat BlankFormat;
	QVector<QRegExp> regexForType;
};
