local function getCenter(area)
   return area.x + 0.5 * area.w, area.y + 0.5 * area.h
end

Utils = {
   getCenter = getCenter
}

return Utils
