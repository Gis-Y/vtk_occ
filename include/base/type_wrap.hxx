#ifndef __TYPE_WRAP_H_INCL__
#define __TYPE_WRAP_H_INCL__

// TODO: your code here

#include <assert.h>

#include <base/type.hxx>
#include <base/function.hxx>
#include <math.h>

#include <iomanip>
#include <iterator>
#include <vector>
using namespace std;

#define COMX_CLOSE_MATRIX_WRAP

namespace KMAS
{
  // TPoint is the cplusplus wrap class of point_t.
  class TPoint : public point_t
  {
  public:
    TPoint()
    {
      x = y = z = 0.0;
    }
    TPoint(double vx, double vy, double vz)
    {
      x = vx;
      y = vy;
      z = vz;
    }
    TPoint(point_t value)
    {
      x = value.x;
      y = value.y;
      z = value.z;
    }
  public:
    double getZ() const{return z;}
    void   setZ(double value){z = value;}
    double getY() const{return y;}
    void   setY(double value){y = value;}
    double getX() const{return x;}
    void   setX(double value){x = value;}
  public:
    template<typename istream_t>
    void Input(istream_t istr)
    {
      istr >> x >> y >> z;
    }
    template<typename ostream_t>
    void Output(ostream_t &ostr)
    {
      ostr << scientific << x << " " << y << " " << z << endl;
    }
  public:
    bool operator==(const TPoint &right)
    {
      return  fabs(getX() - right.getX()) < 1e-10 &&
        fabs(getY() - right.getY()) < 1e-10 &&
        fabs(getZ() - right.getZ()) < 1e-10;
    }
  };
  
  // TTriangle is the cplusplus wrap class of triangle_t.
  class TTriangle : public triangle_t
  {
  public:
    TTriangle()
    {
      pt1 = TPoint(0.0, 0.0, 0.0);
      pt2 = TPoint(1.0, 0.0, 0.0);
      pt3 = TPoint(0.0, 1.0, 0.0);
    }
    TTriangle(point_t vpt1, point_t vpt2, point_t vpt3)
    {
      pt1 = vpt1;
      pt2 = vpt2;
      pt3 = vpt3;
    }
    TTriangle(triangle_t value)
    {
      pt1 = value.pt1;
      pt2 = value.pt2;
      pt3 = value.pt3;
    }
  public:
    TPoint getPt1()const {return pt1;}
    void   setPt1(TPoint value){pt1 = value;}
    TPoint getPt2()const {return pt2;}
    void   setPt2(TPoint value){pt2 = value;}
    TPoint getPt3()const {return pt3;}
    void   setPt3(TPoint value){pt3 = value;}
  public:
    template<typename istream_t>
    void Input(istream_t istr)
    {
      TPoint vpt1 = getPt1();
      vpt1.Input(istr);
      setPt1(vpt1);
      
      TPoint vpt2 = getPt2();
      vpt2.Input(istr);
      setPt2(vpt2);
      
      TPoint vpt3 = getPt3();
      vpt3.Input(istr);
      setPt3(vpt3);
    }
    template<typename ostream_t>
    void Output(ostream_t &ostr)
    {
      getPt1().Output(ostr);
      getPt2().Output(ostr);
      getPt3().Output(ostr);
    }
  public:
    bool operator==(const TTriangle &right)
    {
      return (getPt1() == right.getPt1()) &&
        (getPt2() == right.getPt2()) &&
        (getPt3() == right.getPt3());
    }
  };
  
  // TQuadrangle is the cplusplus wrap class of quadrangle_t.
  class TQuadrangle : public quadrangle_t
  {
  public:
    TQuadrangle()
    {
      pt1 = TPoint(0.0, 0.0, 0.0);
      pt2 = TPoint(1.0, 0.0, 0.0);
      pt3 = TPoint(0.0, 1.0, 0.0);
      pt4 = TPoint(1.0, 1.0, 0.0);
    }
    TQuadrangle(point_t vpt1, point_t vpt2, point_t vpt3, point_t vpt4)
    {
      pt1 = vpt1;
      pt2 = vpt2;
      pt3 = vpt3;
      pt4 = vpt4;
    }
    TQuadrangle(quadrangle_t value)
    {
      pt1 = value.pt1;
      pt2 = value.pt2;
      pt3 = value.pt3;
      pt4 = value.pt4;
    }
  public:
    TPoint getPt1()const {return pt1;}
    void   setPt1(TPoint value){pt1 = value;}
    TPoint getPt2()const {return pt2;}
    void   setPt2(TPoint value){pt2 = value;}
    TPoint getPt3()const {return pt3;}
    void   setPt3(TPoint value){pt3 = value;}
    TPoint getPt4()const {return pt4;}
    void   setPt4(TPoint value){pt4 = value;}
  public:
    template<typename istream_t>
    void Input(istream_t istr)
    {
      TPoint vpt1 = getPt1();
      vpt1.Input(istr);
      setPt1(vpt1);

      TPoint vpt2 = getPt2();
      vpt2.Input(istr);
      setPt2(vpt2);

      TPoint vpt3 = getPt3();
      vpt3.Input(istr);
      setPt3(vpt3);

      TPoint vpt4 = getPt4();
      vpt4.Input(istr);
      setPt4(vpt4);
    }
    template<typename ostream_t>
    void Output(ostream_t &ostr)
    {
      getPt1().Output(ostr);
      getPt2().Output(ostr);
      getPt3().Output(ostr);
      getPt4().Output(ostr);
    }
  public:
    bool operator==(const TQuadrangle &right)
    {
      return (getPt1() == right.getPt1()) &&
        (getPt2() == right.getPt2()) &&
        (getPt3() == right.getPt3()) &&
        (getPt4() == right.getPt4());
    }
  };

