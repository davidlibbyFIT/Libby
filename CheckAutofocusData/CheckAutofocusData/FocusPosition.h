/*!
 * @file FocusPosition.cpp
 * @brief Implements the CFocusPosition class which computes the  time of best focus from data gathered during autofocus
 *
 * @author    	Jim Peterson
 * @version		1.0
 * @date		04/04/2014
 * @copyright   (C) Copyright 2014 Fluid Imaging
 *
 * REVISION     HISTORY
 * Author       Date         Description
 * -----------------------------------------------------------------------------
 * J. Peterson  04/04/2014   Initial version
 *------------------------------------------------------------------------------
*/
#ifndef FOCUS_POSITION
#define FOCUS_POSITION

#include <vector>


enum autofocusReturn_t
{
	AF_OK = 0,                // Everything looks good
	AF_INSUFFICIENT_POINTS,   // Incoming vector did not contain sufficient points
	AF_LOW_GRADIENT,          // The highest gradient seen was lower than should be
	AF_INTERNAL_ALLOCATION,   // The algorithm failed due to a memory allocation error
	AF_INTERNAL_ERROR         // The algorithm failed due to an unspecified internal error
};

struct dataPoint_t 
{
	float sharpness;
	float position;

	//
	// This function is used by std::sort to sort by edge gradient in descending order 
	//
	static bool comp_gradient_decending(const dataPoint_t &lhs, const dataPoint_t &rhs)
	{
		return(lhs.sharpness > rhs.sharpness);
	}
};

typedef std::vector <dataPoint_t> dataPointVector_t;

enum autoFocusMethod_t
{
	AF_MAX_SHARPNESS,
	AF_WEIGHTED_AVERAGE,
	AF_BM_MAX_SHARPNESS,
	AF_BM_WEIGHTED_AVERAGE,
	AF_BM_CURVE_FIT
};

class CFocusPosition
{
public:
	CFocusPosition();

	autofocusReturn_t analyze(const dataPointVector_t &dataPoints);
	double getCenterFocusPosition(autoFocusMethod_t method);
	double getNearFocusPosition(autoFocusMethod_t method);
	double getFarFocusPosition(autoFocusMethod_t method);
	double getLastPosition() { return(m_lastPosition); }

	void setMinPointCount(int count)              { m_minPointCount = count; }
	void setMinEdgeGradient(double grad)          { m_minEdgeGradient = grad; }
	void setGradientPercentile(double percentile) { m_gradientPercentile = percentile; }
	void setEdgeGradientCutoff(double grad)       { m_edgeGradientCutoff = grad; }

private:
	autofocusReturn_t analyze_maxGradient(const dataPointVector_t &dataPoints);
	autofocusReturn_t analyze_weightedAverage(const dataPointVector_t &dataPoints);
	autofocusReturn_t analyze_maxGradient_BM(const dataPointVector_t &dataPoints);
	autofocusReturn_t analyze_weightedAverage_BM(const dataPointVector_t &dataPoints);


private:
	double m_lastPosition;

	double m_positionMaxGradient;
	double m_positionWeightedAverage;

	double m_positionNear_MaxGrad_BM;
	double m_positionCenter_MaxGrad_BM;
	double m_positionFar_MaxGrad_BM;
	double m_positionNear_WeightedAve_BM;
	double m_positionCenter_WeightedAve_BM;
	double m_positionFar_WeightedAve_BM;
	double m_positionNear_CurveFit_BM;
	double m_positionCenter_CurveFit_BM;
	double m_positionFar_CurveFit_BM;


	//
	// Algorithm parameters
	//
	int    m_minPointCount;
	double m_minEdgeGradient;
	double m_edgeGradientCutoff;
	double m_gradientPercentile;
};

#endif  // FOCUS_POSITION

