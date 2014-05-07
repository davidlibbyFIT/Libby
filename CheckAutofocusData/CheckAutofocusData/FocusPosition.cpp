/*!
 * @file FocusPosition.cpp
 * @brief Implements the CFocusPosition class which computes the position of best focus
 *     from data gathered during autofocus
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

#include <algorithm>    // std::sort
#include "FocusPosition.h"



/*!
 * @brief CFocusPosition class constructor
 *
 * @param[in] none
 *
 * @author J. Peterson
 * @date 04/04/2014
*/
CFocusPosition::CFocusPosition()
{
	m_lastPosition = 0.0;
    m_positionMaxGradient = 0.0;
	m_positionWeightedAverage = 0.0;

	m_positionNear_MaxGrad_BM = 0.0;
	m_positionCenter_MaxGrad_BM = 0.0;
	m_positionFar_MaxGrad_BM = 0.0;
	m_positionNear_WeightedAve_BM = 0.0;
	m_positionCenter_WeightedAve_BM = 0.0;
	m_positionFar_WeightedAve_BM = 0.0;
	m_positionNear_CurveFit_BM = 0.0;
	m_positionCenter_CurveFit_BM = 0.0;
	m_positionFar_CurveFit_BM = 0.0;

	m_minPointCount      = 500;
	m_minEdgeGradient    = 100.0;
    m_gradientPercentile = 0.5;
	m_edgeGradientCutoff = 50.0;
}



/*!
 * @brief Finds best focus time from the data provided
 *
 * @param[in] dataPoints - vector of data points
 *
 * Each data point contains a timestamp and an edge gradient.
 * The incoming vector is copied before any manupulation.
 *
 * @author J. Peterson
 * @date 04/04/2014
*/
autofocusReturn_t CFocusPosition::analyze(const dataPointVector_t &dataPoints)
{
    m_lastPosition = 0.0;

	//
	// Check parameters
	//
	if (dataPoints.size() < 1)
	{
		return(AF_INSUFFICIENT_POINTS);
	}


    m_lastPosition = dataPoints[dataPoints.size()-1].position;

	analyze_maxGradient(dataPoints);
    analyze_weightedAverage(dataPoints);
    analyze_maxGradient_BM(dataPoints);
    analyze_weightedAverage_BM(dataPoints);

	m_positionNear_CurveFit_BM = 0.0;
	m_positionCenter_CurveFit_BM = 0.0;
	m_positionFar_CurveFit_BM = 0.0;

	return(AF_OK);
}

autofocusReturn_t CFocusPosition::analyze_maxGradient(const dataPointVector_t &dataPoints)
{
    m_positionMaxGradient = 0.0;
	double maxSharpness = 0.0;
	for (int i=0; i<(int)dataPoints.size(); i++)
	{
		if (dataPoints[i].sharpness > maxSharpness)
		{
			maxSharpness = dataPoints[i].sharpness;
			m_positionMaxGradient = dataPoints[i].position;
		}
	}
    return(AF_OK);
}


autofocusReturn_t CFocusPosition::analyze_weightedAverage(const dataPointVector_t &dataPoints)
{
	m_positionWeightedAverage = 0.0;

	//
	// Copy and Sort the points by edge gradient
	//
    dataPointVector_t dataPointsTmp;
    try
	{
		dataPointsTmp = dataPoints;
		std::sort(dataPointsTmp.begin(), dataPointsTmp.end(), dataPoint_t::comp_gradient_decending);
	}
	catch (...)
	{
		return(AF_INTERNAL_ALLOCATION);
	}
	if ( (dataPointsTmp.size() < 1) || (dataPointsTmp[0].sharpness <= 0.0) )
		return(AF_LOW_GRADIENT);

	//
	// Find the average of the top 1/4 of the points
	//
	double sum = 0.0;
	double weight = 0.0;
	double maxGrad = dataPointsTmp[0].sharpness;
	double minGrad = dataPointsTmp[0].sharpness * m_gradientPercentile;
	if (minGrad < m_edgeGradientCutoff)
		minGrad = m_edgeGradientCutoff;
	if (minGrad > maxGrad)
		minGrad = maxGrad - 1.0;
	double m = (1.0/(maxGrad-minGrad));
	for (int i=0; i<(int)dataPointsTmp.size(); i++)
	{
		if (dataPointsTmp[i].sharpness < minGrad)
			break;
		double pointWeight = m*(dataPointsTmp[i].sharpness - minGrad);
		sum += (pointWeight * dataPointsTmp[i].position);
		weight += pointWeight;
	}

	m_positionWeightedAverage = sum/weight;

	if ((int)dataPointsTmp.size() < m_minPointCount)                    // not enough points
		return(AF_INSUFFICIENT_POINTS);
	else if (dataPointsTmp[0].sharpness < m_minEdgeGradient) // no good focus was found
		return(AF_LOW_GRADIENT);
	return(AF_OK);
}

