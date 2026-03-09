#include <comx_napi.hxx>
#include <type_cast.hxx>
#include <base/function.hxx>

using namespace KMAS::type;

#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <iterator>

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////
/* DCiP plugin Javascript parameter and return value parsing/wrap guide             

   1)Parse callback paramter by index from javascript by using the following line:  
     var cb = JS_EXT_PARA(TComxCallback, [index]);                                  

     Use the following codes to invokd callback function:                           
     cb([parameters list]);                                                         

     cb also can be saved as a global variable.                                     

   2)Parse common paramter by index from javascript by using the following line:    
     var val = JS_EXT_PARA([value_type], [index]);                                  

   3)Parse buffer paramter by index from javascript by using the following line:    
     TComxBuffer buf = JS_EXT_PARA(TComxBuffer, [index]);                           

     Use the following codes to operate buffer:                                     

        void * arr_buf = buf.Data();                                                
        size_t arr_byte_size = buf.ByteLength();                                    

        int16_t *arr_data = (int16_t*)arr_buf;                                      
        size_t arr_len = arr_byte_size / sizeof(int16_t);                           

        for (size_t i = 0; i < arr_len; i++)                                        
        {
             arr_data[i] += 2;
        }

   4)Return a buffer by using the following line:
     JS_EXT_FUNC_ASSIGN_RET_EX(buf, 1);

   5)Return a common value by using the follwoing line
     JS_EXT_FUNC_ASSIGN_RET([value]);
*/

//////////////////////////////////////////////////////////////////////////////////////
// system pre-define segment, please don't modify the following codes.

JS_EXT_DATA_DECLARE()

//0x4cc0c89c-0x0090-0x47e3-0xb7-0x63-0xc1-0xaf-0xf1-0xe7-0x99-0x4e
//please don't modify or delete the previous line codes.


#define HAVEFREENODESINMODEL_FUNC_USAGE "haveFreeNodesInModel Usage: comx_sdk.haveFreeNodesInModel(buf, nlen, tri_offset);"
JS_EXT_FUNC_BEGIN(haveFreeNodesInModel, 3, HAVEFREENODESINMODEL_FUNC_USAGE)
{
        //Put your codes here
        string strBufferStorage = JS_EXT_PARA(string, 0);
        int nlen                = JS_EXT_PARA(int,    1);
        int tri_offset          = JS_EXT_PARA(int,    2);
        
        ifstream ifs(strBufferStorage, ios::in|ios::binary|ios::ate);

        long buf_size  = ifs.tellg();
        
        char *buffer = new char[buf_size];
        
        ifs.seekg(0, ios::beg);
        ifs.read(buffer, buf_size);
        
        int *triangle_buf = (int*)(buffer + tri_offset);
                
        set<int> triangles;
        int tri_num = (buf_size - tri_offset) / sizeof(int);
        copy(triangle_buf, triangle_buf + tri_num, inserter(triangles, triangles.end()));
        
        bool ret = triangles.size() != nlen;
        
        ifs.close();
        
        return Napi::Value::From(info.Env(), ret);
}
JS_EXT_FUNC_END()

#define GENERATEPOSTCANVASPLOTBUFFER_FUNC_USAGE "generatePostCanvasPlotBuffer Usage: comx_sdk.generatePostCanvasPlotBuffer(buf, nlen, vindex, tri_offset);"
JS_EXT_FUNC_BEGIN(generatePostCanvasPlotBuffer, 6, GENERATEPOSTCANVASPLOTBUFFER_FUNC_USAGE)
{
        //Put your codes here
        string strBufferStorage = JS_EXT_PARA(string, 0);
        int nlen                = JS_EXT_PARA(int,    1);
        int vindex              = JS_EXT_PARA(int,    2);
        int tri_offset          = JS_EXT_PARA(int,    3);
        double max_val          = JS_EXT_PARA(double, 4);
        double min_val          = JS_EXT_PARA(double, 5);
        
        ifstream ifs(strBufferStorage, ios::in|ios::binary|ios::ate);

        long buf_size  = ifs.tellg();
        
        char *buffer = new char[buf_size];
        
        ifs.seekg(0, ios::beg);
        ifs.read(buffer, buf_size);
        double *pts_buf = (double*)buffer;
        
        vector<double> pts;
        vector<double> variables;
                
        int tri_num = (buf_size - tri_offset) / sizeof(int);
        int *triangle_buf = (int*)(buffer + tri_offset);
                
        set<int> triangles;
        copy(triangle_buf, triangle_buf + tri_num, inserter(triangles, triangles.end()));
        
        copy(pts_buf, pts_buf + nlen * 3, back_inserter(pts));
        
        int var_offset = (nlen * 3 + nlen * vindex) * sizeof(double);
        double *var_buf = (double*)(buffer + var_offset);
        copy(var_buf, var_buf + nlen, back_inserter(variables));
        
        vector<double> pts_filter;
        vector<double> variables_filter;
                
        for(auto nid = 0; nid < variables.size(); ++nid)
        {
                if(triangles.find(nid) != triangles.end())
                {
                        pts_filter.push_back(pts[nid * 3 + 0]);
                        pts_filter.push_back(pts[nid * 3 + 1]);
                        pts_filter.push_back(pts[nid * 3 + 2]);
                        
                        variables_filter.push_back(variables[nid]);
                }
        }
        
        delete []buffer;
        ifs.close();
        
        if(tri_num == 0)
        {
                return comx::napi::GL::createPlotBufferEx(info.Env(), pts, variables, max_val, min_val);
        }
        else
        {
                return comx::napi::GL::createPlotBufferEx(info.Env(), pts_filter, variables_filter, max_val, min_val);
        }
}
JS_EXT_FUNC_END()

