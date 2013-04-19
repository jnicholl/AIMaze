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
	Function(int size=DEFAULT_FUNCTION_SIZE);

	// Number of actions (including empty ones)
	int count() const;
	int commandCount() const { return m_commandCount; }
	ApplicationUI::CommandType operator[](int index);
	ApplicationUI::CommandType at(int index) const { return m_commands[index]; }

	void append(ApplicationUI::CommandType cmd);

	void remove(int index);

private:
	QList<ApplicationUI::CommandType> m_commands;
	int m_commandCount; // number of non-empty actions
};

class FunctionRunner {
public:
	FunctionRunner(const Function *f)
		: m_function(f)
		, m_pos(0)
	{}

	ApplicationUI::CommandType step() {
		ApplicationUI::CommandType ret = ApplicationUI::CMD_EMPTY;
		if (m_pos < m_function->commandCount()) {
			ret = m_function->at(m_pos);
			m_pos++;
		}
		return ret;
	}

	bool finished() {
		return m_pos >= m_function->commandCount();
	}

private:
	const Function *m_function;
	int m_pos;
};

#endif /* FUNCTION_H_ */
