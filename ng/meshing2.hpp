#pragma once
/**************************************************************************/
/* File:   meshing2.hpp                                                   */
/* Author: Joachim Schoeberl                                              */
/* Date:   01. Okt. 95                                                    */
/**************************************************************************/

enum MESHING2_RESULT
{
  MESHING2_OK = 0,
  MESHING2_GIVEUP = 1
};

/*
The basis class for 2D mesh generation. 
Has the method GenerateMesh

For surface mesh generation, or non-Euklidean meshing,
derive from Meshing2, and replace transformation.
*/

class Meshing2
{
  /// the current advancing front
  AdFront2 * adfront;
  /// rules for mesh generation
  Array<netrule*> rules;
  /// statistics
  Array<int> ruleused, canuse, foundmap;
  /// 
  Box<3> boundingbox;
  ///
  double maxarea;

  Vec3d ex, ey;
  Point3d globp1;

public:
  ///
   Meshing2 (const MeshingParameters & mp, const Box<3> & aboundingbox);

  ///
   virtual ~Meshing2 ();

  /// Load rules, either from file, or compiled rules
  void LoadRules (const char * filename);

  /// 
   MESHING2_RESULT GenerateMesh (Mesh & mesh, const MeshingParameters & mp, double gh, int facenr);

  ///
   void AddPoint (const Point3d & p, PointIndex globind, MultiPointGeomInfo * mgi = NULL,
		 bool pointonsurface = true);

  ///
   void AddBoundaryElement (INDEX i1, INDEX i2,
			   const PointGeomInfo & gi1, const PointGeomInfo & gi2);

  ///
  void SetMaxArea (double amaxarea);

protected:
  ///
  virtual void StartMesh ();
  ///
  virtual double CalcLocalH (const Point3d & p, double gh) const;

  ///
  virtual void DefineTransformation (const Point3d & p1, const Point3d & p2,
				     const PointGeomInfo * geominfo1,
				     const PointGeomInfo * geominfo2);
  ///
  virtual void TransformToPlain (const Point3d & locpoint, const MultiPointGeomInfo &  geominfo,
				 Point2d & plainpoint, double h, int & zone);
  /// return 0 .. ok
  /// return >0 .. cannot transform point to true surface
  virtual int TransformFromPlain (Point2d & plainpoint,
				  Point3d & locpoint, 
				  PointGeomInfo & geominfo, 
				  double h);
  
  /// projects to surface
  /// return 0 .. ok
  virtual int BelongsToActiveChart (const Point3d & p, 
				    const PointGeomInfo & gi);

  /// computes geoinfo data for line with respect to
  /// selected chart
  virtual int ComputePointGeomInfo (const Point3d & p, 
				    PointGeomInfo & gi);

  /// Tries to select unique geominfo on active chart
  /// return 0: success
  /// return 1: failed
  virtual int ChooseChartPointGeomInfo (const MultiPointGeomInfo & mpgi, 
					PointGeomInfo & pgi);

  /*
    tests, whether endpoint (= 1 or 2) of line segment p1-p2
    is inside of the selected chart. The endpoint must be on the
    chart
   */
  virtual int IsLineVertexOnChart (const Point3d & p1, const Point3d & p2,
				   int endpoint, const PointGeomInfo & geominfo);

  /*
    get (projected) boundary of current chart
   */
  virtual void GetChartBoundary (Array<Point2d> & points, 
				 Array<Point3d> & points3d,
				 Array<INDEX_2> & lines, double p) const;

  virtual double Area () const;

/** Applies 2D rules.
 Tests all 2D rules */
  int ApplyRules (Array<Point2d> & lpoints, 
		  Array<int> & legalpoints,
		  int maxlegalpoint,
		  Array<INDEX_2> & llines,
		  int maxlegelline,
		  Array<Element2d> & elements, Array<INDEX> & dellines,
		  int tolerance,
		  const MeshingParameters & mp);
};