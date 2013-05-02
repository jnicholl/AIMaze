/*
 * Map.cpp
 *
 *  Created on: Apr 16, 2013
 *      Author: Jeremy
 */

#include "Map.h"

#include <bb/cascades/Container>
#include <bb/cascades/Label>
#include <bb/cascades/AbsoluteLayout>
#include <bb/cascades/AbsoluteLayoutProperties>
#include <bb/cascades/ColorPaint>
#include <bb/cascades/ImageView>

using namespace bb::cascades;

Map::Map(int rows, int cols, int endX, int endY, int *data,
		Container *mapArea, QObject *parent)
	: QObject(parent)
	, m_rows(rows)
	, m_cols(cols)
	, m_data(data)
	, m_mapArea(0)
{
	const float padding = 10; // Padding of at least 10 on all sides.
	m_mapWidth = mapArea->preferredWidth() - padding*2;
	m_mapHeight = mapArea->preferredHeight() - padding*2;
	const float cellWidth = m_mapWidth / m_cols;
	const float cellHeight = m_mapHeight / m_rows;
	m_cellSize = std::min(cellWidth, cellHeight);
	m_mapArea = Container::create()
		.preferredSize(m_cellSize * m_cols, m_cellSize * m_rows)
		.layout(new AbsoluteLayout());
	m_mapArea->setHorizontalAlignment(HorizontalAlignment::Center);
	m_mapArea->setVerticalAlignment(VerticalAlignment::Center);

	QString emptyFloor("asset:///images/floortile.png");
	QString goalFloor("asset:///images/goal.png");
	for (int y=0; y<m_rows; y++) {
		for (int x=0; x<m_cols; x++) {
			if (positionAvailable(x,y)) {
				ImageView *cell = ImageView::create()
					.scalingMethod(ScalingMethod::Fill)
					.preferredSize(m_cellSize, m_cellSize)
					.layoutProperties(AbsoluteLayoutProperties::create().position(x * m_cellSize, y * m_cellSize));
				if (x == endX && y == endY)
					cell->setImageSource(goalFloor);
				else
					cell->setImageSource(emptyFloor);
				m_mapArea->add(cell);
			}
		}
	}

	QObject::connect(m_mapArea, SIGNAL(preferredHeightChanged(float)), this, SLOT(parentHeightChanged(float)));
	mapArea->add(m_mapArea);

	m_mapOverlay = Container::create()
		.preferredSize(m_cellSize * m_cols, m_cellSize * m_rows)
		.layout(new AbsoluteLayout());
	m_mapOverlay->setHorizontalAlignment(HorizontalAlignment::Center);
	m_mapOverlay->setVerticalAlignment(VerticalAlignment::Center);

	mapArea->add(m_mapOverlay);
}

bool Map::positionAvailable(int x, int y)
{
	if (x < 0 || x >= m_cols) return false;
	if (y < 0 || y >= m_rows) return false;

	return m_data[y * m_cols + x] == 1;
}

void Map::addRobotContainer(Control *robot)
{
	m_mapArea->add(robot);
}

void Map::parentHeightChanged(float parentHeight)
{
	const float padding = 10; // Padding of at least 10 on all sides.
	parentHeight = parentHeight - padding*2;
	const float cellWidth = m_mapWidth / m_cols;
	const float cellHeight = parentHeight / m_rows;
	float oldCellSize = m_cellSize;
	m_cellSize = std::min(cellWidth, cellHeight);
	m_mapArea->setPreferredSize(m_cellSize * m_cols, m_cellSize * m_rows);

	QObjectList list = m_mapArea->children();
	if (m_rows * m_cols + 1 != list.count()) { qDebug("Uhoh: List size not correct!"); }

	int i=0;
	for (int y=0; y<m_rows; y++) {
		for (int x=0; x<m_cols; x++) {
			ImageView *cell = qobject_cast<ImageView*>(list[i]);
			if (cell) {
				cell->setPreferredSize(m_cellSize, m_cellSize);
				AbsoluteLayoutProperties *properties = static_cast<AbsoluteLayoutProperties*>(cell->layoutProperties());
				properties->setPositionX(x * m_cellSize);
				properties->setPositionY(y * m_cellSize);
			}
			i++;
		}
	}

	Container *robot = qobject_cast<Container*>(list[i]);
	if (robot) {
		robot->setPreferredSize(m_cellSize, m_cellSize);
		AbsoluteLayoutProperties *properties = static_cast<AbsoluteLayoutProperties*>(robot->layoutProperties());
		properties->setPositionX(properties->positionX() * m_cellSize / oldCellSize);
		properties->setPositionY(properties->positionY() * m_cellSize / oldCellSize);
	}
}

void Map::eraseOverlay()
{
	m_mapOverlay->removeAll();
}

void Map::drawOverlayConnection(int x1, int y1, int x2, int y2)
{
	if ((x1 != x2 && y1 != y2) || (std::abs(x1-x2) >= 2 || std::abs(y1-y2) >= 2)) {
		qDebug("drawOverlayConnection: Bad data: (%d,%d),(%d,%d)\n", x1, y1, x2, y2);
		return;
	}

	int x = 0, y = 0;
	int w = 1, h = 1;
	if (x1 == x2) {
		x = x1 * m_cellSize + m_cellSize / 2;
		y = std::min(y1, y2) * m_cellSize + m_cellSize / 2;
		h = m_cellSize;
	} else if (y1 == y2) {
		x = std::min(x1, x2) * m_cellSize + m_cellSize / 2;
		y = y1 * m_cellSize + m_cellSize / 2;
		w = m_cellSize;
	}
	Container *c = Container::create()
					.preferredSize(w,h)
					.layoutProperties(AbsoluteLayoutProperties::create().position(x,y))
					.background(Color::fromRGBA(0.0f, 0.7f, 0.7f, 1.0f));
	m_mapOverlay->add(c);
}
