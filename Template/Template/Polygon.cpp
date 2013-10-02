/*!
 * @file Polygon.cpp
 * @brief Implements the CPolygon class which provides basic polygon functions
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

#include "Polygon.h"
#include "math.h"
#include "DebugTimer.h"
#include "DebugTimer.h"



/*!
 * @brief CPolygon class constructor
 *
 * @param[in]
 * @param[out]
 * @return
 *
 * @author J. Peterson
 * @version 1.0 Initial release
 * @date 07/10/2013
*/
CPolygon::CPolygon()
{
    m_vertexCount = 0;
    m_vertices = 0;
	
	//vertex_t point={8.0,8.0};
	//vertex_t origin={0.0,0.0};
	//double angleRadians= 45*(3.14159265/180);
	//Rotate(point, origin, angleRadians);
	//int aa=1;
}

/*!
 * @brief CPolygon class constructor
 *
 * @param[in]
 * @param[out]
 * @return
 *
 * @author J. Peterson
 * @version 1.0 Initial release
 * @date 07/10/2013
*/
CPolygon::~CPolygon()
{
    free();
}


/*!
 * @brief frees all memory allocated
 *
 * @param[in]
 * @param[out]
 * @return
 *
 * @author J. Peterson
 * @version 1.0 Initial release
 * @date 07/10/2013
*/
void CPolygon::free()
{

	delete[] m_vertices;
    m_vertexCount = 0;
    m_vertices = 0;
}


/*!
 * @brief frees all memory allocated
 *
 * @param[in]  n - number of verticies to allocate
 * @param[out]
 * @return
 *
 * Frees any previous allocations and allocates new list of vertices,
 * initializing them to (0.0, 0.0)
 *
 * @author J. Peterson
 * @version 1.0 Initial release
 * @date 07/10/2013
*/
bool CPolygon::create(int n)
{
	

	free();
    m_vertices = new vertex_t[n];
    if (!m_vertices)
        return(false);

    m_vertexCount = n;
    for (int i=0; i<n; i++)
    {
		Utils::DebugTimer MyTimer("Inside CPolygon::create Virt Count Loop ");

		m_vertices[i].x = 0.0;
        m_vertices[i].y = 0.0;
    }
    return(true);
}

/*!
 * @brief sets the coordinates of a vertex
 *
 * @param[in]  n - vertex index
 * @param[in]  x, y - coordinates of vertex
 * @return     false if n is out of bounds, true otherwise
 *
 * Frees any previous allocations and allocates new list of vertices,
 * initializing them to (0.0, 0.0)
 *
 * @author J. Peterson
 * @version 1.0 Initial release
 * @date 07/10/2013
*/
bool CPolygon::setVertex(int n, double x, double y)
{

	if ((n < 0) || (n >= m_vertexCount))
        return(false);

    m_vertices[n].x = x;
    m_vertices[n].y = y;
    return(true);
}


/*!
 * @brief returns the coordinates of the vertex at the specified index
 *
 * @param[in]  n - vertex index
 * @param[out]  x, y - coordinates of vertex
 * @return     false if n is out of bounds, true otherwise
 *
 * @author J. Peterson
 * @version 1.0 Initial release
 * @date 07/10/2013
*/
bool CPolygon::getVertex(int n, double &x, double &y)
{

	if ((n < 0) || (n >= m_vertexCount))
        return(false);

    x = m_vertices[n].x;
    y = m_vertices[n].y;
    return(true);
}

/*!
 * @brief returns the number of vertices
 *
 * @param[in]
 * @param[out]
 * @return     number of vertices
 *
 * @author J. Peterson
 * @version 1.0 Initial release
 * @date 07/10/2013
*/
int CPolygon::getVertexCount()
{
    return(m_vertexCount);
}


/*!
 * @brief creates a polygon from an existing vertex list
 *
 * @param[in]  vertexCount - number of verticies in list
 * @param[in]  vertices - pointer to vertex list
 * @param[out]
 * @return     false if allocation failed, true otherwise
 *
 * @author J. Peterson
 * @version 1.0 Initial release
 * @date 07/10/2013
*/
bool CPolygon::createFromVertexList(int vertexCount, const vertex_t *vertices)
{

	create(vertexCount);
    if (m_vertexCount != vertexCount)
        return(false);

    for (int i=0; i<vertexCount; i++)
        setVertex(i, vertices[i].x, vertices[i].y);

    return(true);
}

