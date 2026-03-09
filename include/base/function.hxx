#ifndef _KMAS_COMX_FUNCTION_INCL_HXX_20080716_
#define _KMAS_COMX_FUNCTION_INCL_HXX_20080716_

#include <math.h>
#include <base/type.hxx>
#include <base/factory.hxx>

#if defined(_WINDOWS) || defined(WIN32)
#include <pcre/pcre.hxx>
#endif

#include <assert.h>

/*
#if defined(_WINDOWS) || defined(WIN32)
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include <gl/gl_wcs.hxx>
#include <gl/gl_sensor.hxx>
#include <gl/gl_content.hxx>
#include <gl/gl_light.hxx>
#endif
*/

#include <base/tag_util.hxx>
#include <base/tag_code.hxx>

using namespace KMAS::Die_maker::comx;

#include <string>
using namespace std;

namespace KMAS
{
namespace func
{
    // function: max.
    // 
    template<typename T>
    T max_value(const T left, const T right)
    {
        return left > right ? left : right;
    }
    template<typename T>
    T max_value(const T first, const T second, const T third)
    {
        return max_value(max_value(first, second), third);
    }

    // function: min.
    // 
    template<typename T>
    T min_value(const T left, const T right)
    {
        return left < right ? left : right;
    }
    template<typename T>
    T min_value(const T first, const T second, const T third)
    {
        return min_value(min_value(first, second), third);
    }

    // fucntion: get_vec_from_two_point.
    template<typename point_t_1,
             typename point_t_2>
    vector3d_t get_vec_from_two_point(point_t_1 spt, point_t_2 ept)
    {
        vector3d_t ret;

        ret.x = ept.x - spt.x;
        ret.y = ept.y - spt.y;
        ret.z = ept.z - spt.z;

        return ret;
    }

	// function: get_end_point_from_vec.
	template<typename point_t_1,
			 typename vector_t>
	point_t_1 get_end_point_from_vec(
		point_t_1 spt, 
		vector_t vec, 
		double scale = 1.0)
	{
		point_t_1 ret;

		ret.x = spt.x + vec.x * scale;
		ret.y = spt.y + vec.y * scale;
		ret.z = spt.z + vec.z * scale;

		return ret;
	}

	// function: get_start_point_from_vec.
	template<typename point_t_1,
		     typename vector_t>
	point_t_1 get_start_point_from_vec(
		point_t_1 ept,
		vector_t vec,
		double scale = 1.0)
	{
		point_t_1 ret;

		ret.x = ept.x - vec.x * scale;
		ret.y = ept.y - vec.y * scale;
		ret.z = ept.z - vec.z * scale;

		return ret;
	}

    // function: get_model.
    template<typename vector3d_t>
    double get_model(const vector3d_t t)
    {
        return sqrt(t.x * t.x + t.y * t.y + t.z * t.z);
    }

    // function: normalize_vec.
    template<typename vector3d_t> 
    vector3d_t normalize_vec(const vector3d_t t)
    {
        vector3d_t ret = t;

        double model = get_model(t);
        assert(model > 1e-10);

        ret.x /= model;
        ret.y /= model;
        ret.z /= model;

        return ret;
    }

    // fucntion: is_unit_vector.
    template<typename vector3d_t>
    bool is_unit_vector(vector3d_t vec)
    {
        return (fabs(get_model(vec) - 1.0) < 1e-10);
    }

    //function: fork_product.
    template<typename vector3d_t>
    vector3d_t fork_product(const vector3d_t left, const vector3d_t right)
    {
        vector3d_t ret;

        ret.x = left.y * right.z - left.z * right.y;
        ret.y = left.z * right.x - left.x * right.z;
        ret.z = left.x * right.y - left.y * right.x;

        return ret;
    }

    //function: dot_product.
    template<typename vector3d_t_1, typename vector3d_t_2>
    double dot_product(const vector3d_t_1 left, const vector3d_t_2 right)
    {
        return left.x * right.x + left.y * right.y + left.z * right.z;
    }

