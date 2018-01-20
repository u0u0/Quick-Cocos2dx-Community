
cc = cc or {}

function cc.clampf(value, min_inclusive, max_inclusive)
    -- body
    local temp = 0
    if min_inclusive > max_inclusive then
        temp = min_inclusive
        min_inclusive =  max_inclusive
        max_inclusive = temp
    end

    if value < min_inclusive then
        return min_inclusive
    elseif value < max_inclusive then
        return value
    else
        return max_inclusive
    end
end

--Point
function cc.p(_x,_y)
    if nil == _y then
         return { x = _x.x, y = _x.y }
    else
         return { x = _x, y = _y }
    end
end

function cc.pAdd(pt1,pt2)
    return {x = pt1.x + pt2.x , y = pt1.y + pt2.y }
end

function cc.pSub(pt1,pt2)
    return {x = pt1.x - pt2.x , y = pt1.y - pt2.y }
end

function cc.pMul(pt1,factor)
    return { x = pt1.x * factor , y = pt1.y * factor }
end

function cc.pMidpoint(pt1,pt2)
    return { x = (pt1.x + pt2.x) / 2.0 , y = ( pt1.y + pt2.y) / 2.0 }
end

function cc.pForAngle(a)
    return { x = math.cos(a), y = math.sin(a) }
end

function cc.pGetLength(pt)
    return math.sqrt( pt.x * pt.x + pt.y * pt.y )
end

function cc.pNormalize(pt)
    local length = cc.pGetLength(pt)
    if 0 == length then
        return { x = 1.0,y = 0.0 }
    end

    return { x = pt.x / length, y = pt.y / length }
end

function cc.pCross(self,other)
    return self.x * other.y - self.y * other.x
end

function cc.pDot(self,other)
    return self.x * other.x + self.y * other.y
end

function cc.pToAngleSelf(self)
    return math.atan2(self.y, self.x)
end

function cc.pGetAngle(self,other)
    local a2 = cc.pNormalize(self)
    local b2 = cc.pNormalize(other)
    local angle = math.atan2(cc.pCross(a2, b2), cc.pDot(a2, b2) )
    if math.abs(angle) < 1.192092896e-7 then
        return 0.0
    end

    return angle
end

function cc.pGetDistance(startP,endP)
    return cc.pGetLength(cc.pSub(startP,endP))
end

function cc.pIsLineIntersect(A, B, C, D, s, t)
    if ((A.x == B.x) and (A.y == B.y)) or ((C.x == D.x) and (C.y == D.y))then
        return false, s, t
    end

    local BAx = B.x - A.x
    local BAy = B.y - A.y
    local DCx = D.x - C.x
    local DCy = D.y - C.y
    local ACx = A.x - C.x
    local ACy = A.y - C.y

    local denom = DCy * BAx - DCx * BAy
    s = DCx * ACy - DCy * ACx
    t = BAx * ACy - BAy * ACx

    if (denom == 0) then
        if (s == 0 or t == 0) then
            return true, s , t
        end

        return false, s, t
    end

    s = s / denom
    t = t / denom

    return true,s,t
end

function cc.pPerp(pt)
    return { x = -pt.y, y = pt.x }
end

function cc.RPerp(pt)
    return { x = pt.y,  y = -pt.x }
end

function cc.pProject(pt1, pt2)
    return { x = pt2.x * (cc.pDot(pt1,pt2) / cc.pDot(pt2,pt2)) , y = pt2.y * (cc.pDot(pt1,pt2) / cc.pDot(pt2,pt2)) }
end

function cc.pRotate(pt1, pt2)
    return { x = pt1.x * pt2.x - pt1.y * pt2.y, y = pt1.x * pt2.y + pt1.y * pt2.x }
end

function cc.pUnrotate(pt1, pt2)
    return { x = pt1.x * pt2.x + pt1.y * pt2.y, pt1.y * pt2.x - pt1.x * pt2.y }
