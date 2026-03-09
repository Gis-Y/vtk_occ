#include <comx_napi.hxx>
#include <type_cast.hxx>
using namespace KMAS::type;

#include <vector>
#include <string>
#include <map>
#include <set>

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


#define TEST2_FUNC_USAGE "test2 Usage: comx_sdk.test2(/*put your parameters list here*/);"
JS_EXT_FUNC_BEGIN(test2, 2, TEST2_FUNC_USAGE)
{
        //Put your codes here

}
JS_EXT_FUNC_END()


#define TEST1_FUNC_USAGE "test1 Usage: comx_sdk.test1(/*put your parameters list here*/);"
JS_EXT_FUNC_BEGIN(test1, 4, TEST1_FUNC_USAGE)
{
        //Put your codes here

}
JS_EXT_FUNC_END()

//////////////////////////////////////////////////////////////////////////////////////
// please set your javascript extension namespace in the following codes.

#define JS_EXT_NS "comx.test"

//////////////////////////////////////////////////////////////////////////////////////
// entry segment, please replace your function name in the following codes.

JS_EXT_ENTRY_BEGIN()
JS_EXT_ENTRY(test2)
JS_EXT_ENTRY(test1)
JS_EXT_ENTRY_END()

JS_EXT_MAIN_BEGIN(JS_EXT_NS, 2)
JS_EXT_FUNC_REG(test2)
JS_EXT_FUNC_REG(test1)
JS_EXT_MAIN_END()

