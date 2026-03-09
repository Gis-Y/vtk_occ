#ifndef _KMAS_STRUCT_STREAM_HXX_
#define _KMAS_STRUCT_STREAM_HXX_

#define DEF_STRUCT_STREAM1(type, num, para1) \
istream& operator>>(istream& istr, type &right) \
{\
	int n = 0; \
	istr >> n; \
	assert(n == num); \
	\
	istr >> right.para1;\
	\
	return istr;\
}\
\
ostream& operator<<(ostream&ostr, type &right) \
{\
	ostr << num << " ";\
	ostr << right.para1;\
	\
	return ostr;\
}

#define DEF_STRUCT_STREAM2(type, num, para1, para2) \
	istream& operator>>(istream& istr, type &right) \
{\
	int n = 0; \
	istr >> n; \
	assert(n == num); \
	\
	istr >> right.para1;\
	istr >> right.para2;\
	\
	return istr;\
}\
	\
ostream& operator<<(ostream&ostr, type &right) \
{\
	ostr << num << " ";\
	ostr << right.para1 << " ";\
	ostr << right.para2;\
	\
	return ostr;\
}

#define DEF_STRUCT_STREAM3(type, num, para1, para2, para3) \
	istream& operator>>(istream& istr, type &right) \
{\
	int n = 0; \
	istr >> n; \
	assert(n == num); \
	\
	istr >> right.para1;\
	istr >> right.para2;\
	istr >> right.para3;\
	\
	return istr;\
}\
	\
ostream& operator<<(ostream&ostr, type &right) \
{\
	ostr << num << " ";\
	ostr << right.para1 << " ";\
	ostr << right.para2 << " ";\
	ostr << right.para3;\
	\
	return ostr;\
}

#define DEF_STRUCT_STREAM4(type, num, para1, para2, para3, para4) \
	istream& operator>>(istream& istr, type &right) \
{\
	int n = 0; \
	istr >> n; \
	assert(n == num); \
	\
	istr >> right.para1;\
	istr >> right.para2;\
	istr >> right.para3;\
	istr >> right.para4;\
	\
	return istr;\
}\
	\
ostream& operator<<(ostream&ostr, type &right) \
{\
	ostr << num << " ";\
	ostr << right.para1 << " ";\
	ostr << right.para2 << " ";\
	ostr << right.para3 << " ";\
	ostr << right.para4;\
	\
	return ostr;\
}

#define DEF_STRUCT_STREAM5(type, num, para1, para2, para3, para4, para5) \
	istream& operator>>(istream& istr, type &right) \
{\
	int n = 0; \
	istr >> n; \
	assert(n == num); \
	\
	istr >> right.para1;\
	istr >> right.para2;\
	istr >> right.para3;\
	istr >> right.para4;\
	istr >> right.para5;\
	\
	return istr;\
}\
	\
	ostream& operator<<(ostream&ostr, type &right) \
{\
	ostr << num << " ";\
	ostr << right.para1 << " ";\
	ostr << right.para2 << " ";\
	ostr << right.para3 << " ";\
	ostr << right.para4 << " ";\
	ostr << right.para5;\
	\
	return ostr;\
}