end
--Calculates the square length of pt
function cc.pLengthSQ(pt)
    return cc.pDot(pt,pt)
end
--Calculates the square distance between pt1 and pt2
function cc.pDistanceSQ(pt1,pt2)
    return cc.pLengthSQ(cc.pSub(pt1,pt2))
end

function cc.pGetClampPoint(pt1,pt2,pt3)
    return { x = cc.clampf(pt1.x, pt2.x, pt3.x), y = cc.clampf(pt1.y, pt2.y, pt3.y) }
end

function cc.pFromSize(sz)
    return { x = sz.width, y = sz.height }
end

function cc.pLerp(pt1,pt2,alpha)
    return cc.pAdd(cc.pMul(pt1, 1.0 - alpha), cc.pMul(pt2,alpha) )
end

function cc.pFuzzyEqual(pt1,pt2,variance)
    if (pt1.x - variance <= pt2.x) and (pt2.x <= pt1.x + variance) and (pt1.y - variance <= pt2.y) and (pt2.y <= pt1.y + variance) then
        return true
    else
        return false
    end
end

function cc.pRotateByAngle(pt1, pt2, angle)
    return cc.pAdd(pt2, cc.pRotate( cc.pSub(pt1, pt2),cc.pForAngle(angle)))
end

function cc.pIsSegmentIntersect(pt1,pt2,pt3,pt4)
    local s,t,ret = 0,0,false
    ret,s,t =cc.pIsLineIntersect(pt1, pt2, pt3, pt4,s,t)

    if ret and  s >= 0.0 and s <= 1.0 and t >= 0.0 and t <= 1.0 then
        return true;
    end

    return false
end

function cc.pGetIntersectPoint(pt1,pt2,pt3,pt4)
    local s,t, ret = 0,0,false
    ret,s,t = cc.pIsLineIntersect(pt1,pt2,pt3,pt4,s,t)
    if ret then
        return cc.p(pt1.x + s * (pt2.x - pt1.x), pt1.y + s * (pt2.y - pt1.y))
    else
        return cc.p(0,0)
    end
end
--Size
function cc.size( _width,_height )
    return { width = _width, height = _height }
end

--Rect
function cc.rect(_x,_y,_width,_height)
    return { x = _x, y = _y, width = _width, height = _height }
end

function cc.rectEqualToRect(rect1,rect2)
    if ((rect1.x ~= rect2.x) or (rect1.y ~= rect2.y) or
        (rect1.width ~= rect2.width) or (rect1.height ~= rect2.height)) then
        return false
    end

    return true
end

function cc.rectGetMaxX(rect)
    return rect.x + rect.width
end

function cc.rectGetMidX(rect)
    return rect.x + rect.width / 2.0
end

function cc.rectGetMinX(rect)
    return rect.x
end

function cc.rectGetMaxY(rect)
    return rect.y + rect.height
end

function cc.rectGetMidY(rect)
    return rect.y + rect.height / 2.0
end

function cc.rectGetMinY(rect)
    return rect.y
end

function cc.rectContainsPoint( rect, point )
    local ret = false

    if (point.x >= rect.x) and (point.x <= rect.x + rect.width) and
       (point.y >= rect.y) and (point.y <= rect.y + rect.height) then
        ret = true
    end

    return ret
end

function cc.rectContainsRect(rect1,rect2)
    if ((rect1.x <= rect2.x) and (rect1.y <= rect2.y) and
        (rect1.x + rect1.width >= rect2.x + rect2.width) and
        (rect1.y + rect1.height >= rect2.y + rect2.height)) then
        return true
    end

    return false
end

function cc.rectIntersectsRect( rect1, rect2 )
    local intersect = not ( rect1.x > rect2.x + rect2.width or
                    rect1.x + rect1.width < rect2.x         or
                    rect1.y > rect2.y + rect2.height        or
                    rect1.y + rect1.height < rect2.y )

    return intersect
end