  // TRectangle is the cplusplus wrap class of rectangle_t.
  class TRectangle : public rectangle_t
  {
  public:
    TRectangle()
    {
      left_top = TPoint(0.0, 0.0, 0.0);
      right_bottom = TPoint(1.0, 1.0, 0.0);
    }
    TRectangle(TPoint lt, TPoint rb)
    {
      left_top = lt;
      right_bottom = rb;
    }
    TRectangle(rectangle_t value)
    {
      left_top = value.left_top;
      right_bottom = value.right_bottom;
    }
  public:
    TPoint getLeftTop()const {return left_top;}
    void   setLeftTop(TPoint value){left_top = value;}
    TPoint getRightBottom()const {return right_bottom;}
    void   setRightBottom(TPoint value){right_bottom = value;}
  public:
    template<typename istream_t>
    void Input(istream_t istr)
    {
      TPoint lt = getLeftTop();
      lt.Input(istr);
      setLeftTop(lt);

      TPoint rb = getRightBottom();
      rb.Input(istr);
      setRightBottom(rb);
    }
    template<typename ostream_t>
    void Output(ostream_t &ostr)
    {
      getLeftTop().Output(ostr);
      getRightBottom().Output(ostr);
    }
  public:
    bool operator==(const TRectangle &right)
    {
      return getLeftTop() == right.getLeftTop() &&
        getRightBottom() == right.getRightBottom();
    }
  };

  // TLine is the cplusplus wrap class of line_t.
  class TLine : public line_t
  {
  public:
    TLine()
    {
      spt = TPoint(0.0, 0.0, 0.0);
      ept = TPoint(1.0, 1.0, 0.0);
    }
    TLine(TPoint vspt, TPoint vept)
    {
      spt = vspt;
      ept = vept;
    }
    TLine(line_t value)
    {
      spt = value.spt;
      ept = value.ept;
    }
  public:
    TPoint getStartPoint()const {return spt;}
    void   setStartPoint(TPoint value){spt = value;}
    TPoint getEndPoint()const {return ept;}
    void   setEndPoint(TPoint value){ept = value;}
  public:
    template<typename istream_t>
    void Input(istream_t istr)
    {
      TPoint vspt = getStartPoint();
      vspt.Input(istr);
      setStartPoint(vspt);

      TPoint vept = getEndPoint();
      vept.Input(istr);
      setEndPoint(vept);
    }
    template<typename ostream_t>
    void Output(ostream_t &ostr)
    {
      getStartPoint().Output(ostr);
      getEndPoint().Output(ostr);
    }
  public:
    bool operator==(const TLine &right)
    {
      return getStartPoint() == right.getStartPoint() &&
        getEndPoint() == right.getEndPoint();
    }
  };


  // TTopology2 is the cplusplus wrap class of topology2_t.
  class TTopology2 : public topology2_t
  {
  public:
    TTopology2()
    {
      n1 = 0;
      n2 = 1;
    }
    TTopology2(int vn1, int vn2)
    {
      n1 = vn1;
      n2 = vn2;
    }
    TTopology2(topology2_t value)
    {
      n1 = value.n1;
      n2 = value.n2;
    }
  public:
    int  getN1()const {return n1;}
    void setN1(int value){n1 = value;}
    int  getN2()const {return n2;}
    void setN2(int value){n2 = value;}
  public:
    template<typename istream_t>
    void Input(istream_t istr)
    {
      istr >> n1 >> n2;
    }
    template<typename ostream_t>
    void Output(ostream_t &ostr)
    {
      ostr.width(10);
      ostr << n1 << " ";
      ostr.width(10);
      ostr << n1 << " ";
      ostr.width(10);
      ostr << n2 << endl;
    }
  public:
    bool operator==(const TTopology2 &right)
    {
      return getN1() == right.getN1() &&
        getN2() == right.getN2();
    }
  };

