#ifndef _KMAS_COMX_GL_PLOT_UTIL_HXX
#define _KMAS_COMX_GL_PLOT_UTIL_HXX

#include <gl/gl_window.hxx>
#include <gl/gl_base.hxx>

using namespace KMAS::Die_maker::comx;

#include <map>
#include <math.h>
using namespace std;

namespace plot{

	namespace common{
		class TRgb{
			double _red , _green , _blue;
		public:
			TRgb(const double &red = 0.,const double &green = 0.,const double &blue = 0.):
			  _red(red),_green(green),_blue(blue){}
			  virtual ~TRgb(){}
		public:
			const double &Red()const{return _red;}
			const double &Green()const{return _green;}
			const double &Blue()const{return _blue;}
		public:
			void Green(const double &green){_green = green;}
			void Red(const double &red){_red = red;}
			void Blue(const double &blue){_blue = blue;}
		};

		class TLineStyle{
			GLint _factor;
			GLushort _pattern;
		public:
			TLineStyle(const GLint &factor = 2,const GLushort &pattern = 0xffff):_factor(factor),_pattern(pattern){}
			virtual ~TLineStyle(){}
		public:
			const GLint &Factor()const{return _factor;}
			const GLushort &Pattern()const{return _pattern;}
		public:
			void Factor(const GLint &factor){_factor = factor;}
			void Pattern(const GLushort &pattern){_pattern = pattern;}
		};

		class TMargin{
			double _left,_right,_top,_bottom;
		public:
			TMargin(const double &left = 0.,const double &right = 0.,const double &top = 0.,const double &bottom =0.)
				:_left(left),_right(right),_top(top),_bottom(bottom){}
				virtual ~TMargin(){}
		public:
			const double &Left()const{return _left;}
			const double &Right()const{return _right;}
			const double &Top()const{return _top;}
			const double &Bottom()const{return _bottom;}
		public:
			void Left(const double &left){_left = left;}
			void Right(const double &right){_right = right;}
			void Top(const double &top){_top = top;}
			void Bottom(const double &bottom){_bottom = bottom;}
		};

		typedef string TTitle;

		class TPoint2D{
			double _cx,_cy;
		public:
			TPoint2D(const double &cx = 0.,const double &cy = 0.):_cx(cx),_cy(cy){}
			virtual ~TPoint2D(){}
		public:
			void X(const double &cx){_cx = cx;}
			const double &X()const{return _cx;}
			void Y(const double &cy){_cy = cy;}
			const double &Y()const{return _cy;}
		};

		typedef TPoint2D TVector2D;

	}//namespace common
	
	namespace shape
	{
		using namespace plot::common;

		typedef unsigned                 TGLFlag;
		typedef TGLPoint2D<double>       TGLPoint;
		typedef TMouseEvent::TAction	 TGLAction;

		class TPlotShap2D{
			bool _interactive_is_valid;
		public:
			TPlotShap2D(IGlWindowTransform *&ptr_tool):_interactive_is_valid(false),_is_lock(false),_is_press(false),_activate_state(-1),_ptr_tool(ptr_tool){}
			virtual ~TPlotShap2D(){}
		public:
			void EnableInteractive(const bool &flag = true){_interactive_is_valid = flag;}
		protected:
			virtual void paint() = 0;
			virtual void paint_interactive() = 0;
			virtual void mouse(TPoint2D point) = 0;
		public:
			inline void paintEvent();
			inline void mouseEvent(TGLFlag nFlags,TGLPoint point,TGLAction action);
		private:
			bool _is_lock;
			bool _is_press;
			int	 _activate_state;
			map<int,TPoint2D*> _map_activate_state;
			IGlWindowTransform *&_ptr_tool;
		private:
			inline void Lock(){_is_lock = true;}
			inline void UnLock(){_is_lock = false;}
			inline bool IsLock(){return _is_lock;}
			inline void RecordAction(TGLAction action);
			inline bool IsInActiveRange(TPoint2D pt1,TPoint2D pt2,const double &range);
			inline void ComputeActivateState(const TPoint2D &pt); 
		protected:
			inline bool IsPress(){return _is_press;}
			inline void RegisterActivateState(const int &key,TPoint2D &pt);
			inline int	GetActivateState(){return _activate_state;}
		};

		void TPlotShap2D::RegisterActivateState(const int &key,TPoint2D &pt){
			_map_activate_state[key] = &pt;			
		}

