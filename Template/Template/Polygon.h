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

	// this is my change


	CPolygon();
	~CPolygon();

    bool   create(int n);
    bool   createFromVertexList(int vertexCount, const vertex_t *vertices);
    bool   createFromChain(int linkCount, const unsigned char *links);
    bool   setVertex(int n, double x, double y);
    bool   getVertex(int n, double &x, double &y);
    int    getVertexCount();
	
	//! Gets the MBR for the current polygon
	void GetMbr(vertex_t &Min,vertex_t &Max);
	//! Rotates the point from the given location.
	void Rotate(vertex_t& point, vertex_t origin, double angleDegrees);
	//! Rotates Polygon
	void Rotate(double angleRadians);
	//! Gets Center point of Poly
	void GetCenter(vertex_t &center);
	//! Gets Feret size and angle.
	void GetFeret(double &minSize,double &maxSize,int &minAngle,int &maxAngle);
	//! returns the distance between points.
	double DistanceBetweenPoints(const vertex_t &Point1,const vertex_t &Point2 );

	



    double area();
	double perimeter();


private:
    void   free();

private:
    int       m_vertexCount;
	vertex_t *m_vertices;
};


#endif // POLYGON_H
