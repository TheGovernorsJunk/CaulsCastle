local function getCenter(area)
   return area.x + 0.5 * area.w, area.y + 0.5 * area.h
end

local function makeAnimationsFromAtlas(atlasID, game, fps)
   local msPerFrame
   if fps then msPerFrame = math.floor(1000 / fps) else msPerFrame = 42 end
   local spriteData = game:getSpriteData(atlasID)
   local textureID = game:loadTexture(spriteData.imagePath)

   local anims = {}
   for i, data in ipairs(spriteData) do
      local filename = string.gsub(string.gsub(data.n, '^.*/', ''), '%..+$', '')
      if (string.match(filename, '^%d+%-%a+$')) then
         local frame = tonumber(string.match(filename, '%d+'))
         local anim = string.match(filename, '%a+')
         anims[anim] = anims[anim] or {}
         anims[anim][frame] = game:makeSprite(textureID, data)
      end
   end

   local animIDs = {}
   for name, frames in pairs(anims) do
      local frameIndices = {}
      for frame,_ in pairs(frames) do
         table.insert(frameIndices, frame)
      end
      table.sort(frameIndices)
      local fillIndex = frameIndices[1]
      for i,frameIndex in ipairs(frameIndices) do
         while fillIndex < frameIndex do
            frames[fillIndex] = frames[frameIndices[i - 1]]
            fillIndex = fillIndex + 1
         end
      end
      animIDs[name] = game:makeAnimation(frames, msPerFrame)
   end

   return animIDs
end

Utils = {
   getCenter = getCenter,
   makeAnimationsFromAtlas = makeAnimationsFromAtlas
}

return Utils
