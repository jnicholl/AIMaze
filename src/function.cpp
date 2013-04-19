/*
 * Function.cpp
 *
 *  Created on: Apr 18, 2013
 *      Author: jnicholl
 */

#include "function.h"

Function::Function(int size)
{
	for (int i = 0; i < size; i++) {
		m_commands.append(ApplicationUI::CMD_EMPTY);
	}
}

int Function::count() const {
	return m_commands.count();
}

ApplicationUI::CommandType Function::operator[](int index) {
	return m_commands[index];
}

void Function::append(ApplicationUI::CommandType cmd) {
	for (int i = 0; i < count(); i++) {
		if (m_commands[i] == ApplicationUI::CMD_EMPTY) {
			m_commands[i] = cmd;
			break;
		}
	}
}

void Function::remove(int index) {
	if (index < 0)
		return;
	if (m_commands[index] == ApplicationUI::CMD_EMPTY)
		return;

	for (int i = index; i < m_commands.count() - 1; i++) {
		m_commands[i] = m_commands[i + 1];
	}

	m_commands[m_commands.count() - 1] = ApplicationUI::CMD_EMPTY;
}