    //function: measure_distance
    template<typename point_t_1, typename point_t_2>
    double measure_distance(const point_t_1 spt, const point_t_2 ept)
    {
		return KMAS::func::get_model(KMAS::func::get_vec_from_two_point(spt, ept));
    }

    // function: intersection_l2p.
    // description: get intersection point between point and plane.
    // parameters:
    //  -ppt: the origin point of plane.
    //  -pnor: the normal of plane.
    //  -lpt: the origin point of line.
    //  -lnor: the direction of line.
    template<typename point_t_1, typename point_t_2, 
		     typename vector3d_t_1, typename vector3d_t_2>
    point_t_1 intersection_l2p(const point_t_1 lpt,
                             const vector3d_t_1 lnor, 
                             const point_t_2 ppt,
                             const vector3d_t_2 pnor)
    {
        assert(is_unit_vector(pnor));
        assert(is_unit_vector(lnor));

        point_t_1 spt = lpt;

        double d = dot_product(pnor, lpt);
        double np = dot_product(pnor, ppt);
        double nd = dot_product(pnor, lnor);
        double t = (np - d) / nd;

        spt.x += t * lnor.x;
        spt.y += t * lnor.y;
        spt.z += t * lnor.z;

        return spt;
    }

    // function: is_pt_in_polygon.
    // description: judge if or nor a point is in a polygon.
    // parameters:
    //  -pt: the target point.
    //  -polygon: the points array of polygon.
    //  -poly_len: the side number of polygon.
    //  -tol: the tolerance of this function, 
    //       and it should be between 0.0 and 1.0.
    // 
    template<typename point_t>
    bool is_pt_in_polygon(const point_t pt, 
                          point_t *polygon, 
                          const int poly_len, 
                          const double tol = 1e-10)
    {
        int i = 0, j = 0, k = 0, l = 0;
        int status = 0;
        int indexxy[4];        //象限标识
        int xyminus[4],xymin = 0;//象限的差
        int arclength = 0;        //弧长的和
        float ff = 0.0;

        memset(indexxy, 0, sizeof(int) * 4);
        memset(xyminus, 0, sizeof(int) * 4);

        const int &nn = poly_len;

        for(i = 0; i < nn; ++i)
        {
            polygon[i].x = polygon[i].x - pt.x;
            polygon[i].y = polygon[i].y - pt.y;
        }

        //****************************************
        for(l = 0; l < nn; ++l)//原点是单元节点
        {
            if(fabs(polygon[l].x) < 1.0e-6 && fabs(polygon[l].y) < 1.0e-6)
            {
                status = 1;
                goto loop;
            }
        }

        for(j = 0; j < nn; ++j)
        {
            if(polygon[j].x >= 0.0)
            {
                if(polygon[j].y >= 0.0)
                    indexxy[j] = 1;
                else
                    indexxy[j] = 4;
            } 
            else if(polygon[j].y >= 0.0)
                indexxy[j] = 2;
            else
                indexxy[j] = 3;
        }

        //****************************************
        for(k = 0;k < nn - 1;++k)
        {
            xyminus[k] = indexxy[k+1] - indexxy[k];
        }
        xyminus[nn - 1] = indexxy[0] - indexxy[nn - 1];
        arclength = 0;
        for(l = 0; l < nn; ++l)
        {
            xymin = xyminus[l];
            if(xymin ==  0) arclength = arclength;
            if(xymin ==  1) arclength = arclength + 1;
            if(xymin == -1) arclength = arclength - 1;
            if(xymin ==  3) arclength = arclength - 1;
            if(xymin == -3) arclength = arclength + 1;

            if(abs(xymin) == 2)
            {
                if(l < nn - 1)
                    ff = polygon[l + 1].y * polygon[l].x 
                    - polygon[l + 1].x * polygon[l].y;
                else
                    ff = polygon[l + 1 - nn].y * polygon[l].x
                    - polygon[l + 1 - nn].x * polygon[l].y;
                if(fabs(ff) <= tol) //在边上
                {
                    status = 1;
                    goto loop;
                } 
                else if(ff > tol)//f>0 ,则弧长代数和增加 
                    arclength += 2;
                else //f<0 ,则弧长代数和减少 
                    arclength += 2;
            }
        }
        if(arclength == 0)
            status = 0;
        else
            status = 1;
        loop:
            return (status);
    }

