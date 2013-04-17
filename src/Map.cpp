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

using namespace bb::cascades;

Map::Map(int rows, int cols, int *data, Container *mapArea, QObject *parent)
	: QObject(parent)
	, m_rows(rows)
	, m_cols(cols)
	, m_data(data)
	, m_mapArea(0)
{
	const float padding = 10; // Padding of at least 10 on all sides.
	const float mapWidth = mapArea->preferredWidth() - padding*2;
	const float mapHeight = mapArea->preferredHeight() - padding*2;
	const float cellWidth = mapWidth / m_cols;
	const float cellHeight = mapHeight / m_rows;
	m_cellSize = std::min(cellWidth, cellHeight);
	m_mapArea = Container::create()
		.preferredSize(m_cellSize * m_cols, m_cellSize * m_rows)
		.layout(new AbsoluteLayout());
	m_mapArea->setHorizontalAlignment(HorizontalAlignment::Center);
	m_mapArea->setVerticalAlignment(VerticalAlignment::Center);

	// We know the size of each cell now, but we don't know the top left corner.
	// FIXME: Center this.
	for (int y=0; y<m_rows; y++) {
		for (int x=0; x<m_cols; x++) {
			Container *cell = Container::create()
				.preferredSize(m_cellSize, m_cellSize)
				.layoutProperties(AbsoluteLayoutProperties::create().position(x * m_cellSize, y * m_cellSize))
				.add(Label::create(QString::number(m_data[y*m_rows+x])));
			m_mapArea->add(cell);
		}
	}
	mapArea->add(m_mapArea);
}

bool Map::positionAvailable(int x, int y)
{
	if (x < 0 || x >= m_cols) return false;
	if (y < 0 || y >= m_rows) return false;

	return m_data[y * m_rows + x] == 1;
}

void Map::addRobotContainer(Container *robot)
{
	m_mapArea->add(robot);
}