int check_sys(){                                                                                                                                                          
    int a =0x1122;                                                                                                                                                        
    char *p =(char *) &a;                                                                                                                                                 
    return (int)*p;                                                                                                                                                       
}    

#define GENERATEPOSTCANVASBUFFER_FUNC_USAGE "generatePostCanvasBuffer Usage: comx_sdk.generatePostCanvasBuffer(buf, nlen, elen, tri_offset, mid, sid);"
JS_EXT_FUNC_BEGIN(generatePostCanvasBuffer, 6, GENERATEPOSTCANVASBUFFER_FUNC_USAGE)
{
        string strBufferStorage = JS_EXT_PARA(string, 0);
                                
        int nlen = JS_EXT_PARA(int, 1);
        int elen = JS_EXT_PARA(int, 2);
       
        int tri_offset = JS_EXT_PARA(int, 3);
        
        int mid = JS_EXT_PARA(int, 4);
        int sid = JS_EXT_PARA(int, 5);
        
        ifstream ifs(strBufferStorage, ios::in|ios::binary|ios::ate);

        long buf_size  = ifs.tellg();
        
        int tri_num = (buf_size - tri_offset) / sizeof(int);
        //cout << tri_num << endl;
        
        char *buffer = new char[buf_size];
        
        ifs.seekg(0, ios::beg);
        ifs.read(buffer, buf_size);
        double *pts_buf = (double*)buffer;
        int *triangle_buf = (int*)(buffer + tri_offset);
                
        vector<double> pts;
        vector<int> triangles;

        copy(pts_buf, pts_buf + nlen * 3, back_inserter(pts));
        copy(triangle_buf, triangle_buf + /*elen * 2 * 3*/tri_num, back_inserter(triangles));
        
        set<int> triangles_set;
        copy(triangles.begin(), triangles.end(), inserter(triangles_set, triangles_set.end()));
        
        bool bHaveFreeNode = triangles_set.size() != nlen;
        
        //set<int> triangles_set;
        //copy(triangle_buf, triangle_buf + tri_num, inserter(triangles_set, triangles_set.end()));
        
        delete []buffer;
        
        ifs.close();
        
        vector<int> vertexs;
        
        if(bHaveFreeNode || elen == 0)
        {
                for(auto nid = 0; nid < pts.size() / 3; ++ nid)
                {
                        vertexs.push_back(nid);
                }
        }
        
        Napi::Array ret = Napi::Array::New(info.Env());
        Napi::Object retObj = ret.As<Napi::Object>();
        
        bool auto_normal = true;
        
        if(triangles.size() == 0)
        {
                uint32_t arr_idx = 0;
                Napi::Value rec = comx::napi::GL::createBuffer(info.Env(), mid, sid, pts, {}, {}, {}, {}, {}, vertexs, {}, auto_normal);
                retObj.Set(arr_idx, rec);
                return ret;
        }
        else if(vertexs.size() != 0)
        {
                uint32_t arr_idx = 0;
                Napi::Value rec1 = comx::napi::GL::createBuffer(info.Env(), mid, sid, pts, {}, triangles, {}, {}, {}, {}, {}, auto_normal); 
                Napi::Value rec2 = comx::napi::GL::createBuffer(info.Env(), mid+1, sid, pts, {}, {}, {}, {}, {}, vertexs, {}, auto_normal);
                retObj.Set(arr_idx++, rec1);
                retObj.Set(arr_idx, rec2);
                return ret;
        }
        else
        {
                uint32_t arr_idx = 0;
                Napi::Value rec = comx::napi::GL::createBuffer(info.Env(), mid, sid, pts, {}, triangles, {}, {}, {}, {}, {}, auto_normal);
                retObj.Set(arr_idx, rec);
                return ret;
        }
}
JS_EXT_FUNC_END()

//////////////////////////////////////////////////////////////////////////////////////
// please set your javascript extension namespace in the following codes.

#define JS_EXT_NS "comx.fluid_buffer"

//////////////////////////////////////////////////////////////////////////////////////
// entry segment, please replace your function name in the following codes.

JS_EXT_ENTRY_BEGIN()
JS_EXT_ENTRY(haveFreeNodesInModel)
JS_EXT_ENTRY(generatePostCanvasPlotBuffer)
JS_EXT_ENTRY(generatePostCanvasBuffer)
JS_EXT_ENTRY_END()

JS_EXT_MAIN_BEGIN(JS_EXT_NS, 3)
JS_EXT_FUNC_REG(haveFreeNodesInModel)
JS_EXT_FUNC_REG(generatePostCanvasPlotBuffer)
JS_EXT_FUNC_REG(generatePostCanvasBuffer)
JS_EXT_MAIN_END()

