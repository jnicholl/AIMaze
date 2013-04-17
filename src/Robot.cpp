/*
 * Robot.cpp
 *
 *  Created on: Apr 16, 2013
 *      Author: Jeremy
 */

#include "Robot.h"
#include "Map.h"

#include <bb/cascades/AbsoluteLayoutProperties>
#include <bb/cascades/Container>
#include <bb/cascades/Label>

using namespace bb::cascades;

Robot::Robot(Map *map, int x, int y, int ex, int ey, Direction d, QObject *parent)
	: QObject(parent)
	, m_direction(d)
	, m_x(x)
	, m_y(y)
	, m_endX(ex)
	, m_endY(ey)
	, m_map(map)
	, m_container(0)
	, m_label(0)
{
	const float cellSize = m_map->cellSize();
	m_label = Label::create(Robot::directionToString(m_direction));
	m_container = Container::create()
		.preferredSize(cellSize, cellSize)
		.background(Color::fromRGBA(1, 0, 0, 1))
		.layoutProperties(AbsoluteLayoutProperties::create().position(x * cellSize, y * cellSize))
		.add(m_label);
	m_map->addRobotContainer(m_container);
}

Robot::Direction Robot::getDirection(const QString &dirString)
{
	if (!dirString.compare("right", Qt::CaseInsensitive)) {
		return RIGHT;
	} else if (!dirString.compare("left", Qt::CaseInsensitive)) {
		return LEFT;
	} else if (!dirString.compare("down", Qt::CaseInsensitive)) {
		return DOWN;
	} else {
		return UP;
	}
}

const QString Robot::directionToString(Robot::Direction dir)
{
	switch (dir) {
	case LEFT:
		return "left";
	case UP:
		return "up";
	case RIGHT:
		return "right";
	case DOWN:
		return "down";
	default:
		return "unknown";
	}
}

void Robot::turnRight() {
	switch (m_direction) {
	case LEFT:
		m_direction = UP;
		draw();
		break;
	case UP:
		m_direction = RIGHT;
		draw();
		break;
	case RIGHT:
		m_direction = DOWN;
		draw();
		break;
	case DOWN:
		m_direction = LEFT;
		draw();
		break;
	default:
		break;
	}
}

void Robot::turnLeft() {
	switch (m_direction) {
	case LEFT:
		m_direction = DOWN;
		draw();
		break;
	case DOWN:
		m_direction = RIGHT;
		draw();
		break;
	case RIGHT:
		m_direction = UP;
		draw();
		break;
	case UP:
		m_direction = LEFT;
		draw();
		break;
	default:
		break;
	}
}

bool Robot::moveForward() {
	bool didMove = false;
	switch (m_direction) {
	case LEFT:
		if (m_map->positionAvailable(m_x - 1, m_y)) {
			m_x--;
			didMove = true;
		}
		break;
	case DOWN:
		if (m_map->positionAvailable(m_x, m_y + 1)) {
			m_y++;
			didMove = true;
		}
		break;
	case RIGHT:
		if (m_map->positionAvailable(m_x + 1, m_y)) {
			m_x++;
			didMove = true;
		}
		break;
	case UP:
		if (m_map->positionAvailable(m_x, m_y - 1)) {
			m_y--;
			didMove = true;
		}
		break;
	default:
		break;
	}
	if (didMove) {
		draw();
		emit moved(m_x, m_y);
	}
	return didMove;
}

void Robot::draw()
{
	AbsoluteLayoutProperties *properties = qobject_cast<AbsoluteLayoutProperties*>(m_container->layoutProperties());
	properties->setPositionX(m_x * m_map->cellSize());
	properties->setPositionY(m_y * m_map->cellSize());
	m_label->setText(directionToString(m_direction));
}