/*!
 * @brief creates a polygon from an existing chain
 *
 * @param[in]  linkCount - number of links in the chain
 * @param[in]  links - pointer to chain
 * @param[out]
 * @return     false if allocation failed, true otherwise
 *
 * @author J. Peterson
 * @version 1.0 Initial release
 * @date 07/10/2013
*/
bool CPolygon::createFromChain(int linkCount, const unsigned char *links)
{

	static const int x_step[9] = {0, 1, 1, 0, -1, -1, -1, 0, 1};
    static const int y_step[9] = {0, 0, 1, 1, 1, 0, -1, -1, -1};

    // count the vertices needed
    int vertexCount = 1;
    int d = -1;
    for (int i=0; i<linkCount; i++)
    {
        if (links[i] != d)
            vertexCount++;
        d = links[i];
    }

    create(vertexCount);
    if (m_vertexCount != (vertexCount))
        return(false);

    int x = 0;
    int y = 0;
    setVertex(0, 0.0, 0.0);
    int vIndex = 1;
    for (int i=0; i<linkCount; i++)
    {
        if ((links[i] < 1) || (links[i] > 8))
            continue;

        // count the duplicate steps
        int c = 1;
        int j = 1;
        while (i+j < linkCount)
        {
            if (links[i+j] != links[i])
                break;
            j++;
            c++;
        }

        x += c * x_step[links[i]];
        y += c * y_step[links[i]];
        i += c-1;
        setVertex(vIndex++, x, y);
    }
    m_vertexCount = vIndex;

    return(true);
}


/*!
 * @brief computes the area of the polygon
 *
 * @param[in]
 * @return     area of the polygon
 *
 * computes the positive area of the simple polygon
 *
 * @author J. Peterson
 * @version 1.0 Initial release
 * @date 07/10/2013
*/
double CPolygon::area()
{
    double sum = 0.0;

    for (int i=0; i<m_vertexCount-1; i++)
    {
        sum += (m_vertices[i].y*m_vertices[i+1].x - m_vertices[i].x*m_vertices[i+1].y);
    }
    if ((m_vertices[m_vertexCount-1].x != m_vertices[0].x) || (m_vertices[m_vertexCount-1].y != m_vertices[0].y))
        sum += (m_vertices[m_vertexCount-1].y*m_vertices[0].x - m_vertices[m_vertexCount-1].x*m_vertices[0].y);
    sum /= 2.0;

    if (sum < 0.0)
        sum = -sum;

    return(sum);
}


double CPolygon::perimeter()
{
	double sum = 0.0;

	for (int i=0; i<m_vertexCount-1; i++)
	{		
		sum += DistanceBetweenPoints(m_vertices[i],m_vertices[i+1]);
	}

	sum+=DistanceBetweenPoints(m_vertices[m_vertexCount-1],m_vertices[0]);
	return sum;
}

/**
* FUNCTION DistanceBetweenPoints*
* @brief Calc distance between 2 points.
*
* @version 1.0 
*
* @author David Libby
* @date 8/16/2013 10:32:29 AM
*
* @param Point1 
* @param Point2 
*
* @return double 
*/
double CPolygon::DistanceBetweenPoints(const vertex_t &Point1,const vertex_t &Point2 )
{
	double firsthalf = pow(Point2.x - Point1.x,2);
	double secondhalf = pow(Point2.y - Point1.y,2);
	double length = sqrt(firsthalf+secondhalf);
	return length;
}

/**
* FUNCTION GetMbr*
* @brief Get Polygon MBR
*
* @version 1.0 
*
* @author David Libby
* @date 8/16/2013 10:32:52 AM
*
* @param Min 
* @param Max 
*/
void CPolygon::GetMbr(vertex_t &Min,vertex_t &Max)
{
	Min.x=Min.y=100000;
	Max.x=Max.y=-100000;;
	
	
	for (int i=0; i<m_vertexCount; i++)
	{
		if(m_vertices[i].x < Min.x)
			Min.x = m_vertices[i].x;
		if(m_vertices[i].y < Min.y)
			Min.y = m_vertices[i].y;

		if(m_vertices[i].x > Max.x)
			Max.x = m_vertices[i].x;
		if(m_vertices[i].y > Max.y)
			Max.y = m_vertices[i].y;

	}

}