function cc.rectUnion( rect1, rect2 )
    local rect = cc.rect(0, 0, 0, 0)
    rect.x = math.min(rect1.x, rect2.x)
    rect.y = math.min(rect1.y, rect2.y)
    rect.width = math.max(rect1.x + rect1.width, rect2.x + rect2.width) - rect.x
    rect.height = math.max(rect1.y + rect1.height, rect2.y + rect2.height) - rect.y
    return rect
end

function cc.rectIntersection( rect1, rect2 )
    local intersection = cc.rect(
        math.max(rect1.x, rect2.x),
        math.max(rect1.y, rect2.y),
        0, 0)

    intersection.width = math.min(rect1.x + rect1.width, rect2.x + rect2.width) - intersection.x
    intersection.height = math.min(rect1.y + rect1.height, rect2.y + rect2.height) - intersection.y
    return intersection
end

--Color3B
function cc.c3b( _r,_g,_b )
    return { r = _r, g = _g, b = _b }
end

--Color4B
function cc.c4b( _r,_g,_b,_a )
    return { r = _r, g = _g, b = _b, a = _a }
end

--Color4F
function cc.c4f( _r,_g,_b,_a )
    return { r = _r, g = _g, b = _b, a = _a }
end

--Vertex2F
function cc.vertex2F(_x,_y)
    return { x = _x, y = _y }
end

--Vertex3F
function cc.Vertex3F(_x,_y,_z)
    return { x = _x, y = _y, z = _z }
end

--Tex2F
function cc.tex2F(_u,_v)
    return { u = _u, v = _v }
end

--PointSprite
function cc.PointSprite(_pos,_color,_size)
    return { pos = _pos, color = _color, size = _size }
end

--Quad2
function cc.Quad2(_tl,_tr,_bl,_br)
    return { tl = _tl, tr = _tr, bl = _bl, br = _br }
end

--Quad3
function cc.Quad3(_tl, _tr, _bl, _br)
    return { tl = _tl, tr = _tr, bl = _bl, br = _br }
end

--V2F_C4B_T2F
function cc.V2F_C4B_T2F(_vertices, _colors, _texCoords)
    return { vertices = _vertices, colors = _colors, texCoords = _texCoords }
end

--V2F_C4F_T2F
function cc.V2F_C4F_T2F(_vertices, _colors, _texCoords)
    return { vertices = _vertices, colors = _colors, texCoords = _texCoords }
end

--V3F_C4B_T2F
function cc.V3F_C4B_T2F(_vertices, _colors, _texCoords)
    return { vertices = _vertices, colors = _colors, texCoords = _texCoords }
end

--V2F_C4B_T2F_Quad
function cc.V2F_C4B_T2F_Quad(_bl, _br, _tl, _tr)
    return { bl = _bl, br = _br, tl = _tl, tr = _tr }
end

--V3F_C4B_T2F_Quad
function cc.V3F_C4B_T2F_Quad(_tl, _bl, _tr, _br)
    return { tl = _tl, bl = _bl, tr = _tr, br = _br }
end

--V2F_C4F_T2F_Quad
function cc.V2F_C4F_T2F_Quad(_bl, _br, _tl, _tr)
    return { bl = _bl, br = _br, tl = _tl, tr = _tr }
end

--T2F_Quad
function cc.T2F_Quad(_bl, _br, _tl, _tr)
    return { bl = _bl, br = _br, tl = _tl, tr = _tr }
end

--AnimationFrameData
function cc.AnimationFrameData( _texCoords, _delay, _size)
    return { texCoords = _texCoords, delay = _delay, size = _size }
end

--PhysicsMaterial
function cc.PhysicsMaterial(_density, _restitution, _friction)
	return { density = _density, restitution = _restitution, friction = _friction }
end

function cc.vec3(_x, _y, _z)
    return { x = _x, y = _y, z = _z }
end

function cc.vec4(_x, _y, _z, _w)
    return { x = _x, y = _y, z = _z, w = _w }
end

