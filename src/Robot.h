/*
 * Robot.h
 *
 *  Created on: Apr 16, 2013
 *      Author: Jeremy
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include <QtCore/QObject>

namespace bb {
	namespace cascades {
		class Container;
		class Label;
	}
}

class Map;

class Robot : public QObject {
	Q_OBJECT
public:
	enum Direction {
		LEFT, UP, RIGHT, DOWN,
	};

	Robot(Map *map, int x = 0, int y = 0, int ex = 0, int ey = 0, Direction d = UP, QObject *parent = 0);

	static Direction getDirection(const QString &dirString);
	static const QString directionToString(Robot::Direction dir);

	void turnRight();
	void turnLeft();
	bool moveForward();

	int x() const { return m_x; }
	int y() const { return m_y; }
	int endX() const { return m_endX; }
	int endY() const { return m_endY; }

	bool finished() const { return m_x == m_endX && m_y == m_endY; }

signals:
	void moved(int x, int y);

private:
	void draw();

	Direction m_direction;
	int m_x, m_y;
	int m_endX, m_endY;
	Map* m_map;
	bb::cascades::Container *m_container;
	bb::cascades::Label *m_label;
};

#endif /* ROBOT_H_ */