#define DEF_STRUCT_STREAM6(type, num, para1, para2, para3, para4, para5, para6) \
	istream& operator>>(istream& istr, type &right) \
{\
	int n = 0; \
	istr >> n; \
	assert(n == num); \
	\
	istr >> right.para1;\
	istr >> right.para2;\
	istr >> right.para3;\
	istr >> right.para4;\
	istr >> right.para5;\
    istr >> right.para6;\
	\
	return istr;\
}\
	\
	ostream& operator<<(ostream&ostr, type &right) \
{\
	ostr << num << " ";\
	ostr << right.para1 << " ";\
	ostr << right.para2 << " ";\
	ostr << right.para3 << " ";\
	ostr << right.para4 << " ";\
	ostr << right.para5 << " ";\
	ostr << right.para6;\
	\
	return ostr;\
}
#define DEF_STRUCT_STREAM7(type, num, para1, para2, para3, para4, para5, para6, para7) \
	istream& operator>>(istream& istr, type &right) \
{\
	int n = 0; \
	istr >> n; \
	assert(n == num); \
	\
	istr >> right.para1;\
	istr >> right.para2;\
	istr >> right.para3;\
	istr >> right.para4;\
	istr >> right.para5;\
    istr >> right.para6;\
	istr >> right.para7;\
	\
	return istr;\
}\
	\
	ostream& operator<<(ostream&ostr, type &right) \
{\
	ostr << num << " ";\
	ostr << right.para1 << " ";\
	ostr << right.para2 << " ";\
	ostr << right.para3 << " ";\
	ostr << right.para4 << " ";\
	ostr << right.para5 << " ";\
	ostr << right.para6 << " ";\
	ostr << right.para7;\
	\
	return ostr;\
}
#define DEF_STRUCT_STREAM8(type, num, para1, para2, para3, para4, para5, para6, para7, para8) \
	istream& operator>>(istream& istr, type &right) \
{\
	int n = 0; \
	istr >> n; \
	assert(n == num); \
	\
	istr >> right.para1;\
	istr >> right.para2;\
	istr >> right.para3;\
	istr >> right.para4;\
	istr >> right.para5;\
    istr >> right.para6;\
	istr >> right.para7;\
	istr >> right.para8;\
	\
	return istr;\
}\
	\
	ostream& operator<<(ostream&ostr, type &right) \
{\
	ostr << num << " ";\
	ostr << right.para1 << " ";\
	ostr << right.para2 << " ";\
	ostr << right.para3 << " ";\
	ostr << right.para4 << " ";\
	ostr << right.para5 << " ";\
	ostr << right.para6 << " ";\
	ostr << right.para7 << " ";\
	ostr << right.para8;\
	\
	return ostr;\
}
#define DEF_STRUCT_STREAM9(type, num, para1, para2, para3, para4, para5, para6, para7, para8, para9) \
	istream& operator>>(istream& istr, type &right) \
{\
	int n = 0; \
	istr >> n; \
	assert(n == num); \
	\
	istr >> right.para1;\
	istr >> right.para2;\
	istr >> right.para3;\
	istr >> right.para4;\
	istr >> right.para5;\
    istr >> right.para6;\
	istr >> right.para7;\
	istr >> right.para8;\
	istr >> right.para9;\
	\
	return istr;\
}\
	\
	ostream& operator<<(ostream&ostr, type &right) \
{\
	ostr << num << " ";\
	ostr << right.para1 << " ";\
	ostr << right.para2 << " ";\
	ostr << right.para3 << " ";\
	ostr << right.para4 << " ";\
	ostr << right.para5 << " ";\
	ostr << right.para6 << " ";\
	ostr << right.para7 << " ";\
	ostr << right.para8 << " ";\
	ostr << right.para9;\
	\
	return ostr;\
}
#define DEF_STRUCT_STREAM10(type, num, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10) \
	istream& operator>>(istream& istr, type &right) \
{\
	int n = 0; \
	istr >> n; \
	assert(n == num); \
	\
	istr >> right.para1;\
	istr >> right.para2;\
	istr >> right.para3;\
	istr >> right.para4;\
	istr >> right.para5;\
    istr >> right.para6;\
	istr >> right.para7;\
	istr >> right.para8;\
	istr >> right.para9;\
	istr >> right.para10;\
	\
	return istr;\
}\
	\
	ostream& operator<<(ostream&ostr, type &right) \
{\
	ostr << num << " ";\
	ostr << right.para1 << " ";\
	ostr << right.para2 << " ";\
	ostr << right.para3 << " ";\
	ostr << right.para4 << " ";\
	ostr << right.para5 << " ";\
	ostr << right.para6 << " ";\
	ostr << right.para7 << " ";\
	ostr << right.para8 << " ";\
	ostr << right.para9 << " ";\
	ostr << right.para10;\
	\
	return ostr;\
}
#define DEF_STRUCT_STREAM11(type, num, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11) \
	istream& operator>>(istream& istr, type &right) \
{\
	int n = 0; \
	istr >> n; \
	assert(n == num); \
	\
	istr >> right.para1;\
	istr >> right.para2;\
	istr >> right.para3;\
	istr >> right.para4;\
	istr >> right.para5;\
    istr >> right.para6;\
	istr >> right.para7;\
	istr >> right.para8;\
	istr >> right.para9;\
	istr >> right.para10;\
	istr >> right.para11;\
	\
	return istr;\
}\
	\
	ostream& operator<<(ostream&ostr, type &right) \
{\
	ostr << num << " ";\
	ostr << right.para1 << " ";\
	ostr << right.para2 << " ";\
	ostr << right.para3 << " ";\
	ostr << right.para4 << " ";\
	ostr << right.para5 << " ";\
	ostr << right.para6 << " ";\
	ostr << right.para7 << " ";\
	ostr << right.para8 << " ";\
	ostr << right.para9 << " ";\
	ostr << right.para10 << " ";\
	ostr << right.para11;\
	\
	return ostr;\
}
#define DEF_STRUCT_STREAM12(type, num, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12) \
	istream& operator>>(istream& istr, type &right) \
{\
	int n = 0; \
	istr >> n; \
	assert(n == num); \
	\
	istr >> right.para1;\
	istr >> right.para2;\
	istr >> right.para3;\
	istr >> right.para4;\
	istr >> right.para5;\
    istr >> right.para6;\
	istr >> right.para7;\
	istr >> right.para8;\
	istr >> right.para9;\
	istr >> right.para10;\
	istr >> right.para11;\
	istr >> right.para12;\
	\
	return istr;\
}\
	\
	ostream& operator<<(ostream&ostr, type &right) \
{\
	ostr << num << " ";\
	ostr << right.para1 << " ";\
	ostr << right.para2 << " ";\
	ostr << right.para3 << " ";\
	ostr << right.para4 << " ";\
	ostr << right.para5 << " ";\
	ostr << right.para6 << " ";\
	ostr << right.para7 << " ";\
	ostr << right.para8 << " ";\
	ostr << right.para9 << " ";\
	ostr << right.para10 << " ";\
	ostr << right.para11 << " ";\
	ostr << right.para12;\
	\
	return ostr;\
}
#define DEF_STRUCT_STREAM13(type, num, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13) \
	istream& operator>>(istream& istr, type &right) \
{\
	int n = 0; \
	istr >> n; \
	assert(n == num); \
	\
	istr >> right.para1;\
	istr >> right.para2;\
	istr >> right.para3;\
	istr >> right.para4;\
	istr >> right.para5;\
    istr >> right.para6;\
	istr >> right.para7;\
	istr >> right.para8;\
	istr >> right.para9;\
	istr >> right.para10;\
	istr >> right.para11;\
	istr >> right.para12;\
	istr >> right.para13;\
	\
	return istr;\
}\
	\
	ostream& operator<<(ostream&ostr, type &right) \
{\
	ostr << num << " ";\
	ostr << right.para1 << " ";\
	ostr << right.para2 << " ";\
	ostr << right.para3 << " ";\
	ostr << right.para4 << " ";\
	ostr << right.para5 << " ";\
	ostr << right.para6 << " ";\
	ostr << right.para7 << " ";\
	ostr << right.para8 << " ";\
	ostr << right.para9 << " ";\
	ostr << right.para10 << " ";\
	ostr << right.para11 << " ";\
	ostr << right.para12 << " ";\
	ostr << right.para13;\
	\
	return ostr;\
}
#define DEF_STRUCT_STREAM14(type, num, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14) \
	istream& operator>>(istream& istr, type &right) \
{\
	int n = 0; \
	istr >> n; \
	assert(n == num); \
	\
	istr >> right.para1;\
	istr >> right.para2;\
	istr >> right.para3;\
	istr >> right.para4;\
	istr >> right.para5;\
    istr >> right.para6;\
	istr >> right.para7;\
	istr >> right.para8;\
	istr >> right.para9;\
	istr >> right.para10;\
	istr >> right.para11;\
	istr >> right.para12;\
	istr >> right.para13;\
	istr >> right.para14;\
	\
	return istr;\
}\
	\
	ostream& operator<<(ostream&ostr, type &right) \
{\
	ostr << num << " ";\
	ostr << right.para1 << " ";\
	ostr << right.para2 << " ";\
	ostr << right.para3 << " ";\
	ostr << right.para4 << " ";\
	ostr << right.para5 << " ";\
	ostr << right.para6 << " ";\
	ostr << right.para7 << " ";\
	ostr << right.para8 << " ";\
	ostr << right.para9 << " ";\
	ostr << right.para10 << " ";\
	ostr << right.para11 << " ";\
	ostr << right.para12 << " ";\
	ostr << right.para13 << " ";\
	ostr << right.para14;\
	\
	return ostr;\
}
#define DEF_STRUCT_STREAM15(type, num, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14, para15) \
	istream& operator>>(istream& istr, type &right) \
{\
	int n = 0; \
	istr >> n; \
	assert(n == num); \
	\
	istr >> right.para1;\
	istr >> right.para2;\
	istr >> right.para3;\
	istr >> right.para4;\
	istr >> right.para5;\
    istr >> right.para6;\
	istr >> right.para7;\
	istr >> right.para8;\
	istr >> right.para9;\
	istr >> right.para10;\
	istr >> right.para11;\
	istr >> right.para12;\
	istr >> right.para13;\
	istr >> right.para14;\
	istr >> right.para15;\
	\
	return istr;\
}\
	\
	ostream& operator<<(ostream&ostr, type &right) \
{\
	ostr << num << " ";\
	ostr << right.para1 << " ";\
	ostr << right.para2 << " ";\
	ostr << right.para3 << " ";\
	ostr << right.para4 << " ";\
	ostr << right.para5 << " ";\
	ostr << right.para6 << " ";\
	ostr << right.para7 << " ";\
	ostr << right.para8 << " ";\
	ostr << right.para9 << " ";\
	ostr << right.para10 << " ";\
	ostr << right.para11 << " ";\
	ostr << right.para12 << " ";\
	ostr << right.para13 << " ";\
	ostr << right.para14 << " ";\
	ostr << right.para15;\
	\
	return ostr;\
}
#define DEF_STRUCT_STREAM16(type, num, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14, para15, para16) \
	istream& operator>>(istream& istr, type &right) \
{\
	int n = 0; \
	istr >> n; \
	assert(n == num); \
	\
	istr >> right.para1;\
	istr >> right.para2;\
	istr >> right.para3;\
	istr >> right.para4;\
	istr >> right.para5;\
    istr >> right.para6;\
	istr >> right.para7;\
	istr >> right.para8;\
	istr >> right.para9;\
	istr >> right.para10;\
	istr >> right.para11;\
	istr >> right.para12;\
	istr >> right.para13;\
	istr >> right.para14;\
	istr >> right.para15;\
	istr >> right.para16;\
	\
	return istr;\
}\
	\
	ostream& operator<<(ostream&ostr, type &right) \
{\
	ostr << num << " ";\
	ostr << right.para1 << " ";\
	ostr << right.para2 << " ";\
	ostr << right.para3 << " ";\
	ostr << right.para4 << " ";\
	ostr << right.para5 << " ";\
	ostr << right.para6 << " ";\
	ostr << right.para7 << " ";\
	ostr << right.para8 << " ";\
	ostr << right.para9 << " ";\
	ostr << right.para10 << " ";\
	ostr << right.para11 << " ";\
	ostr << right.para12 << " ";\
	ostr << right.para13 << " ";\
	ostr << right.para14 << " ";\
	ostr << right.para15 << " ";\
	ostr << right.para16;\
	\
	return ostr;\
}
#define DEF_STRUCT_STREAM17(type, num, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14, para15, para16, para17) \
	istream& operator>>(istream& istr, type &right) \
{\
	int n = 0; \
	istr >> n; \
	assert(n == num); \
	\
	istr >> right.para1;\
	istr >> right.para2;\
	istr >> right.para3;\
	istr >> right.para4;\
	istr >> right.para5;\
    istr >> right.para6;\
	istr >> right.para7;\
	istr >> right.para8;\
	istr >> right.para9;\
	istr >> right.para10;\
	istr >> right.para11;\
	istr >> right.para12;\
	istr >> right.para13;\
	istr >> right.para14;\
	istr >> right.para15;\
	istr >> right.para16;\
	istr >> right.para17;\
	\
	return istr;\
}\
	\
	ostream& operator<<(ostream&ostr, type &right) \
{\
	ostr << num << " ";\
	ostr << right.para1 << " ";\
	ostr << right.para2 << " ";\
	ostr << right.para3 << " ";\
	ostr << right.para4 << " ";\
	ostr << right.para5 << " ";\
	ostr << right.para6 << " ";\
	ostr << right.para7 << " ";\
	ostr << right.para8 << " ";\
	ostr << right.para9 << " ";\
	ostr << right.para10 << " ";\
	ostr << right.para11 << " ";\
	ostr << right.para12 << " ";\
	ostr << right.para13 << " ";\
	ostr << right.para14 << " ";\
	ostr << right.para15 << " ";\
	ostr << right.para16 << " ";\
	ostr << right.para17;\
	\
	return ostr;\
}
#define DEF_STRUCT_STREAM18(type, num, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14, para15, para16, para17, para18) \
	istream& operator>>(istream& istr, type &right) \
{\
	int n = 0; \
	istr >> n; \
	assert(n == num); \
	\
	istr >> right.para1;\
	istr >> right.para2;\
	istr >> right.para3;\
	istr >> right.para4;\
	istr >> right.para5;\
    istr >> right.para6;\
	istr >> right.para7;\
	istr >> right.para8;\
	istr >> right.para9;\
	istr >> right.para10;\
	istr >> right.para11;\
	istr >> right.para12;\
	istr >> right.para13;\
	istr >> right.para14;\
	istr >> right.para15;\
	istr >> right.para16;\
	istr >> right.para17;\
	istr >> right.para18;\
	\
	return istr;\
}\
	\
	ostream& operator<<(ostream&ostr, type &right) \
{\
	ostr << num << " ";\
	ostr << right.para1 << " ";\
	ostr << right.para2 << " ";\
	ostr << right.para3 << " ";\
	ostr << right.para4 << " ";\
	ostr << right.para5 << " ";\
	ostr << right.para6 << " ";\
	ostr << right.para7 << " ";\
	ostr << right.para8 << " ";\
	ostr << right.para9 << " ";\
	ostr << right.para10 << " ";\
	ostr << right.para11 << " ";\
	ostr << right.para12 << " ";\
	ostr << right.para13 << " ";\
	ostr << right.para14 << " ";\
	ostr << right.para15 << " ";\
	ostr << right.para16 << " ";\
	ostr << right.para17 << " ";\
	ostr << right.para18;\
	\
	return ostr;\
}
#define DEF_STRUCT_STREAM19(type, num, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14, para15, para16, para17, para18, para19) \
	istream& operator>>(istream& istr, type &right) \
{\
	int n = 0; \
	istr >> n; \
	assert(n == num); \
	\
	istr >> right.para1;\
	istr >> right.para2;\
	istr >> right.para3;\
	istr >> right.para4;\
	istr >> right.para5;\
    istr >> right.para6;\
	istr >> right.para7;\
	istr >> right.para8;\
	istr >> right.para9;\
	istr >> right.para10;\
	istr >> right.para11;\
	istr >> right.para12;\
	istr >> right.para13;\
	istr >> right.para14;\
	istr >> right.para15;\
	istr >> right.para16;\
	istr >> right.para17;\
	istr >> right.para18;\
	istr >> right.para19;\
	\
	return istr;\
}\
	\
	ostream& operator<<(ostream&ostr, type &right) \
{\
	ostr << num << " ";\
	ostr << right.para1 << " ";\
	ostr << right.para2 << " ";\
	ostr << right.para3 << " ";\
	ostr << right.para4 << " ";\
	ostr << right.para5 << " ";\
	ostr << right.para6 << " ";\
	ostr << right.para7 << " ";\
	ostr << right.para8 << " ";\
	ostr << right.para9 << " ";\
	ostr << right.para10 << " ";\
	ostr << right.para11 << " ";\
	ostr << right.para12 << " ";\
	ostr << right.para13 << " ";\
	ostr << right.para14 << " ";\
	ostr << right.para15 << " ";\
	ostr << right.para16 << " ";\
	ostr << right.para17 << " ";\
	ostr << right.para18 << " ";\
	ostr << right.para19;\
	\
	return ostr;\
}
#define DEF_STRUCT_STREAM20(type, num, para1, para2, para3, para4, para5, para6, para7, para8, para9, para10, para11, para12, para13, para14, para15, para16, para17, para18, para19, para20) \
	istream& operator>>(istream& istr, type &right) \
{\
	int n = 0; \
	istr >> n; \
	assert(n == num); \
	\
	istr >> right.para1;\
	istr >> right.para2;\
	istr >> right.para3;\
	istr >> right.para4;\
	istr >> right.para5;\
    istr >> right.para6;\
	istr >> right.para7;\
	istr >> right.para8;\
	istr >> right.para9;\
	istr >> right.para10;\
	istr >> right.para11;\
	istr >> right.para12;\
	istr >> right.para13;\
	istr >> right.para14;\
	istr >> right.para15;\
	istr >> right.para16;\
	istr >> right.para17;\
	istr >> right.para18;\
	istr >> right.para19;\
	istr >> right.para20;\
	\
	return istr;\
}\
	\
	ostream& operator<<(ostream&ostr, type &right) \
{\
	ostr << num << " ";\
	ostr << right.para1 << " ";\
	ostr << right.para2 << " ";\
	ostr << right.para3 << " ";\
	ostr << right.para4 << " ";\
	ostr << right.para5 << " ";\
	ostr << right.para6 << " ";\
	ostr << right.para7 << " ";\
	ostr << right.para8 << " ";\
	ostr << right.para9 << " ";\
	ostr << right.para10 << " ";\
	ostr << right.para11 << " ";\
	ostr << right.para12 << " ";\
	ostr << right.para13 << " ";\
	ostr << right.para14 << " ";\
	ostr << right.para15 << " ";\
	ostr << right.para16 << " ";\
	ostr << right.para17 << " ";\
	ostr << right.para18 << " ";\
	ostr << right.para19 << " ";\
	ostr << right.para20;\
	\
	return ostr;\
}

#endif
