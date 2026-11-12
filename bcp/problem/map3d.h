#include "problem/map.h"

class Map3D : public Map {
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

        inline Int32 get_n(Position x, Position y, Position z) const
        {
            return static_cast<Int32>(z) * width() * height()
                + static_cast<Int32>(y) * width()
                + x;
        }
        inline Position get_y(const Int32 n) const
        {
            return (n / width()) % height();
        }
        inline Position get_z(const Int32 n) const
        {
            return n / (width() * height());
        }
        inline Int32 climb_up(const Int32 n) const
        {
            return n + static_cast<Int32>(width()) * height();
        }

        inline Int32 climb_down(const Int32 n) const
        {
            return n - static_cast<Int32>(width()) * height();
        }
        inline Position rotate_left(const Position turn) const
        {
            return (turn + 270) % 360;
        }

        inline Position rotate_right(const Position turn) const
        {
            return (turn + 90) % 360;
        }

        inline Node get_destination(const Node n, const Direction d) const
        {
            if (d == Direction::CLIMB_UP)
            {
                return Node{climb_up(n.n), n.orientation};
            }

            if (d == Direction::CLIMB_DOWN)
            {
                return Node{climb_down(n.n), n.orientation};
            }

            return Map::get_destination(n, d);
        }

    private:
        void resize(Position width, Position height, Position elevation);

};