function cc.vec3normalize(vec3)
    local n = vec3.x * vec3.x + vec3.y * vec3.y + vec3.z * vec3.z
    if n == 1.0 then
        return vec3
    end

    n = math.sqrt(n)

    if n < 2e-37 then
        return vec3
    end

    n = 1.0 / n
    return {x = vec3.x * n, y = vec3.y * n, z = vec3.z * n}
end

function cc.vec3Add(vec3a, vec3b)
    return {x = vec3a.x + vec3b.x, y = vec3a.y + vec3b.y, z = vec3a.z + vec3b.z}
end

function cc.vec3Sub(vec3a, vec3b)
    return {x = vec3a.x - vec3b.x, y = vec3a.y - vec3b.y, z = vec3a.z - vec3b.z}
end

function cc.vec3Mid(vec3a, vec3b)
    return {x = (vec3a.x + vec3b.x) / 2, y = (vec3a.y + vec3b.y) / 2, z = (vec3a.z + vec3b.z) / 2}
end

function cc.vec3Mul(vec3, factor)
    return {x = vec3.x * factor, y = vec3.y * factor, z = vec3.z * factor}
end

function cc.vec3Dot(vec3a, vec3b)
    return vec3a.x * vec3b.x + vec3a.y * vec3b.y + vec3a.z * vec3b.z
end

function cc.vec3Cross(vec3a, vec3b)
    return {x = vec3a.y * vec3b.z - vec3a.z * vec3b.y, y = vec3a.z * vec3b.x - vec3a.x * vec3b.z, z = vec3a.x * vec3b.y - vec3a.y * vec3b.x}
end

function cc.vec3Equal(vec3a, vec3b)
    return vec3a.x == vec3b.x and vec3a.y == vec3b.y and vec3a.z == vec3b.z
end

-- 根据位置,朝向,顶方向计算欧拉角
function cc.vec3RotationAngle(position, tangent, up)
    local xaxis = cc.vec3normalize(tangent)
    local zaxis = cc.vec3normalize(cc.vec3Cross(xaxis, up))
    local yaxis = cc.vec3normalize(cc.vec3Cross(zaxis, xaxis))

    local quaternion = cc.quaternion(0, 0, 0, 0)
    local trace = xaxis.x + yaxis.y + zaxis.z + 1.0

    if trace > 0.000001 then
        local s = 0.5 / math.sqrt(trace)
        quaternion.w = 0.25 / s
        quaternion.x = (yaxis.z - zaxis.y) * s
        quaternion.y = (zaxis.x - xaxis.z) * s
        quaternion.z = (xaxis.y - yaxis.x) * s
    else
        if xaxis.x > yaxis.y and xaxis.x > zaxis.z then
            local s = 0.5 / math.sqrt(1.0 + xaxis.x - yaxis.y - zaxis.z)
            quaternion.w = (yaxis.z - zaxis.y) * s
            quaternion.x = 0.25 / s
            quaternion.y = (yaxis.x + xaxis.y) * s
            quaternion.z = (zaxis.x + xaxis.z) * s
        elseif yaxis.y > zaxis.z then
            local s = 0.5 / math.sqrt(1.0 + yaxis.y - xaxis.x - zaxis.z)
            quaternion.w = (zaxis.x - xaxis.z) * s
            quaternion.x = (yaxis.x + xaxis.y) * s
            quaternion.y = 0.25 / s
            quaternion.z = (zaxis.y + yaxis.z) * s
        else
            local s = 0.5 / math.sqrt(1.0 + zaxis.z - xaxis.x - yaxis.y)
            quaternion.w = (xaxis.y - yaxis.x ) * s
            quaternion.x = (zaxis.x + xaxis.z ) * s
            quaternion.y = (zaxis.y + yaxis.z ) * s
            quaternion.z = 0.25 / s
        end
    end

    local rotx = math.atan2(2 * (quaternion.w * quaternion.x + quaternion.y * quaternion.z), 1 - 2 * (quaternion.x * quaternion.x + quaternion.y * quaternion.y))
    local roty = math.asin(cc.clampf(2 * (quaternion.w * quaternion.y - quaternion.z * quaternion.x), -1.0 , 1.0))
    local rotz = -1 * math.atan2(2 * (quaternion.w * quaternion.z + quaternion.x * quaternion.y), 1 - 2 * (quaternion.y * quaternion.y + quaternion.z * quaternion.z))

    local degreesx = rotx * 57.295779513082     --  180 / math.pi
    local degreesy = roty * 57.295779513082
    local degreesz = rotz * 57.295779513082

    return cc.vec3(degreesx, degreesy, degreesz)
