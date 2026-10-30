#include "problem/map.h"

class Map3D : Map {
    private:
        Position elevation_ = 0; // to keep height in edwards we are choosing elevation 

        // now we have to set things are passable or not. 
        // FOR THAT we need some binding script          
    
    public:
        Map3D(Position width, Position height, Position elevation);

        Position elevation() const
        {
            return elevation_;
        }

        inline Node get_n(Position x, Position y, Position z) const
        {
            return static_cast<Node>(z) * width() * height()
                + static_cast<Node>(y) * width()
                + x;
        }
        inline Position get_y(const Node n) const
        {
            return (n / width()) % height();
        }
        inline Position get_z(const Node n) const
        {
            return n / (width() * height());
        }
        inline Node climb_up(const Node n) const
        {
            return n + static_cast<Node>(width()) * height();
        }

        inline Node climb_down(const Node n) const
        {
            return n - static_cast<Node>(width()) * height();
        }
        inline Position rotate_left(const Position turn) const
        {
            return (turn + 270) % 360;
        }

        inline Position rotate_right(const Position turn) const
        {
            return (turn + 90) % 360;
        }

    private:
        void resize(Position width, Position height, Position elevation);

};