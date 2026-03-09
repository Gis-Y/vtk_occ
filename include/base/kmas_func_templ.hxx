#ifndef _KMAS_FUNC_TEMPL_HXX_20170523
#define _KMAS_FUNC_TEMPL_HXX_20170523

namespace KMAS {
namespace func {

#ifdef DOUBLE_PRECISION_MODE
typedef double real_t;
#else
typedef float real_t;
#endif

struct vector3d_t
{
    real_t x;
    real_t y;
    real_t z;
}; 
    
   template<typename real_t>
    inline bool IsRubbish(real_t val)
    {
        ostringstream sz_val;

        sz_val << val << '\n';
        string msg = sz_val.str();

        bool ret = false;
        for (int loop = 0; loop < static_cast<int>(msg.size()); ++loop)
        {
            if (msg[loop] == '#' || msg[loop] == 'N' || msg[loop] == 'Q' || msg[loop] == 'A')
            {
                ret = true;
                break;
            }
        }

        return ret;
    }

    template<typename normal_t>
    real_t Normalize(normal_t &normal)
    {
        double model = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
        assert(model > 1e-8 && !IsRubbish(model));

        if (model < 1e-8 || IsRubbish(model))
        {
            throw "normalize_failed";
        }
            normal.x /= static_cast<real_t>(model);
            normal.y /= static_cast<real_t>(model);
            normal.z /= static_cast<real_t>(model);

        return model;
    }

    /************************************************************************/
    /*  Template type constrain description                                 */
    /*    real_t: it should be float or double                              */
    /************************************************************************/

    template<typename real_t>
    void INVERT_MATRIX(real_t DALAD[3][3], real_t REDALAD[3][3])
    {
        real_t AA;


        AA=DALAD[0][0]*DALAD[1][1]*DALAD[2][2]+DALAD[1][0]*DALAD[2][1]*DALAD[0][2]+DALAD[2][0]*DALAD[0][1]*DALAD[1][2]
        -DALAD[0][2]*DALAD[1][1]*DALAD[2][0]-DALAD[1][0]*DALAD[0][1]*DALAD[2][2]-DALAD[0][0]*DALAD[1][2]*DALAD[2][1];

        REDALAD[0][0]=(DALAD[1][1]*DALAD[2][2]-DALAD[1][2]*DALAD[2][1])/AA;
        REDALAD[1][0]=-(DALAD[1][0]*DALAD[2][2]-DALAD[1][2]*DALAD[2][0])/AA;
        REDALAD[2][0]=(DALAD[1][0]*DALAD[2][1]-DALAD[1][1]*DALAD[2][0])/AA;
        REDALAD[0][1]=-(DALAD[0][1]*DALAD[2][2]-DALAD[0][2]*DALAD[2][1])/AA;
        REDALAD[1][1]=(DALAD[0][0]*DALAD[2][2]-DALAD[0][2]*DALAD[2][0])/AA;
        REDALAD[2][1]=-(DALAD[0][0]*DALAD[2][1]-DALAD[0][1]*DALAD[2][0])/AA;
        REDALAD[0][2]=(DALAD[0][1]*DALAD[1][2]-DALAD[0][2]*DALAD[1][1])/AA;
        REDALAD[1][2]=-(DALAD[0][0]*DALAD[1][2]-DALAD[0][2]*DALAD[1][0])/AA;
        REDALAD[2][2]=(DALAD[0][0]*DALAD[1][1]-DALAD[0][1]*DALAD[1][0])/AA;
    }


