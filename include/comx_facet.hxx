#ifndef _COMX_FACET_9786C39307C74299B1C9F6BE6E665E43
#define _COMX_FACET_9786C39307C74299B1C9F6BE6E665E43

#include <comx_napi.hxx>
#include <type_cast.hxx>

namespace comx {
  namespace facet {
    struct facet_func_list_t {
      bool(*facetLine)(double *pt1/*I*/, double *pt2/*I*/, 
		       double *&pts/*O*/, int &pts_len/*O*/, int *&lines/*O*/, int &lines_len/*O*/);
      bool(*facetArc)(double *pt/*I*/, double radius/*I*/, double angle1/*I*/, double angle2/*I*/,
		      double *&pts/*O*/, int &pts_len/*O*/, int *&lines/*O*/, int &lines_len/*O*/);
      bool(*facetCircle)(double *pt/*I*/, double radius/*I*/, 
			 double *&pts/*O*/, int &pts_len/*O*/, int *&lines/*O*/, int &lines_len/*O*/);
      bool(*facetSolid)(double *pt1/*I*/, double *pt2/*I*/, double *pt3/*I*/, double *pt4/*I*/,
			double *&pts/*O*/, double *&normals/*O*/, int &pts_len/*O*/, int *&triangle/*O*/, int &triangles_len/*O*/);
      bool(*facetEllipse)(double *pt/*I*/, double major_radius/*I*/, double minor_radius/*I*/, double *major_axis/*I*/, double *min_axis/*I*/, double angle1/*I*/, double angle2/*I*/,
			  double *&pts/*O*/, int &pts_len/*O*/, int *&lines/*O*/, int &lines_len/*O*/);
      bool(*facetText)(const char *text/*I*/, double height/*I*/, int hJustification/*I*/, int vJustification/*I*/, const char *codepage/*I*/,
		       double *&pts/*O*/, int &pts_len/*O*/, int *&lines/*O*/, int &lines_len/*O*/);
      bool(*facetMText)(const char *text/*I*/, double height/*I*/, int attachmentPoint/*I*/, double lineSpacingFactor/*I*/, const char *codepage/*I*/,
		       double *&pts/*O*/, int &pts_len/*O*/, int *&lines/*O*/, int &lines_len/*O*/);
    };

    struct IComxFacet {
      virtual facet_func_list_t GetFacetFuncList() = 0;
    };
    
    namespace C3F9A822DAE045BDA5409D3A243ABD48 {
      inline IComxFacet* _GetInf(Napi::Env *pEnv = NULL)
      {
	static IComxFacet *s_ptrComxFacetInf = NULL;
	if (s_ptrComxFacetInf) return s_ptrComxFacetInf;
	
	if (pEnv == NULL) return NULL;
	
	Napi::Value result = comx::napi::InvokeGlobalFunction(*pEnv, { "comx", "facet", "GetInterface" }, {});
	if (result.IsUndefined()) return NULL;
	
	std::string strInf = result.ToString().Utf8Value();
	
	unsigned long long ullInf = KMAS::type::type_cast<unsigned long long>(strInf);
	
	s_ptrComxFacetInf = (IComxFacet*)((void*)ullInf);
	return s_ptrComxFacetInf;
      }
    }
    
    inline bool InitInterface(Napi::Env &env)
    {
      return C3F9A822DAE045BDA5409D3A243ABD48::_GetInf(&env) != NULL;
    }
    
    inline bool DoLine(double *pt1/*I*/, double *pt2/*I*/, double *&pts/*O*/, int &pts_len/*O*/, int *&lines/*O*/, int &lines_len/*O*/)
    {
      static bool(*entry)(double *pt1/*I*/, double *pt2/*I*/, double *&pts/*O*/, int &pts_len/*O*/, int *&lines/*O*/, int &lines_len/*O*/) = NULL;
      
      if (entry == NULL)
	{
	  IComxFacet *inf = C3F9A822DAE045BDA5409D3A243ABD48::_GetInf();
	  if (inf == NULL) return false;
	  
	  entry = inf->GetFacetFuncList().facetLine;
	}
      if (entry == NULL) return false;
      
      return (*entry)(pt1, pt2, pts, pts_len, lines, lines_len);
    }
    
    inline bool DoArc(double *pt/*I*/, double radius/*I*/, double angle1/*I*/, double angle2/*I*/,
		      double *&pts/*O*/, int &pts_len/*O*/, int *&lines/*O*/, int &lines_len/*O*/)
    {
      static bool(*entry)(double *pt/*I*/, double radius/*I*/, double angle1/*I*/, double angle2/*I*/,
			  double *&pts/*O*/, int &pts_len/*O*/, int *&lines/*O*/, int &lines_len/*O*/) = NULL;
      
      if (entry == NULL)
	{
	  IComxFacet *inf = C3F9A822DAE045BDA5409D3A243ABD48::_GetInf();
	  if (inf == NULL) return false;
	  
	  entry = inf->GetFacetFuncList().facetArc;
	}
      if (entry == NULL) return false;
      
      return (*entry)(pt, radius, angle1, angle2, pts, pts_len, lines, lines_len);
    }
    
