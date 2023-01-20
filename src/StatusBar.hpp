#include <ncurses.h>
#include <string>

class StatusBar
{
    WINDOW *_parent;
    std::string _text;
    std::string _textstatus;
    std::string _textlastupdate;
    attr_t _attributes;

public:
    StatusBar(WINDOW *parent = stdscr)
    {
        _parent = parent;
        _text = "";
        _textstatus = "";
        _textlastupdate = "";
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

    
    std::string getTextStatus()
    {
        return _textstatus;
    }

    void setTextStatus(std::string textstatus, attr_t attributes = 0)
    {
        _textstatus = textstatus;
        if (attributes != 0)
        {
            _attributes = attributes;
        }
    }

    std::string getLastUpdate()
    {
        return _textlastupdate;
    }

    void setLastUpdate(std::string textlastupdate, attr_t attributes = 0)
    {
        _textlastupdate = textlastupdate;
        if (attributes != 0)
        {
            _attributes = attributes;
        }
    }


    attr_t getAttributes()
    {
        return _attributes;
    }

    void setAttributes(attr_t attributes)
    {
        _attributes = attributes;
    }


    void draw()
    {
        int output_row = _parent->_maxy;
        attron(_attributes);
        //mvwprintw(_parent, output_row,0, getText().c_str());
        mvwprintw(_parent, output_row,0, getTextStatus().c_str());
        attroff(_attributes);
    }
    
    void drawBg()
    {
        int output_row = _parent->_maxy;
        int x_max = getmaxx(_parent);
        attron(_attributes);
        for (int i = 0; i < x_max; i++) {
             mvwaddch(_parent, output_row, i, ' ' | A_REVERSE);
        }
        attroff(_attributes);
    }

    void drawLastUpdate()
    {
        int output_row = _parent->_maxy;
        int x_max = getmaxx(_parent);
        attron(_attributes);
        mvwaddstr(_parent,output_row,x_max - 20, getLastUpdate().c_str());
        attroff(_attributes);
    }

     void drawKey(std::string teste,int x)
    {
        int output_row = _parent->_maxy;
        int x_max = getmaxx(_parent);
        attron(_attributes);
        mvwaddstr(_parent,output_row,x, teste.c_str());
        attroff(_attributes);
    }
};
