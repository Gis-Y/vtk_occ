#ifndef _KMAS_COMX_FUNCTION_ACIS_INCL_HXX_20080716_
#define _KMAS_COMX_FUNCTION_ACIS_INCL_HXX_20080716_

#include <math.h>
#include <base/type.hxx>
#include <base/factory.hxx>
#include <pcre/pcre.hxx>

#include <assert.h>

#include <gl/gl.h>
#include <gl/glu.h>

#include <gl/gl_wcs.hxx>
#include <gl/gl_sensor.hxx>
#include <gl/gl_content.hxx>
#include <gl/gl_light.hxx>

#include <base/tag_util.hxx>
#include <base/tag_code.hxx>

// Include Acis Head Files

#include "acis.hxx"
#include "api.hxx"
#include "cstrapi.hxx"
#include "kernapi.hxx"
#include "af_api.hxx"
#include "getowner.hxx"
#include "fct_utl.hxx"
#include "intrapi.hxx"
#include "curve.hxx"
#include "curdef.hxx"
#include "edge.hxx"
#include "intsect.hxx"
#include "pcurve.hxx"
#include "surface.hxx"
#include "interval.hxx"
#include "acistype.hxx"
#include "point.hxx"
#include "vertex.hxx"
#include "face.hxx"
#include "surface.hxx"
#include "surdef.hxx"

#include <base/function.hxx>

using namespace KMAS::Die_maker::comx;

#include <string>
using namespace std;

namespace KMAS
{
namespace func
{

	template<typename point_t>
	void get_vertex_coords(const tag_t tag, point_t &pt)
	{
		KMAS::tag_util::TTagUtil tag_obj(tag);
		if(tag_obj.GetCode() == KMAS::tag_code::VERTEX) 
		{
			VERTEX *vertice = tag_obj.GetValue();
			const SPAtransf* transform = get_owner_transf_ptr(vertice);
			SPAposition position = vertice->geometry()->coords(); 
			position = position * transform;

			pt.x = position.x();
			pt.y = position.y();
			pt.z = position.z();
		}
	}

	template<typename point_t>
    bool is_pt_on_face(tag_t tag_face, const point_t pt)
	{
		KMAS::tag_util::TTagUtil tag_obj(tag_face);
		assert(tag_obj.GetCode() == KMAS::tag_code::FACE);

		FACE *face = (FACE*)tag_obj.GetValue();

		SURFACE* fs = face->geometry();
		const surface &sfs = fs->equation();
		logical is_pt_on_face = sfs.test_point(SPAposition(pt.x, pt.y, pt.z));

		return is_pt_on_face;
	}

}// end namespace func
}// end namespace KMAS

#endif

