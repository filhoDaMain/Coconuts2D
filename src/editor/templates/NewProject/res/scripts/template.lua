-- Coconuts2D Scripting template file --
-- version X
-- 2026

local entity = require("coconuts2D.entity_api")
local this = entity:extend()

-- Other requires:


-- Entity instantiation
function this:init()
    print("Hello " .. self:id() .. " " .. this:get_component(TagComponent).tag)
end


-- Render Loop
function this:update()
    print("Update " .. self:id() .. " " .. this:get_component(TagComponent).tag)
end


-- Entity destruction
function this:destroy()
    print("Destroy " .. self:id() .. " " .. this:get_component(TagComponent).tag)
end

return this