    // function: get_area_of_triangle.
    template<typename point_t_1, 
             typename point_t_2, 
             typename point_t_3>
    double get_area_of_triangle(const point_t_1 pt1, 
                                const point_t_2 pt2,
                                const point_t_3 pt3)
    {
		return KMAS::func::get_model(KMAS::func::fork_product(KMAS::func::get_vec_from_two_point(pt1, pt2),
                                      KMAS::func::get_vec_from_two_point(pt1, pt3))) / 2.0;
    }

    // fucntion: get_normal_of_triangle.
    template<typename point_t_1,
             typename point_t_2,
             typename point_t_3>
    vector3d_t get_normal_of_triangle(const point_t_1 pt1,
                                    const point_t_2 pt2,
                                    const point_t_3 pt3)
    {
		return KMAS::func::normalize_vec(KMAS::func::fork_product(KMAS::func::get_vec_from_two_point(pt1, pt2), 
                                          KMAS::func::get_vec_from_two_point(pt1, pt3)));
    }

    // fucntion: is_three_pts_in_common_line.
    // description: judge three points if or nor is in the common line.
    // 
    template<typename point_t_1,
             typename point_t_2,
             typename point_t_3>
    bool is_three_pts_in_common_line(const point_t_1 pt1,
                                     const point_t_2 pt2,
                                     const point_t_3 pt3)
    {
        double d12 = measure_distance(pt1, pt2);
        double d23 = measure_distance(pt2, pt3);
        double d13 = measure_distance(pt1, pt3);

        double max_distance = max_value(d12, d23, d13);
        double perimeter = d12 + d23 + d13;

        return fabs(perimeter - 2 * max_distance) < max_distance * 1e-3;
    }

    // function: get_pedal_point_p2l.
    // description: get the pedal point from a point to a line.
    // 
    template<typename point_t_1,
             typename point_t_2,
             typename vector3d_t>
    point_t_1 get_pedal_point_p2l(const point_t_1 pt,
                                 const point_t_2 lpt,
                                 const vector3d_t lnor)
    {
        assert(is_unit_vector(lnor));

        double proj_distance = dot_product(get_vec_from_two_point(lpt, pt), lnor);

        point_t_1 ret;
        ret.x = lpt.x + proj_distance * lnor.x;
        ret.y = lpt.y + proj_distance * lnor.y;
        ret.z = lpt.z + proj_distance * lnor.z;

        return ret;
    }

    // function: get_pedal_point_p2p.
    // description: get the pedal point form point to plane.
    // parameters:
    //  -pt: the target point.
    //  -ppt: the origin point of the plane.
    //  -pnor: the normal of the plane.
    // 
    template<typename point_t_1,
             typename point_t_2,
             typename normal_t>
    point_t_1 get_pedal_point_p2p(point_t_1 pt,
                                  point_t_2 ppt,
                                  normal_t pnor)
    {
        assert(KMAS::func::is_unit_vector(pnor));
		double proj_distance = KMAS::func::dot_product(KMAS::func::get_vec_from_two_point(pt, ppt), pnor);

        point_t_1 ret;

        ret.x = pt.x + proj_distance * pnor.x;
        ret.y = pt.y + proj_distance * pnor.y;
        ret.z = pt.z + proj_distance * pnor.z;

        return ret;
    }

    // function: mirror_p2l.
    // description: mirror point by line.
    template<typename point_t_1,
             typename point_t_2,
             typename normal_t>
    point_t_1 mirror_p2l(point_t_1 pt,
                         point_t_2 lpt,
                         normal_t lnor)
    {
        point_t_1 pedal_pt = get_pedal_point_p2l(pt, lpt, lnor);
        vector3d_t vec = get_vec_from_two_point(pt, pedal_pt);

        point_t_1 ret;

        ret.x = pedal_pt.x + vec.x;
        ret.y = pedal_pt.y + vec.y;
        ret.z = pedal_pt.z + vec.z;

        return ret;
    }