		void TPlotShap2D::ComputeActivateState(const TPoint2D &pt){
			if (IsLock()) {
				return;
			}
			map<int,TPoint2D*>::iterator iter;
			for(iter = _map_activate_state.begin() ; iter != _map_activate_state.end() ; iter++){
				int key = (*iter).first;
				TPoint2D pt1 = *(*iter).second;
				TPoint2D pt2 = pt;

				double cx = 0, cy = 0;
				
				//_ptr_tool->Transform(cx,cy);
				_ptr_tool->Transform(LCS, GCS, pt1.X(), pt1.Y(), cx, cy);
				pt1.X(cx),pt1.Y(cy);

				cx = 0.0, cy = 0.0;
				//_ptr_tool->WndToGl(cx,cy);
				_ptr_tool->Transform(WCS1, GCS, pt2.X(), pt2.Y(), cx, cy);
				pt2.X(cx),pt2.Y(cy);

				if (IsInActiveRange(pt1,pt2,3.)) {
					_activate_state = key;
					//cx = pt1.X(),cy = pt1.Y();
					//_ptr_tool->GlToWnd(cx,cy);
					//_ptr_tool->SetCursorPos(cx,cy);
					return;
				}
			}
			_activate_state = -1;
		}

		void TPlotShap2D::RecordAction(TGLAction action){
			switch(action) {
				case TGLAction::LPRESS:
					_is_press = true;
					Lock();
					break;
				case TGLAction::LRELEASE:
					_is_press = false;
					UnLock();
					break;
			}
		}

		bool TPlotShap2D::IsInActiveRange(TPoint2D pt1,TPoint2D pt2,const double &range){
			return (fabs(pt1.X()-pt2.X()) < range && fabs(pt1.Y()-pt2.Y()) < range);
		}

		void TPlotShap2D::paintEvent(){
			assert(_ptr_tool);
			if (_interactive_is_valid) {
				paint_interactive();
			}
			paint();
		}

		void TPlotShap2D::mouseEvent(TGLFlag nFlags,TGLPoint point,TGLAction action){
			assert(_ptr_tool);

			TPoint2D pt1(point.x,point.y);
			ComputeActivateState(pt1);
			RecordAction(action);

			if (_interactive_is_valid) {

				if (!(IsPress() && action == TGLAction::MOTION))return;

				double cx = 0,cy = 0;
				_ptr_tool->Transform(WCS1, LCS, point.x, point.y, cx, cy);
				//_ptr_tool->WndToGl(cx,cy);
				//_ptr_tool->TransformInverse(cx,cy);

				mouse(TPoint2D(cx,cy));
			}
		}

		class TPlotLine2DAppearance{
			TRgb _rgb_base,_rgb_highlight,_rgb_ctl_pt,_rgb_centriod;
			GLdouble _line_width,_ctl_pt_size;
			TLineStyle _line_sytle;
			TLineStyle _ctl_line_style;
			bool _is_smooth;
			bool _is_default;
		private:
			TRgb _rgb_default;
			GLdouble _line_width_default,_ctl_pt_size_default;
			GLint factor;GLint pattern;
		protected:
			TPlotLine2DAppearance():
				 _rgb_base(.5,.0,.0),_rgb_highlight(1.0,.0,.0),
					 _rgb_ctl_pt(.0,.0,.85),_rgb_centriod(.0,.6,.0),
					 _line_width(1.0),_ctl_pt_size(6.0),
					 _line_sytle(2,0xffff),
					 _ctl_line_style(2,0xfaff),
					 _is_smooth(true),
					 _is_default(true){}
				 virtual ~TPlotLine2DAppearance(){}
		public:
			inline void SetColor(const TRgb &color){_rgb_base = color;}
			inline void SetCtlPointColor(const TRgb &color){_rgb_ctl_pt = color;}
			inline void SetHighLightColor(const TRgb &color){_rgb_highlight = color;}
			inline void SetCentriodColor(const TRgb &color){_rgb_centriod = color;}
			inline void SetWidth(const double &width){_line_width = width;}
			inline void SetCtlPointSize(const double &size){_ctl_pt_size = size;}
			inline void SetStyle(const TLineStyle &style){_line_sytle = style;}
			inline void SetCtlStyle(const TLineStyle &style){_ctl_line_style = style;}
			inline void SetSmooth(const bool &flag){_is_smooth = flag;}
		protected:
			enum EStatus{
				COLOR			 = 0x0001,
				CTL_PT_COLOR	 = 0x0002,
				HIGH_LIGHT_COLOR = 0x0004,
				CENTRIOD_COLOR   = 0x0008,
				WIDTH			 = 0x0010,
				CTL_PT_SIZE		 = 0x0020,
				STYLE			 = 0x0040,
				CTL_STYLE		 = 0x0080,
				SMOOTH			 = 0x0100,
				DEFAULT			 = 0x0200
			};
			inline void SwitchTo(int status);
		private:
			inline void SaveState();
			inline void RestoreState();
		};

