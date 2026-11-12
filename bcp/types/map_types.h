/*
This file is part of BCP2-MAPF. BCP2-MAPF is licensed under the PolyForm
Noncommercial License 1.0.0. A copy of this license can found in LICENSE.md.

Author: Edward Lam <ed@ed-lam.com>
*/

#pragma once

#include "types/basic_types.h"
#include <cstring>
#include <type_traits>

using Position = Int16;
// using Node = Int32;

//orientation with NOde
struct Node
{
    Int32 n;
    uint8_t orientation; //0 = Y-axis, 1 = X-axis

    inline Bool operator==(const Node& other) const
    {
        return n == other.n &&
            orientation == other.orientation;
    }
    constexpr auto operator<=>(const Node& other) const = default;
};

struct XY
{
    Position x;
    Position y;

    constexpr XY() noexcept = default;
    constexpr XY(const Position x, const Position y) noexcept :
        x(x),
        y(y)
    {
    }
};
static_assert(sizeof(Position) == 2);
static_assert(sizeof(XY) == 4);
static_assert(std::is_trivial_v<XY>);
static_assert(std::is_trivially_default_constructible_v<XY>);
static_assert(std::is_trivially_destructible_v<XY>);
static_assert(std::has_unique_object_representations_v<XY>);

struct XYT
{
    Position x;
    Position y;
    Time t;

    constexpr XYT() noexcept = default;
    constexpr XYT(const Position x, const Position y, const Time t) noexcept :
        x(x),
        y(y),
        t(t)
    {
    }

    inline UInt64 id() const
    {
        static_assert(sizeof(XYT) == sizeof(UInt64));
        UInt64 mem;
        std::memcpy(&mem, this, sizeof(UInt64));
        return mem;
    }

    inline Bool operator==(const XYT& other) const
    {
        return this->id() == other.id();
    }
    inline Bool operator!=(const XYT& other) const
    {
        return this->id() != other.id();
    }
};
static_assert(sizeof(Position) == 2);
static_assert(sizeof(Time) == 4);
static_assert(sizeof(XYT) == 8);
static_assert(std::is_trivial_v<XYT>);
static_assert(std::is_trivially_default_constructible_v<XYT>);
static_assert(std::is_trivially_destructible_v<XYT>);
static_assert(std::has_unique_object_representations_v<XYT>);


struct NodeTime
{
    Node n;
    Time t;
    

    constexpr NodeTime() noexcept = default;
    constexpr NodeTime(const Node n, const Time t) noexcept :
        n(n),
        t(t)
    {
    }

    // inline UInt64 id() const
    // {
    //     // static_assert(sizeof(NodeTime) == sizeof(UInt64));
    //     static_assert(sizeof(NodeTime) == 12);
    //     UInt64 mem;
    //     std::memcpy(&mem, this, sizeof(UInt64));
    //     return mem;
    // }

    inline unsigned __int128 id() const noexcept
    {
        unsigned __int128 mem = static_cast<UInt32>(n.n);

        mem = (mem << 8) | static_cast<UInt8>(n.orientation);
        mem = (mem << 32) | static_cast<UInt32>(t);

        return mem;
    }

    inline Bool operator==(const NodeTime& other) const
    {
        // return this->id() == other.id();
         return n.n == other.n.n &&
           n.orientation == other.n.orientation &&
           t == other.t;
    }
    inline Bool operator!=(const NodeTime& other) const
    {
        return this->id() != other.id();
        // return !(*this == other);
    }
};
static_assert(sizeof(Node) == 8);
static_assert(sizeof(Time) == 4);
static_assert(sizeof(NodeTime) == 12);
static_assert(std::is_trivial_v<NodeTime>);
static_assert(std::is_trivially_default_constructible_v<NodeTime>);
static_assert(std::is_trivially_destructible_v<NodeTime>);
// static_assert(std::has_unique_object_representations_v<NodeTime>);

enum Direction : UInt8
{
    NORTH = 0,  // 0000
    SOUTH = 1,  // 0001
    WEST = 2,   // 0010
    EAST = 3,   // 0011
    WAIT = 4,   // 0100
    INVALID = 5, // 0101
    ROTATE = 6,
    