    // function: mirror_p2p.
    // description: mirror point by plane.
    // 
    template<typename point_t_1,
             typename point_t_2,
             typename normal_t>
    point_t_1 mirror_p2p(const point_t_1 pt,
                         const point_t_2 ppt,
                         const normal_t pnor)
    {
        point_t_1 pedal_pt = get_pedal_point_p2p(pt, ppt, pnor);
        vector3d_t vec = get_vec_from_two_point(pt, pedal_pt);

        point_t_1 ret;

        ret.x = pedal_pt.x + vec.x;
        ret.y = pedal_pt.y + vec.y;
        ret.z = pedal_pt.z + vec.z;

        return ret;
    }

	template<typename point_t, 
		typename point_t_1,
		typename point_t_2>
	double min_distance_point_to_line_segment(point_t pt, point_t_1 start, point_t_2 end, point_t &min_pt)
	{
		vector3d_t lnor = normalize_vec(get_vec_from_two_point(start, end));
		point_t ped_pt = get_pedal_point_p2l(pt, start, lnor);

		if (is_pt_on_line_segment(ped_pt, start, end))
		{
			min_pt = ped_pt;
			return measure_distance(ped_pt, pt);
		}
		else
		{
			double d1 = measure_distance(pt, start);
			double d2 = measure_distance(pt, end);

			if (d1 < d2)
			{
				min_pt = start;
				return d1;
			}
			else
			{
				min_pt = end;
				return d2;
			}
		}
	}

	template<typename point_t_1,
		typename point_t_2,
		typename point_t_3>
	double get_perimeter_of_triangle(point_t_1 pt1, point_t_2 pt2, point_t_3 pt3)
	{
		return measure_distance(pt1, pt2) + 
			measure_distance(pt2, pt3) +
			measure_distance(pt3, pt1);
	}

	template<typename point_t,
		typename point_t_1,
		typename point_t_2,
		typename point_t_3>
	bool is_pt_in_triangle(point_t pt, point_t_1 pt1, point_t_2 pt2, point_t_3 pt3)
	{
		vector3d_t nor_tri = get_normal_of_triangle(pt1, pt2, pt3);
		vector3d_t nor = get_normal_of_triangle(pt1, pt2, pt);

		if (dot_product(nor, nor_tri) < 0.0)
		{
			return false;
		}

		nor = get_normal_of_triangle(pt2, pt3, pt);

		if (dot_product(nor, nor_tri) < 0.0)
		{
			return false;
		}

		nor = get_normal_of_triangle(pt3, pt1, pt);

		if (dot_product(nor, nor_tri) < 0.0)
		{
			return false;
		}

		return true;
	}

	template<typename point_t,
		typename point_t_1,
		typename point_t_2,
		typename point_t_3>
	double min_distance_point_to_triangle(point_t pt, point_t_1 pt1, point_t_2 pt2, point_t_3 pt3, point_t &min_pt)
	{
		vector3d_t pnor = get_normal_of_triangle(pt1, pt2, pt3);
		point_t ped_point = get_pedal_point_p2p(pt, pt1, pnor);

		point_t polygon[3];
		polygon[0] = pt1, polygon[1] = pt2, polygon[2] = pt3;
		
		if(is_pt_in_triangle(ped_point, pt1, pt2, pt3))
		{
			min_pt = ped_point;
			return measure_distance(ped_point, pt);
		}
		else
		{
			point_t min_pt1, min_pt2, min_pt3;
			double d1 = min_distance_point_to_line_segment(ped_point, pt1, pt2, min_pt1);
			double d2 = min_distance_point_to_line_segment(ped_point, pt2, pt3, min_pt2);
			double d3 = min_distance_point_to_line_segment(ped_point, pt1, pt3, min_pt3);

			double ret = 0;
			if (d1 < d2)
			{
				ret = d1;
				min_pt = min_pt1;
			}
			else
			{
				ret = d2;
				min_pt = min_pt2;
			}

			if (ret > d3)
			{
				ret = d3;
				min_pt = min_pt3;
			}

			return ret;
		}
	}
#if defined(_WINDOWS) || defined(WIN32)
	inline bool regex_parser(
		const char* patten,
		const char *src,
		vector<int> &ret,
		unsigned int MAX_REC_NUM = 30)
	{
		const char *err;
		int erroroffset;
		vector<int> rec(MAX_REC_NUM);

		pcre *re = pcre_compile(
			patten,
			0,
			&err, 
			&erroroffset, 
			NULL);

		assert(re != NULL);
		
		int rec_num = pcre_exec(
			re,
			NULL,
			src,
			static_cast<int>(strlen(src)),
			0,
			0,
			&rec[0],
			MAX_REC_NUM);

		if (rec_num == -1)
			rec_num = 0;

		assert(rec_num >= 0);

		ret.clear();
		for(int loop = 0; loop < rec_num * 2; ++loop)
			ret.push_back(rec[loop]);

		pcre_free(re);// please don't use free(re), 
					  // and this usage is error.

		return rec_num > 0;
	}
#endif
	inline string get_ext_filename(const string &fname)
	{
		// vector<int> rec;
		// bool is_okay = regex_parser(
		// 	"(?<=\\.)\\w+$",
		// 	fname.c_str(),
		// 	rec);

		// if (is_okay) {
		// 	return fname.substr(rec[0]);
		// }
		// else
		// 	return "";

	        size_t pos = fname.find_last_of('.');
		return string(fname.substr(pos + 1));
	}

