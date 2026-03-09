#ifndef _______COMX_NAPI_HXX
#define _______COMX_NAPI_HXX

#include <napi.h>
#include <type_cast.hxx>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <utility>
#include <map>
#include <math.h>
using namespace std;


namespace comx {
        namespace napi {


                template <typename Input, typename Ret>
                class Worker : public Napi::AsyncWorker {
                public:
                        Worker(Napi::Function& callback, Input& para,
                                std::function<Ret(decltype(para)&)> work_routine,
                                std::function<Napi::Value(Napi::Env, Ret&)> ret_routine)
                                : Napi::AsyncWorker(callback)
                                , _para(para)
                                , _work_routine(work_routine)
                                , _ret_routine(ret_routine)
                        {
                                // Nothing.
                        }
                        ~Worker() {}

                        // Executed inside the worker-thread.
                        // It is not safe to access JS engine data structure
                        // here, so everything we need for input and output
                        // should go on `this`.
                        void Execute() {
                                _result = _work_routine(_para);
                        }

                        // Executed when the async work is complete_
                        // this function will be run inside the main event loop
                        // so it is safe to use JS engine data again
                        void OnOK()
                        {
                                Napi::HandleScope scope(Env());
                                Callback().Call({ Env().Undefined(), _ret_routine(Env(), _result) });
                        }

                private:
                        Input _para;
                        Ret _result;

                        std::function<Ret(Input&)> _work_routine;
                        std::function<Napi::Value(Napi::Env, Ret&)> _ret_routine;
                };