    template<class TPoint, class TNormal>
    class TOstpLCSMatrixEx
    {
    public:
        TOstpLCSMatrixEx(
            const TPoint &pt, 
            const TNormal &x_axis, 
            const TNormal &y_axis, 
            const TNormal &z_axis)
        {
            _origin_point.x = pt.x;
            _origin_point.y = pt.y;
            _origin_point.z = pt.z;

            TNormal xaxis;
            xaxis.x = x_axis.x;
            xaxis.y = x_axis.y;
            xaxis.z = x_axis.z;
            Normalize(xaxis);

            TNormal yaxis;
            yaxis.x = y_axis.x;
            yaxis.y = y_axis.y;
            yaxis.z = y_axis.z;
            Normalize(yaxis);

            TNormal zaxis;
            zaxis.x = z_axis.x;
            zaxis.y = z_axis.y;
            zaxis.z = z_axis.z;
            Normalize(zaxis);

            _matrix[0][0] = xaxis.x ;
            _matrix[0][1] = xaxis.y ; 
            _matrix[0][2] = xaxis.z; 
            _matrix[1][0] = yaxis.x ; 
            _matrix[1][1] = yaxis.y ; 
            _matrix[1][2] = yaxis.z; 
            _matrix[2][0] = zaxis.x ; 
            _matrix[2][1] = zaxis.y ; 
            _matrix[2][2] = zaxis.z; 

            INVERT_MATRIX(_matrix,_invert_matrix);
        }
        TOstpLCSMatrixEx(const TPoint &pt, const TNormal &z_axis)
        {
            _origin_point.x = pt.x;
            _origin_point.y = pt.y;
            _origin_point.z = pt.z;

            TNormal zaxis;
            zaxis.x = z_axis.x;
            zaxis.y = z_axis.y;
            zaxis.z = z_axis.z;
            Normalize(zaxis);

            int flag = 0;

            double min_axis = fabs(zaxis.x);

            if (min_axis > fabs(zaxis.y))
            {
                flag = 1;
                min_axis = fabs(zaxis.y);
            }

            if (min_axis > fabs(zaxis.z))
            {
                flag = 2;
            }

            TNormal xaxis;

            switch(flag) {
            case 0:
                xaxis.x = 0.0;
                xaxis.y = -zaxis.z;
                xaxis.z = zaxis.y;
                break;
            case 1:
                xaxis.x = -zaxis.z;
                xaxis.y = 0.0;
                xaxis.z = zaxis.x;
                break;
            case 2:
                xaxis.x = -zaxis.y;
                xaxis.y = zaxis.x;
                xaxis.z = 0.0;
                break;
            default:
                assert(false);
            }

            Normalize(xaxis);

            TNormal yaxis;
            fork_product(zaxis, xaxis, yaxis);
            Normalize(yaxis);

            _matrix[0][0] = xaxis.x ; _matrix[0][1] = xaxis.y ; _matrix[0][2] = xaxis.z; 
            _matrix[1][0] = yaxis.x ; _matrix[1][1] = yaxis.y ; _matrix[1][2] = yaxis.z; 
            _matrix[2][0] = zaxis.x ; _matrix[2][1] = zaxis.y ; _matrix[2][2] = zaxis.z; 

            INVERT_MATRIX(_matrix,_invert_matrix);
        }
        ~TOstpLCSMatrixEx()
        {
            _init();
        }
    private:
        TPoint _origin_point;
        double _matrix[3][3];
        double _invert_matrix[3][3];
    private:
        void _init()
        {
            _origin_point.x = 0.0;
            _origin_point.y = 0.0;
            _origin_point.z = 0.0;

            int i = 0 ;
            for (i = 0 ; i < 3 ; ++i)
            {
                for (int j = 0 ; j < 3 ; ++j)
                {
                    _matrix[i][j] = 0.0;
                }
            }

            _matrix[0][0] = _matrix[1][1] = _matrix[2][2] = 1.0;

            for (i = 0 ; i < 3 ; ++i)
            {
                for (int j = 0 ; j < 3 ; ++j)
                {
                    _invert_matrix[i][j] = 0.0;
                }
            }

            _invert_matrix[0][0] = 
                _invert_matrix[1][1] = 
                _invert_matrix[2][2] = 1.0;
        }
    public:
        void Trans(double &x, double &y, double &z)
        {
            double x_new = x, y_new = y, z_new = z;

            x_new -= _origin_point.x;
            y_new -= _origin_point.y;
            z_new -= _origin_point.z;

            x = x_new * _matrix[0][0] + y_new * _matrix[0][1] + z_new * _matrix[0][2];
            y = x_new * _matrix[1][0] + y_new * _matrix[1][1] + z_new * _matrix[1][2];
            z = x_new * _matrix[2][0] + y_new * _matrix[2][1] + z_new * _matrix[2][2];
        }
        void InverseTrans(double &x, double &y, double &z)
        {
            double x_new = 
                x * _invert_matrix[0][0] + 
                y * _invert_matrix[0][1] + 
                z * _invert_matrix[0][2];
            double y_new = 
                x * _invert_matrix[1][0] + 
                y * _invert_matrix[1][1] + 
                z * _invert_matrix[1][2];
            double z_new = 
                x * _invert_matrix[2][0] + 
                y * _invert_matrix[2][1] + 
                z * _invert_matrix[2][2];

            x = x_new + _origin_point.x;
            y = y_new + _origin_point.y;
            z = z_new + _origin_point.z;
        }
    };
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
    real_t get_model(const vector3d_t t)
    {
        return sqrt(t.x * t.x + t.y * t.y + t.z * t.z);
    }