/**
* FUNCTION Rotate*
* @brief Rotates line around Center.
*
* @version 1.0 
*
* @author David Libby
* @date 8/16/2013 10:33:12 AM
*
* @param point 
* @param origin 
* @param angleRadians 
*/
void CPolygon::Rotate(vertex_t& point, vertex_t origin, double angleRadians)
{

	vertex_t newPoint={0,0};
	double Radius = DistanceBetweenPoints(point,origin);
	newPoint.x=origin.x+(point.x-origin.x)*cos(angleRadians)-(point.y-origin.y)*sin(angleRadians);
	newPoint.y=origin.y+(point.y-origin.y)*cos(angleRadians)+(point.x-origin.x)*sin(angleRadians);
	point=newPoint;
}

/**
* FUNCTION Rotate*
* @brief Rotates Polygon in Degrees
*
* @version 1.0 
*
* @author David Libby
* @date 8/16/2013 10:33:39 AM
*
* @param angleDegrees 
*/
void CPolygon::Rotate(double angleDegrees)
{
	SetDebugTimer();

	double radians = angleDegrees*(3.14159265/180);
	vertex_t cent ;
	
	GetCenter(cent);
	
	for (int i = 0; 
		i < m_vertexCount; i++)
		Rotate(m_vertices[i], cent, radians);
}


/**
* FUNCTION GetCenter*
* @brief Gets Center of Polygon
*
* @version 1.0 
*
* @author David Libby
* @date 8/16/2013 10:33:55 AM
*
* @param center 
*/
void CPolygon::GetCenter(vertex_t &center)
{
	center.x=0;
	center.y=0;

	for (int i=0; i<m_vertexCount; i++)
	{
		Utils::DebugTimer MyTimew("THis is the Loop in CPolygon::GetCenter");
		center.x += m_vertices[i].x;
		center.y += m_vertices[i].y;
	}

	center.x /=m_vertexCount;
	center.y /=m_vertexCount;

}

/**
* FUNCTION GetFeret*
* @brief Gets Feret (Calaper Size of Polygon)
*
* @version 1.0 
*
* @author David Libby
* @date 8/16/2013 10:34:10 AM
*
* @param minSize 
* @param maxSize 
* @param minAngle 
* @param maxAngle 
*/
void CPolygon::GetFeret(double &minSize,double &maxSize,int &minAngle,int &maxAngle)
{
	minAngle=0;
	maxAngle=0;
	double TempWidth,TempHeight=0;
	
	vertex_t tmpMin,tmpMax;	
	GetMbr(tmpMin,tmpMax);
	TempWidth =  tmpMax.x-tmpMin.x;
	TempHeight =  tmpMax.y-tmpMin.y;

	minSize=0;
	maxSize=0;

	maxSize = minSize = TempWidth<TempHeight?TempHeight:TempWidth;

	for (int angle=0;angle<=180;angle++)
	{


		CPolygon TempPolly;	
		int vCount=getVertexCount();
		TempPolly.create(vCount);
		for (int index=0;index<vCount;index++)
		{
			double x;
			double y;
			getVertex(index,x,y);
			TempPolly.setVertex(index,x,y);
		}

		if(angle==45)
		{
			int aa=1;
		}

		TempPolly.Rotate(angle);
		//TempPolly.Rotate(141);
		TempPolly.GetMbr(tmpMin,tmpMax);

		

		TempWidth =  tmpMax.x-tmpMin.x;
		TempHeight =  tmpMax.y-tmpMin.y;

		if(TempWidth>maxSize)
		{
			maxSize=TempWidth;
			maxAngle=angle;
		}
		if(TempHeight>maxSize)
		{
			maxSize=TempHeight;
			maxAngle=angle;
		}

		if(TempHeight<minSize)
		{
			minSize=TempHeight;
			minAngle=angle;
		}

		if(TempWidth<minSize)
		{
			minSize=TempWidth;
			minAngle=angle;
		}

	}

}