autofocusReturn_t CFocusPosition::analyze_maxGradient_BM(const dataPointVector_t &dataPoints)
{
	m_positionFar_MaxGrad_BM = 0.0;
	m_positionNear_MaxGrad_BM = 0.0;
	m_positionCenter_MaxGrad_BM = 0.0;

	//
	// Copy and Sort the points by edge gradient
	//
    dataPointVector_t dataPointsTmp;
    try
	{
		dataPointsTmp = dataPoints;
		std::sort(dataPointsTmp.begin(), dataPointsTmp.end(), dataPoint_t::comp_gradient_decending);
	}
	catch (...)
	{
		return(AF_INTERNAL_ALLOCATION);
	}
	if ( (dataPointsTmp.size() < 1) || (dataPointsTmp[0].sharpness <= 0.0) )
		return(AF_LOW_GRADIENT);

	//
	// Take the top points
	//
	double maxGrad = dataPointsTmp[0].sharpness;
	double minGrad = dataPointsTmp[0].sharpness * m_gradientPercentile;
	if (minGrad < m_edgeGradientCutoff)
		minGrad = m_edgeGradientCutoff;
	if (minGrad > maxGrad)
		minGrad = maxGrad - 1.0;

	for (int i=0; i<(int)dataPointsTmp.size(); i++)
	{
		if (dataPointsTmp[i].sharpness < minGrad)
		{
			int last = (int)dataPointsTmp.size() - 1;
			if (last >= 0)
			    dataPointsTmp.erase(dataPointsTmp.begin()+i, dataPointsTmp.end());
			break;
		}
	}

	//
	// compute the median position
	//
	int last = (int)dataPointsTmp.size() - 1;
	double maxPosition = dataPoints[0].position;
	double minPosition = dataPoints[dataPoints.size()-1].position;
	for (int i=0; i<(int)dataPointsTmp.size(); i++)
	{
		if (dataPointsTmp[i].position < minPosition)
			minPosition = dataPointsTmp[i].position;
		if (dataPointsTmp[i].position > maxPosition)
			maxPosition = dataPointsTmp[i].position;
	}
	double medianPosition = (minPosition + maxPosition)/2.0;

	//
	// find the max gradient of each half
	//
	double maxGradient_near = 0.0;
	double maxGradient_far = 0.0;
	for (int i=0; i<(int)dataPointsTmp.size(); i++)
	{
		if (dataPointsTmp[i].position <= medianPosition)
		{
			if (dataPointsTmp[i].sharpness > maxGradient_near)
			{
				maxGradient_near = dataPointsTmp[i].sharpness;
				m_positionNear_MaxGrad_BM = dataPointsTmp[i].position;
			}
		}
		else
		{
			if (dataPointsTmp[i].sharpness > maxGradient_far)
			{
				maxGradient_far = dataPointsTmp[i].sharpness;
				m_positionFar_MaxGrad_BM = dataPointsTmp[i].position;
			}
		}
	}

	m_positionCenter_MaxGrad_BM = (m_positionNear_MaxGrad_BM + m_positionFar_MaxGrad_BM)/2.0;


	if ((int)dataPointsTmp.size() < m_minPointCount)                    // not enough points
		return(AF_INSUFFICIENT_POINTS);
	else if (dataPointsTmp[0].sharpness < m_minEdgeGradient) // no good focus was found
		return(AF_LOW_GRADIENT);
	return(AF_OK);
}


