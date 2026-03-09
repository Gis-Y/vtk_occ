/*******************************************************************************

程序说明

曲线离散程序，对Curve数据进行离散，用于后续的显示；

*******************************************************************************/

#ifndef CURVEADAPTIVEDISCRETE_H
#define CURVEADAPTIVEDISCRETE_H

#include <vector>
#include <BRepAdaptor_Curve.hxx>
#include <GCPnts_QuasiUniformDeflection.hxx>
#include <GCPnts_TangentialDeflection.hxx>
#include <GCPnts_UniformDeflection.hxx>

using namespace std;

enum CurveDiscreteMethod {
        TangentialDeflection = 0,
        QuasiUniformDeflection = 1,
        UniformDeflection = 2
};


void CurveAdaptiveDiscrete(const BRepAdaptor_Curve &adaptorCurve,
                                 vector<double> &points, vector<int> &lines,
                                 CurveDiscreteMethod discreteMethod) {

        points.clear();
        lines.clear();

        Standard_Integer pointsLength = 0;

        double edgeLength = CPnts_AbscissaPoint::Length(adaptorCurve, 1e-7);
        // double adaptingDeflection = 0.1 * edgeLength;

        switch (discreteMethod) {
        case TangentialDeflection: {

                double adaptingDeflection = 0.005 * edgeLength;
                GCPnts_TangentialDeflection thePointsOnCurve;
                Standard_Real AngularDeflection = 0.3;
                Standard_Real CurvatureDeflection = adaptingDeflection;
                thePointsOnCurve.Initialize(adaptorCurve, AngularDeflection,
                                            CurvatureDeflection);
                pointsLength = thePointsOnCurve.NbPoints();

                for (Standard_Integer i = 1; i <= pointsLength; ++i) {
                        gp_Pnt pt;

                        pt = adaptorCurve.Value(thePointsOnCurve.Parameter(i));

                        points.push_back(pt.X());
                        points.push_back(pt.Y());
                        points.push_back(pt.Z());

                        if (i != pointsLength) {
                                lines.push_back(i - 1);
                                lines.push_back(i);
                        }
                }
        } break;
        case QuasiUniformDeflection: {

                double adaptingDeflection = 0.007 * edgeLength;
                GCPnts_QuasiUniformDeflection thePointsOnCurve;
                thePointsOnCurve.Initialize(adaptorCurve, adaptingDeflection);
                pointsLength = thePointsOnCurve.NbPoints();
                for (Standard_Integer i = 1; i <= pointsLength; ++i) {
                        gp_Pnt pt;

                        pt = adaptorCurve.Value(thePointsOnCurve.Parameter(i));

                        points.push_back(pt.X());
                        points.push_back(pt.Y());
                        points.push_back(pt.Z());

                        if (i != pointsLength) {
                                lines.push_back(i - 1);
                                lines.push_back(i);
                        }
                }
        } break;
        default: {
                // cout << "UniformDeflection" << endl;
                GCPnts_UniformDeflection thePointsOnCurve;
                double adaptingDeflection = 0.007 * edgeLength;
                thePointsOnCurve.Initialize(adaptorCurve, adaptingDeflection);
                pointsLength = thePointsOnCurve.NbPoints();
                for (Standard_Integer i = 1; i <= pointsLength; ++i) {
                        gp_Pnt pt;

                        pt = adaptorCurve.Value(thePointsOnCurve.Parameter(i));

                        points.push_back(pt.X());
                        points.push_back(pt.Y());
                        points.push_back(pt.Z());

                        if (i != pointsLength) {
                                lines.push_back(i - 1);
                                lines.push_back(i);
                        }
                }
        } break;
        }
}

#endif