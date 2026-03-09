#ifndef _KMAS_COMX_TYPE_HXX_200807091856
#define _KMAS_COMX_TYPE_HXX_200807091856

#include <base/root.hxx>
#include <assert.h>

namespace KMAS
{
    struct point_t
    {
        double x;
        double y;
        double z;
    };
    
	struct point2d_t
	{
		double u;
		double v;
	};

	struct interval_t
	{
		double low;
		double up;
	};

	struct par_box_t
	{
	};
    
    struct triangle_t
    {
        point_t pt1;
        point_t pt2;
        point_t pt3;
    };

    struct quadrangle_t
    {
        point_t pt1;
        point_t pt2;
        point_t pt3;
        point_t pt4;
    };

    struct rectangle_t
    {
        point_t left_top;
        point_t right_bottom;
    };

    struct line_t
    {
        point_t spt;
        point_t ept;
    };

    struct topology2_t
    {
        int n1;
        int n2;
    };

    struct topology3_t
    {
        int n1;
        int n2;
        int n3;
    };

    struct topology4_t
    {
        int n1;
        int n2;
        int n3;
        int n4;
    };

    struct color_t
    {
        double red;
        double green;
        double blue;
        double alpha;
    };

    struct vector3d_t
    {
        double x;
        double y;
        double z;
    };
	struct unit_vector3d_t
	{
		double x;
		double y;
		double z;
	};

	struct surf_cvt
	{
		double cvt1;
		double cvt2;

		unit_vector3d_t dir1;
		unit_vector3d_t dir2;
	};

	struct bound_box_t
	{
		interval_t xb;
		interval_t yb;
		interval_t zb;
	};

    struct plane_t
    {
        point_t pt;
        vector3d_t direction;
    };

    struct matrix2d_t
    {
        double data[2][2];
    };

    struct matrix3d_t
    {
        double data[3][3];
    };

    struct matrix4d_t
    {
        double data[4][4];
    };

    typedef KMAS::Die_maker::comx::IRoot* interface_ptr;

    struct KVARIANT
    {
        enum type_t
        {
            KUNKNOWN,
            KSHORT,
            KINT,
            KLONG,
            KUSHORT,
            KUINT,
            KULONG,
            KFLOAT,
            KDOUBLE,
            KLDOUBLE,
            KCHAR,
            KUCHAR,
            KPOINT,
            KLINE,
            KTRIANGLE,
            KQUADRANGLE,
            KRECTANGLE,
            KCOLOR,
            KVECTOR3D,
            KPLANE,
            KMATRIX2D,
            KMATRIX3D,
            KMATRIX4D,
            KTOPOLOGY2,
            KTOPOLOGY3,
            KTOPOLOGY4,
            KSHORT_PTR,
            KINT_PTR,
            KLONG_PTR,
            KUSHORT_PTR,
            KUINT_PTR,
            KULONG_PTR,
            KFLOAT_PTR,
            KDOUBLE_PTR,
            KLDOUBLE_PTR,
            KCHAR_PTR,
            KUCHAR_PTR,
            KPOINT_PTR,
            KLINE_PTR,
            KTRIANGLE_PTR,
            KQUADRANGLE_PTR,
            KRECTANGLE_PTR,
            KCOLOR_PTR,
            KVECTOR3D_PTR,
            KPLANE_PTR,
            KMATRIX2D_PTR,
            KMATRIX3D_PTR,
            KMATRIX4D_PTR,
            KTOPOLOGY2_PTR,
            KTOPOLOGY3_PTR,
            KTOPOLOGY4_PTR,
            KVOID_PTR,
            KKVARIANT_PTR,
            KINTERFACE_PTR
        };

        int type;

        union value_t
        {
            short vshort;
            int   vint;
            long  vlong;
            unsigned short vushort;
            unsigned int vuint;
            unsigned long vulong;
            float vfloat;
            double vdouble;
            long double vldouble;
            char vchar;
            unsigned char vuchar;
            point_t vpoint;
            line_t vline;
            triangle_t vtriangle;
            quadrangle_t vquadrangle;
            rectangle_t vrectangle;
            color_t vcolor;
            vector3d_t vvector3d;
            plane_t vplane;
            matrix2d_t vmatrix2d;
            matrix3d_t vmatrix3d;
            matrix4d_t vmatrix4d;
            topology2_t vtopology2;
            topology3_t vtopology3;
            topology4_t vtopology4;
            short *vshort_ptr;
            int   *vint_ptr;
            long  *vlong_ptr;
            unsigned short *vushort_ptr;
            unsigned int *vuint_ptr;
            unsigned long *vulong_ptr;
            float *vfloat_ptr;
            double *vdouble_ptr;
            long double *vldouble_ptr;
            char *vchar_ptr;
            unsigned char *vuchar_ptr;
            point_t *vpoint_ptr;
            line_t *vline_ptr;
            triangle_t *vtriangle_ptr;
            quadrangle_t *vquadrangle_ptr;
            rectangle_t *vrectangle_ptr;
            color_t* vcolor_ptr;
            vector3d_t* vvector3d_ptr;
            plane_t* vplane_ptr;
            matrix2d_t *vmatrix2d_ptr;
            matrix3d_t *vmatrix3d_ptr;
            matrix4d_t *vmatrix4d_ptr;
            topology2_t *vtopology2_ptr;
            topology3_t *vtopology3_ptr;
            topology4_t *vtopology4_ptr;
            void* vvoid_ptr;
            KVARIANT *vkvariant_ptr;
            interface_ptr vinterface_ptr;
        }value;

        unsigned int length;
    };
}

#endif