autofocusReturn_t CFocusPosition::analyze_weightedAverage_BM(const dataPointVector_t &dataPoints)
{
	m_positionNear_WeightedAve_BM = 0.0;
	m_positionCenter_WeightedAve_BM = 0.0;
	m_positionFar_WeightedAve_BM = 0.0;

	//
	// Copy and Sort the points by edge gradient
	//
    dataPointVector_t dataPointsTmp;
    try
	{
		dataPointsTmp = dataPoints;
		std::sort(dataPointsTmp.begin(), dataPointsTmp.end(), dataPoint_t::comp_gradient_decending);
	}
	catch (...)
	{
		return(AF_INTERNAL_ALLOCATION);
	}
	if ( (dataPointsTmp.size() < 1) || (dataPointsTmp[0].sharpness <= 0.0) )
		return(AF_LOW_GRADIENT);

	//
	// Take the top points
	//
	double maxGrad = dataPointsTmp[0].sharpness;
	double minGrad = dataPointsTmp[0].sharpness * m_gradientPercentile;
	if (minGrad < m_edgeGradientCutoff)
		minGrad = m_edgeGradientCutoff;
	if (minGrad > maxGrad)
		minGrad = maxGrad - 1.0;

	for (int i=0; i<(int)dataPointsTmp.size(); i++)
	{
		if (dataPointsTmp[i].sharpness < minGrad)
		{
			int last = (int)dataPointsTmp.size() - 1;
			if (last >= 0)
			    dataPointsTmp.erase(dataPointsTmp.begin()+i, dataPointsTmp.end());
			break;
		}
	}

	//
	// compute the median position
	//
	int last = (int)dataPointsTmp.size() - 1;
	double maxPosition = dataPoints[0].position;
	double minPosition = dataPoints[dataPoints.size()-1].position;
	for (int i=0; i<(int)dataPointsTmp.size(); i++)
	{
		if (dataPointsTmp[i].position < minPosition)
			minPosition = dataPointsTmp[i].position;
		if (dataPointsTmp[i].position > maxPosition)
			maxPosition = dataPointsTmp[i].position;
	}
	double medianPosition = (minPosition + maxPosition)/2.0;

	//
	// find the weighted average of each half
	//
	double sum_near = 0.0;
	double weight_near = 0.0;
	double sum_far = 0.0;
	double weight_far = 0.0;
	double m = (1.0/(maxGrad-minGrad));
	for (int i=0; i<(int)dataPointsTmp.size(); i++)
	{
		if (dataPointsTmp[i].position <= medianPosition)
		{
			double pointWeight = m*(dataPointsTmp[i].sharpness - minGrad);
			sum_near += (pointWeight * dataPointsTmp[i].position);
			weight_near += pointWeight;
		}
		else
		{
			double pointWeight = m*(dataPointsTmp[i].sharpness - minGrad);
			sum_far += (pointWeight * dataPointsTmp[i].position);
			weight_far += pointWeight;
		}
	}

	m_positionNear_WeightedAve_BM = sum_near/weight_near;
	m_positionFar_WeightedAve_BM = sum_far/weight_far;
	m_positionCenter_WeightedAve_BM = (m_positionNear_WeightedAve_BM + m_positionFar_WeightedAve_BM)/2.0;

	if ((int)dataPointsTmp.size() < m_minPointCount)         // not enough points
		return(AF_INSUFFICIENT_POINTS);
	else if (dataPointsTmp[0].sharpness < m_minEdgeGradient) // no good focus was found
		return(AF_LOW_GRADIENT);
	return(AF_OK);
}



double CFocusPosition::getCenterFocusPosition(autoFocusMethod_t method)
{
	double position = 0.0;

	switch (method)
	{
	case AF_MAX_SHARPNESS:
		position = m_positionMaxGradient;
		break;

	case AF_WEIGHTED_AVERAGE:
		position = m_positionWeightedAverage;
		break;

	case AF_BM_MAX_SHARPNESS:
		position = m_positionCenter_MaxGrad_BM;
		break;

	case AF_BM_WEIGHTED_AVERAGE:
		position = m_positionCenter_WeightedAve_BM;
		break;

	case AF_BM_CURVE_FIT:
		position = m_positionCenter_CurveFit_BM;
		break;
	};

	return(position);
}


double CFocusPosition::getNearFocusPosition(autoFocusMethod_t method)
{
	double position = 0.0;

	switch (method)
	{
	case AF_MAX_SHARPNESS:
		position = m_positionWeightedAverage;
		break;

	case AF_WEIGHTED_AVERAGE:
		position = m_positionMaxGradient;
		break;

	case AF_BM_MAX_SHARPNESS:
		position = m_positionNear_MaxGrad_BM;
		break;

	case AF_BM_WEIGHTED_AVERAGE:
		position = m_positionNear_WeightedAve_BM;
		break;

	case AF_BM_CURVE_FIT:
		position = m_positionNear_CurveFit_BM;
		break;
	};

	return(position);
}


double CFocusPosition::getFarFocusPosition(autoFocusMethod_t method)
{
	double position = 0.0;

	switch (method)
	{
	case AF_MAX_SHARPNESS:
		position = m_positionWeightedAverage;
		break;

	case AF_WEIGHTED_AVERAGE:
		position = m_positionMaxGradient;
		break;

	case AF_BM_MAX_SHARPNESS:
		position = m_positionFar_MaxGrad_BM;
		break;

	case AF_BM_WEIGHTED_AVERAGE:
		position = m_positionFar_WeightedAve_BM;
		break;

	case AF_BM_CURVE_FIT:
		position = m_positionFar_CurveFit_BM;
		break;
	};

	return(position);
}
