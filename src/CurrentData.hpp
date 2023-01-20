#include <ncurses.h>
#include <string>

class CurrentData
{
    WINDOW *_parent;
    std::string _text;

    public:
    CurrentData(WINDOW *parent = stdscr)
    {
        _parent = parent;
        _text = "";
    }

    std::string getText()
    {
        return _text;
    }

    void setText(std::string text, attr_t attributes = 0)
    {
        _text = text;
        if (attributes != 0)
        {
            _attributes = attributes;
        }
    }
    
    void draw()
    {
        
    }
};