    // function: normalize_vec.
    template<typename vector3d_t> 
    vector3d_t normalize_vec(const vector3d_t t)
    {
        vector3d_t ret = t;

        real_t model = get_model(t);
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
    real_t dot_product(const vector3d_t_1 left, const vector3d_t_2 right)
    {
        return left.x * right.x + left.y * right.y + left.z * right.z;
    }

    //function: measure_distance
    template<typename point_t_1, typename point_t_2>
    real_t measure_distance(const point_t_1 spt, const point_t_2 ept)
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

    template<typename vector3d_t>
    inline vector3d_t scale_vector3d(vector3d_t vec, real_t scale)
    {
        vec.x *= scale;
        vec.y *= scale;
        vec.z *= scale;

        return vec;
    }

    // fucntion: get_normal_of_triangle.
    template<typename point_t_1,
             typename point_t_2,
             typename point_t_3>
    vector3d_t get_normal_of_triangle(const point_t_1 pt1,
                                    const point_t_2 pt2,
                                    const point_t_3 pt3)
    {
        return KMAS::func::normalize_vec(KMAS::func::fork_product(scale_vector3d(KMAS::func::get_vec_from_two_point(pt1, pt2), 1E5), 
                                          scale_vector3d(KMAS::func::get_vec_from_two_point(pt1, pt3), 1E5)));
    }

            // fucntion: get_normal_of_quadangle.
            template<typename point_t_1,
                typename point_t_2,
                typename point_t_3,
                typename point_t_4>
                vector3d_t get_normal_of_quadangle(
                const point_t_1 pt1,
                const point_t_2 pt2,
                const point_t_3 pt3,
                const point_t_4 pt4)
            {
                return KMAS::func::normalize_vec(KMAS::func::fork_product(scale_vector3d(KMAS::func::get_vec_from_two_point(pt1, pt3), 1E5), 
                    scale_vector3d(KMAS::func::get_vec_from_two_point(pt2, pt4), 1E5)));
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

    template<typename point_t_1,
        typename point_t_2,
        typename point_t_3>
    bool is_triangle(const point_t_1 pt1,
        const point_t_2 pt2,
        const point_t_3 pt3)
    {
        double d12 = measure_distance(pt1, pt2);
        double d23 = measure_distance(pt2, pt3);
        double d13 = measure_distance(pt1, pt3);

        double max_distance = max_value(d12, d23, d13);
        double perimeter = d12 + d23 + d13;

        return fabs(perimeter - 2 * max_distance) > 1e-3;
    }

    template<typename point_t_1,
        typename point_t_2,
        typename point_t_3>
    double get_triangle_coefficient(const point_t_1 pt1,
        const point_t_2 pt2,
        const point_t_3 pt3)
    {
        double d12 = measure_distance(pt1, pt2);
        double d23 = measure_distance(pt2, pt3);
        double d13 = measure_distance(pt1, pt3);

        double max_distance = max_value(d12, d23, d13);
        double perimeter = d12 + d23 + d13;

        return fabs(perimeter - 2 * max_distance) / max_distance;
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

        real_t proj_distance = dot_product(get_vec_from_two_point(lpt, pt), lnor);

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
	real_t proj_distance = KMAS::func::dot_product(KMAS::func::get_vec_from_two_point(pt, ppt), pnor);

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

        double up = 0.0, down = 0.0, dis = -1e10;

        if (fabs(spt.x - ept.x) > dis)
        {
            dis = fabs(spt.x - ept.x);
            up = pedal_pt.x - spt.x;
            down = ept.x - spt.x;
        }

        if (fabs(spt.y - ept.y) > dis)
        {
            dis = fabs(spt.y - ept.y);
            up = pedal_pt.y - spt.y;
            down = ept.y - spt.y;
        }

        if (fabs(spt.z - ept.z) > dis)
        {
            dis = fabs(spt.z - ept.z);
            up = pedal_pt.z - spt.z;
            down = ept.z - spt.z;
        }

        double t = up / down;

        return (measure_distance(pt, pedal_pt) < tol) &&
            (t >= 0 && t <= 1.0);
    }

    template<typename point_t_1,
        typename point_t_2,
        typename point_t_3>
        bool is_pt_on_line_segment_ex(
        point_t_1 pt,
        point_t_2 spt,
        point_t_3 ept,
        double &t,
        point_t_1 &vped_pt,
        const double tol = 1.0)
    {
        vector3d_t lnor = get_vec_from_two_point(spt, ept);

        if (get_model(lnor) < 1e-6)
            return false;

        lnor = normalize_vec(lnor);
        point_t_1 pedal_pt = get_pedal_point_p2l(pt, spt, lnor);

        double up = 0.0, down = 0.0, dis = -1e10;

        if (fabs(spt.x - ept.x) > dis)
        {
            dis = fabs(spt.x - ept.x);
            up = pedal_pt.x - spt.x;
            down = ept.x - spt.x;
        }

        if (fabs(spt.y - ept.y) > dis)
        {
            dis = fabs(spt.y - ept.y);
            up = pedal_pt.y - spt.y;
            down = ept.y - spt.y;
        }

        if (fabs(spt.z - ept.z) > dis)
        {
            dis = fabs(spt.z - ept.z);
            up = pedal_pt.z - spt.z;
            down = ept.z - spt.z;
        }

        t = up / down;

        vped_pt = pedal_pt;

        return (measure_distance(pt, pedal_pt) < tol) &&
            (t >= 0 && t <= 1.0);
    }

	template<typename point_t, 
		typename point_t_1,
		typename point_t_2>
	double min_distance_point_to_line_segment(point_t pt, point_t_1 start, point_t_2 end, point_t &min_pt)
	{
		vector3d_t lnor = normalize_vec(get_vec_from_two_point(start, end));
		point_t ped_pt = get_pedal_point_p2l(pt, start, lnor);

                double d3 = measure_distance(ped_pt, start);
                double d4 = measure_distance(ped_pt, end);
                double d = measure_distance(start, end);

                if (fabs(d - d3 - d4) < 1e-6/*is_pt_on_line_segment(ped_pt, start, end)*/)
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

            // 3D vector
            class Vector3
	{
            public:
                Vector3(float fx, float fy, float fz)
                    :x(fx), y(fy), z(fz)
        {
                }

                // Subtract
                Vector3 operator - (const Vector3& v) const
		{
                    return Vector3(x - v.x, y - v.y, z - v.z) ;
        }

                // Dot product
                float Dot(const Vector3& v) const
        {
                    return x * v.x + y * v.y + z * v.z ;
                }

                // Cross product
                Vector3 Cross(const Vector3& v) const
		{
                    return Vector3(
                        y * v.z - z * v.y,
                        z * v.x - x * v.z,
                        x * v.y - y * v.x ) ;
        }

            public:
                float x, y, z ;
            };

            // Determine whether two vectors v1 and v2 point to the same direction
            // v1 = Cross(AB, AC)
            // v2 = Cross(AB, AP)
            inline bool SameSide(Vector3 A, Vector3 B, Vector3 C, Vector3 P)
            {
                Vector3 AB = B - A ;
                Vector3 AC = C - A ;
                Vector3 AP = P - A ;

                Vector3 v1 = AB.Cross(AC) ;
                Vector3 v2 = AB.Cross(AP) ;

                // v1 and v2 should point to the same direction
                return v1.Dot(v2) >= 0 ;
            }

            // Same side method
            // Determine whether point P in triangle ABC
            inline bool PointinTriangle1(Vector3 A, Vector3 B, Vector3 C, Vector3 P)
		{
                return SameSide(A, B, C, P) &&
                    SameSide(B, C, A, P) &&
                    SameSide(C, A, B, P) ;
           }
        
            template<typename point_t,
                typename point_t_1,
                typename point_t_2,
                typename point_t_3>
            bool is_pt_in_triangle(point_t pt, point_t_1 pt1, point_t_2 pt2, point_t_3 pt3)
            {
                Vector3 A(pt1.x, pt1.y, pt1.z), B(pt2.x, pt2.y, pt2.z), C(pt3.x, pt3.y, pt3.z), P(pt.x, pt.y, pt.z);
                return PointinTriangle1(A, B, C, P);
	}

	template<typename point_t,
		typename point_t_1,
		typename point_t_2,
		typename point_t_3>
        double min_distance_point_to_triangle(point_t pt, point_t_1 pt1, point_t_2 pt2, point_t_3 pt3, point_t &min_pt, bool &bRet)
	{
		vector3d_t pnor = get_normal_of_triangle(pt1, pt2, pt3);
		point_t ped_point = get_pedal_point_p2p(pt, pt1, pnor);

		point_t polygon[3];
		polygon[0] = pt1, polygon[1] = pt2, polygon[2] = pt3;
		
		if(is_pt_in_triangle(ped_point, pt1, pt2, pt3))
		{
			min_pt = ped_point;

            bRet = true;
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

            ret = measure_distance(min_pt, pt);

            bRet = false;
            return ret;
        }
    }
}// end namespace func
}// end namespace KMAS

#endif