		void TPlotLine2DAppearance::SaveState(){
			if (!_is_default) return;

			_is_default = false;

			GLdouble color[4];
			glGetDoublev(GL_CURRENT_COLOR,color);
			_rgb_default.Red(color[0]);
			_rgb_default.Green(color[1]);
			_rgb_default.Blue(color[2]);
			glGetDoublev(GL_LINE_WIDTH,&_line_width_default);
			glGetDoublev(GL_POINT_SIZE,&_ctl_pt_size_default);

			glGetIntegerv(GL_LINE_STIPPLE_REPEAT,&factor);
			glGetIntegerv(GL_LINE_STIPPLE_PATTERN,&pattern);
		}
		void TPlotLine2DAppearance::RestoreState(){
			if (_is_default) return;

			_is_default = true;

			glColor3d(_rgb_default.Red(),_rgb_default.Green(),_rgb_default.Blue());
			glLineWidth((GLfloat)_line_width_default);
			glPointSize((GLfloat)_ctl_pt_size_default);
			glLineStipple(factor,pattern);

			glDisable(GL_LINE_SMOOTH);
			glDisable(GL_BLEND);
		}
		void TPlotLine2DAppearance::SwitchTo(int status){
			SaveState();

			if (status&COLOR) 
				glColor3d(_rgb_base.Red(),_rgb_base.Green(),_rgb_base.Blue());
			if (status&CTL_PT_COLOR) 
				glColor3d(_rgb_ctl_pt.Red(),_rgb_ctl_pt.Green(),_rgb_ctl_pt.Blue());
			if (status&HIGH_LIGHT_COLOR) 
				glColor3d(_rgb_highlight.Red(),_rgb_highlight.Green(),_rgb_highlight.Blue());
			if (status&CENTRIOD_COLOR)
				glColor3d(_rgb_centriod.Red(),_rgb_centriod.Green(),_rgb_centriod.Blue());
			if (status&WIDTH)
				glLineWidth((GLfloat)_line_width);
			if (status&CTL_PT_SIZE)
				glPointSize((GLfloat)_ctl_pt_size);
			if (status&STYLE){
				glEnable(GL_LINE_STIPPLE);
				glLineStipple(_line_sytle.Factor(),_line_sytle.Pattern());
			}
			if (status&CTL_STYLE){
				glEnable(GL_LINE_STIPPLE);
				glLineStipple(_ctl_line_style.Factor(),_ctl_line_style.Pattern());
			}
			if (status&SMOOTH){
				glEnable(GL_LINE_SMOOTH);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
				glHint(GL_LINE_SMOOTH_HINT,GL_DONT_CARE);
			}
			if (status&DEFAULT)
				RestoreState();
		}

		class TPlotPoint2DAppearance{
			TRgb _rgb_base,_rgb_highlight;
			GLdouble _size,_ctl_pt_size;
			bool _is_smooth;
		private:
			TRgb _rgb_default;
			GLdouble _pt_size_default;
			bool _is_default;
		protected:
			TPlotPoint2DAppearance():_rgb_base(.0,.0,.85),_rgb_highlight(1.0,.0,.0),_size(6.0),_ctl_pt_size(6.0),_is_default(true),_is_smooth(true){}
			virtual ~TPlotPoint2DAppearance(){}
		public:
			void SetColor(const TRgb &color){_rgb_base = color;}
			void SetHighLightColor(const TRgb &color){_rgb_highlight = color;}
			void SetSize(const double &size){_size = size;}
			void SetCtlPointSize(const double &size){_ctl_pt_size = size;}
			void SetSmooth(const bool &flag){_is_smooth = flag;}
		protected:
			enum EStatus{
				COLOR			 = 0x0001,
				HIGH_LIGHT_COLOR = 0x0002,
				CTL_PT_SIZE		 = 0x0004,
				SIZE			 = 0x0008,
				SMOOTH			 = 0x0010,
				DEFAULT			 = 0x0020
			};
			inline void SwitchTo(int status);
		private:
			inline void SaveState();
			inline void RestoreState();
		};

		void TPlotPoint2DAppearance::SaveState(){
			if (!_is_default) return;

			_is_default = false;

			GLdouble color[4];
			glGetDoublev(GL_CURRENT_COLOR,color);
			_rgb_default.Red(color[0]);
			_rgb_default.Green(color[1]);
			_rgb_default.Blue(color[2]);
			glGetDoublev(GL_POINT_SIZE,&_pt_size_default);
		}

		void TPlotPoint2DAppearance::RestoreState(){
			if (_is_default) return;

			_is_default = true;

			glColor3d(_rgb_default.Red(),_rgb_default.Green(),_rgb_default.Blue());
			glPointSize((GLfloat)_pt_size_default);

			glDisable(GL_POINT_SMOOTH);
			glDisable(GL_BLEND);
		}

		void TPlotPoint2DAppearance::SwitchTo(int status){
			SaveState();
			if (status&COLOR)
				glColor3d(_rgb_base.Red(),_rgb_base.Green(),_rgb_base.Blue());
			if (status&HIGH_LIGHT_COLOR)
				glColor3d(_rgb_highlight.Red(),_rgb_highlight.Green(),_rgb_highlight.Blue());
			if (status&CTL_PT_SIZE)
				glPointSize((GLfloat)_ctl_pt_size);
			if (status&SIZE)
				glPointSize((GLfloat)_size);
			if (status&SMOOTH){
				glEnable(GL_POINT_SMOOTH);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
				glHint(GL_POINT_SMOOTH_HINT,GL_DONT_CARE);
			}
			if (status&DEFAULT)
				RestoreState();
		}
	}//namespace shape.
}//namespace plot

#endif