                template <typename Input, typename WorkRoutine, typename RetRoutine>
                void InvokeWorker(Input& para, Napi::Function& napi_callback,
                        WorkRoutine work_routine, RetRoutine ret_routine)
                {
                        typedef decltype(work_routine(para)) Ret;
                        typedef Worker<Input, Ret> TWorker;

                        TWorker* worker = new TWorker(napi_callback, para, work_routine, ret_routine);
                        worker->Queue();
                }

#define JS_EXT_DATA_DECLARE() \
        string S_namespace;   \
	map<string, string> S_comments;

#define JS_EXT_ENTRY_BEGIN()
#define JS_EXT_ENTRY(func_name)
#define JS_EXT_ENTRY_END()

#define JS_EXT_MAIN_BEGIN(ns,num)                                 \
    Napi::Object                                                  \
    Init(Napi::Env env, Napi::Object exports)                     \
    {                                                             \
        S_namespace = ns;                                         \
                                                                  \
        string::size_type sc_index = S_namespace.find('.');       \
        assert(sc_index != string::npos);                         \
                                                                  \
        string top_ns = S_namespace.substr(0, sc_index);          \
        string sub_ns = S_namespace.substr(sc_index + 1);         \
                                                                  \
        Napi::Object global = env.Global();                       \
                                                                  \
        Napi::Object top_obj;                                     \
                                                                  \
        if (global.Has(top_ns)) {                                 \
            top_obj = global.Get(top_ns).ToObject();              \
        } else {                                                  \
            top_obj = Napi::Object::New(env);                     \
            global.Set(Napi::String::New(env, top_ns), top_obj);  \
        }                                                         \
                                                                  \
        Napi::Object sub_obj;                                     \
        if (top_obj.Has(sub_ns)) {                                \
            sub_obj = top_obj.Get(sub_ns).ToObject();             \
        } else {                                                  \
            sub_obj = Napi::Object::New(env);                     \
            top_obj.Set(Napi::String::New(env, sub_ns), sub_obj); \
        }

#define JS_EXT_FUNC_REG(name)                             \
        if (!sub_obj.Has(#name)) {                        \
            sub_obj.Set(Napi::String::New(env, #name),    \
            Napi::Function::New(env, name));              \
	    __comments_##name();                          \
        }

#define JS_EXT_MAIN_END()                                                      \
        Napi::Object comments_obj;                                             \
        if (!sub_obj.Has("_Comments")) {                                       \
	    comments_obj = Napi::Object::New(env);                             \
            sub_obj.Set(Napi::String::New(env, "_Comments"),                   \
            comments_obj);                                                     \
        }                                                                      \
	else                                                                   \
        {                                                                      \
            comments_obj = sub_obj.Get("_Comments").ToObject();                \
        }                                                                      \
        for(auto iter = S_comments.begin(); iter != S_comments.end(); ++iter)  \
        {                                                                      \
            comments_obj.Set(Napi::String::New(env, iter->first),              \
		       Napi::String::New(env, iter->second));                  \
        }                                                                      \
        exports = sub_obj;                                                     \
        return exports;                                                        \
    }                                                                          \
    NODE_API_MODULE(addon, Init)

#define __COMX__TOSTRING(x) #x
#define __COMX__STR(x) __COMX__TOSTRING(x)

#define JS_EXT_FUNC_BEGIN(name, num, comments)             \
    void __comments_##name(){S_comments[#name] = comments;}\
    Napi::Value name(const Napi::CallbackInfo& info) {     \
        Napi::Env env = info.Env();                        \
                                                           \
        if (info.Length() != num) {                        \
            Napi::TypeError::New(env, comments)            \
                .ThrowAsJavaScriptException();             \
	    return info.Env().Undefined();                 \
        }                                                  \
        string strKnownException = string("Plugin Unknown Exception: at function - ") + #name + " in " + __FILE__ + " line(" + __COMX__STR(__LINE__) +  ")."; \
        try

#define JS_EXT_FUNC_END()                                  \
                                                           \
        catch(...){                                        \
             Napi::Error::New(env, strKnownException.c_str()) \
                .ThrowAsJavaScriptException();             \
        }                                                  \
        return info.Env().Undefined();}

#define JS_EXT_FUNC_DEBUG_BEGIN(name, num, comments)       \
    void __comments_##name(){S_comments[#name] = comments;}\
    Napi::Value name(const Napi::CallbackInfo& info) {     \
        Napi::Env env = info.Env();                        \
                                                           \
        if (info.Length() != num) {                        \
            Napi::TypeError::New(env, comments)            \
                .ThrowAsJavaScriptException();             \
	    return info.Env().Undefined();                 \
        }

#define JS_EXT_FUNC_DEBUG_END()                            \
        return info.Env().Undefined();}

size_t get_element_num_of_parameter(const Napi::Value &input)
{
        if (input.IsTypedArray())
        {
                return input.As<Napi::TypedArray>().ElementLength();
        }

        if (input.IsArrayBuffer())
        {
                return input.As<Napi::ArrayBuffer>().ByteLength();
        }

        return 1;
}

#define JS_EXT_PARA_LENGTH(para_index) comx::napi::get_element_num_of_parameter(info[para_index])

                template<typename T>
                Napi::Value constructur_ret_value(T val, Napi::Env env, int element_num)
                {
                        assert(false);
                }

                template<>
                Napi::Value constructur_ret_value<Napi::ArrayBuffer>(Napi::ArrayBuffer val, Napi::Env env, int elment_num)
                {
                        return val;
                }

                template<>
                Napi::Value constructur_ret_value<string>(string val, Napi::Env env, int elment_num)
                {
                        string ret = val;
                        //transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
                        if (ret == "undefined")
                        {
                                return env.Undefined();
                        }
                        else
                        {
                                return Napi::String::New(env, ret.c_str());
                        }
                }

                template<>
                Napi::Value constructur_ret_value<bool>(bool val, Napi::Env env, int elment_num)
                {
                        KMAS::type::TNodeEnvWrap env_wrap(env);
                        return KMAS::type::type_cast<Napi::Value>(val);
                }

                template<>
                Napi::Value constructur_ret_value<int>(int val, Napi::Env env, int elment_num)
                {
                        KMAS::type::TNodeEnvWrap env_wrap(env);
                        return KMAS::type::type_cast<Napi::Value>(val);
                }

                template<>
                Napi::Value constructur_ret_value<float>(float val, Napi::Env env, int elment_num)
                {
                        KMAS::type::TNodeEnvWrap env_wrap(env);
                        return KMAS::type::type_cast<Napi::Value>(val);
                }

                template<>
                Napi::Value constructur_ret_value<double>(double val, Napi::Env env, int elment_num)
                {
                        KMAS::type::TNodeEnvWrap env_wrap(env);
                        return KMAS::type::type_cast<Napi::Value>(val);
                }

                typedef uint8_t *uint8_ptr_t;
                template<>
                Napi::Value constructur_ret_value<uint8_ptr_t>(uint8_ptr_t val, Napi::Env env, int elment_num)
                {
                        Napi::ArrayBuffer buf = Napi::ArrayBuffer::New(env, (void*)val, sizeof(uint8_t) * elment_num);
                        return Napi::Uint8Array::New(env, elment_num, buf, 0, napi_uint8_array);
                }

                typedef int8_t *int8_ptr_t;
                template<>
                Napi::Value constructur_ret_value<int8_ptr_t>(int8_ptr_t val, Napi::Env env, int elment_num)
                {
                        Napi::ArrayBuffer buf = Napi::ArrayBuffer::New(env, (void*)val, sizeof(int8_t) * elment_num);
                        return Napi::Int8Array::New(env, elment_num, buf, 0, napi_int8_array);
                }

                typedef uint16_t *uint16_ptr_t;
                template<>
                Napi::Value constructur_ret_value<uint16_ptr_t>(uint16_ptr_t val, Napi::Env env, int elment_num)
                {
                        Napi::ArrayBuffer buf = Napi::ArrayBuffer::New(env, (void*)val, sizeof(uint16_t) * elment_num);
                        return Napi::Uint16Array::New(env, elment_num, buf, 0, napi_uint16_array);
                }

                typedef int16_t *int16_ptr_t;
                template<>
                Napi::Value constructur_ret_value<int16_ptr_t>(int16_ptr_t val, Napi::Env env, int elment_num)
                {
                        Napi::ArrayBuffer buf = Napi::ArrayBuffer::New(env, (void*)val, sizeof(int16_t) * elment_num);
                        return Napi::Int16Array::New(env, elment_num, buf, 0, napi_int16_array);
                }

                typedef int32_t *int32_ptr_t;
                template<>
                Napi::Value constructur_ret_value<int32_ptr_t>(int32_ptr_t val, Napi::Env env, int elment_num)
                {
                        Napi::ArrayBuffer buf = Napi::ArrayBuffer::New(env, (void*)val, sizeof(int32_t) * elment_num);
                        return Napi::Int32Array::New(env, elment_num, buf, 0, napi_int32_array);
                }

                typedef uint32_t *uint32_ptr_t;
                template<>
                Napi::Value constructur_ret_value<uint32_ptr_t>(uint32_ptr_t val, Napi::Env env, int elment_num)
                {
                        Napi::ArrayBuffer buf = Napi::ArrayBuffer::New(env, (void*)val, sizeof(uint32_t) * elment_num);
                        return Napi::Uint32Array::New(env, elment_num, buf, 0, napi_uint32_array);
                }

                typedef float *float32_ptr_t;
                template<>
                Napi::Value constructur_ret_value<float32_ptr_t>(float32_ptr_t val, Napi::Env env, int elment_num)
                {
                        Napi::ArrayBuffer buf = Napi::ArrayBuffer::New(env, (void*)val, sizeof(float) * elment_num);
                        return Napi::Float32Array::New(env, elment_num, buf, 0, napi_float32_array);
                }

                typedef double *float64_ptr_t;
                template<>
                Napi::Value constructur_ret_value<float64_ptr_t>(float64_ptr_t val, Napi::Env env, int elment_num)
                {
                        Napi::ArrayBuffer buf = Napi::ArrayBuffer::New(env, (void*)val, sizeof(double) * elment_num);
                        return Napi::Float64Array::New(env, elment_num, buf, 0, napi_float64_array);
                }

                template<>
                Napi::Value constructur_ret_value<void*>(void* val, Napi::Env env, int byte_size)
                {
                        Napi::ArrayBuffer buf = Napi::ArrayBuffer::New(env, (void*)val, byte_size);
                        return buf;
                }

                class TCallbackWrap
                {
                public:
                        Napi::FunctionReference _cb;
                        vector<napi_value> args;
                public:
                        class TBuffer
                        {
                        public:
                                void *_buf;
                                int _length;
                                napi_typedarray_type _type;
                        public:
                                template<typename T>
                                TBuffer(T *buf, int length)
                                {
                                        _buf = (void*)buf;
                                        _length = length;
                                        _type = get_type(buf);
                                }
                        private:
                                template<typename T>
                                napi_typedarray_type get_type(T *)
                                {
                                        assert(false);
                                }

                                napi_typedarray_type get_type(uint8_t *)
                                {
                                        return napi_uint8_array;
                                }

                                napi_typedarray_type get_type(int8_t *)
                                {
                                        return napi_int8_array;
                                }

                                napi_typedarray_type get_type(int16_t *)
                                {
                                        return napi_int16_array;
                                }

                                napi_typedarray_type get_type(uint16_t *)
                                {
                                        return napi_uint16_array;
                                }

                                napi_typedarray_type get_type(int32_t *)
                                {
                                        return napi_int32_array;
                                }

                                napi_typedarray_type get_type(uint32_t *)
                                {
                                        return napi_uint32_array;
                                }

                                napi_typedarray_type get_type(float *)
                                {
                                        return napi_float32_array;
                                }

                                napi_typedarray_type get_type(double *)
                                {
                                        return napi_float64_array;
                                }

                        };
                public:
                        TCallbackWrap(Napi::Function cb) {
                                _cb = Napi::Weak(cb);
                        }
                        TCallbackWrap() {}
                        void Reset() {
                                _cb.Reset();
                        }
                public:
                        Napi::Value operator()()
                        {
                                Napi::Value ret = _cb.Call(args);
                                args.clear();
                                return ret;
                        }
                        template<typename T, typename... Targs>
                        Napi::Value operator()(T val, Targs... Fargs)
                        {
                                assert(!_cb.IsEmpty());
                                args.push_back(generate_napi_val(val));
                                return operator()(Fargs...);
                        }

                        template<typename T>
                        Napi::Value generate_napi_val(T val)
                        {
                                Napi::Env env = _cb.Env();
                                KMAS::type::TNodeEnvWrap env_wrap(env);
                                Napi::Value napi_val = KMAS::type::type_cast<Napi::Value>(val);
                                return napi_val;
                        }

                        Napi::Value generate_napi_val(TBuffer val)
                        {
                                if (val._type == napi_uint8_array)
                                {
                                        return constructur_ret_value((uint8_t*)val._buf, _cb.Env(), val._length);
                                }

                                if (val._type == napi_int8_array)
                                {
                                        return constructur_ret_value((int8_t*)val._buf, _cb.Env(), val._length);
                                }

                                if (val._type == napi_int16_array)
                                {
                                        return constructur_ret_value((int16_t*)val._buf, _cb.Env(), val._length);
                                }

                                if (val._type == napi_uint16_array)
                                {
                                        return constructur_ret_value((uint16_t*)val._buf, _cb.Env(), val._length);
                                }

                                if (val._type == napi_int32_array)
                                {
                                        return constructur_ret_value((int32_t*)val._buf, _cb.Env(), val._length);
                                }

                                if (val._type == napi_uint32_array)
                                {
                                        return constructur_ret_value((uint32_t*)val._buf, _cb.Env(), val._length);
                                }

                                if (val._type == napi_float32_array)
                                {
                                        return constructur_ret_value((float*)val._buf, _cb.Env(), val._length);
                                }

                                if (val._type == napi_float64_array)
                                {
                                        return constructur_ret_value((double*)val._buf, _cb.Env(), val._length);
                                }

                                return _cb.Env().Undefined();
                        }
                };

                typedef Napi::ArrayBuffer TBuffer;

                template<typename T>
                T comx_type_cast(Napi::Value val, Napi::Env env)
                {
                        return KMAS::type::type_cast<T>(val);
                }

                template<>
                comx::napi::TCallbackWrap comx_type_cast<comx::napi::TCallbackWrap>(Napi::Value val, Napi::Env env)
                {
                        return comx::napi::TCallbackWrap(KMAS::type::type_cast<Napi::Function>(val));
                }

                template<typename T>
                TCallbackWrap::TBuffer make_buffer(T* val, int len)
                {
                        return TCallbackWrap::TBuffer(val, len);
                }

#define JS_EXT_PARA(type_inf, para_index) \
  comx::napi::comx_type_cast<type_inf>(info[para_index], env)
                //type_cast<type_inf>(info[para_index])

#define JS_EXT_CALLBACK(para_index)        \
       comx::napi::TCallbackWrap(type_cast<Napi::Function>(info[para_index]), env)

#define JS_EXT_FUNC_ASSIGN_RET(ret_val)                         \
       return comx::napi::constructur_ret_value((ret_val), info.Env(), 0)

#define JS_EXT_FUNC_ASSIGN_RET_EX(ret_val, num)                 \
       return comx::napi::constructur_ret_value(ret_val, info.Env(), num)

                inline string JS_EXT_FixedFileExtName(string fname, string ext_fnames[], int len)
                {
                        string::size_type idx = fname.find_last_of('.');

                        if (std::find(ext_fnames, ext_fnames + len, fname.substr(idx + 1)) == ext_fnames + len)
                        {
                                fname += ".";
                                fname += ext_fnames[0];
                        }

                        return fname;
                }

                inline string JS_EXT_FixedFileExtName(string fname, string ext_fname)
                {
                        string::size_type idx = fname.find_last_of('.');

                        if (fname.substr(idx + 1) != ext_fname)
                        {
                                fname += ".";
                                fname += ext_fname;
                        }

                        return fname;
                }

                template<typename Reader, typename Value>
                inline bool JS_EXT_LOAD_JSON(string json_fpath, Reader &reader, Value &root)
                {
                        bool ret = true;

                        bool bExist = true;
                        ifstream ifos(json_fpath.c_str());
                        if (!ifos)
                        {
                                bExist = false;
                        }

                        if (bExist)
                        {
                                ifstream ifs;

                                ifs.open(json_fpath.c_str());

                                if (!reader.parse(ifs, root, false))
                                {
                                        ret = false;
                                }
                        }

                        return ret;
                }

                namespace GL {
                        namespace __func {
                                struct vector3d_t {
                                        double x, y, z;
                                };
                                // function: get_model.
                                template<typename vector3d_t>
                                double get_model(const vector3d_t t)
                                {
                                        return sqrt(t.x * t.x + t.y * t.y + t.z * t.z);
                                }

                                template<typename vector3d_t>
                                vector3d_t normalize_vec(const vector3d_t t)
                                {
                                        vector3d_t ret = t;

                                        double model = get_model(t);
                                        if (model < 1e-10)
                                        {
                                                ret.x = 0.0;
                                                ret.y = 0.0;
                                                ret.z = 0.0;
                                        }
                                        else
                                        {
                                                ret.x /= model;
                                                ret.y /= model;
                                                ret.z /= model;
                                        }

                                        return ret;
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

                                template<typename point_t_1,
                                        typename point_t_2,
                                        typename point_t_3>
                                        vector3d_t get_normal_of_triangle(const point_t_1 pt1,
                                                const point_t_2 pt2,
                                                const point_t_3 pt3)
                                {
                                        return normalize_vec(fork_product(get_vec_from_two_point(pt1, pt2),
                                                get_vec_from_two_point(pt1, pt3)));
                                }

                                vector<double> getNodeNormal(const vector<double> &pts, const vector<int> &triangles)
                                {
                                        vector<double> ret(pts.size(), 0);

                                        vector<vector3d_t> enormals;
                                        map<int, set<int> > adelems_of_nodes;
                                        for (size_t eid = 0; eid < triangles.size() / 3; ++eid)
                                        {
                                                int n1 = triangles[eid * 3 + 0];
                                                int n2 = triangles[eid * 3 + 1];
                                                int n3 = triangles[eid * 3 + 2];

                                                vector3d_t pt1 = { pts[n1 * 3 + 0], pts[n1 * 3 + 1], pts[n1 * 3 + 2] };
                                                vector3d_t pt2 = { pts[n2 * 3 + 0], pts[n2 * 3 + 1], pts[n2 * 3 + 2] };
                                                vector3d_t pt3 = { pts[n3 * 3 + 0], pts[n3 * 3 + 1], pts[n3 * 3 + 2] };
                                                try
                                                {
                                                        vector3d_t nor = get_normal_of_triangle(pt1, pt2, pt3);
                                                        enormals.push_back(nor);
                                                }
                                                catch (...)
                                                {
                                                        //cout << n1 << "," << n2 << "," << n3 << endl;
                                                        enormals.push_back({ 0.0,0.0,1.0 });
                                                }
                                                adelems_of_nodes[n1].insert(eid);
                                                adelems_of_nodes[n2].insert(eid);
                                                adelems_of_nodes[n3].insert(eid);
                                        }

                                        for (size_t nid = 0; nid < pts.size() / 3; ++nid)
                                        {
                                                if (adelems_of_nodes.find(nid) != adelems_of_nodes.end())
                                                {
                                                        double dx = 0.0, dy = 0.0, dz = 0.0;
                                                        for (auto eid = adelems_of_nodes[nid].begin(); eid != adelems_of_nodes[nid].end(); ++eid)
                                                        {
                                                                dx += enormals[*eid].x;
                                                                dy += enormals[*eid].y;
                                                                dz += enormals[*eid].z;
                                                        }

                                                        dx /= adelems_of_nodes[nid].size();
                                                        dy /= adelems_of_nodes[nid].size();
                                                        dz /= adelems_of_nodes[nid].size();

                                                        ret[nid * 3 + 0] = dx;
                                                        ret[nid * 3 + 1] = dy;
                                                        ret[nid * 3 + 2] = dz;
                                                }
                                                else
                                                {
                                                        ret[nid * 3 + 0] = 0.0;
                                                        ret[nid * 3 + 1] = 0.0;
                                                        ret[nid * 3 + 2] = 1.0;
                                                }
                                        }

                                        return ret;
                                }
                        }
                        inline Napi::Value createPlotBuffer(Napi::Env env,
                                vector<double> pts,
                                vector<double> values)
                        {
                                uint32_t pts_len = pts.size() / 3;
                                uint32_t values_len = values.size();

                                if (pts_len != values_len || pts_len == 0)
                                {
                                        return Napi::Value::From(env, false);
                                }

                                uint32_t buffer_size = pts_len * 3 * 8/*nodes*/ + values_len * 8/*values*/;

                                Napi::ArrayBuffer ret = Napi::ArrayBuffer::New(env, buffer_size);
                                Napi::DataView    gl_view = Napi::DataView::New(env, ret);

                                uint32_t base_address = 0;
                                for (uint32_t idx = 0; idx < pts_len; ++idx)
                                {
                                        double x = pts[idx * 3 + 0];
                                        double y = pts[idx * 3 + 1];
                                        double z = pts[idx * 3 + 2];

                                        gl_view.SetFloat64(base_address, x);
                                        base_address += 8;
                                        gl_view.SetFloat64(base_address, y);
                                        base_address += 8;
                                        gl_view.SetFloat64(base_address, z);
                                        base_address += 8;
                                }

                                for (uint32_t idx = 0; idx < values_len; ++idx)
                                {
                                        double value = values[idx];

                                        gl_view.SetFloat64(base_address, value);
                                        base_address += 8;
                                }

                                return ret;
                        }
                    inline Napi::Value createPlotBufferEx(Napi::Env env,
                                                          vector<double> pts,
                                                          vector<double> values,
                                                          double max_val, double min_val
                                                          )
                        {
                                uint32_t pts_len = pts.size() / 3;
                                uint32_t values_len = values.size();

                                if (pts_len != values_len || pts_len == 0)
                                {
                                        return Napi::Value::From(env, false);
                                }

                                uint32_t buffer_size = pts_len * 3 * 8/*nodes*/ + values_len * 8/*values*/ + 2 * 8;

                                Napi::ArrayBuffer ret = Napi::ArrayBuffer::New(env, buffer_size);
                                Napi::DataView    gl_view = Napi::DataView::New(env, ret);

                                uint32_t base_address = 0;
                                for (uint32_t idx = 0; idx < pts_len; ++idx)
                                {
                                        double x = pts[idx * 3 + 0];
                                        double y = pts[idx * 3 + 1];
                                        double z = pts[idx * 3 + 2];

                                        gl_view.SetFloat64(base_address, x);
                                        base_address += 8;
                                        gl_view.SetFloat64(base_address, y);
                                        base_address += 8;
                                        gl_view.SetFloat64(base_address, z);
                                        base_address += 8;
                                }

                                for (uint32_t idx = 0; idx < values_len; ++idx)
                                {
                                        double value = values[idx];

                                        gl_view.SetFloat64(base_address, value);
                                        base_address += 8;
                                }

                                gl_view.SetFloat64(base_address, max_val);
                                base_address += 8;

                                gl_view.SetFloat64(base_address, min_val);
                                base_address += 8;

                                return ret;
                        }
                        inline Napi::Value createBuffer(Napi::Env env,
                                int mid, int sid,
                                vector<double> pts,
                                vector<double> normals,
                                vector<int> triangles,
                                vector<int> triprops,
                                vector<int> lines,
                                vector<int> lineprops,
                                vector<int> vertex,
                                vector<int> vertexprops,
                                bool auto_normal = false)
                        {
                                if (normals.size() == 0 && triangles.size() != 0 && auto_normal)
                                {
                                        normals = __func::getNodeNormal(pts, triangles);
                                }

                                uint32_t pts_len = pts.size() / 3;
                                uint32_t normals_len = normals.size() / 3;

                                //cout << "normals_len is " << normals_len << endl;

                                if (pts_len != normals_len && normals_len != 0)
                                {
                                        return Napi::Value::From(env, false);
                                }

                                uint32_t triangles_len = triangles.size() / 3;
                                uint32_t triprops_len = triprops.size();
                                if (triangles_len != triprops_len && triprops_len != 0)
                                {
                                        return Napi::Value::From(env, false);
                                }

                                uint32_t lines_len = lines.size() / 2;
                                uint32_t lineprops_len = lineprops.size();
                                if (lines_len != lineprops_len && lineprops_len != 0)
                                {
                                        return Napi::Value::From(env, false);
                                }

                                uint32_t vertex_len = vertex.size();
                                uint32_t vertexprop_len = vertexprops.size();
                                if (vertex_len != vertexprop_len && vertexprop_len != 0)
                                {
                                        return Napi::Value::From(env, false);
                                }

                                uint32_t buffer_size = 4/*mid*/ + 4/*sid*/ +
                                        4/*ns*/ + 4/*nns*/ + 4/*tris*/ + 4/*trips*/ +
                                        4/*ls*/ + 4/*lps*/ + 4/*vs*/ + 4/*vps*/ +
                                        pts_len * 3 * 8/*nodes*/ + normals_len * 3 * 8/*node normals*/ +
                                        triangles_len * 3 * 4/*triangles*/ + triprops_len * 4/*triprops*/ +
                                        lines_len * 2 * 4/*lines*/ + lineprops_len * 4/*lineprops*/ +
                                        vertex_len * 4/*vertex*/ + vertexprop_len * 4/*vertexprops*/;

                                Napi::ArrayBuffer ret = Napi::ArrayBuffer::New(env, buffer_size);
                                Napi::DataView    gl_view = Napi::DataView::New(env, ret);

                                uint32_t base_address = 0;
                                gl_view.SetUint32(base_address, mid);
                                base_address += 4;
                                gl_view.SetUint32(base_address, sid);
                                base_address += 4;
                                gl_view.SetUint32(base_address, pts_len);
                                base_address += 4;
                                gl_view.SetUint32(base_address, normals_len);
                                base_address += 4;
                                gl_view.SetUint32(base_address, triangles_len);
                                base_address += 4;
                                gl_view.SetUint32(base_address, triprops_len);
                                base_address += 4;
                                gl_view.SetUint32(base_address, lines_len);
                                base_address += 4;
                                gl_view.SetUint32(base_address, lineprops_len);
                                base_address += 4;
                                gl_view.SetUint32(base_address, vertex_len);
                                base_address += 4;
                                gl_view.SetUint32(base_address, vertexprop_len);
                                base_address += 4;

                                for (uint32_t idx = 0; idx < pts_len; ++idx)
                                {
                                        double x = pts[idx * 3 + 0];
                                        double y = pts[idx * 3 + 1];
                                        double z = pts[idx * 3 + 2];

                                        gl_view.SetFloat64(base_address, x);
                                        base_address += 8;
                                        gl_view.SetFloat64(base_address, y);
                                        base_address += 8;
                                        gl_view.SetFloat64(base_address, z);
                                        base_address += 8;
                                }

                                for (uint32_t idx = 0; idx < normals_len; ++idx)
                                {
                                        double nx = normals[idx * 3 + 0];
                                        double ny = normals[idx * 3 + 1];
                                        double nz = normals[idx * 3 + 2];

                                        gl_view.SetFloat64(base_address, nx);
                                        base_address += 8;
                                        gl_view.SetFloat64(base_address, ny);
                                        base_address += 8;
                                        gl_view.SetFloat64(base_address, nz);
                                        base_address += 8;
                                }

                                for (uint32_t idx = 0; idx < triangles_len; ++idx)
                                {
                                        int n1 = triangles[idx * 3 + 0];
                                        int n2 = triangles[idx * 3 + 1];
                                        int n3 = triangles[idx * 3 + 2];

                                        gl_view.SetUint32(base_address, n1);
                                        base_address += 4;
                                        gl_view.SetUint32(base_address, n2);
                                        base_address += 4;
                                        gl_view.SetUint32(base_address, n3);
                                        base_address += 4;
                                }

                                for (uint32_t idx = 0; idx < triprops_len; ++idx)
                                {
                                        int prop = triprops[idx];
                                        gl_view.SetUint32(base_address, prop);
                                        base_address += 4;
                                }

                                for (uint32_t idx = 0; idx < lines_len; ++idx)
                                {
                                        int n1 = lines[idx * 2 + 0];
                                        int n2 = lines[idx * 2 + 1];

                                        gl_view.SetUint32(base_address, n1);
                                        base_address += 4;
                                        gl_view.SetUint32(base_address, n2);
                                        base_address += 4;
                                }

                                for (uint32_t idx = 0; idx < lineprops_len; ++idx)
                                {
                                        int prop = lineprops[idx];
                                        gl_view.SetUint32(base_address, prop);
                                        base_address += 4;
                                }

                                for (uint32_t idx = 0; idx < vertex_len; ++idx)
                                {
                                        int n = vertex[idx];
                                        gl_view.SetUint32(base_address, n);
                                        base_address += 4;
                                }

                                for (uint32_t idx = 0; idx < vertexprop_len; ++idx)
                                {
                                        int prop = vertexprops[idx];
                                        gl_view.SetUint32(base_address, prop);
                                        base_address += 4;
                                }

                                return ret;
                        }

                        inline Napi::Value createBufferEx(Napi::Env env,
                                int mid, int sid,
                                vector<double> pts,
                                vector<double> normals,
                                vector<int> triangles,
                                vector<int> triprops,
                                vector<int> lines,
                                vector<int> lineprops,
                                vector<int> vertex,
                                vector<int> vertexprops,
                                int red, int green, int blue)
                        {
                                uint32_t pts_len = pts.size() / 3;
                                uint32_t normals_len = normals.size() / 3;

                                //cout << "normals_len is " << normals_len << endl;

                                if (pts_len != normals_len && normals_len != 0)
                                {
                                        return Napi::Value::From(env, false);
                                }

                                uint32_t triangles_len = triangles.size() / 3;
                                uint32_t triprops_len = triprops.size();
                                if (triangles_len != triprops_len && triprops_len != 0)
                                {
                                        return Napi::Value::From(env, false);
                                }

                                uint32_t lines_len = lines.size() / 2;
                                uint32_t lineprops_len = lineprops.size();
                                if (lines_len != lineprops_len && lineprops_len != 0)
                                {
                                        return Napi::Value::From(env, false);
                                }

                                uint32_t vertex_len = vertex.size();
                                uint32_t vertexprop_len = vertexprops.size();
                                if (vertex_len != vertexprop_len && vertexprop_len != 0)
                                {
                                        return Napi::Value::From(env, false);
                                }

                                uint32_t buffer_size = 4/*mid*/ + 4/*sid*/ +
                                        4/*ns*/ + 4/*nns*/ + 4/*tris*/ + 4/*trips*/ +
                                        4/*ls*/ + 4/*lps*/ + 4/*vs*/ + 4/*vps*/ +
                                        pts_len * 3 * 8/*nodes*/ + normals_len * 3 * 8/*node normals*/ +
                                        triangles_len * 3 * 4/*triangles*/ + triprops_len * 4/*triprops*/ +
                                        lines_len * 2 * 4/*lines*/ + lineprops_len * 4/*lineprops*/ +
                                        vertex_len * 4/*vertex*/ + vertexprop_len * 4/*vertexprops*/ +
                                        4/*red*/ + 4/*green*/ + 4/*blue*/;

                                Napi::ArrayBuffer ret = Napi::ArrayBuffer::New(env, buffer_size);
                                Napi::DataView    gl_view = Napi::DataView::New(env, ret);

                                uint32_t base_address = 0;
                                gl_view.SetUint32(base_address, mid);
                                base_address += 4;
                                gl_view.SetUint32(base_address, sid);
                                base_address += 4;
                                gl_view.SetUint32(base_address, pts_len);
                                base_address += 4;
                                gl_view.SetUint32(base_address, normals_len);
                                base_address += 4;
                                gl_view.SetUint32(base_address, triangles_len);
                                base_address += 4;
                                gl_view.SetUint32(base_address, triprops_len);
                                base_address += 4;
                                gl_view.SetUint32(base_address, lines_len);
                                base_address += 4;
                                gl_view.SetUint32(base_address, lineprops_len);
                                base_address += 4;
                                gl_view.SetUint32(base_address, vertex_len);
                                base_address += 4;
                                gl_view.SetUint32(base_address, vertexprop_len);
                                base_address += 4;

                                for (uint32_t idx = 0; idx < pts_len; ++idx)
                                {
                                        double x = pts[idx * 3 + 0];
                                        double y = pts[idx * 3 + 1];
                                        double z = pts[idx * 3 + 2];

                                        gl_view.SetFloat64(base_address, x);
                                        base_address += 8;
                                        gl_view.SetFloat64(base_address, y);
                                        base_address += 8;
                                        gl_view.SetFloat64(base_address, z);
                                        base_address += 8;
                                }

                                for (uint32_t idx = 0; idx < normals_len; ++idx)
                                {
                                        double nx = normals[idx * 3 + 0];
                                        double ny = normals[idx * 3 + 1];
                                        double nz = normals[idx * 3 + 2];

                                        gl_view.SetFloat64(base_address, nx);
                                        base_address += 8;
                                        gl_view.SetFloat64(base_address, ny);
                                        base_address += 8;
                                        gl_view.SetFloat64(base_address, nz);
                                        base_address += 8;
                                }

                                for (uint32_t idx = 0; idx < triangles_len; ++idx)
                                {
                                        int n1 = triangles[idx * 3 + 0];
                                        int n2 = triangles[idx * 3 + 1];
                                        int n3 = triangles[idx * 3 + 2];

                                        gl_view.SetUint32(base_address, n1);
                                        base_address += 4;
                                        gl_view.SetUint32(base_address, n2);
                                        base_address += 4;
                                        gl_view.SetUint32(base_address, n3);
                                        base_address += 4;
                                }

                                for (uint32_t idx = 0; idx < triprops_len; ++idx)
                                {
                                        int prop = triprops[idx];
                                        gl_view.SetUint32(base_address, prop);
                                        base_address += 4;
                                }

                                for (uint32_t idx = 0; idx < lines_len; ++idx)
                                {
                                        int n1 = lines[idx * 2 + 0];
                                        int n2 = lines[idx * 2 + 1];

                                        gl_view.SetUint32(base_address, n1);
                                        base_address += 4;
                                        gl_view.SetUint32(base_address, n2);
                                        base_address += 4;
                                }

                                for (uint32_t idx = 0; idx < lineprops_len; ++idx)
                                {
                                        int prop = lineprops[idx];
                                        gl_view.SetUint32(base_address, prop);
                                        base_address += 4;
                                }

                                for (uint32_t idx = 0; idx < vertex_len; ++idx)
                                {
                                        int n = vertex[idx];
                                        gl_view.SetUint32(base_address, n);
                                        base_address += 4;
                                }

                                for (uint32_t idx = 0; idx < vertexprop_len; ++idx)
                                {
                                        int prop = vertexprops[idx];
                                        gl_view.SetUint32(base_address, prop);
                                        base_address += 4;
                                }

                                gl_view.SetUint32(base_address, red);
                                base_address += 4;

                                gl_view.SetUint32(base_address, green);
                                base_address += 4;

                                gl_view.SetUint32(base_address, blue);
                                base_address += 4;

                                return ret;
                        }
                        /*
                        inline Napi::Value mergeBuffer(Napi::Env env, Napi::Value buf1, Napi::Value buf2)
                        {
                          if(!buf1.IsArrayBuffer())
                            {
                        return Napi::Value::From(env, false);
                            }

                          if(!buf2.IsArrayBuffer())
                            {
                        return Napi::Value::From(env, false);
                            }

                          Napi::ArrayBuffer arr_buf1 = buf1.As<Napi::ArrayBuffer>();
                          Napi::ArrayBuffer arr_buf2 = buf2.As<Napi::ArrayBuffer>();

                          uint32_t totalLength = arr_buf1.ByteLength() + arr_buf2.ByteLength() - 4;
                          Napi::ArrayBuffer ret = Napi::ArrayBuffer::New(env, totalLength);
                          Napi::DataView    retView = Napi::DataView::New(env, ret);

                          Napi::DataView dv1 = Napi::DataView::New(env, arr_buf1);
                          Napi::DataView dv2 = Napi::DataView::New(env, arr_buf2);

                          retView.SetUint32(0, dv1.GetUint32(0) + dv2.GetUint32(0));

                          void *data1 = arr_buf1.Data();
                          void *data2 = arr_buf2.Data();
                          void *dataRet = ret.Data();

                          memcpy((void*)((uint8_t*)dataRet + 4),
                           (void*)((uint8_t*)data1 + 4),
                           arr_buf1.ByteLength() - 4);

                          memcpy((void*)((uint8_t*)dataRet + arr_buf1.ByteLength()),
                           (void*)((uint8_t*)data2 + 4),
                           arr_buf2.ByteLength() - 4);

                          return ret;
                        }
                        */
                }//namespace GL
                inline Napi::Function GetGlobalFunction(Napi::Env &env, const std::vector<std::string> &ns_func)
                {
                        //Napi::Env
                        Napi::Object parent = env.Global();

                        std::vector<std::string> ns(ns_func.begin(), ns_func.end() - 1);
                        for (auto iter = ns.begin(); iter < ns.end(); ++iter)
                        {
                                std::string cur_ns = *iter;
                                assert(parent.Has(cur_ns));

                                parent = parent.Get(cur_ns).As<Napi::Object>();
                        }

                        return parent.Get(*(ns_func.end() - 1)).As<Napi::Function>();
                }

                inline bool IsExistGlobalFunction(Napi::Env &env, const std::vector<std::string> &ns_func)
                {
                        //Napi::Env
                        Napi::Object parent = env.Global();

                        std::vector<std::string> ns(ns_func.begin(), ns_func.end() - 1);
                        for (auto iter = ns.begin(); iter < ns.end(); ++iter)
                        {
                                std::string cur_ns = *iter;
                                if (!parent.Has(cur_ns)) return false;

                                parent = parent.Get(cur_ns).As<Napi::Object>();
                        }

                        return parent.Has(*(ns_func.end() - 1));
                }

                inline Napi::Value InvokeGlobalFunction(Napi::Env &env, const std::vector<std::string> &ns_func, const std::initializer_list<napi_value>& args)
                {
                        if (!IsExistGlobalFunction(env, ns_func)) return env.Undefined();

                        return GetGlobalFunction(env, ns_func).Call(args);
                }

        }//namespace napi
}//namespace comx

typedef comx::napi::TCallbackWrap TComxCallback;
typedef comx::napi::TCallbackWrap::TBuffer TComxCallbackBuffer;
typedef comx::napi::TBuffer TComxBuffer;
#endif
