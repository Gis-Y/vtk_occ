#ifndef A_91D4C9509D46478390A9C7B07B0FA6B4
#define A_91D4C9509D46478390A9C7B07B0FA6B4

#ifdef _WIN32
#undef MK_LBUTTON
#undef MK_RBUTTON
#undef MK_SHIFT
#undef MK_CONTROL
#undef MK_MBUTTON
#undef MK_ALT
#endif

enum TMouseKeyFlags
{
        MK_NONE = 0,
        MK_LBUTTON = 1,
        MK_RBUTTON = 2,
        MK_SHIFT = 4,
        MK_CONTROL = 8,
        MK_MBUTTON = 16,
        MK_ALT = 32,
        MK_META = 64,
        MK_KEYPAD = 128,      //key only
        MK_GROUP_SWITCH = 256 //key only
};

class TMouseEvent {
public:
        enum TAction {
                LPRESS,
                LRELEASE,
                MPRESS,
                MRELEASE,
                RPRESS,
                RRELEASE,
                MOTION,
                WHEEL_INCREMENT,
                WHEEL_DECREMENT
        };
private:
        unsigned int _flags;
        int _x, _y;
        TAction _action;
public:
        TMouseEvent(unsigned int flags, int x, int y, TAction action) :
                _flags(flags), _x(x), _y(y), _action(action) {}
        ~TMouseEvent() {}
public:
        const unsigned int &Flags()const { return _flags; }
        void Flags(const unsigned int &flags) { _flags = flags; }
        const int &X()const { return _x; }
        void X(const int &x) { _x = x; }
        const int &Y()const { return _y; }
        void Y(const int &y) { _y = y; }
        const TAction &Action()const { return _action; }
        void Action(const TAction &action) { _action = action; }
};

class TKeyEvent {
public:
    enum TAction {
        PRESS,
        RELEASE
    };
private:
    unsigned int _flags;
    int _key;
    TAction _action;
    char _szText[10];
    int _x, _y;
public:
    TKeyEvent(unsigned int flags, int x, int y, int key, char *szText, TAction action) :
        _flags(flags), _x(x), _y(y), _key(key), _action(action) {
        strcpy(_szText, szText);
    }
    ~TKeyEvent() {}
public:
    const unsigned int &Flags()const { return _flags; }
    void Flags(const unsigned int &flags) { _flags = flags; }
    const int &Key()const { return _key; }
    void Key(const int &key) { _key = key; }
    const char *text()const { return _szText; }
    void text(const char* szText) { strcpy(_szText, szText); }
    const TAction &Action()const { return _action; }
    void Action(const TAction &action) { _action = action; }
    const int &X()const { return _x; }
    void X(const int &x) { _x = x; }
    const int &Y()const { return _y; }
    void Y(const int &y) { _y = y; }
};

#endif