	inline bool is_ext_filename(const string &fname, const string &ext)
	{
		return get_ext_filename(fname) == ext;
	}

	inline bool is_char(char c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
	}

	inline bool is_digit(char c)
	{
		return (c >= '0' && c <= '9');
	}
#if false//defined(_WINDOWS) || defined(WIN32)
	inline void gl_project(
		double objx, double objy, double objz, 
		double &winx, double &winy, double &winz)
	{
		GLdouble modelMatrix[16];
		GLdouble projMatrix[16];
		GLint viewport[4];

		glGetDoublev(GL_MODELVIEW_MATRIX, (double*)modelMatrix);
		glGetDoublev(GL_PROJECTION_MATRIX, (double*)projMatrix);
		glGetIntegerv(GL_VIEWPORT, (int*)viewport);

		gluProject(objx, objy, objz,
		modelMatrix, projMatrix, viewport,
		&winx, &winy, &winz);
	}

	inline void gl_unproject(
		double winx, double winy, double winz,
		double &objx, double &objy, double &objz)
	{
		GLdouble modelMatrix[16];
		GLdouble projMatrix[16];
		GLint viewport[4];

		glGetDoublev(GL_MODELVIEW_MATRIX, (double*)modelMatrix);
		glGetDoublev(GL_PROJECTION_MATRIX, (double*)projMatrix);
		glGetIntegerv(GL_VIEWPORT, (int*)viewport);

		gluUnProject(winx, winy, winz,
   		modelMatrix, projMatrix, viewport,
   		&objx, &objy, &objz);
	}

	template<typename TPoint>
	TPoint gl_project(
		TPoint obj)
	{
		TPoint win;
		gl_project(
			obj.x, obj.y, obj.z, 
			win.x, win.y, win.z);

		return win;
	}

	template<typename TPoint>
	TPoint gl_unproject(
		TPoint win)
	{
		TPoint obj;
		gl_unproject(
			win.x, win.y, win.z, 
			obj.x, obj.y, obj.z);

		return obj;
	}

	// function: gl_trans_w2v.
	// description: trans coordinate from window CS to viewport CS.
	// 
	inline void gl_trans_w2v(
		double wx, double wy,
		double &vx, double &vy)
	{
		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, (int*)viewport);

		vx = wx;
		vy = viewport[3] -  wy;
	}

	inline void gl_trans_v2w(
		double wx, double wy,
		double &vx, double &vy)
	{
		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, (int*)viewport);

