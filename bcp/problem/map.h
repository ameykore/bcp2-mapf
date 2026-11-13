/*
This file is part of BCP2-MAPF. BCP2-MAPF is licensed under the PolyForm
Noncommercial License 1.0.0. A copy of this license can found in LICENSE.md.

Author: Edward Lam <ed@ed-lam.com>
*/

#pragma once

#include "types/debug.h"
#include "types/file_system.h"
#include "types/map_types.h"
#include "types/tuple.h"
#include "types/vector.h"

class Map
{
  protected:
    Vector<Bool> passable_; // Row-major matrix
    Position width_ = 0;
    Position height_ = 0;

  public:
    // Constructors and destructor
    Map() = default;
    ~Map() = default;
    Map(const Map&) = default;
    Map(Map&&) = default;
    Map& operator=(const Map&) = default;
    Map& operator=(Map&&) = default;

    // Custom constructor
    Map(const FilePath& map_path);

    // Getters
    inline Int32 size() const
    {
        return passable_.size();
    }
    inline Bool empty() const
    {
        return !size();
    }
    inline Position width() const
    {
        return width_;
    }
    inline Position height() const
    {
        return height_;
    }
    inline Bool operator[](const Node n) const
    {
        DEBUG_ASSERT(0 <= n.n && n.n < size());
        return passable_[n.n];
    }
    inline Int32 get_n(const Position x, const Position y) const
    {
        return y * width_ + x;
    }
    inline Int32 get_n(const Pair<Position, Position> xy) const
    {
        return get_n(xy.first, xy.second);
    }
    inline Int32 get_n(const XY xy) const
    {
        return get_n(xy.x, xy.y);
    }
    inline Position get_x(const Int32 n) const
    {
        return static_cast<Position>(n % width_);
    }
    inline Position get_y(const Int32 n) const
    {
        return static_cast<Position>(n / width_);
    }
    inline XY get_xy(const Int32 n) const
    {
        return {get_x(n), get_y(n)};
    }
    inline Node get_north(const Node n) const
    {
        DEBUG_ASSERT(0 <= n.n && n.n < size());
        return Node{n.n - width_, n.orientation};
    }
    inline Node get_south(const Node n) const
    {
        DEBUG_ASSERT(0 <= n.n && n.n < size());
        return Node{n.n + width_, n.orientation};
    }
    inline Node get_west(const Node n) const
    {
        DEBUG_ASSERT(0 <= n.n && n.n < size());
        return Node{n.n - 1, n.orientation};
    }
    inline Node get_east(const Node n) const
    {
        DEBUG_ASSERT(0 <= n.n && n.n < size());
        return Node{n.n + 1, n.orientation};
    }
    inline Node get_wait(const Node n) const
    {
        DEBUG_ASSERT(0 <= n.n && n.n < size());
        return n;
    }
    inline Size64 get_degree(const Node n) const
    {
        return (*this)[get_north(n)] + (*this)[get_south(n)] + (*this)[get_west(n)] +
               (*this)[get_east(n)];
    }
    inline Direction get_direction(const Node n1, const Node n2) const
    {
        // Same position.
        if (n1.n == n2.n)
        {
            return n1.orientation == n2.orientation
                    ? Direction::WAIT
                    : Direction::ROTATE;
        }

        // Horizontal movement must preserve orientation.
        DEBUG_ASSERT(n1.orientation == n2.orientation);

        if (n2.n == n1.n - width_)
            return Direction::NORTH;

        if (n2.n == n1.n + width_)
            return Direction::SOUTH;

        if (n2.n == n1.n - 1)
            return Direction::WEST;

        if (n2.n == n1.n + 1)
            return Direction::EAST;

        DEBUG_ASSERT(false);
        return Direction::INVALID;
    }
    inline Int32 get_direction_n_offset(const Direction d) const
    {
        return width_ * (static_cast<Int32>(d == Direction::SOUTH) -
                         static_cast<Int32>(d == Direction::NORTH)) +
               (static_cast<Int32>(d == Direction::EAST) - static_cast<Int32>(d == Direction::WEST));
    }
    virtual inline Node get_destination(const Node n, const Direction d) const
    {
        DEBUG_ASSERT(d == Direction::NORTH || d == Direction::SOUTH || d == Direction::EAST ||
                     d == Direction::WEST || d == Direction::WAIT || d == Direction::ROTATE);
        
        
        Node next = n;
        next.n += get_direction_n_offset(d);

        if (d == Direction::ROTATE)
        {
            DEBUG_ASSERT(next.orientation <= 1);
            next.orientation = static_cast<UInt8>(1 - next.orientation);
        }

        return next;
    }
    inline Node get_destination(const Edge e) const
    { //where the robot would end up after given action
        return get_destination(e.n, e.d);
    }
    inline Node get_destination(const EdgeTime et) const
    {
        return get_destination(et.n, et.d);
    }
    inline XY get_destination_xy(const Edge e) const
    {
        return get_xy(get_destination(e).n);
    }
    inline XY get_destination_xy(const EdgeTime et) const
    {
        return get_xy(get_destination(et).n);
    }
    inline Edge get_undirected_edge(const Edge e) const
    {
        switch (e.d)
        {
            case (Direction::EAST):
            {
                return Edge(get_east(e.n), Direction::WEST);
            }
            case (Direction::SOUTH):
            {
                return Edge(get_south(e.n), Direction::NORTH);
            }
            default:
            {
                return e;
            }
        }
    }
    static inline Direction get_opposite_direction(const Direction d)
    {
        switch (d)
        {
            case (Direction::NORTH):
            {
                return Direction::SOUTH;
            }
            case (Direction::SOUTH):
            {
                return Direction::NORTH;
            }
            case (Direction::WEST):
            {
                return Direction::EAST;
            }
            case (Direction::EAST):
            {
                return Direction::WEST;
            }
            default:
            {
                return d;
            }
        }
    }
    inline Edge get_opposite_edge(const Edge e) const
    {
        switch (e.d)
        {
            case (Direction::NORTH):
            {
                return Edge(get_north(e.n), Direction::SOUTH);
            }
            case (Direction::SOUTH):
            {
                return Edge(get_south(e.n), Direction::NORTH);
            }
            case (Direction::WEST):
            {
                return Edge(get_west(e.n), Direction::EAST);
            }
            case (Direction::EAST):
            {
                return Edge(get_east(e.n), Direction::WEST);
            }
            default:
            {
                return e;
            }
        }
    }

    // Setters
    void set_passable(const Node n);
    void set_obstacle(const Node n);

    // Debug
    void print() const;

  private:
    // Setters
    void resize(const Position width, const Position height);
  protected:
    void resize(const Position width, const Position height, const Position elevation);
};