    NORTH_UP = 7,
    SOUTH_UP = 8,
    WEST_UP = 9,
    EAST_UP = 10,

    NORTH_DOWN = 11,
    SOUTH_DOWN = 12,
    WEST_DOWN = 13,
    EAST_DOWN = 14
};

struct Edge
{
    Node n;
    Direction d;

    constexpr Edge() noexcept = default;
    constexpr Edge(const Node n, const Direction d) noexcept :
        n(n),
        d(d)
    {
    }

    // inline UInt32 id() const
    // {
    //     static_assert(sizeof(Edge) == sizeof(UInt32));
    //     UInt32 mem;
    //     std::memcpy(&mem, this, sizeof(UInt32));
    //     return mem;
    // }
    inline UInt64 id() const noexcept
    {
        UInt64 mem = static_cast<UInt32>(n.n);

        mem = (mem << 8) | static_cast<UInt8>(n.orientation);
        mem = (mem << 8) | static_cast<UInt8>(d);

        return mem;
    }

    inline Bool operator==(const Edge& other) const
    {
        return this->id() == other.id();
    }
    inline Bool operator!=(const Edge& other) const
    {
        return this->id() != other.id();
    }
};
static_assert(sizeof(Node) == 8);
static_assert(sizeof(Edge) == 12);
static_assert(std::is_trivial_v<Edge>);
static_assert(std::is_trivially_default_constructible_v<Edge>);
static_assert(std::is_trivially_destructible_v<Edge>);
// static_assert(std::has_unique_object_representations_v<Edge>);

struct EdgeTime
{
    Node n;
    Direction d;
    Time t;

    constexpr EdgeTime() noexcept = default;
    constexpr EdgeTime(const Node n, const Direction d, const Time t) noexcept :
        n(n),
        d(d),
        t(t)
    {
    }
    constexpr EdgeTime(const Edge e, const Time t) noexcept :
        EdgeTime(e.n, e.d, t)
    {
    }
    constexpr EdgeTime(const NodeTime nt, const Direction d) noexcept :
        EdgeTime(nt.n, d, nt.t)
    {
    }

    inline NodeTime nt() const noexcept
    {
        return NodeTime(n, t);
    }
    inline Edge e() const noexcept
    {
        return Edge{n, d};
    }

    // inline UInt64 id() const
    // {
    //     static_assert(sizeof(EdgeTime) == sizeof(UInt64));
    //     UInt64 mem;
    //     std::memcpy(&mem, this, sizeof(UInt64));
    //     return mem;
    // }
    inline unsigned __int128 id() const noexcept
    {
        unsigned __int128 mem = static_cast<UInt32>(n.n);

        mem = (mem << 8) | static_cast<UInt8>(n.orientation);
        mem = (mem << 8) | static_cast<UInt8>(d);
        mem = (mem << 32) | static_cast<UInt32>(t);

        return mem;
    }

    inline Bool operator==(const EdgeTime& other) const
    {
        return id() == other.id();
    }
    inline Bool operator!=(const EdgeTime& other) const
    {
        return id() != other.id();
    }
};
static_assert(sizeof(EdgeTime) == 16);
static_assert(std::is_trivial_v<EdgeTime>);
static_assert(std::is_trivially_default_constructible_v<EdgeTime>);
static_assert(std::is_trivially_destructible_v<EdgeTime>);
// static_assert(std::has_unique_object_representations_v<EdgeTime>);

struct AgentNode
{
    Agent a;
    Node n;

    constexpr AgentNode() noexcept = default;
    constexpr AgentNode(const Agent a, const Node n) noexcept :
        a(a),
        n(n)
    {
    }

    // inline UInt64 id() const
    // {
    //     static_assert(sizeof(AgentNode) == sizeof(UInt64));
    //     UInt64 mem;
    //     std::memcpy(&mem, this, sizeof(UInt64));
    //     return mem;
    // }
    inline unsigned __int128 id() const noexcept
    {
        unsigned __int128 mem = static_cast<UInt32>(a);

        mem = (mem << 32) | static_cast<UInt32>(n.n);
        mem = (mem << 8) | static_cast<UInt8>(n.orientation);

        return mem;
    }