		vx = wx;
		vy = viewport[3] -  wy;
	}

	inline double get_module_scale()
	{
		double scale = 0.0;

		TFactory factory;
		QUERY_SERVICE(factory, CLSID_IGlWCS, IGlWCS, pGlWCS);
		pGlWCS->GetScale(scale);
		pGlWCS->Release();

		return scale;
	}

	template<typename point_t>
	void get_module_center(point_t &center)
	{
		TFactory factory;
		QUERY_SERVICE(factory, CLSID_IGlWCS, IGlWCS, pGlWCS);
		pGlWCS->GetCenter(center.x, center.y, center.z);
		pGlWCS->Release();
	}

	template<typename point_t>
	void get_module_coefficient(double &scale, point_t &center)
	{
		TFactory factory;
		QUERY_SERVICE(factory, CLSID_IGlWCS, IGlWCS, pGlWCS);
		pGlWCS->GetScale(scale);
		pGlWCS->GetCenter(center);
		pGlWCS->Release();
	}

	inline double get_gl_sensor_scale()
	{
		TFactory factory;

		QUERY_SERVICE(
			factory, 
			CLSID_IGlContent, 
			IGlSensorContent, 
			pGlSensorContent);

		double scale = 0.0;
		pGlSensorContent->GetScaleFactor(scale);

		pGlSensorContent->Release();

		return scale;
	}

	template<typename point_t_1>
	point_t_1 gl_trans_m2g(point_t_1 pt_m)
	{
		point_t_1 pt_g;

		double scale = get_module_scale();

		point_t_1 center;
		get_module_center(center);

		pt_g.x = (pt_m.x - center.x) / scale;
		pt_g.y = (pt_m.y - center.y) / scale;
		pt_g.z = (pt_m.z - center.z) / scale;

		return pt_g;
	}

	template<typename point_t_1>
	point_t_1 gl_trans_g2m(
		point_t_1 pt_g)
	{
		point_t_1 pt_m;

		double scale = get_module_scale();

		point_t_1 center;
		get_module_center(center);
		
		pt_m.x = pt_g.x * scale + center.x;
		pt_m.y = pt_g.y * scale + center.y;
		pt_m.z = pt_g.z * scale + center.z;

		return pt_m;
	}
#endif
	template<typename point_t_1,
		typename point_t_2,
		typename point_t_3>
	bool is_pt_on_line_segment(
		point_t_1 pt,
		point_t_2 spt,
		point_t_3 ept,
		const double tol = 1.0)
	{
		vector3d_t lnor = get_vec_from_two_point(spt, ept);

		if (get_model(lnor) < 1e-6)
			return false;

		lnor = normalize_vec(lnor);
		point_t_1 pedal_pt = get_pedal_point_p2l(pt, spt, lnor);

		double t = (pedal_pt.x - spt.x) / (ept.x - spt.x);

		return (measure_distance(pt, pedal_pt) < tol) &&
			   (t >= 0 && t <= 1.0);
	}
