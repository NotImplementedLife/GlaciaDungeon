#include "langs.h"

int CURRENT_LANGUAGE;

void set_current_language(int LANGUAGE)
{
	CURRENT_LANGUAGE = LANGUAGE;
}

const char* const get_message(int LMSG)
{
	return MESSAGES[LANGUAGES_COUNT * LMSG + CURRENT_LANGUAGE];
}