end

function cc.quaternion(_x, _y ,_z,_w)
    return { x = _x, y = _y, z = _z, w = _w }
end

cc.mat4 = cc.mat4 or {}

function cc.mat4.new(...)
    local params = {...}
    local size   = #params

    local obj = {}

    if 1 == size then
        assert(type(params[1]) == "table" , "type of input params are wrong to new a mat4 when num of params is 1")
        for i= 1, 16 do
            if params[1][i] ~= nil then
                obj[i] = params[1][i]
            else
                obj[i] = 0
            end
        end
    elseif 16 == size then
        if params[i] ~= nil then
            mat4[i] = params[i]
        else
            mat4[i] = 0
        end
    end

    setmetatable(obj, {__index = cc.mat4})

    return obj
end

function cc.mat4.getInversed(self)
    return mat4_getInversed(self)
end

function cc.mat4.transformVector(self, vector, dst)
    return mat4_transformVector(self, vector, dst)
end

-- 2D 到 3D 映射
function cc.unproject(viewProjection, viewPort, src, dst)
    local screen = cc.vec4(src.x / viewPort.width, src.y / viewPort.height, src.z, 1.0)
    screen.x = screen.x * 2.0 - 1.0
    screen.y = screen.y * 2.0 - 1.0
    screen.z = screen.z * 2.0 - 1.0

    local b = viewProjection:getInversed()
    local inversed = cc.mat4.new(viewProjection:getInversed())
    screen = inversed:transformVector(screen, screen)

    if screen.w ~= 0.0 then
        screen.x = screen.x / screen.w
        screen.y = screen.y / screen.w
        screen.z = screen.z / screen.w
    end

    dst.x = screen.x
    dst.y = screen.y
    dst.z = screen.z

    return dst
end

-- 3D 到 2D 映射
function cc.project(viewProjection, viewPort, src, dst)
    local clipPos = cc.vec4(src.x, src.y, src.z, 1);
    clipPos = viewProjection:transformVector(cc.vec4(src.x, src.y, src.z, 1), clipPos);

    if clipPos.w == 0 then return dst end
    local ndcX = clipPos.x / clipPos.w;
    local ndcY = clipPos.y / clipPos.w;

    dst.x = (ndcX + 1.0) * 0.5 * viewPort.width;
    dst.y = (ndcY + 1.0) * 0.5 * viewPort.height;

    return dst;
end

-- 获取在Camera中的射线
function cc.getRayByLocation(ray, location, camera)
    local viewProjection = cc.mat4.new(camera:getViewProjectionMatrix())
    local viewPort = display.size

    local nearPoint = cc.vec3(location.x, location.y, -1)
    nearPoint = cc.unproject(viewProjection, viewPort, nearPoint, nearPoint)
    local farPoint = cc.vec3(location.x, location.y, 1)
    farPoint = cc.unproject(viewProjection, viewPort, farPoint, farPoint)
    local direction = cc.vec3Sub(farPoint, nearPoint)

    ray._origin    = nearPoint
    ray._direction = cc.vec3normalize(direction)

    return ray
end

-- 获取Camera中3D转屏幕坐标值
function cc.getLocationByPosition3D(position, camera)
    local viewProjection = cc.mat4.new(camera:getViewProjectionMatrix())
    local viewPort = display.size

    local location = cc.p(0, 0)
    location = cc.project(viewProjection, viewPort, position, location)

    return location
end