    inline Bool operator==(const AgentNode& other) const
    {
        return this->id() == other.id();
    }
    inline Bool operator!=(const AgentNode& other) const
    {
        return this->id() != other.id();
    }
};
static_assert(sizeof(AgentNode) == 12);
static_assert(std::is_trivial_v<AgentNode>);
static_assert(std::is_trivially_default_constructible_v<AgentNode>);
static_assert(std::is_trivially_destructible_v<AgentNode>);
// static_assert(std::has_unique_object_representations_v<AgentNode>);

struct AgentTime
{
    Agent a;
    Time t;

    constexpr AgentTime() noexcept = default;
    constexpr AgentTime(const Agent a, const Time t) noexcept :
        a(a),
        t(t)
    {
    }

    inline UInt64 id() const
    {
        static_assert(sizeof(AgentTime) == sizeof(UInt64));
        UInt64 mem;
        std::memcpy(&mem, this, sizeof(UInt64));
        return mem;
    }

    inline Bool operator==(const AgentTime& other) const
    {
        return this->id() == other.id();
    }
    inline Bool operator!=(const AgentTime& other) const
    {
        return this->id() != other.id();
    }
};
static_assert(sizeof(AgentTime) == 8);
static_assert(std::is_trivial_v<AgentTime>);
static_assert(std::is_trivially_default_constructible_v<AgentTime>);
static_assert(std::is_trivially_destructible_v<AgentTime>);
static_assert(std::has_unique_object_representations_v<AgentTime>);

struct AgentNodeTime
{
    UInt16 a;
    UInt16 t;
    Node n;

    constexpr AgentNodeTime() noexcept = default;
    constexpr AgentNodeTime(const Agent a, const Node n, const Time t) noexcept :
        a(a),
        t(t),
        n(n)
    {
    }

    inline NodeTime nt() const noexcept
    {
        return NodeTime(n, t);
    }

    // inline UInt64 id() const
    // {
    //     static_assert(sizeof(AgentNodeTime) == sizeof(UInt64));
    //     UInt64 mem;
    //     std::memcpy(&mem, this, sizeof(UInt64));
    //     return mem;
    // }
    inline unsigned __int128 id() const noexcept
    {
        unsigned __int128 mem = static_cast<UInt16>(a);

        mem = (mem << 16) | static_cast<UInt16>(t);
        mem = (mem << 32) | static_cast<UInt32>(n.n);
        mem = (mem << 8) | static_cast<UInt8>(n.orientation);

        return mem;
    }

    inline Bool operator==(const AgentNodeTime& other) const
    {
        return this->id() == other.id();
    }
    inline Bool operator!=(const AgentNodeTime& other) const
    {
        return this->id() != other.id();
    }
};
static_assert(sizeof(AgentNodeTime) == 12);
static_assert(std::is_trivial_v<AgentNodeTime>);
static_assert(std::is_trivially_default_constructible_v<AgentNodeTime>);
static_assert(std::is_trivially_destructible_v<AgentNodeTime>);
// static_assert(std::has_unique_object_representations_v<AgentNodeTime>);

struct AgentAgent
{
    Agent a1;
    Agent a2;

    constexpr AgentAgent() noexcept = default;
    constexpr AgentAgent(const Agent a1, const Agent a2) noexcept :
        a1(a1),
        a2(a2)
    {
    }

    inline UInt64 id() const
    {
        static_assert(sizeof(AgentAgent) == sizeof(UInt64));
        UInt64 mem;
        std::memcpy(&mem, this, sizeof(UInt64));
        return mem;
    }

    inline Bool operator==(const AgentAgent& other) const
    {
        return this->id() == other.id();
    }
    inline Bool operator!=(const AgentAgent& other) const
    {
        return this->id() != other.id();
    }
};
static_assert(sizeof(AgentAgent) == 8);
static_assert(std::is_trivial_v<AgentAgent>);
static_assert(std::is_trivially_default_constructible_v<AgentAgent>);
static_assert(std::is_trivially_destructible_v<AgentAgent>);
static_assert(std::has_unique_object_representations_v<AgentAgent>);