  // TTopology3 is the cplusplus wrap class of topology3_t.
  class TTopology3 : public topology3_t
  {
  public:
    TTopology3()
    {
      n1 = 0;
      n2 = 1;
      n3 = 2;
    }
    TTopology3(int vn1, int vn2, int vn3)
    {
      n1 = vn1;
      n2 = vn2;
      n3 = vn3;
    }
    TTopology3(topology3_t value)
    {
      n1 = value.n1;
      n2 = value.n2;
      n3 = value.n3;
    }
  public:
    int  getN1()const {return n1;}
    void setN1(int value){n1 = value;}
    int  getN2()const {return n2;}
    void setN2(int value){n2 = value;}
    int  getN3()const {return n3;}
    void setN3(int value){n3 = value;}
  public:
    template<typename istream_t>
    void Input(istream_t istr)
    {
      istr >> n1 >> n2 >> n3;
    }
    template<typename ostream_t>
    void Output(ostream_t &ostr)
    {
      ostr.width(10);
      ostr << n1 << " ";
      ostr.width(10);
      ostr << n2 << " ";
      ostr.width(10);
      ostr << n3 << endl;
    }
  public:
    bool operator==(const TTopology3 &right)
    {
      return getN1() == right.getN1() &&
        getN2() == right.getN2() &&
        getN3() == right.getN3();
    }
  };

  // TTopology4 is the cplusplus wrap class of topology4_t.
  class TTopology4 : public topology4_t
  {
  public:
    TTopology4()
    {
      n1 = 0;
      n2 = 1;
      n3 = 2;
      n4 = 3;
    }
    TTopology4(int vn1, int vn2, int vn3, int vn4)
    {
      n1 = vn1;
      n2 = vn2;
      n3 = vn3;
      n4 = vn4;
    }
    TTopology4(topology4_t value)
    {
      n1 = value.n1;
      n2 = value.n2;
      n3 = value.n3;
      n4 = value.n4;
    }
  public:
    int  getN1()const {return n1;}
    void setN1(int value){n1 = value;}
    int  getN2()const {return n2;}
    void setN2(int value){n2 = value;}
    int  getN3()const {return n3;}
    void setN3(int value){n3 = value;}
    int  getN4()const {return n4;}
    void setN4(int value){n4 = value;}
  public:
    template<typename istream_t>
    void Input(istream_t istr)
    {
      istr >> n1 >> n2 >> n3 >> n4;
    }
    template<typename ostream_t>
    void Output(ostream_t &ostr)
    {
      ostr.width(10);
      ostr << n1 << " ";
      ostr.width(10);
      ostr << n2 << " ";
      ostr.width(10);
      ostr << n3 << " ";
      ostr.width(10);
      ostr << n4 << endl;
    }
  public:
    bool operator==(const TTopology4 &right)
    {
      return getN1() == right.getN1() &&
        getN2() == right.getN2() &&
        getN3() == right.getN3() &&
        getN4() == right.getN4();
    }
  };

  // TColor is the cplusplus wrap class of color_t.
  class TColor : public color_t
  {
#define RED_IS_NOD_VALID (red < 0.0 || red > 1.0)
#define GREEN_IS_NOT_VALID (green < 0.0 || green > 1.0)
#define BLUE_IS_NOT_VALID (blue < 0.0 || blue > 1.0)
#define ALPHA_IS_NOT_VALID (alpha < 0.0 || blue > 1.0)

    void _checkValid()
    {
      assert(!RED_IS_NOD_VALID);
      assert(!GREEN_IS_NOT_VALID);
      assert(!BLUE_IS_NOT_VALID);
      assert(!ALPHA_IS_NOT_VALID);
    }

#undef RED_IS_NOD_VALID
#undef GREEN_IS_NOT_VALID
#undef BLUE_IS_NOT_VALID
#undef ALPHA_IS_NOT_VALID
  public:
    TColor()
    {
      red = green = blue = 0.0;
      alpha = 1.0;
    }
    TColor(double vred,
           double vgreen, 
           double vblue, 
           double valhpa = 1.0)
    {
      red = vred;
      green = vgreen;
      blue = vblue;
      alpha = valhpa;

      _checkValid();
    }
    TColor(color_t value)
    {
      red = value.red;
      green = value.green;
      blue = value.blue;
      alpha = value.alpha;

      _checkValid();
    }
  public:
    double getRed() const{return red;}
    void   setRed(double value){red = value;_checkValid();}
    double getGreen() const{return green;}
    void   setGreen(double value){green = value;_checkValid();}
    double getBlue() const{return blue;}
    void   setBlue(double value){blue = value;_checkValid();}
    double getAlpha() const{return alpha;}
    void   setAlpha(double value){alpha = value;_checkValid();}
  public:
    template<typename istream_t>
    void Input(istream_t istr)
    {
      istr >> red >> green >> blue >> alpha;
      _checkValid();
    }
    template<typename ostream_t>
    void Output(ostream_t &ostr)
    {
      ostr << scientific
           << red << " "
           << green << " "
           << blue << " "
           << alpha
           << endl;
    }
  public:
    bool operator==(const TColor &right)
    {
      return fabs(getRed() - right.getRed()) < 1e-10 &&
        fabs(getGreen() - right.getGreen()) < 1e-10 &&
        fabs(getBlue() - right.getBlue()) < 1e-10 &&
        fabs(getAlpha() - right.getAlpha()) < 1e-10;
    }
  };

