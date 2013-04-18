/*
 * Function.h
 *
 *  Created on: Apr 18, 2013
 *      Author: jnicholl
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <QtCore/QList>

#include "applicationui.hpp"

#define DEFAULT_FUNCTION_SIZE 7

class Function {
public:
	Function(int size=DEFAULT_FUNCTION_SIZE) {
		for (int i=0; i<size; i++) {
			m_commands.append(ApplicationUI::CMD_EMPTY);
		}
	}

	int count() const { return m_commands.count(); }
	ApplicationUI::CommandType operator[](int index) { return m_commands[index]; }

	void append(ApplicationUI::CommandType cmd) {
		for (int i=0; i<count(); i++) {
			if (m_commands[i] == CMD_EMPTY) {
				m_commands[i] = cmd;
				break;
			}
		}
	}

	void remove(int index) {

	}
private:
	QList<ApplicationUI::CommandType> m_commands;
};

#endif /* FUNCTION_H_ */
