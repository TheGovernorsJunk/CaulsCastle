Vector = {}
Vector_mt = {
    __index = Vector,
    __add = function(lhs, rhs)
        return Vector:new(lhs.x + rhs.x, lhs.y + rhs.y)
    end,
    __sub = function(lhs, rhs)
        return Vector:new(lhs.x - rhs.x, lhs.y - rhs.y)
    end,
    __mul = function(scalar, vector)
        return Vector:new(scalar * vector.x, scalar * vector.y)
    end,
    __div = function(vector, scalar)
        return Vector:new(vector.x / scalar, vector.y / scalar)
    end
}

function Vector:new(x, y)
    return setmetatable({x=x, y=y}, Vector_mt)
end

function Vector:dot(v)
    return self.x * v.x + self.y * v.y
end

function Vector:magnitude()
    return math.sqrt(self:dot(self))
end

function Vector:normalize()
    return self / self:magnitude()
end