  // TVector3d is the cplusplus wrap class of vector3d_t.
  class TVector3d : public vector3d_t
  {
  public:
    TVector3d()
    {
      x = y = z = 0.0;
    }
    TVector3d(double vx, double vy, double vz)
    {
      x = vx;
      y = vy;
      z = vz;
    }
    TVector3d(vector3d_t value)
    {
      x = value.x;
      y = value.y;
      z = value.z;
    }
  public:
    double getZ() const{return z;}
    void   setZ(double value){z = value;}
    double getY() const{return y;}
    void   setY(double value){y = value;}
    double getX() const{return x;}
    void   setX(double value){x = value;}
  public:
    template<typename istream_t>
    void Input(istream_t istr)
    {
      istr >> x >> y >> z;
    }
    template<typename ostream_t>
    void Output(ostream_t &ostr)
    {
      ostr << scientific << x << " " << y << " " << z << endl;
    }
  public:
    bool operator==(const TVector3d &right)
    {
      return  fabs(getX() - right.getX()) < 1e-10 &&
        fabs(getY() - right.getY()) < 1e-10 &&
        fabs(getZ() - right.getZ()) < 1e-10;
    }
  };

  // TPlane is the cplusplus wrap class of plane_t.
  class TPlane : public plane_t
  {
  public:
    TPlane()
    {
      pt = TPoint();
      direction = TVector3d(0.0, 0.0, 1.0);
    }
    TPlane(TPoint vpt, TVector3d vdirection)
    {
      pt = vpt;
      direction = func::normalize_vec(vdirection);
    }
    TPlane(plane_t value)
    {
      pt = value.pt;
      direction = value.direction;
    }
  public:
    TPoint getPoint() const{return pt;}
    void   setPoint(TPoint value){pt = value;}
    TVector3d getDirection() const{return direction;}
    void   setDirection(TVector3d value){direction = value;}
  public:
    template<typename istream_t>
    void Input(istream_t istr)
    {
      TPoint vpt = getPoint();
      vpt.Input(istr);
      setPoint(vpt);

      TVector3d vdirection = getDirection();
      vdirection.Input(istr);
      setDirection(vdirection);
    }
    template<typename ostream_t>
    void Output(ostream_t &ostr)
    {
      getPoint().Output(ostr);
      getDirection().Output(ostr);
    }
  public:
    bool operator==(const TPlane &right)
    {
      return getPoint() == right.getPoint() &&
        getDirection() == right.getDirection();
    }
  };

  // the namespace matrix_wrap shouldn't be used by the following codes:
  //  using namespace matrix_wrap; or using namespace KMAS::func::matrix_wrap;
  // 
#ifndef	COMX_CLOSE_MATRIX_WRAP
  namespace matrix_wrap
  {
    template<int DIM>class __matrix_inner_t;

    template<typename T>
    double determinant_matrix(T t);

    template<>
    double determinant_matrix<__matrix_inner_t<2> >(__matrix_inner_t<2> t);
        
    template<typename T>
    T inverse_matrix(T t);

    template<>
    __matrix_inner_t<2> inverse_matrix<__matrix_inner_t<2> >(__matrix_inner_t<2> t);
        
    template<int DIM>
    class __matrix_inner_t
    {
    public:
      double data[DIM][DIM];
      static const int DIM_CONST = DIM;
    public:
      __matrix_inner_t(double value[DIM][DIM])
      {
        memcpy(data, value, sizeof(double) * DIM * DIM);
      }
    public:
      __matrix_inner_t<DIM - 1> GetSubMatrix(const int i, const int j)
      {
        return _getSubMatrix(i, j);
      }
      double Determinant()
      {
        return determinant_matrix(*this);
      }
      __matrix_inner_t Inverse()
      {
        return inverse_matrix(*this);
      }
    private:
      __matrix_inner_t<DIM - 1> _getSubMatrix(const int i, const int j)
      {
        double value[DIM - 1][DIM - 1];
                
        for (int l = 0; l < DIM; ++l)
          for (int m = 0; m < DIM; ++m)
            {
              if (l == i || m == j) continue;
                        
              int k = (l > i) ? l - 1 : l;
              int n = (m > j) ? m - 1 : m;
                        
              value[k][n] = data[l][m];
            }
                    
        return __matrix_inner_t<DIM - 1>(value);
      }
    };

