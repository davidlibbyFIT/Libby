/*!
 * @file Polygon.h
 * @brief Defines the CPolygon class which provides basic polygon functions
 *
 * @author    	Jim Peterson
 * @version		1.0
 * @date		07/10/2013
 * @copyright   (C) Copyright Fluid Imaging
 *
 * REVISION     HISTORY
 * Author       Date         Description
 * -----------------------------------------------------------------------------
 * J. Peterson  07/10/2013   Initial version
 *------------------------------------------------------------------------------
*/

#ifndef POLYGON_H
#define POLYGON_H

//#include <stdint.h>

class CPolygon
{

public:

	typedef struct 
	{
		double x;
		double y;
	}vertex_t;

	CPolygon();
	~CPolygon();

    bool   create(int n);
    bool   createFromVertexList(int vertexCount, const vertex_t *vertices);
    bool   createFromChain(int linkCount, const unsigned char *links);
    bool   setVertex(int n, double x, double y);
    bool   getVertex(int n, double &x, double &y);
    int    getVertexCount();
	void   GetMbr(vertex_t &Min,vertex_t &Max);
	void Rotate(vertex_t& point, vertex_t origin, double angle);
	void Rotate(double angle);
	void GetCenter(vertex_t &center);
	void GetFeret(double &minSize,double &maxSize,int &minAngle,int &maxAngle);

	



    double area();
	double perimeter();

	double DistanceBetweenPoints(const vertex_t &Point1,const vertex_t &Point2 );

private:
    void   free();

private:
    int       m_vertexCount;
	vertex_t *m_vertices;
};


#endif // POLYGON_H
