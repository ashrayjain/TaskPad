/*
 * =====================================================================================
 *
 *		Filename:  Messenger.h
 *
 *      Version:  1.0
 *
 *      Author:  Thyaegsh Manikandan (A0100124J)
 *		Organization:  NUS, SoC
 *
 * =====================================================================================

 ** Gist of File Contents:
  *
  * This file defines and implements a Show Command. 
  * The Command_Show class inherits from Command class.
  *
 */
#ifndef _COMMAND_SHOW_H_ 
#define _COMMAND_SHOW_H_

#include "Command.h"

class Command_Show: public Command {
public:
	Command_Show	() {init(); setCommandType(TP::SHOW); }
	~Command_Show	() { }

	int		getIndex()			    { return Command::getIndex();		}
	void	setIndex(int idx)		{ Command::setIndex(idx);			}
	bool	getFlagIndex()			{ return Command::getFlagIndex();	}
};

#endif