    inline bool DoCircle(double *pt/*I*/, double radius/*I*/, 
			 double *&pts/*O*/, int &pts_len/*O*/, int *&lines/*O*/, int &lines_len/*O*/)
    {
      static bool(*entry)(double *pt/*I*/, double radius/*I*/,
			  double *&pts/*O*/, int &pts_len/*O*/, int *&lines/*O*/, int &lines_len/*O*/) = NULL;
      
      if (entry == NULL)
	{
	  IComxFacet *inf = C3F9A822DAE045BDA5409D3A243ABD48::_GetInf();
	  if (inf == NULL) return false;
	  
	  entry = inf->GetFacetFuncList().facetCircle;
	}
      if (entry == NULL) return false;
      
      return (*entry)(pt, radius, pts, pts_len, lines, lines_len);
    }
    
    inline bool DoSolid(double *pt1/*I*/, double *pt2/*I*/, double *pt3/*I*/, double *pt4/*I*/,
			double *&pts/*O*/, double *&normals/*O*/, int &pts_len/*O*/, int *&triangles/*O*/, int &triangles_len/*O*/)
    {
      static bool(*entry)(double *pt1/*I*/, double *pt2/*I*/, double *pt3/*I*/, double *pt4/*I*/,
			  double *&pts/*O*/, double *&normals/*O*/, int &pts_len/*O*/, int *&triangle/*O*/, int &triangles_len/*O*/) = NULL;
      
      if (entry == NULL)
	{
	  IComxFacet *inf = C3F9A822DAE045BDA5409D3A243ABD48::_GetInf();
	  if (inf == NULL) return false;
	  
	  entry = inf->GetFacetFuncList().facetSolid;
	}
      if (entry == NULL) return false;
      
      return (*entry)(pt1, pt2, pt3, pt4, pts, normals, pts_len, triangles, triangles_len);
    }
    
    inline bool DoEllipse(double *pt/*I*/, double major_radius/*I*/, double minor_radius/*I*/, double *major_axis/*I*/, double *min_axis/*I*/, double angle1/*I*/, double angle2/*I*/,
			  double *&pts/*O*/, int &pts_len/*O*/, int *&lines/*O*/, int &lines_len/*O*/)
    {
      static bool(*entry)(double *pt/*I*/, double major_radius/*I*/, double minor_radius/*I*/, double *major_axis/*I*/, double *min_axis/*I*/, double angle1/*I*/, double angle2/*I*/,
			  double *&pts/*O*/, int &pts_len/*O*/, int *&lines/*O*/, int &lines_len/*O*/) = NULL;
      
      if (entry == NULL)
	{
	  IComxFacet *inf = C3F9A822DAE045BDA5409D3A243ABD48::_GetInf();
	  if (inf == NULL) return false;
	  
	  entry = inf->GetFacetFuncList().facetEllipse;
	}
      if (entry == NULL) return false;
      
      return (*entry)(pt, major_radius, minor_radius, major_axis, min_axis, angle1, angle2, pts, pts_len, lines, lines_len);
    }
    
    inline bool DoText(const char *text/*I*/, double height/*I*/, int hJustification/*I*/, int vJustification/*I*/, const char *codepage/*I*/,
		       double *&pts/*O*/, int &pts_len/*O*/, int *&lines/*O*/, int &lines_len/*O*/)
    {
      static bool(*entry)(const char *text/*I*/, double height/*I*/, int hJustification/*I*/, int vJustification/*I*/, const char *codepage/*I*/,
			  double *&pts/*O*/, int &pts_len/*O*/, int *&lines/*O*/, int &lines_len/*O*/) = NULL;
      
      if (entry == NULL)
	{
	  IComxFacet *inf = C3F9A822DAE045BDA5409D3A243ABD48::_GetInf();
	  if (inf == NULL) return false;
	  
	  entry = inf->GetFacetFuncList().facetText;
	}
      if (entry == NULL) return false;
      
      return (*entry)(text, height, hJustification, vJustification, codepage, pts, pts_len, lines, lines_len);
    }

    inline bool DoMText(const char *text/*I*/, double height/*I*/, int attachmentPoint/*I*/, double lineSpacingFactor/*I*/, const char *codepage/*I*/,
		       double *&pts/*O*/, int &pts_len/*O*/, int *&lines/*O*/, int &lines_len/*O*/)
    {
      static bool(*entry)(const char *text/*I*/, double height/*I*/, int attachmentPoint/*I*/, double lineSpacingFactor/*I*/, const char *codepage/*I*/,
		       double *&pts/*O*/, int &pts_len/*O*/, int *&lines/*O*/, int &lines_len/*O*/) = NULL;
      
      if (entry == NULL)
	{
	  IComxFacet *inf = C3F9A822DAE045BDA5409D3A243ABD48::_GetInf();
	  if (inf == NULL) return false;
	  
	  entry = inf->GetFacetFuncList().facetMText;
	}
      if (entry == NULL) return false;
      
      return (*entry)(text, height, attachmentPoint, lineSpacingFactor, codepage, pts, pts_len, lines, lines_len);
    }
    
  } // end namespace facet
} // end namespace comx

#endif // !_COMX_FACET_9786C39307C74299B1C9F6BE6E665E43

