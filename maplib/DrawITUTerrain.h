#ifndef DRAW_ITU_TERRAIN_H
#define DRAW_ITU_TERRAIN_H


#include "ProfileWidget.h"


class ITUTerrain;


class DrawITUTerrain : public ProfileDrawPart
{
public:
	DrawITUTerrain(ITUTerrain *t, ProfileWidget *parent);
	virtual void draw(QPainter *painter, QPaintEvent *ev);
	ITUTerrain *t;

private:
	void drawText(QPainter *painter, double x, double y, const QString &s);

};


#endif