    template<typename T>
    double determinant_matrix(T t)
    {
      double ret = 0.0;
        
      for (int j = 0; j < T::DIM_CONST; ++j)
        {
          double s = (j % 2 == 0) ? 1.0 : -1.0;
          ret += determinant_matrix(t.GetSubMatrix(0, j)) * t.data[0][j] * s;
        }
        
      return ret;
    }
        
    template<>
    double determinant_matrix<__matrix_inner_t<2> >(__matrix_inner_t<2> t)
    {
      return t.data[0][0] * t.data[1][1] - t.data[0][1] * t.data[1][0];
    }
        
    template<typename T>
    T inverse_matrix(T t)
    {
      double d = t.Determinant();
      assert(fabs(d) > 1e-10);
        
      double vdata[T::DIM_CONST][T::DIM_CONST];
      for (int i = 0; i < T::DIM_CONST; ++i)
        {
          for (int j = 0; j < T::DIM_CONST; ++j)
            {
              double s = ((i + j) % 2 == 0) ? 1.0 : -1.0;
              vdata[j][i] = t.GetSubMatrix(i, j).Determinant() * s / d; 
            }
        }
        
      return T(vdata);
    }
        
    template<>
    __matrix_inner_t<2> inverse_matrix<__matrix_inner_t<2> >(__matrix_inner_t<2> t)
    {
      double vdata[2][2];
      double d = t.Determinant();

      assert(fabs(d) > 1e-10);
            
      vdata[0][0] =  t.data[1][1] / d;
      vdata[0][1] = -t.data[0][1] / d;
      vdata[1][0] = -t.data[1][0] / d;
      vdata[1][1] =  t.data[0][0] / d;
            
      return __matrix_inner_t<2>(vdata);
    }

    template<typename T, int DIM>
    class TMatrixImpl : public T
    {
    private:
      void _setIdentity()
      {
        for(int i = 0; i < DIM; ++i)
          for(int j = 0; j < DIM; ++j)
            T::data[i][j] = (i == j) ? 1.0 : 0.0;
      }
      void _setValue(double value[DIM][DIM])
      {
        for(int i = 0; i < DIM; ++i)
          for(int j = 0; j < DIM; ++j)
            T::data[i][j] = value[i][j];
      }
    public:
      TMatrixImpl()
      {
        _setIdentity();
      }
      TMatrixImpl(double value[DIM][DIM])
      {
        _setValue(value);
      }
      TMatrixImpl(T value)
      {
        _setValue(value.data);
      }
    public:
      double getValue(const int i, const int j) const
      {
        assert(i >= 0 && i < DIM); 
        assert(j >= 0 && j < DIM);

        return T::data[i][j];
      }
      void   setValue(const int i, const int j, const double value)
      {
        assert(i >= 0 && i < DIM); 
        assert(j >= 0 && j < DIM);

        T::data[i][j] = value;
      }
    public:
      template<typename istream_t>
      void Input(istream_t istr)
      {
        for(int i = 0; i < DIM; ++i)
          for(int j = 0; j < DIM; ++j)
            istr >> T::data[i][j];
      }
      template<typename ostream_t>
      void Output(ostream_t &ostr)
      {
        for(int i = 0; i < DIM; ++i)
          {
            for(int j = 0; j < DIM; ++j)
              {
                ostr << scientific << T::data[i][j];
                if (j != (DIM - 1))
                  {
                    ostr << " ";
                  }
              }
            ostr << endl;
          }
      }
    public:
      TMatrixImpl Transpose()
      {
        TMatrixImpl ret;

        for(int i = 0; i < DIM; ++i)
          for(int j = 0; j < DIM; ++j)
            ret.data[i][j] = T::data[j][i];

        return ret;
      }
      TMatrixImpl Inverse()
      {
        return TMatrixImpl(__matrix_inner_t<DIM>(T::data).Inverse().data);
      }
      void LoadIdentity()
      {
        _setIdentity();
      }
      double Determinant()
      {
        return __matrix_inner_t<DIM>(T::data).Determinant();
      }
    public:
      TMatrixImpl operator*(const TMatrixImpl &right)
      {
        double value[DIM][DIM];
        memset(value, 0, sizeof(double) * DIM * DIM);

        for (int i = 0; i < DIM; ++i)
          for(int j = 0; j < DIM; ++j)
            for (int k = 0; k < DIM; ++k)
              value[i][j] += T::data[i][k] * right.data[k][j];

        return TMatrixImpl(value);
      }

      TMatrixImpl operator+(const TMatrixImpl &right)
      {
        double value[DIM][DIM];

        for (int i = 0; i < DIM; ++i)
          for(int j = 0; j < DIM; ++j)
            value[i][j] = T::data[i][j] + right.data[i][j];

        return TMatrixImpl(value);
      }

