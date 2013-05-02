/*
 * Map.h
 *
 *  Created on: Apr 16, 2013
 *      Author: Jeremy
 */

#ifndef MAP_H_
#define MAP_H_

#include <QtCore/QObject>

namespace bb {
	namespace cascades {
		class Container;
		class Control;
	}
}

class Map: public QObject {
	Q_OBJECT
public:
	Map(int rows, int cols, int endX, int endY, int *data,
			bb::cascades::Container *mapArea, QObject *parent = 0);

	bool positionAvailable(int x, int y);
	void addRobotContainer(bb::cascades::Control *robot);
	void resize(int width, int height);

	float cellSize() const { return m_cellSize; }

	Q_SLOT void parentHeightChanged(float parentHeight);

	void eraseOverlay();
	void drawOverlayConnection(int x1, int y1, int x2, int y2);

private:
	int m_rows;
	int m_cols;
	int *m_data;
	float m_cellSize;
	bb::cascades::Container *m_mapArea;
	bb::cascades::Container *m_mapOverlay;

	float m_mapWidth;
	float m_mapHeight;
};

#endif /* MAP_H_ */