#if false//defined(_WINDOWS) || defined(WIN32)
	inline void gl_enable_simple_light(bool flag = true)
	{
        TFactory factory;
		QUERY_SERVICE(
			factory, 
			CLSID_IGlLight, 
			IGlLightConnector, 
			p_gl_light);

		flag ? p_gl_light->Activate() : p_gl_light->Closure();

		if (flag)
		{
			glEnable(GL_COLOR_MATERIAL);
			glColorMaterial(
				GL_FRONT_AND_BACK, 
				GL_AMBIENT_AND_DIFFUSE);
		}
		else
		{
			glDisable(GL_COLOR_MATERIAL);
		}

		p_gl_light->Release();
	}

	template<typename point_t, typename color_t>
	void gl_draw_sphere(
		point_t pt, 
		double size, 
		color_t color, 
		bool is_blend = true, 
		bool is_fixed = true) 
	{
        glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslated(pt.x, pt.y, pt.z);

		glScaled(size, size, size);

		if (is_fixed)
		{
			double scale = get_gl_sensor_scale();
			glScaled(
				1.0 / scale, 
				1.0 / scale, 
				1.0 / scale);
		}

		glColor4d(color.red, color.green, color.blue, color.alpha);

		gl_enable_simple_light(true);
	
		if (is_blend)
		{
			glEnable(GL_LINE_SMOOTH);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
		}

		static int s_display_list = -1;

		if(s_display_list == -1) {
			s_display_list = glGenLists(1);

			glNewList(s_display_list, GL_COMPILE);
			    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			    GLUquadricObj *quadratic = gluNewQuadric();
			    gluQuadricNormals(quadratic, GLU_SMOOTH);
			    gluQuadricTexture(quadratic, GL_TRUE);
			    gluSphere(quadratic, 1.0, 16, 16);
			    gluDeleteQuadric(quadratic);
			glEndList();
		}

		glCallList(s_display_list);

		if (is_blend)
		{
			glDisable(GL_LINE_SMOOTH);
			glDisable(GL_BLEND);
		}

		gl_enable_simple_light(false);

		glPopMatrix();
	}

	template<typename point_t>
	void gl_draw_sphere(
		point_t pt, 
		double size, 
        bool is_blend = true, 
		bool is_fixed = true) 
	{
        glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslated(pt.x, pt.y, pt.z);

		glScaled(size, size, size);

		if (is_fixed)
		{
			double scale = get_gl_sensor_scale();
			glScaled(
				1.0 / scale, 
				1.0 / scale, 
				1.0 / scale);
		}

		gl_enable_simple_light(true);
	
		if (is_blend)
		{
			glEnable(GL_LINE_SMOOTH);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
		}


			    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			    GLUquadricObj *quadratic = gluNewQuadric();
			    gluQuadricNormals(quadratic, GLU_SMOOTH);
			    gluQuadricTexture(quadratic, GL_TRUE);
			    gluSphere(quadratic, 1.0, 16, 16);
			    gluDeleteQuadric(quadratic);


		if (is_blend)
		{
			glDisable(GL_LINE_SMOOTH);
			glDisable(GL_BLEND);
		}

		gl_enable_simple_light(false);

		glPopMatrix();
	}

	template<typename point_t, typename color_t>
	void gl_draw_arrow(
		const point_t spt,
		const point_t ept,
		double width,
		const color_t color,
		bool is_blend = true,
		bool is_fixed = true)
	{
		gl_enable_simple_light(true);

		glColor4d(color.red, color.green, color.blue, color.alpha);

		if (is_blend)
		{
			glEnable(GL_LINE_SMOOTH);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
		}

		double line_length = KMAS::func::measure_distance(spt, ept);
		if (is_fixed) 
		{
			width /= get_gl_sensor_scale();
		}
		else
		{
			line_length *= get_gl_sensor_scale();
		}

		KMAS::TVector3d vec(ept.x - spt.x, ept.y - spt.y, ept.z - spt.z);
		vec = normalize_vec(vec);

		double angle = acos(vec.z) / 3.1415926 * 180.;

		KMAS::TVector3d axis;
	
		if (fabs(angle) > 1e-6)
		{
			axis = fork_product(vec, KMAS::TVector3d(0.0, 0.0, 1.0));
			normalize_vec(axis);
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		GLUquadricObj *quadratic = gluNewQuadric();
		gluQuadricNormals(quadratic, GLU_SMOOTH);
		gluQuadricTexture(quadratic, GL_TRUE);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslated(spt.x, spt.y, spt.z);
		if (fabs(angle) > 1e-6)
		{
			glRotated(angle, -axis.x, -axis.y, -axis.z);
		}
	
		gluCylinder(
			quadratic,
			width,
			width,
			line_length,
			32,
			32);	

		glPopMatrix();
		glPushMatrix();

		//glTranslated(ept.x, ept.y, ept.z);
		glTranslated(
			spt.x + vec.x * line_length, 
			spt.y + vec.y * line_length, 
			spt.z + vec.z * line_length);

		if (fabs(angle) > 1e-6)
		{
			glRotated(angle, -axis.x, -axis.y, -axis.z);
		}
	
		gluCylinder(
			quadratic,
			width * 3,
			0.0f,
			width * 8,
			32,
			32);	

		glPopMatrix();

		gluDeleteQuadric(quadratic);

		if (is_blend)
		{
			glDisable(GL_LINE_SMOOTH);
			glDisable(GL_BLEND);
		}

		glLineWidth(1.0);

		gl_enable_simple_light(false);
	}
#endif
}// end namespace func
}// end namespace KMAS

#endif