      TMatrixImpl operator-(const TMatrixImpl &right)
      {
        double value[DIM][DIM];

        for (int i = 0; i < DIM; ++i)
          for(int j = 0; j < DIM; ++j)
            value[i][j] = T::data[i][j] - right.data[i][j];

        return TMatrixImpl(value);
      }

      bool operator==(const TMatrixImpl &right)
      {
        for (int i = 0; i < DIM; ++i)
          for(int j = 0; j < DIM; ++j)
            if (fabs(T::data[i][j] - right.data[i][j]) >= 1e-10)
              return false;

        return true;
      }

      bool IsIdentity()
      {
        return *this == TMatrixImpl();
      }

      bool IsInversable()
      {
        return fabs(Determinant()) > 1e-10;
      }

      bool IsSymmetry()
      {
        return *this == Transpose();
      }
    };
  }// end namespace matrix.

  typedef matrix_wrap::TMatrixImpl<matrix2d_t, 2> TMatrix2d;
  typedef matrix_wrap::TMatrixImpl<matrix3d_t, 3> TMatrix3d;
  typedef matrix_wrap::TMatrixImpl<matrix4d_t, 4> TMatrix4d;
#endif

  /*
   * The following statement shouldn't be written:
   *  using namespace KMAS::variant_wrap 
   * or
   *  using namespace variant_wrap.
   */
  namespace variant_wrap
  {
#define DEFINE_TYPE(t, id)                      \
    TVariant(t v##t, uint vlen = 1)             \
    {                                           \
      type = id;                                \
      value.v##t = v##t;                        \
      if (type == KCHAR_PTR) {                  \
        length = static_cast<int>(strlen(value.vchar_ptr));       \
      }                                         \
      else                                      \
        length = vlen;                          \
    }                                           \
    operator t()                                \
    {                                           \
      assert(type == id);                       \
      return value.v##t;                        \
    }

#define BEGIN_DEFINE_GET_TYPE_NAME_FUNC()       \
    const char *GetTypeName() const             \
    {                                           \
    switch(type)                                \
      {

#define DEFINE_GET_TYPE_NAME_FUNC_ITEM(type, name)      \
    case type:                                          \
    return name;

#define END_DEFINE_GET_TYPE_NAME_FUNC()         \
    default:                                    \
    assert(THESE_CODES_SHOULD_NOT_BE_EXECUTED); \
    return "null type";                         \
  }                                             \
  }

    typedef unsigned short ushort;
    typedef unsigned int uint;
    typedef unsigned long ulong;
    typedef unsigned char uchar;
    typedef long double ldouble;
    typedef point_t point;
    typedef line_t line;
    typedef triangle_t triangle;
    typedef quadrangle_t quadrangle;
    typedef rectangle_t rectangle;
    typedef topology2_t topology2;
    typedef topology3_t topology3;
    typedef topology4_t topology4;
    typedef color_t color;
    typedef vector3d_t vector3d;
    typedef plane_t plane;
    typedef matrix2d_t matrix2d;
    typedef matrix3d_t matrix3d;
    typedef matrix4d_t matrix4d;
    
    typedef short* short_ptr;
    typedef int* int_ptr;
    typedef long* long_ptr;
    typedef ushort* ushort_ptr;
    typedef uint* uint_ptr;
    typedef ulong* ulong_ptr;
    typedef float* float_ptr;
    typedef double* double_ptr;
    typedef ldouble *ldouble_ptr;
    typedef char* char_ptr;
    typedef uchar* uchar_ptr;
    typedef point* point_ptr;
    typedef line* line_ptr;
    typedef triangle* triangle_ptr;
    typedef quadrangle* quadrangle_ptr;
    typedef rectangle* rectangle_ptr;
    typedef topology2* topology2_ptr;
    typedef topology3* topology3_ptr;
    typedef topology4* topology4_ptr;
    typedef color* color_ptr;
    typedef vector3d* vector3d_ptr;
    typedef plane* plane_ptr;
    typedef matrix2d* matrix2d_ptr;
    typedef matrix3d* matrix3d_ptr;
    typedef matrix4d* matrix4d_ptr;
        
    typedef void* void_ptr;
    typedef KVARIANT *kvariant_ptr;

    class TVariant : public KVARIANT
    {
      const static bool THESE_CODES_SHOULD_NOT_BE_EXECUTED = false;
    public:
      TVariant(KVARIANT t)
      {
        type = t.type;
        value = t.value;
        length = t.length;
      }
    public:
      operator bool()
      {
        assert(type == KINT);
        return value.vint != 0;
      }
    public:
      DEFINE_TYPE(short, KSHORT)
      DEFINE_TYPE(int, KINT)
      DEFINE_TYPE(long, KLONG)
      DEFINE_TYPE(ushort, KUSHORT)
      DEFINE_TYPE(uint, KUINT)
      DEFINE_TYPE(ulong, KULONG)
      DEFINE_TYPE(float, KFLOAT)
      DEFINE_TYPE(double, KDOUBLE)
      DEFINE_TYPE(ldouble, KLDOUBLE)
      DEFINE_TYPE(char, KCHAR)
      DEFINE_TYPE(uchar, KUCHAR)
      DEFINE_TYPE(point, KPOINT)
      DEFINE_TYPE(line, KLINE)
      DEFINE_TYPE(triangle, KTRIANGLE)
      DEFINE_TYPE(quadrangle, KQUADRANGLE)
      DEFINE_TYPE(rectangle, KRECTANGLE)
      DEFINE_TYPE(color, KCOLOR)
      DEFINE_TYPE(vector3d, KVECTOR3D)
      DEFINE_TYPE(plane, KPLANE)
      DEFINE_TYPE(matrix2d, KMATRIX2D)
      DEFINE_TYPE(matrix3d, KMATRIX3D)
      DEFINE_TYPE(matrix4d, KMATRIX4D)
      DEFINE_TYPE(topology2, KTOPOLOGY2)
      DEFINE_TYPE(topology3, KTOPOLOGY3)
      DEFINE_TYPE(topology4, KTOPOLOGY4)
      DEFINE_TYPE(short_ptr, KSHORT_PTR)
      DEFINE_TYPE(int_ptr, KINT_PTR)
      DEFINE_TYPE(long_ptr, KLONG_PTR)
      DEFINE_TYPE(ushort_ptr, KUSHORT_PTR)
      DEFINE_TYPE(uint_ptr, KUINT_PTR)
      DEFINE_TYPE(ulong_ptr, KULONG_PTR)
      DEFINE_TYPE(float_ptr, KFLOAT_PTR)
      DEFINE_TYPE(double_ptr, KDOUBLE_PTR)
      DEFINE_TYPE(ldouble_ptr, KLDOUBLE_PTR)
      DEFINE_TYPE(char_ptr, KCHAR_PTR)
      DEFINE_TYPE(uchar_ptr, KUCHAR_PTR)
      DEFINE_TYPE(point_ptr, KPOINT_PTR)
      DEFINE_TYPE(line_ptr, KLINE_PTR)
      DEFINE_TYPE(triangle_ptr, KTRIANGLE_PTR)
      DEFINE_TYPE(quadrangle_ptr, KQUADRANGLE_PTR)
      DEFINE_TYPE(rectangle_ptr, KRECTANGLE_PTR)
      DEFINE_TYPE(color_ptr, KCOLOR_PTR)
      DEFINE_TYPE(vector3d_ptr, KVECTOR3D_PTR)
      DEFINE_TYPE(plane_ptr, KPLANE_PTR)
      DEFINE_TYPE(matrix2d_ptr, KMATRIX2D_PTR)
      DEFINE_TYPE(matrix3d_ptr, KMATRIX3D_PTR)
      DEFINE_TYPE(matrix4d_ptr, KMATRIX4D_PTR)
      DEFINE_TYPE(topology2_ptr, KTOPOLOGY2_PTR)
      DEFINE_TYPE(topology3_ptr, KTOPOLOGY3_PTR)
      DEFINE_TYPE(topology4_ptr, KTOPOLOGY4_PTR)
      DEFINE_TYPE(kvariant_ptr, KKVARIANT_PTR)
      DEFINE_TYPE(interface_ptr, KINTERFACE_PTR)
      DEFINE_TYPE(void_ptr, KVOID_PTR)
    public:
      BEGIN_DEFINE_GET_TYPE_NAME_FUNC()
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KUNKNOWN, "Unknown")
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KSHORT, "short")
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KINT, "int")
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KLONG, "long")
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KUSHORT, "unsigned short")
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KUINT, "unsigned int")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KULONG, "unsigned long")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KFLOAT, "float")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KDOUBLE, "double")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KLDOUBLE, "long double")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KCHAR, "char")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KUCHAR, "unsigned char")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KPOINT, "point")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KLINE, "line")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KTRIANGLE, "triangle")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KQUADRANGLE, "quadangle")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KRECTANGLE, "rectangle")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KCOLOR, "color")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KVECTOR3D, "vector3d")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KPLANE, "plane")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KMATRIX2D, "matrix2d")    
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KMATRIX3D, "matrix3d")
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KMATRIX4D, "matrix4d")    
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KTOPOLOGY2, "topology2")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KTOPOLOGY3, "topology3")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KTOPOLOGY4, "topology4")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KSHORT_PTR, "short pointer")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KINT_PTR, "int pointer")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KLONG_PTR, "long pointer")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KUSHORT_PTR, "unsigned short pointer")  
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KUINT_PTR, "unsigned int pointer")
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KULONG_PTR, "unsigned long pointer")
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KFLOAT_PTR, "float pointer")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KDOUBLE_PTR, "double pointer")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KLDOUBLE_PTR, "long double pointer")
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KCHAR_PTR, "char pointer")
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KUCHAR_PTR, "unsigned char pointer")
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KPOINT_PTR, "point pointer")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KLINE_PTR, "line pointer")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KTRIANGLE_PTR, "triangle pointer")  
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KQUADRANGLE_PTR, "quadrangle pointer")
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KRECTANGLE_PTR, "rectangle pointer")  
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KCOLOR_PTR, "color pointer")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KVECTOR3D_PTR, "vector3d pointer")    
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KPLANE_PTR, "plane pointer")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KMATRIX2D_PTR, "matrix2d pointer")    
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KMATRIX3D_PTR, "matrix3d pointer")
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KMATRIX4D_PTR, "matrix4d pointer")
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KTOPOLOGY2_PTR, "topology2 pointer")  
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KTOPOLOGY3_PTR, "topology3 pointer")  
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KTOPOLOGY4_PTR, "topology4 pointer")  
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KVOID_PTR, "void pointer")        
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KKVARIANT_PTR, "KVARIANT pointer")    
      DEFINE_GET_TYPE_NAME_FUNC_ITEM(KINTERFACE_PTR, "interface pointer")  
      END_DEFINE_GET_TYPE_NAME_FUNC()
    public:
      template<typename T>
      bool IsTypeOf(){return type == TVariant(T()).type;}
    public:
      bool IsValid(){return type != KUNKNOWN;}
      unsigned int GetLength(){return length;}
      bool IsValue(){return type <= KTOPOLOGY4;}
      bool IsPointer(){return !IsValue();}
      bool IsVoidPointer(){return (type == KVOID_PTR);}
      bool IsBaseValue(){ return type >= KSHORT && type <= KUCHAR;}
      bool IsCompositeValue(){ return IsValue() && !IsBaseValue();}
      bool IsBasePointer(){ return type >= KSHORT_PTR && type <= KUCHAR_PTR;}
      bool IsCompositePointer(){ return IsPointer() && !IsBasePointer() &&
          !IsVoidPointer() && !IsVariantPointer() && !IsInterfacePointer();} 
      bool IsVariantPointer(){ return type == KKVARIANT_PTR;}
      bool IsInterfacePointer(){ return type == KINTERFACE_PTR;}
    };
  }

  using variant_wrap::TVariant;

  class TParameter
  {
    vector<TVariant> _paras;
  public:
    TParameter()
    {
      _paras.push_back(-1);
    }

    operator KVARIANT*()
    {
      if (!_isLock()) _close();
      return &_paras[1];
    }
  private:
    bool _isLock()
    {
      int iGuard = _paras[0];
      return (iGuard != -1);
    }
    void _open()
    {
      _paras[0].value.vint = -1;
    }
    void _close()
    {
      _paras[0].value.vint = static_cast<int>(_paras.size() - 1);
    }
    void _reset()
    {
      _paras.clear();
      _paras.push_back(-1);
    }
  public:
    void Reset(){_reset();}
    void Append(TVariant var)
    {
      if (_isLock()) _open();
      _paras.push_back(var);
    }
    int Length()
    {
      if (!_isLock()) _close();
      return static_cast<int>(_paras.size() - 1);
    }
  };

  class TParameterParser
  {
    KVARIANT *pparas;
  public:
    TParameterParser(KVARIANT* pvalue)
    {
      pparas = pvalue;
    }
  public:
    int Size()
    {
      return _getLength();
    }
    TVariant operator[](const int index)
    {
      assert(index >= 0 && index < _getLength());

      return pparas[index];
    }
  private:
    int _getLength()
    {
      return (pparas - 1)->value.vint;
    }
  };

  class TStringArrayParser
  {
    const char * const &m_str_arr;
    vector<int> m_base_address;
  public:
    TStringArrayParser(const char * const &str_arr, const int len) : m_str_arr(str_arr)
    {
      m_base_address.resize(len);
      m_base_address[0] = 0;
    
      for(int loop = 1; loop < len; ++loop)
        {
          m_base_address[loop] = m_base_address[loop - 1] + 
            static_cast<int>(strlen(m_str_arr + m_base_address[loop - 1])) + 1;
        }
    }
  public:
    const char * operator[](const int index)
    {
      assert(index >= 0 && index < static_cast<int>(m_base_address.size()));
      return m_str_arr + m_base_address[index];
    }
    int Size()
    {
      return static_cast<int>(m_base_address.size());
    }
  };

  class TStringArray
  {
    vector<char> m_buffer;
    int m_size;
  public:
    TStringArray() : m_size(0){}
  public:
    void Insert(const char * const str)
    {
      m_buffer.reserve(m_buffer.size() + strlen(str));
      copy(str, str + strlen(str) + 1,back_inserter(m_buffer));
      ++m_size;
    }
    int Size()
    {
      return m_size;
    }
  public:
    operator char*()
    {
      return &m_buffer[0];
    }
  };
}

#endif // __TYPE_WRAP_H_INCL__

