/*
 * cgraphiccontrol.h
 *
 *  Created on: 22.10.2008
 *      Author: livid
 */

#ifndef CGRAPHICCONTROL_H_
#define CGRAPHICCONTROL_H_

#include <QtGui>

class graphic_window : public QGraphicsView
{
	Q_OBJECT

private:
	QGraphicsScene scene;
	QGraphicsSimpleTextItem sX, sY;
	QGraphicsSimpleTextItem maxX, maxY;
	QGraphicsSimpleTextItem minX, minY;
	QGraphicsLineItem lX,lY,llX,llY;
	QGraphicsPathItem graph_item;
	QGraphicsPathItem grid_item;//10x10
	QPainterPath graph;
	QPainterPath grid;
	QTimer timer;

	void updatescene()
	{
		graph_item.setPath(graph);
		graph_item.setPos(-graph.boundingRect().left(),-graph.boundingRect().bottom());

		lX.setLine(0,0,graph.boundingRect().width(),0);
		lY.setLine(0,0,0,-graph.boundingRect().height());
		llX.setLine(0,-graph.boundingRect().height(),graph.boundingRect().width(),-graph.boundingRect().height());
		llY.setLine(graph.boundingRect().width(),0,graph.boundingRect().width(),-graph.boundingRect().height());

		QVariant mx=graph.boundingRect().right(),
			my=-graph.boundingRect().top();
		maxX.setText(mx.toString());
		maxY.setText(my.toString());

		mx=graph.boundingRect().left();
		my=-graph.boundingRect().bottom();
		minX.setText(mx.toString());
		minY.setText(my.toString());

		qreal sx=graph.boundingRect().width()/grid_item.transform().mapRect(grid_item.boundingRect()).width();
		qreal sy=graph.boundingRect().height()/grid_item.transform().mapRect(grid_item.boundingRect()).height();
		if(sx!=0 && sy!=0)
			grid_item.scale(sx,sy);
	}

	QRectF mapBR(QGraphicsItem *item)
	{
		return item->transform().mapRect(item->boundingRect());
	}

public slots:
	void do_update()
	{
		fitInView(scene.itemsBoundingRect());
		sX.setTransform(transform().inverted());
		sY.setTransform(transform().inverted());
		maxX.setTransform(transform().inverted());
		maxY.setTransform(transform().inverted());
		minX.setTransform(transform().inverted());
		minY.setTransform(transform().inverted());

		sX.setPos((graph.boundingRect().width()-mapBR(&sX).width())/2,mapBR(&sX).height());

		sY.rotate(-90);
		sY.setPos(-mapBR(&sY).width()*2,-(graph.boundingRect().height()-mapBR(&sY).height())/2);

		minX.setPos(0,0);

		minY.rotate(-90);
		minY.setPos(-mapBR(&minY).width(),0);

		maxX.setPos(graph.boundingRect().width()-mapBR(&maxX).width(),0);

		maxY.rotate(-90);
		maxY.setPos(-mapBR(&maxY).width(),mapBR(&maxY).height()-graph.boundingRect().height());
	}

public:
	graphic_window(QString title, QString X, QString Y) : QGraphicsView()
	{
		setWindowTitle(title);
		setScene(&scene);
		//setParent(this);
		sX.setText(X);
		sY.setText(Y);
		scene.addItem(&sX);
		scene.addItem(&sY);
		scene.addItem(&lX);
		scene.addItem(&lY);
		scene.addItem(&llX);
		scene.addItem(&llY);
		scene.addItem(&graph_item);
		scene.addItem(&grid_item);
		scene.addItem(&maxX);
		scene.addItem(&maxY);
		scene.addItem(&minX);
		scene.addItem(&minY);

		graph_item.setPen(Qt::SolidLine);
		graph_item.setBrush(Qt::NoBrush);
		grid_item.setPen(Qt::SolidLine);
		grid_item.setBrush(Qt::NoBrush);

		for(int i=1;i<10;i++)
		{
			grid.moveTo(0,-i);
			grid.lineTo(10,-i);
			grid.moveTo(i,0);
			grid.lineTo(i,-10);
		}

		grid_item.setPath(grid);
		show();

		connect(&timer, SIGNAL(timeout()), this, SLOT(do_update()));
		timer.start(100);
	}

	~graphic_window()
	{

	}

	void addpoint(qreal x, qreal y)
	{
		if (graph.elementCount()==0)
			graph.moveTo(x,-y);
		else
			graph.lineTo(x,-y);
		updatescene();
	}

	QString x_name()
	{
		return sX.text();
	}

	QString y_name()
	{
		return sY.text();
	}

	QString title()
	{
		return windowTitle();
	}
};

#endif /* CGRAPHICCONTROL_H_ */
